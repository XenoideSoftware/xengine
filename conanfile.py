from conan import ConanFile
from conan.tools.files import copy

import os


class xeRecipe(ConanFile):
    name = "xe"
    version = "0.1.0"  # version file (source of truth); tags are v0.x.y markers
    python_requires = "xenoide-base/0.1.0"
    python_requires_extend = "xenoide-base.XenoideCMake"
    package_type = "application"
    build_info_prefix = "XE"

    # Optional metadata
    license = "MIT"
    author = "Felipe Apablaza <felipe.apablaza@protonmail.com>"
    url = "https://github.com/XenoideSoftware/xe-engine"
    description = "Game engine for C++ development"
    topics = ("game-engine", "graphics", "rendering", "vulkan", "opengl", "cross-platform")

    options = {
        "with_demos": [True, False],
        "with_engine_renderer_gl": [True, False],
        "with_engine_renderer_vk": [True, False],
        "with_engine_tool_ktxc": [True, False],
        "with_engine_tool_gltfc": [True, False],
        "with_engine_tool_gltfv": [True, False],
    }

    default_options = {
        "with_demos": False,
        "with_engine_renderer_gl": True,
        "with_engine_renderer_vk": True,
        "with_engine_tool_ktxc": True,
        "with_engine_tool_gltfc": True,
        "with_engine_tool_gltfv": False,
    }

    # Sources are located in the same place as this recipe.
    exports_sources = "CMakeLists.txt", "src/*"

    def _require_cli(self):
        # cxxopts is only needed by the command-line tools (ktxc/gltfc).
        return (self.options.get_safe("with_engine_tool_ktxc")
                or self.options.get_safe("with_engine_tool_gltfc"))

    def requirements(self):
        if self._require_cli():
            self.requires("cxxopts/3.3.1")

        self.requires("fmt/[>=11 <12]")
        self.requires("ms-gsl/4.2.0")
        self.requires("backport-cpp/1.2.0")
        self.requires("tl-expected/1.2.0")
        self.requires("gsl-lite/1.1.0")
        self.requires("lodepng/cci.20230410")

        if self.options.get_safe("with_engine_renderer_vk"):
            self.requires("vulkan-loader/1.4.313.0")

        if self.options.get_safe("with_engine_renderer_gl"):
            self.requires("glfw/3.4")
            self.requires("glazed/0.1.0",
                          options={"language": "both",
                                   "apis": "gl:4.6,gles2:3.2,gl_compat:2.1"})

        if self.options.get_safe("with_engine_tool_ktxc"):
            self.requires("ktx/4.4.2")
            self.requires("devil/1.8.0")

        if self.options.get_safe("with_engine_tool_gltfc"):
            self.requires("assimp/6.0.2")
            self.requires("cgltf/1.13")

        if self.options.get_safe("with_engine_tool_gltfv"):
            self.requires("imgui/1.92.2b")
            self.requires("sdl/2.32.10")

    def _enumerate_imgui_backends(self):
        if "imgui" not in self.dependencies:
            return []

        backends = []
        if "glazed" in self.dependencies:
            backends.append("opengl3")
        if "glfw" in self.dependencies:
            backends.append("glfw")
        if "sdl" in self.dependencies:
            backends.append("sdl2")
        return backends

    def xenoide_toolchain(self, tc):
        tc.variables["XE_ENABLE_DEMOS"] = "ON" if self.options.get_safe("with_demos") else "OFF"
        tc.variables["XE_ENABLE_ENGINE_TOOL_KTXC"] = "ON" if self.options.get_safe("with_engine_tool_ktxc") else "OFF"
        tc.variables["XE_ENABLE_ENGINE_TOOL_GLTFC"] = "ON" if self.options.get_safe("with_engine_tool_gltfc") else "OFF"
        tc.variables["XE_ENABLE_ENGINE_TOOL_GLTFV"] = "ON" if self.options.get_safe("with_engine_tool_gltfv") else "OFF"

        imgui_backends = self._enumerate_imgui_backends()
        if not imgui_backends:
            return

        # Never write generated sources into the checkout: they live under the
        # build tree and are reachable through an include directory.
        package_folder = self.dependencies["imgui"].package_folder
        bindings_src = os.path.join(package_folder, "res", "bindings")
        generated_root = os.path.join(self.build_folder, "generated", "imgui")
        generated_include = os.path.join(generated_root, "xe", "imgui")
        os.makedirs(generated_include, exist_ok=True)

        for backend in imgui_backends:
            copy(self, f"imgui_impl_{backend}*", bindings_src, generated_include)

        tc.variables["XE_ENGINE_IMGUI_BACKENDS"] = ";".join(imgui_backends)
        tc.variables["XE_ENGINE_IMGUI_INCLUDE_DIR"] = generated_root.replace("\\", "/")
