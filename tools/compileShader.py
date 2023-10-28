import subprocess
import os
from sys import argv
import platform
import pathlib
"""
def getExe() : str
    if platform == "linux" or platform == "linux2":
    return 
    elif platform == "darwin":
    # OS X
    elif platform == "win32":
"""
#$ python3 compileShader.py  "../out/build/x64-Debug/_deps/glslang-build/StandAlone/" "../_Resources_/shaders/"
if __name__ == '__main__':
    exePath = argv[1]
    if platform.system() == "Windows":
        exePath += "glslang.exe"
    else:
        exePath += "glslang"
    shadersFolder = argv[2]
    for folder in (os.listdir(shadersFolder)):
        path = pathlib.Path(shadersFolder + folder)
        if path.suffix == ".vkshader":
            target = "vulkan1.3"
            shaderVert = path.joinpath(path.stem + ".vert")
            output = path.joinpath("SPIR-V")
            if output.exists() == False:
                os.mkdir(output)

            subprocess.run([exePath, shaderVert, "--target-env", target, "-e", "main", "-o", output.joinpath("vert.spv")])

            shaderFrag = path.joinpath(path.stem + ".frag")
            subprocess.run([exePath, shaderFrag, "--target-env", target, "-e", "main", "-o", output.joinpath("frag.spv")])



    