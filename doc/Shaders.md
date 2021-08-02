# Shaders
During the times of the legacy OpenGL, some stages of the rendering pipeline was configurable. However, these configurations were available through OpenGL functions and this concept is named as `Fixed Function Pipeline`. This also means that this feature is available on the client side (CPU) and requires communication from client to server (GPU) side each time the user needs a custom configuration. This is slow and also not so configurable either.

Shaders are designed to be replace `Fixed Function Pipeline` with the intent of the following features:
- Handle things in the video graphics (GPU) (server) side.
- It is highly customizable and programmable.
- Since, processing happens on the GPU side, it is fast during rendering.

In other words, the programmable stages of the rendering pipeline are called `Shaders`. A shader can run multiple taks in parallel. 

Each stage/shader in the rendering pipeline performs after the previous one is completed and each stage/shader 
feeds the next stage/shader by producing an output. In other words, each shader is fed (when required) from the previous shader except `vertex shader`. (Check vertex shader doc.)

Shaders are isolated programs and they are not allowed to communicate directly with each other except specifying inputs/outputs.

OpenGL shader are written in `OpenGL Shader Language (GLSL)`.

## Terminology
- **Shader**: Each mini-programme
- **Program**: Linked collection of each shader. A complete shader program to store all shaders.

## How it works?
Shaders are programmed to control specific part of the rendering pipeline and as mentioned before they are feeding each other in an order through inputs/outputs. 

Each shader is compiled and the whole set of shaders is linked together to form the executable, ultimate shader program which is called `program`. Yes, naming convention is horrible.

Since the shaders are `GPU` mechanism, GLSL should be compiled, linked and run on the GPU side. Therefore, the GLSL code should be passed to GPU as `string (const char *)` using OpenGL commands.

Steps:
1. Read the shader (mini-programme) from file or as string and assign it to char[] (const char *).
2. Create the shader using `glCreateShader()`.
3. Define the source and compile shader using `glShaderSource()` and `glCompileShader()`.
4. Check for compile errors using `glGetShaderiv()`.
5. Repeat steps through 1-4 for other shaders defined if required.
6. Since all the shaders are required to be packed together under `program`, create it using `glCreateProgram()`.

7. Attach & link shaders to the program using `glAttachShader()` and `glLinkProgram()`.
8. Check against linking errors using `glGetProgramiv()`.
9. Use the shader program through `glUseProgram()`.
10. **DO NOT** forget to delete the created shaders using `glDeleteShader()` when you're done.

> Steps 9 & 10 can be replaced with each other. Order might not matter.

### Shader Parallelism
Shaders run on the GPU and they are highly parallelised. Each shader can run multiple process at the sime time thanks to GPU.
Since, each shader feeds the next one, this should be still done sequentially.

In other words, shaders are highly parallelized within themselves but not multiple-different shader can run in parallel.

For instance, each vertex shader can only handle one vertex and 200 vertex shaders are required to transform 200 vertices. They all can run in parallel but the next step after the vertex shader(s) SHOULD wait until the vertex shaders are done.

## The flow in the pipeline
- Vertex Specification
- Vertex Shader
- Tessellation
- Geometry Shader
- Vertex Post-Processing
- Primitive Assembly
- Rasterization
- Fragment Shader
- Per-Sample Operations

Modern OpenGL at least requires to customize(program) the vertex and the fragment shaders.

## OpenGL Shader Language (GLSL)
Each shader throughout the rendering pipeline resembles a small C programme and it has specific syntax called `GLSL`.

### Data types
GLSL has several data types. Some of them are common with C language while some others are unique.

- `void`, `bool`, `int`, `float`.
- `vec3`, `vec4` (use .x, .y, .z, .w to access components) (Vectors allows to do swizzling.)
```cpp
//swizzling
vec2 someVec;
vec4 otherVec = someVec.xyxx;
vec4 anotherVec = otherVec.yxzy + someVec.xxxx;

//
vec2 vect = vec2(0.5, 0.3);
vec4 other = vec4(vect, 0.33, 0.21);
vec4 anotherVec = vec4(other.xyz, 1.0);
```
- `mat3` (3x3 matrix), `mat4` (4x4 matrix)
- `sampler2D`, `samplerCube`, `sampler2DShadow`

Check out the documentation for the complete list of the data types.

### File formats for shaders
Each shader programme can be provided to the corresponding OpenGL functions using `const char *` directly. However, this is not so practical and error prone.

The modern and common practice is to define a seperate file for the shader programmes using the following formats.
- `.glsl`
- `.vert` (vertex shader)
- `.frag` (fragment shader)
- `.geom` (geometry shader)

### Syntax
```cpp
/*Define the version first.*/
#version <version_number>
/*Declare Input variables*/
in <type> <input_variable_name>
/*Declare output variables*/
out <type> <output_variable_name>

/*Declare global variables (if required)*/
uniform <type> <uniform_variable_name>
/*Layout (if required)*/
layout(location=0) in vec<n> <layout_name>

/*Define the main func to be run on GPU*/
void main()
{

    /*Process data*/
    //....

    /*Update output variables*/
    output_variable_name = doSomething();
}
```
`Input(in)` and `Output(out)` variables can be considered as interface between the shaders while `global(uniform)` variables can be used as an interface between the `CPU` (client) and `GPU` (server)
side.

Each shader might have built-in `Input(in)` and `Output(out)` variables defined.

### Input variables (`in`)
It is used to specify vertex attribute which comes from the `VBO`. It means that each input variable is also known as `vertex attribute`. 

There is maximum number for vertex attribute and that number depends on the hardware.
```cpp
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, GLint* max_limit);
```

### Output variables (`out`)
This is to specify output variable of the shader. Since it passed to the next shader, its name should match with at least one of the `input(in)` variables of the feeded shader.

> If `layout(location)` tag is used then this name requirement might not be enforced. **??**

Output variables have no effect on the shader they are declared and defined.

### `Uniform`
`Uniform`s are the global variables to pass data from CPU to the shaders on `GPU`.

Features:
- Unique per shader program object so can be accessed from any shader at any stage in that shader program.
- Since it is a CPU (client) global, it can be seen or updated by any other shader. **???**
- Declared in `.glsl` files.
```cpp
uniform vec4 our_color;
```
- It can be queried or set/update from CPU (client) side.
```cpp
// Query
// Since this is a query, no need to call glUseProgram in advance.
GLuint ourColorLoc = glGetUniformLocation(<shader_programme_name>, "our_color");
// Can make the call to use the program
glUseProgram(<shader_programme_name>);

// Set/Update
// Requirement: Make sure glUseProgram() is called before this for the corresponding program
glUniform4f(ourColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);

// Make sure the glUniform*() syntax and its arguments are matching with the decleration of the uniform variable.

// vec4 -> glUniform4f()
```
> Calling any `glUniform*()` during run-time is very expensive. Try to avoid.

- If declared uniform is not used anywhere, OpenGL will silently remove it.

> Uniforms can be considered as an interface between the CPU(client) and GPU(server). `in`s and `out`s can be considered as an interface between shaders/stages. 

### How to handle vertex attribute locations (indexes)?
The shaders and so the rendering pipeline should know where the vertex attributes sit at in terms of index. `VAO`s already have some layout defined for their contained `vertex attributes` through `VBO`s. However, the shaders do NOT have a clear idea where those vertex attributes sit at.

We have to let the shader know about the correct information about the vertex attributes' location. Three ways to do it:
1. Use `layout(location)` syntax in .glsl file.
2. Let the `shader's linker` know where those locations sit:

```cpp
// ...
// Make sure the shader is compiled --> glCompileShader() but not linked yet

glBindAttribLocation(<shader_programme_name>, 0, "vertex_position");
glBindAttribLocation(<shader_programme_name>, 1, "vertex_color");
// Now you can link
glLinkProgram(<shader_programme_name>);
```
> Use case for this: If you want your GLSL code to be compatible with OpenGL 3.2

3. If you don't select either 1 or 2 then the GPU linker will decide what to happen and you can query the location for a vertex attribute as follows:
```cpp
glVertexAttribPointer(glGetAttribLocation(), ...);
```

### Layout
`Layout` syntax is a tag to identify the index(es) of the input(`in`) and ouput(`out`) attributes `AND` they have to **match** with the `index` argument in `glVertexAttribPointer()`.

The very typical usage for this identifying the indexes of the vertex attributes in shader. Check Vertex Shader for details.

They have a size. They can be used in conjuction with custom GLSL data structures. ??? The following setup is quite possible:
```cpp
struct OutData { vec1 data1; dvec4 data2; float val[3]; };

layout(location=0) out vec3 some_vals[4];
// Should start from 4 since some_vals occupies the first 4 index
layout(location=4) out OutData myOut;
// myOut consumes 6 indexes
layout(location=10) out vec2 tex_Coord;
```

## Vertex Shaders
Fatures:
- Transforms the vertex positions into the clip space.
- Can send data from the vertex buffer (VBO) to the fragment shaders.
- Each vertex shader can handle one vertex data at a time and multiple vertex shaders can run in parallel.

Since the vertex shaders are the first shader in the pipeline, there is no way for them to be fed by the previous shader. There is no previous shader for them.

Vertex shaders are fed with the `vertex attribute` which specifies the layout of the `Vertex Array Object (VAO)`. It basically receives a single vertex from the vertex stream and generates one to the output stream.

### Syntax
Assume you have tightly-packed multiple VBOs or a loosely-packed VBO for different features of the vertex. Somehow you need to tell the shader the correct indexes of each future. This is achieved with `layout` syntax.
```cpp
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

out vec3 colour;

void main() {
  colour = vertex_colour;
  gl_Position = vec4(vertex_position, 1.0);
}
```

### GLSL defaults
- Input variables:
```cpp
in int gl_Vertex;
in int gl_InstanceID;
in int gl_DrawID;
in int gl_BaseVertex;
in int gl_BaseInstance;
```

- Output variables:
```cpp
out gl_PerVertex
{
    vec4 gl_Position; // position of the current vertex
    float gl_PointSize;// size of rasterized points
    float gl_ClipDistance[];
};
```

## Fragments vs. Pixels
**Pixel**: The pixels are the elements that make up the final 2D image that is drawn inside a window display. 

**Fragment**: It can be defined in several ways:
- It is the data necessary to generate a single pixel's worth of a drawing primitive in the frame buffer. 
- All data required for OpenGL to render a single pixel.
- It is a pixel-sized area of surface.

Sometimes surfaces are overlap and there is more than one fragment for one pixel in that case.

> All of the fragments are drawn even the hidden ones.

## Fragment Shaders
Once all the vertex shaders have computed the position of every vertex in clip space, then the `Fragment shader` run for every fragment (pixel-sized space) between vertices and **determines the color of each fragment**.

> Interesting fact: The fragment shader is optional.

Each fragment shader is responsible for one fragment to color up. For this reason, **it requires a `vec4` output variable to be defined as first and populated**.

> The hardware pipeline already knows that the first `vec4` output from the fragment shader should be the `color of the fragment`.

### Data
Fragment shader's data source is `Rasterization` stage which is the previous stage. 

> Rasterization is not a shader type.

The fragment shader's data is basically a sample-sized segment of a rasterized primitive. Data might include but not limited to:
- raster position
- depth
- *interpolated* attributes from vertex processing stage (colors, textures, coordinates etc)
- stencil
- alpha
- window ID
- also the data required to test whether the fragment survives to become a pixel.

There will be at least one fragment produced for every pixel area covered by the primitive being tested.

### Color data format
```cpp
vec4(r, g, b, alpa)
```

### How does it work?
As a general rule, shaders can work in parallel and each fragment shader is responsible from one fragment to process. If there is n fragments to process, n fragment shader will work in parallel.

1. After a primitive is rasterized, each sample of pixels is covered by a primitive called `fragment`.
2. Now, the data is populated for the fragment to be processed.
3. Each fragment is written into the framebuffer image that will be displayed as the final pixels. Check `Difference Between Fragments and Pixels` part of this [resource](https://antongerdelan.net/opengl/shaders.html).
4. Pass the result to the Blending stage.

> Each fragment gets an interpolated feature (color, texture coords, normals, etc.) based on it's location. Assume you have a triangle with three vertices and three colors but there multiple fragments that fills between these vertices so the values for each fragment comes as interpolated.

> This means that vertex shaders do NOT send constant value data. They send interpolated data.

### GLSL defaults
- Input variables:
```cpp
in vec4 gl_FragCoord;
in bool gl_FrontFacing;
in vec2 gl_PointCoord;
// For OpenGL 4.0 or later
in int gl_SampleID;
in vec2 gl_SamplePosition;
in int gl_SampleMasking[];
```

- Output variables:
```cpp
out float gl_FragDepth;
out int gl_SampleMask[];
```

- User-defined outputs: They usually represent series of colors and directed into the specific buffers based on `glDrawBuffers()` state. Should be only allowed GLSL types.

### Misc
There are three ways to associate an output variable with a color number:
1. In-shader specification using `vec4` output var and `layout(location) syntax`. (The latter is optional TODO: ???)
2. Pre-link specification
3. Automatic assignment





