#include "ShaderCompiler.h"
#include "glslang/Public/ResourceLimits.h"
#include "glslang/SPIRV/SpvTools.h"
#include "glslang/SPIRV/GlslangToSpv.h"
#include "Core/FilePath.h"
#include "Core/Debug.h"
using namespace fm;
using namespace gui;

/*
class GlslShaderIncluder : public glslang::TShader::Includer
{
public:
	//    explicit GlslShaderIncluder(fileio::Directory* shaderdir)
	//        : mShaderdir(shaderdir) {}

		// Note "local" vs. "system" is not an "either/or": "local" is an
		// extra thing to do over "system". Both might get called, as per
		// the C++ specification.
		//
		// For the "system" or <>-style includes; search the "system" paths.
	virtual IncludeResult* includeSystem(
		const char* headerName, const char* includerName, size_t inclusionDepth) override
	{
		return nullptr;
	}

	// For the "local"-only aspect of a "" include. Should not search in the
	// "system" paths, because on returning a failure, the parser will
	// call includeSystem() to look in the "system" locations.
	virtual IncludeResult* includeLocal(
		const char* headerName, const char* includerName, size_t inclusionDepth) override
	{
		{
			//log_debug("includeLocal({}, {}, {})", headerName, includerName, inclusionDepth);

			//    std::string resolvedHeaderName =
			//        fileio::directory_get_absolute_path(mShaderdir, headerName);
			if (auto it = mIncludes.find(resolvedHeaderName); it != mIncludes.end())
			{
				// `headerName' was already present, so return that, and probably log about it
				return it->second;
			}

			//    if (!fileio::file_exists(mShaderdir, headerName))
			//    {
			//        log_error("#Included GLSL shader file \"{}\" does not exist!", resolvedHeaderName);
			//        return &smFailResult;
			//    }

			//    mSources[resolvedHeaderName] = {}; // insert an empty vector!
			//    fileio::File* file = fileio::file_open(
			//        mShaderdir, headerName, fileio::FileModeFlag::read);
			//    if (file == nullptr)
			//    {
			//        log_error("Failed to open #included GLSL shader file: {}", resolvedHeaderName);
			//        return &smFailResult;
			//    }

			//    if (!fileio::file_read_into_buffer(file, mSources[resolvedHeaderName]))
			//    {
			//        log_error("Failed to read #included GLSL shader file: {}", resolvedHeaderName);
			//        fileio::file_close(file);
			//        return &smFailResult;
			//    }

			IncludeResult* result = new IncludeResult(
				resolvedHeaderName, mSources[resolvedHeaderName].data(),
				mSources[resolvedHeaderName].size(), nullptr);

			auto [it, b] = mIncludes.emplace(std::make_pair(resolvedHeaderName, result));
			if (!b)
			{
				log_error("Failed to insert IncludeResult into std::map!");
				return &smFailResult;
			}
			return it->second;
		}
	}

	virtual void releaseInclude(IncludeResult*) override
	{
		
		//log_debug("releaseInclude(result->headerName: {})", result->headerName);
		if (auto it = mSources.find(result->headerName); it != mSources.end())
		{
			mSources.erase(it);
		}
		if (auto it = mIncludes.find(result->headerName); it != mIncludes.end())
		{
			// EDIT: I have forgotten to use "delete" here on the IncludeResult, but should probably be done!
			mIncludes.erase(it);
		}
		
	}

private:
	static inline const std::string sEmpty = "";
	static inline IncludeResult smFailResult =
		IncludeResult(sEmpty, "Header does not exist!", 0, nullptr);

	//    const fileio::Directory* mShaderdir {nullptr};
	std::map<std::string, IncludeResult*> mIncludes;
	std::map<std::string, std::vector<char>> mSources;
};
*/

ShaderCompiler::ShaderCompiler()
{
	ShInitialize();
}


std::unique_ptr<glslang::TShader> ShaderCompiler::_CompileLang(EShLanguage inLang, const fm::File& inFile, EShMessages messageFlags)
{
	EShLanguage lang = EShLanguage::EShLangFragment;
	std::unique_ptr<glslang::TShader> shader = std::make_unique<glslang::TShader>(inLang);

	std::string content = inFile.GetContent();
	const char* sources[1] = { content.c_str()};
	shader->setStrings(sources, 1);
	glslang::EShTargetClientVersion targetApiVersion = glslang::EShTargetVulkan_1_3;
	shader->setEnvClient(glslang::EShClientVulkan, targetApiVersion);

	glslang::EShTargetLanguageVersion spirvVersion = glslang::EShTargetSpv_1_3;
	shader->setEnvTarget(glslang::EshTargetSpv, spirvVersion);

	shader->setEntryPoint("main"); // We can specify a different entry point

	// The resource is an entire discussion in and by itself, here just use default.
	//TBuiltInResource* resources = GetDefaultResources();
	// int defaultVersion = 110, // use 100 for ES environment, overridden by #version in shader
	const TBuiltInResource* resources = GetDefaultResources();
	const int defaultVersion = 450;
	const bool forwardCompatible = false;
	EProfile defaultProfile = ENoProfile; // NOTE: Only for desktop, before profiles showed up!

	// NOTE: Here a custom file I/O library is used, your implementation may be different.
	//fileio::Directory* shaderdir = ...
	//	GlslShaderIncluder includer(shaderdir);
	glslang::TShader::ForbidIncluder includer;

	std::string preprocessedStr;
	if (!shader->preprocess(
		resources, defaultVersion, defaultProfile, false, forwardCompatible, messageFlags, &preprocessedStr, includer))
	{
		const std::string failure = shader->getInfoLog();
		DEBUG_ERROR(failure)
			//log_error("Failed to preprocess shader: {}", shader.getInfoLog());
			// FAIL
	}
	const char* preprocessedSources[1] = { preprocessedStr.c_str() };
	shader->setStrings(preprocessedSources, 1);

	if (!shader->parse(resources, defaultVersion, defaultProfile, false,
		forwardCompatible, messageFlags, includer))
	{
		const std::string failure = shader->getInfoLog();
		DEBUG_ERROR(failure)
		return nullptr;
		//vtek_log_error("Failed to parse shader: {}", shader.getInfoLog());
		// FAIL
	}
	return shader;
}

fm::ValuesType ConvertGLSLTypeToMaterial(const glslang::TType* inType)
{
	if (inType == nullptr)
		return fm::ValuesType::VALUE_NONE;

	if (inType->isTexture())
		return fm::ValuesType::VALUE_TEXTURE;
	else if(inType->isIntegerDomain())
		return fm::ValuesType::VALUE_INT;
	else if (inType->isFloatingDomain())
		return fm::ValuesType::VALUE_FLOAT;
	else if (inType->isMatrix())
		return fm::ValuesType::VALUE_MATRIX_FLOAT;
	else if (inType->isVector() && inType->getVectorSize() == 3)
		return fm::ValuesType::VALUE_VECTOR3_FLOAT;
	else if (inType->isVector() && inType->getVectorSize() == 4)
		return fm::ValuesType::VALUE_VECTOR4_FLOAT;
	else if (inType->isVector() && inType->getVectorSize() == 2)
		return fm::ValuesType::VALUE_VECTOR2_FLOAT;
}

bool ShaderCompiler::Compile(const fm::FilePath& inPath, Reflection& outReflection)
{
	fm::FilePath path(inPath);

	//_path.ToSub("SPIR-V");
	File frag(Folder(path), inPath.GetName(true) + ".frag");
	File vert(Folder(path), inPath.GetName(true) + ".vert");

	const EShMessages messageFlags = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
	EShLanguage lang = EShLanguage::EShLangFragment;
	std::unique_ptr<glslang::TShader> shaderFrag = _CompileLang(EShLanguage::EShLangFragment, frag, messageFlags);
	std::unique_ptr<glslang::TShader> shaderVert = _CompileLang(EShLanguage::EShLangVertex, vert, messageFlags);
	if (shaderFrag == nullptr || shaderVert == nullptr)
		return false;
	

	glslang::TProgram program;
	program.addShader(shaderFrag.get());
	program.addShader(shaderVert.get());


	if (!program.link(messageFlags))
	{
		std::string failed = program.getInfoLog();
		DEBUG_ERROR(failed)
		//vtek_log_error("Failed to link shader: {}", program.getInfoLog());
		// FAIL
	}

	program.buildReflection();
	//program.dumpReflection()
	for (int i = 0; i < program.getNumUniformVariables(); i++)
	{
		std::string name = program.getUniformName(i);
		Uniform uniform;
		uniform.name = name;
		uniform.binding = program.getUniform(i).getBinding();
		if (uniform.binding == -1)
			continue;
		uniform.set = program.getUniform(i).getType()->getQualifier().layoutSet;
		uniform.type = ConvertGLSLTypeToMaterial(program.getUniform(i).getType());
		outReflection.uniforms.emplace(name, uniform);
	}

	for (int i = 0; i < program.getNumUniformBlocks(); i++)
	{
		std::string name = program.getUniformBlockName(i);
		Uniform uniform;
		uniform.name = name;
		uniform.binding = program.getUniformBlock(i).getBinding();
		if (uniform.binding == -1)
			continue;
		uniform.set = program.getUniformBlock(i).getType()->getQualifier().layoutSet;

		outReflection.uniforms.emplace(name, uniform);
	}

	// Convert the intermediate generated by glslang to Spir-V
	glslang::TIntermediate* intermediateRef = (program.getIntermediate(lang));
	std::vector<uint32_t> spirv;
	glslang::SpvOptions options{};
	options.validate = true;
	// TODO: We can also provide a logger to glslang.
	// glslang::spv::SpvBuildLogger logger;
	// glslang::GlslangToSpv(intermediateRef, spirv, &logger, &options);
	glslang::GlslangToSpv(*intermediateRef, spirv, &options);

	return true;
}
