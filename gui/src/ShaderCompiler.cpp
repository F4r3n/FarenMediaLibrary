#include "ShaderCompiler.h"
#include "glslang/Public/ResourceLimits.h"
#include "glslang/SPIRV/SpvTools.h"
#include "glslang/SPIRV/GlslangToSpv.h"
#include "Core/FilePath.h"
#include "Core/Debug.h"
#include <fstream>
#include <unordered_map>
using namespace fm;
using namespace gui;
class DirStackFileIncluder : public glslang::TShader::Includer {
public:
	DirStackFileIncluder() : externalLocalDirectoryCount(0) { }

	virtual IncludeResult* includeLocal(const char* headerName,
		const char* includerName,
		size_t inclusionDepth) override
	{
		return readLocalPath(headerName, includerName, (int)inclusionDepth);
	}

	virtual IncludeResult* includeSystem(const char* headerName,
		const char* /*includerName*/,
		size_t /*inclusionDepth*/) override
	{
		return readSystemPath(headerName);
	}

	// Externally set directories. E.g., from a command-line -I<dir>.
	//  - Most-recently pushed are checked first.
	//  - All these are checked after the parse-time stack of local directories
	//    is checked.
	//  - This only applies to the "local" form of #include.
	//  - Makes its own copy of the path.
	virtual void pushExternalLocalDirectory(const std::string& dir)
	{
		directoryStack.push_back(dir);
		externalLocalDirectoryCount = (int)directoryStack.size();
	}

	virtual void releaseInclude(IncludeResult* result) override
	{
		if (result != nullptr) {
			delete[] static_cast<tUserDataElement*>(result->userData);
			delete result;
		}
	}

	virtual std::set<std::string> getIncludedFiles()
	{
		return includedFiles;
	}

	virtual ~DirStackFileIncluder() override { }

protected:
	typedef char tUserDataElement;
	std::vector<std::string> directoryStack;
	int externalLocalDirectoryCount;
	std::set<std::string> includedFiles;

	// Search for a valid "local" path based on combining the stack of include
	// directories and the nominal name of the header.
	virtual IncludeResult* readLocalPath(const char* headerName, const char* includerName, int depth)
	{
		// Discard popped include directories, and
		// initialize when at parse-time first level.
		directoryStack.resize(depth + externalLocalDirectoryCount);
		if (depth == 1)
			directoryStack.back() = getDirectory(includerName);

		// Find a directory that works, using a reverse search of the include stack.
		for (auto it = directoryStack.rbegin(); it != directoryStack.rend(); ++it) {
			std::string path = *it + '/' + headerName;
			std::replace(path.begin(), path.end(), '\\', '/');
			std::ifstream file(path, std::ios_base::binary | std::ios_base::ate);
			if (file) {
				directoryStack.push_back(getDirectory(path));
				includedFiles.insert(path);
				return newIncludeResult(path, file, (int)file.tellg());
			}
		}

		return nullptr;
	}

	// Search for a valid <system> path.
	// Not implemented yet; returning nullptr signals failure to find.
	virtual IncludeResult* readSystemPath(const char* /*headerName*/) const
	{
		return nullptr;
	}

	// Do actual reading of the file, filling in a new include result.
	virtual IncludeResult* newIncludeResult(const std::string& path, std::ifstream& file, int length) const
	{
		char* content = new tUserDataElement[length];
		file.seekg(0, file.beg);
		file.read(content, length);
		return new IncludeResult(path, content, length, content);
	}

	// If no path markers, return current working directory.
	// Otherwise, strip file name and return path leading up to it.
	virtual std::string getDirectory(const std::string path) const
	{
		size_t last = path.find_last_of("/\\");
		return last == std::string::npos ? "." : path.substr(0, last);
	}
};

void ShaderCompiler::ShaderMacro::DefineMacro(const std::string& inMacro, uint32_t inValue)
{
	_macros[inMacro] = inValue;
}

void ShaderCompiler::ShaderMacro::UndefMacro(const std::string& inMacro)
{
	_undefMacros.emplace(inMacro);
}

void ShaderCompiler::ShaderMacro::RemoveMacro(const std::string& inMacro)
{
	_macros.erase(inMacro);
	_undefMacros.erase(inMacro);
}

std::vector<std::string> ShaderCompiler::ShaderMacro::ToPreprocess() const
{
	std::vector<std::string> preprocess;
	preprocess.reserve(_macros.size());
	for (const auto& [macro, value] : _macros)
	{
		preprocess.emplace_back("define-macro " + macro);
	}

	for (const auto& macro : _undefMacros)
	{
		preprocess.emplace_back("undef-macro " + macro);
	}
	return preprocess;
}

std::string ShaderCompiler::ShaderMacro::ToPreamble() const
{
	std::string preamble;
	for (const auto& [macro, value] : _macros)
	{
		preamble += "#define " + macro + " " + std::to_string(value) + "\n";
	}

	for (const auto& macro : _undefMacros)
	{
		preamble += "#undef " + macro + "\n";
	}
	return preamble;
}

ShaderCompiler::ShaderCompiler()
{
	ShInitialize();
}


std::unique_ptr<glslang::TShader> ShaderCompiler::_CompileLang(const ShaderCompilerSettings& inSettings, EShLanguage inLang, const fm::File& inFile, EShMessages messageFlags)
{
	std::unique_ptr<glslang::TShader> shader = std::make_unique<glslang::TShader>(inLang);

	std::string content = inFile.GetContent();
	const char* sources[1] = { content.c_str()};
	shader->setStrings(sources, 1);

	//preambleString += "#extension GL_GOOGLE_include_directive : require\n";
	if (inSettings.api == GRAPHIC_API::OPENGL)
	{
		glslang::EShTargetClientVersion targetApiVersion = glslang::EShTargetOpenGL_450;
		glslang::EShClient client = glslang::EShClientOpenGL;
		shader->setEnvClient(client, targetApiVersion);
		shader->setEnvInput(glslang::EShSourceGlsl, inLang, glslang::EShClientOpenGL, 420);
		shader->setEnvTarget(glslang::EshTargetSpv, (glslang::EShTargetLanguageVersion)0);
		shader->setAutoMapLocations(true);
	}
	else if (inSettings.api == GRAPHIC_API::VULKAN)
	{
		glslang::EShTargetClientVersion targetApiVersion = glslang::EShTargetVulkan_1_3;
		glslang::EShClient client = glslang::EShClientVulkan;
		glslang::EShTargetLanguageVersion spirvVersion = glslang::EShTargetSpv_1_3;
		shader->setEnvTarget(glslang::EshTargetSpv, spirvVersion);
		shader->setEnvClient(client, targetApiVersion);
	}

	std::string preamble = inSettings.macros.ToPreamble();
	shader->setPreamble(preamble.c_str());
	shader->addProcesses(inSettings.macros.ToPreprocess());
	shader->setEntryPoint("main"); // We can specify a different entry point


	const TBuiltInResource* resources = GetDefaultResources();
	const int defaultVersion = 460;
	const bool forwardCompatible = false;
	EProfile defaultProfile = ECoreProfile;


	DirStackFileIncluder includer;
	std::for_each(inSettings.listFoldersToInclude.begin(), inSettings.listFoldersToInclude.end(), [&includer](const fm::Folder& inFolder) {
		includer.pushExternalLocalDirectory(inFolder.GetPathString());
	});

	std::string preprocessedStr;
	if (!shader->preprocess(
		resources, defaultVersion, defaultProfile, true, forwardCompatible, messageFlags, &preprocessedStr, includer))
	{
		const std::string failure = shader->getInfoLog();
		assert(false);
		DEBUG_ERROR(failure)
	}

	if (inSettings.generatePreprocess)
	{
		std::string outputName = "vert.vert";
		if (EShLanguage::EShLangFragment == inLang)
		{
			outputName = "frag.frag";
		}
		fm::FilePath preprocessedFolder = fm::FilePath(inSettings.GetDestinationPath()).ToSub("Preprocessed");
		if (!fm::Folder(preprocessedFolder).Exist())
		{
			fm::Folder(preprocessedFolder).CreateFolder();
		}
		fm::File finalFile = fm::File(fm::FilePath(preprocessedFolder).ToSub(outputName));
		if (finalFile.Exist())
			finalFile.Delete();

		finalFile.SetContent(preprocessedStr);
	}

	const char* preprocessedSources[1] = { preprocessedStr.c_str() };
	shader->setStrings(preprocessedSources, 1);

	if (!shader->parse(resources, defaultVersion, defaultProfile, false,
		forwardCompatible, messageFlags, includer))
	{
		const std::string failure = shader->getInfoLog();
		assert(false);

		DEBUG_ERROR(failure)
		return nullptr;
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

	return fm::ValuesType::VALUE_NONE;
}

void ShaderCompiler::_BuildReflection(glslang::TProgram& program, fm::SubShader::Reflection& outReflection)
{
	program.buildReflection();
	//program.dumpReflection()
	for (int i = 0; i < program.getNumUniformVariables(); i++)
	{
		std::string name = program.getUniformName(i);
		fm::SubShader::Uniform uniform;
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
		fm::SubShader::Uniform uniform;
		uniform.name = name;
		uniform.binding = program.getUniformBlock(i).getBinding();
		if (uniform.binding == -1)
			continue;
		uniform.set = program.getUniformBlock(i).getType()->getQualifier().layoutSet;

		outReflection.uniforms.emplace(name, uniform);
	}
}




bool ShaderCompiler::Compile(const ShaderCompilerSettings& inSettings, fm::SubShader::Reflection& outReflection)
{
	fm::FilePath path(inSettings.shaderFolder.GetPath());

	//_path.ToSub("SPIR-V");
	File frag(inSettings.shaderFolder, inSettings.shaderFolder.GetPath().GetName(true) + ".frag");
	File vert(inSettings.shaderFolder, inSettings.shaderFolder.GetPath().GetName(true) + ".vert");

	EShMessages messageFlags = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
	if (inSettings.api == GRAPHIC_API::OPENGL)
	{
		messageFlags = (EShMessages)(EShMsgSpvRules);
	}

	std::unique_ptr<glslang::TShader> shaderFrag = _CompileLang(inSettings, EShLanguage::EShLangFragment, frag, messageFlags);
	std::unique_ptr<glslang::TShader> shaderVert = _CompileLang(inSettings, EShLanguage::EShLangVertex, vert, messageFlags);
	if (shaderFrag == nullptr || shaderVert == nullptr)
		return false;
	

	glslang::TProgram program;
	program.addShader(shaderFrag.get());
	program.addShader(shaderVert.get());


	if (!program.link(messageFlags))
	{
		std::string failed = program.getInfoLog();
		DEBUG_ERROR(failed)
	}
	if (inSettings.generateReflection)
	{
		_BuildReflection(program, outReflection);
	}


	if (inSettings.generateSPV)
	{
		fm::FilePath spirvPath = inSettings.GetDestinationPath();
		spirvPath.ToSub("SPIR-V");
		fm::Folder spirvFolder(spirvPath);
		if (!spirvFolder.Exist())
		{
			spirvFolder.CreateFolder();
		}
		else
		{
			//spirvFolder.Delete(true);
		}

		_WriteSPV(program.getIntermediate(EShLanguage::EShLangFragment), fm::FilePath(spirvFolder.GetPath()).ToSub("frag.spv"));
		_WriteSPV(program.getIntermediate(EShLanguage::EShLangVertex), fm::FilePath(spirvFolder.GetPath()).ToSub("vert.spv"));
	}


	return true;
}


void ShaderCompiler::_WriteSPV(glslang::TIntermediate* intermediateRef, const fm::FilePath& inDestination)
{
	std::vector<uint32_t> spirv;
	glslang::SpvOptions options{};
	options.validate = true;
	// TODO: We can also provide a logger to glslang.
	// glslang::spv::SpvBuildLogger logger;
	// glslang::GlslangToSpv(intermediateRef, spirv, &logger, &options);
	glslang::GlslangToSpv(*intermediateRef, spirv, &options);
	if (fm::File(inDestination).Exist())
	{
		fm::File(inDestination).Delete();
	}
	fm::File(inDestination).SetContent(spirv);
}
