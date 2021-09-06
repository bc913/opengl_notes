# Advanced Opengl
- [Stencil Testing](#Stencil-Testing)
- [Depth Testing](#Depth-Testing)
- [Transparancy](#Transparency)
- [Blending](#Blending)
- [Face Culling](#Face-Culling)
- [Geometry Shader](#Geometry-Shader)
- [Instancing](#Instancing)
- [Anti Aliasing](#Anti-Aliasing)

## Stencil Testing
(If enabled) the fragments which pass the stencil test are passed to the next stage to be rendered. The failed ones are discarded. `Stencil Testing` is used to achieve interesting effects.

### Where is the data stored at?
The stencil values are stored in `stencil buffers`. A stencil buffer (usually) contains 8 bits per stencil value that amounts to a total of 256 different stencil values per pixel. We can set these stencil values to values of our liking and we can discard or keep fragments whenever a particular fragment has a certain stencil value. 


### How it works?
The stencil buffer is first cleared with zeros and then an open rectangle of 1s is stored in the stencil buffer. The fragments of the scene are then only rendered (the others are discarded) wherever the stencil value of that fragment contains a 1.

Stencil buffer operations allow us to set the stencil buffer at specific values wherever we're rendering fragments. By changing the content of the stencil buffer while we're rendering, we're writing to the stencil buffer. In the same (or following) frame(s) we can read these values to discard or pass certain fragments. When using stencil buffers you can get as crazy as you like, but the general outline is usually as follows:

    Enable writing to the stencil buffer.
    Render objects, updating the content of the stencil buffer.
    Disable writing to the stencil buffer.
    Render (other) objects, this time discarding certain fragments based on the content of the stencil buffer.

By using the stencil buffer we can thus discard certain fragments based on the fragments of other drawn objects in the scene

### How to enable?
It is disabled by default. To enable, use the following OpenGL command.

```cpp
glEnable(GL_STENCIL_TEST);
```

> You need to clear the stencil buffer after each iteration such as color and depth buffer.
```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
```

### When it works?
Stencil testing after the fragment shader and before the depth testing. It means that failed fragments in stencil testing (if enabled) are discarded and can not be part of the depth testing.

### Stencil Functions
There are a total of two functions we can use to configure stencil testing: `glStencilFunc` and `glStencilOp`. 
- **`glStencilFunc(GLenum func, GLint ref, GLuint mask)`**: You provide a test function(`func`) to test against a reference (`ref`) value. `mask` specifies a mask that is ANDed with both the reference value and the stored stencil value before the test compares them. Initially set to all 1s. If the test passes the fragment is drawn, discarded otherwise.
- **`glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)`**: This tells how to take action against the passed and/or failed fragments after the test. This function is used to update the `stencil buffer`.

### Use Cases
1. Object outlining: 
2. drawing textures inside a rear-view mirror so it neatly fits into the mirror shape, or 
3. rendering real-time shadows with a stencil buffer technique called shadow volumes. 

### Misc
- Also, just like the depth testing's glDepthMask function, there is an equivalent function for the stencil buffer. The function glStencilMask allows us to set a bitmask that is ANDed with the stencil value about to be written to the buffer. By default this is set to a bitmask of all 1s not affecting the output, but if we were to set this to 0x00 all the stencil values written to the buffer end up as 0s. This is equivalent to depth testing's glDepthMask(GL_FALSE): 

```cpp
glStencilMask(0xFF); // each bit is written to the stencil buffer as is
glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
```

## Depth Testing
(If enabled) the depth values of the fragments are tested against a depth test function and the fragments which pass the test are rendered. The ones failed are discarded.

### Where is the data stored at?
The depth values of the fragments are stored in the `depth buffers`. Depth buffers (such as color buffer) stores depth value information per fragment. It defines if a fragment is in front or back. It has the same width and height as the color buffer. The depth buffer is usually created by the windowing system.

### How it works?
When depth testing is enabled, OpenGL tests the depth value of a fragment against the content of the depth buffer. OpenGL performs a depth test and if this test passes, the fragment is rendered and the depth buffer is updated with the new depth value. If the depth test fails, the fragment is discarded.

### How to enable?
It is disabled by default. To enable, use the following OpenGL command.

```cpp
glEnable(GL_DEPTH_TEST);
```

> Once enabled, OpenGL automatically stores fragments their z-values in the depth buffer if they passed the depth test and discards fragments if they failed the depth test accordingly. If you have depth testing enabled you should also clear the depth buffer before each frame using `GL_DEPTH_BUFFER_BIT`; otherwise you're stuck with the depth values from last frame: 

```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
```

### When it works?
Depth testing is done in screen space after the fragment shader has run (and after the stencil test which we'll get to in the next chapter). 

### How to access?
The screen space coordinates relate directly to the viewport defined by OpenGL's glViewport function and can be accessed via GLSL's built-in `gl_FragCoord` variable in the fragment shader. The x and y components of `gl_FragCoord` represent the fragment's screen-space coordinates (with (0,0) being the bottom-left corner). The `gl_FragCoord` variable also contains a z-component which contains the depth value of the fragment. This z value is the value that is compared to the depth buffer's content. 

### What happens if the depth test is disabled?
The depth test always passes (or never tested) so the fragments that are drawn last are rendered in front of the fragments that were drawn before, even though they should've been at the front. Since we've drawn the floor plane last, the plane's fragments overwrite each of the container's previously written fragments: 

### Depth value precision
The depth buffer contains depth values between 0.0 and 1.0 and it compares its content with the z-values of all the objects in the scene as seen from the viewer. These z-values in view space can be any value between the projection-frustum's near and far plane. We thus need some way to transform these view-space z-values to the range of [0,1] and one way is to linearly transform them. The following (linear) equation transforms the z-value to a depth value between 0.0 and 1.0: 

There are several ways to compute the precision but the main two approaches are linear and nonlinear:

### Z-fighting
A common visual artifact may occur when two planes or triangles are so closely aligned to each other that the depth buffer does not have enough precision to figure out which one of the two shapes is in front of the other. The result is that the two shapes continually seem to switch order which causes weird glitchy patterns. This is called z-fighting, because it looks like the shapes are fighting over who gets on top. 

**How to avoid?**
1. Never place the objects too close to each other so their triangles will not overlap. Always keep a distance.
2. Set the near plane as far as possible.
3. Use higher precision buffer.

### Misc
- There are certain scenarios imaginable where you want to perform the depth test on all fragments and discard them accordingly, but not update the depth buffer. Basically, you're (temporarily) using a read-only depth buffer. OpenGL allows us to disable writing to the depth buffer by setting its depth mask to GL_FALSE: 
```cpp
glDepthMask(GL_FALSE);
```




## Transparency
Transparency is all about objects (or parts of them) not having a solid color, but having a combination of colors from the object itself and any other object behind it with varying intensity. A colored glass window is a transparent object; the glass has a color of its own, but the resulting color contains the colors of all the objects behind the glass as well. This is also where the name blending comes from, since we blend several pixel colors (from different objects) to a single color. Transparency thus allows us to see through objects. 

The amount of transparency of an object is defined by its color's alpha value. 
- 0.0: Object having complete transparency
- 0.5: Object's color consists of 50% of its own color and %50 of the color of the objects behind it.
- 1.0: No transparency. Object reflects only its own color.

### How to process transparent textures?
i.e. Grass. Let's assume you have an image of grass with transparency which means that the parts of the image other than the grass are transparent. 
`stb_image` API already knows how to handle transparent images.

Opengl does not know what to with transparent images so it renders it by default. We can discard the transparent fragments in the fragment shader using `discard` GLSL directive.

```cpp
// Client side
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  

// Server side (GLSL) - Fragment Shader
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, TexCoords);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}

```






## Blending
Opengl does not provide a default way to handle semi transparent objects and to achieve this we have to enable blending.

### Where is data stored at?
Since this is a color related operation, it operates on the `color buffer` and `the fragment's color (output)`.

### When it works?
After the fragment shader has run and all the other tests completed, the given `blend equation` let us loose the `fragment color` with whatever available in the `color buffer`. 

### How to enable?
```cpp
glEnable(GL_BLEND);
```

### How it works?
After blending is enabled, we can now blend the fragment's output color value with color vector stored in the color buffer. The resulting color is then stored in the color buffer by replacing the previous value.

There are several functions to be used for this purpose:
- **`glBlendFunc(GLenum sfactor, GLenum dfactor)`**: 
- **`glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO)`**: To set different options for the RGB and alpha channel individually
- **`glBlendEquation(GLenum mode)`**: allows us to set this operation and has 5 possible options: 

```cpp
// Client side
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

// Server side - Fragment shader
// No need to discard
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;

void main()
{             
    FragColor = texture(texture1, TexCoords);
}
```

However, there might be an issue when depth testing is in place. Depth testing does not care about the blending. When writing to the depth buffer, the depth test does not care if the fragment has transparency or not, so the transparent parts are written to the depth buffer as any other value. 

To make blending work for multiple objects we have to draw the most distant object first and the closest object last. The normal non-blended objects can still be drawn as normal using the depth buffer so they don't have to be sorted. We do have to make sure they are drawn first before drawing the (sorted) transparent objects. When drawing a scene with non-transparent and transparent objects the general outline is usually as follows:

1. Draw all opaque objects first.
2. Sort all the transparent objects.
3. Draw all the transparent objects in sorted order.

```cpp
// client side
// windows (from furthest to nearest)
glBindVertexArray(transparentVAO);
glBindTexture(GL_TEXTURE_2D, transparentTexture);
for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, it->second);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

```



## Face Culling
An object can have multiple faces but only some of them can be seen from the viewer's point of view based on the angle and the position of the camera. Therefore, we don't need to render all the faces of an object. It is quite adequate to render only the visible faces and this will avoid unneccessary computing.

This is exactly what face culling does. OpenGL checks all the faces that are front facing towards the viewer and renders those while discarding all the faces that are back facing, saving us a lot of fragment shader calls. We do need to tell OpenGL which of the faces we use are actually the front faces and which faces are the back faces. OpenGL uses a clever trick for this by analyzing the winding order of the vertex data. 

### Winding order
In order Opengl to determine which faces are backface or frontface, we have to define the vertices in clockwise or counter-clockwise order and this should be consistent throught the all objects(vertices) defined.

OpenGL uses this information when rendering your primitives to determine if a triangle is a front-facing or a back-facing triangle. By default, triangles defined with counter-clockwise vertices are processed as front-facing triangles. 

so each triangle that you're specifying should be counter-clockwise as if you're directly facing that triangle. The cool thing about specifying all your vertices like this is that the actual winding order is calculated at the rasterization stage, so when the vertex shader has already run. The vertices are then seen as from the viewer's point of view. 

### How to enable?
```cpp
glEnable(GL_CULL_FACE);
```

### How to use?
```cpp
// Client side
glEnable(GL_CULL_FACE);

/*
    GL_BACK: Culls only the back faces.
    GL_FRONT: Culls only the front faces.
    GL_FRONT_AND_BACK: Culls both the front and back faces.
*/
glEnable(GL_CULL_FACE);

/*
    GL_CCW: for counter-clockwise ordering
    GL_CW:clockwise ordering. 
*/
glFrontFace(GL_CCW);
```

## Geometry Shader
It is an optional and intermediate stage between `vertex` and `fragment` shader. It takes a set of vertices that form a single primitive (i.e. point or triangle), transforms these vertices and pass them to the next shader stage.

### Use cases
- Object exploding
- Normal vector visualization

## Instancing
Some models might use (share) the same set of vertex data. If you want to render all of these models aka telling the GPU to render each model again and again, it will create a bottle neck. There should be a way to tell OpenGl to use the same vertex data for all of these models. This is called `instancing`. It is drawing multiple objects using the same vertex data by calling the corresponding draw function once.

This saves all CPU-GPU communications.

### How to use?
In order to render instanced models, we should call Instanced versions of the correspoding draw methods `glDrawArraysInstanced()` or `glDrawElementsInstanced()`. In order to access those from the server side, 

- Naive approach
```cpp
// Client side

glm::vec2 translations[100];
int index = 0;
float offset = 0.1f;
for(int y = -10; y < 10; y += 2)
{
    for(int x = -10; x < 10; x += 2)
    {
        glm::vec2 translation;
        translation.x = (float)x / 10.0f + offset;
        translation.y = (float)y / 10.0f + offset;
        translations[index++] = translation;
    }
}

shader.use();
for(unsigned int i = 0; i < 100; i++)
{
    shader.setVec2(("offsets[" + std::to_string(i) + "]")), translations[i]);
}

// Within the loop
glBindVertexArray(quadVAO);
glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);  

// Server side
// Vertex shader
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 fColor;

uniform vec2 offsets[100];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(aPos + offset, 0.0, 1.0);
    fColor = aColor;
} 

// Fragment shader
#version 330 core
out vec4 FragColor;
in vec3 fColor;

void main()
{
    FragColor = vec4(fColor, 1.0);
}
```
- Better approach: Instanced Arrays
`Instanced Array`s are defined as vertex attribute. With vertex attributes, at the start of each run of the vertex shader, the GPU will retrieve the next set of vertex attributes that belong to the current vertex. When defining a vertex attribute as an instanced array however, the vertex shader only updates the content of the vertex attribute per instance. This allows us to use the standard vertex attributes for data per vertex and use the instanced array for storing data that is unique per instance. 

```cpp
// Client side

/*
Since indexed arrays are vertex attribute, they have to be stored in vertex buffer objects.
*/
float quadVertices[] = {
    // positions     // colors
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};
unsigned int quadVAO, quadVBO;
glGenVertexArrays(1, &quadVAO);
glGenBuffers(1, &quadVBO);
glBindVertexArray(quadVAO);
glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
// also set instance data
glEnableVertexAttribArray(2);
glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
/*
This function tells OpenGL when to update the content of a vertex attribute to the next element. 
Its first parameter is the vertex attribute in question and the second parameter the attribute divisor. 
*/

// Server side
// Vertex shader
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main()
{
    /*
    For downscaling
    vec2 pos = aPos * (gl_InstanceID / 100.0);
    gl_Position = vec4(pos + aOffset, 0.0, 1.0);
    */
    gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
    fColor = aColor;
} 

// Fragment shader - same as the previous one

```

## Anti Aliasing
Jagged edges might appear alongside the edges in the model. It avoids you to generate smoother edges. This is called `aliasing` and the patterns to avoid aliasing are called `anti-aliasing`.

### How aliasing occurs?
Aliasing is the product of the rasterization stage. The rasterization stage sits after your final processed vertices and before the fragment shader. The rasterizer takes all vertices belonging to a single primitive and transforms this to a set of fragments. Vertex coordinates can theoretically have any coordinate, but fragments can't since they are bound by the resolution of your screen. There will almost never be a one-on-one mapping between vertex coordinates and fragments, so the rasterizer has to determine in some way what fragment/screen-coordinate each specific vertex will end up at.

Your vertex might be in any value but fragments can not because the fragments are totally dependent on the screen's pixels which is dependent to screen resolution. Consider the pixels as grids and only the fragments will be drawn for the pixels on which the rendered object covers. The covered pixels are determined with `sampling points` which are located at the center of the pixel. If the rendered object covers that sampling point, then a fragment will be generated for that pixel.

When single sampling point is used, there will be discontinuity between covered and uncovered neighboring pixels. This decreases the quality. By using `multi-sampling points` the resolution can be improved. Representing a pixel with multiple (i.e. 4) points, provide smoother transitions between covered and uncovered neighboring pixels.

### How to enable?
In order to store more than one sampling point per pixel, we need to store that additional data and this can be achieved by `multisampling buffer` rather than a default buffer. Most windowing systems provide this capability.

- For GLFW:
```cpp
glfwWindowHint(GLFW_SAMPLES, 4);
```

We also need to let OpenGL know about this with the following call:
```cpp
glEnable(GL_MULTISAMPLE);
```

## Off-screen Multisampling
The abovementioned approach works well with the default framebuffers but we need additional adjustments for user-defined framebuffers. There are two ways we can create multisampled buffers to act as attachments for framebuffers: texture attachments and renderbuffer attachments. Quite similar to normal attachments like we've discussed in the framebuffers chapter.

### Multisampled texture attachments
```cpp
// Bind
glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
// attach
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
```

### Multisampled renderbuffer attachments
Binding is same as with regular binding. Only attachment differs.
```cpp
// Attachment
glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
```

### REnder the framebuffer
```cpp
glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFBO);
glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST); 

```
