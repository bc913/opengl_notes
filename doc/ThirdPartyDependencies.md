# Third Party Dependencies
There are several external dependencies required to be consumed during build and/or runtime. When it comes to OpenGL projects it gets a bit nasty so this section is dedicated for explaining how to consume commonly used third party libraries/frameworks/APIs in this repo.

## Ways to consume third party dependencies
There is no single and universal way (unfortunately) of consuming third-party dependencies especially for C/C++ projects. For this specific repo, I seek cross-platform nature so my preferred ways are selected in that direction and my explanations will be built upon this argument. However, I will try to explain some other ways too.

Criterias:
- Integration with the build tools
- Accessing & consuming them in a cross-platform manner.
- Less OS dependent tweaks during build, devops and maintenance.

1. Packages & Package manager(s): There is no single(or multiple) bulletproof package manager that can handle cross-platform package consumption. `conan` and `vcpkg` has achieved a huge progress since their debut but still not fully integrable to an existing repo.

As a developer, you have to add OS dependent commands to your build tools/pipelines.

> I'm not an expert yet on those package managers. I have these arguments based on my professional experience. As I learn and experience more about them, I will share my opinions here.

2. `git submodule`: This is highly convenient for lightweight open source dependencies. With single command, one can easily integrate them with an existing consumer repo. This also eliminate possible compiler/linker incompatibilities when consuming them as static or dynamic libraries. Better object code will be generated. 

This way of consuming packages works well with CMake.

The burden it brings is that the source files are now part of your project tree.

3. Manual integration: This is not usually the preferred way but sometimes this is the only solution. If the corresponding tp dependecy has very less number of files to compile or is header-only or not maintained actively anymore, this is the way to go. 

## Common graphics related third party dependencies
### [GLFW](https://github.com/glfw/glfw):
As referenced in their documentation:
> GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan application development.  It provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.

GLFW is consumed through `git submodule` in this repo. It can be consumed by package managers or manual integration.

### [GLAD](https://github.com/Dav1dde/glad): 
GLAD is a GL loader/generator. It is consumed through `git submodule`. 

I've preffered this over `GLEW` because it is actively maintained. Also, GLEW's build in MacOs is really cumbersome.

It can also be consumed through package managers and manual integration. Check this [issue](https://github.com/Dav1dde/glad/issues/186) if you want to consume through manual integration and use CMake as the build tool.

```bash
cmake_minimum_required(VERSION 3.0)
project(Glad)

add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)

# Then in your main CMakeLists.txt file put:

add_subdirectory(glad/)
target_link_libraries(<your target> PRIVATE glad)
```

### [GLEW](http://glew.sourceforge.net/):
It is a GL Loader/Generator such as GLAD. It is not used in this repo.

### [GLM](https://github.com/g-truc/glm):
GLM is an header-only math library tailored for graphics applications. It is consumed through `git submodule` in this repo. Consuming it by package managers or manual integration is still possible.