from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class PixelVectors(ConanFile):
    name = "pixel_vectors"
    version = "1.0.0"
    license = "MIT"
    author = "mmacz"
    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("cxxopts/3.3.1")
        self.requires("opencv/4.11.0")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.20 <4.0]")
        self.tool_requires("ninja/[>=1.10 <2.0]")

    def generate(self):
        tc = CMakeToolchain(self, generator="Ninja")
        tc.variables["CMAKE_CXX_STANDARD"] = 17
        tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = True
        tc.variables["CMAKE_CXX_EXTENSIONS"] = False
        tc.variables["EXPORT_COMPILE_COMMANDS"] = True
        tc.generate()

        ts = CMakeDeps(self)
        ts.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

