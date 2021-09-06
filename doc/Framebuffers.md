# Framebuffer Objects
## What are they?
There are several types of screen buffers:
- Color buffer
- Depth buffer
- Stencil buffer

The combination of these buffers are stored in the server (GPU) side and it is called `framebuffer`. This is provided by default and this is also called `default framebuffer`. Generating custom framebuffers helps us to implement different features which are not there by default:
- Mirrors in a scene
- post-processing effects

## How it works?
1. First,  we need to generate it such as the other objects:
```cpp
unsigned int fbo;
glGenFramebuffers(1, &fbo);
```
2. Bind it to the context:
```cpp
/*
GL_DRAW_FRAMEBUFFER:
GL_READ_FRAMEBUFFER:
GL_FRAMEBUFFER: For both read and write
 
*/
glBindFramebuffer(GL_FRAMEBUFFER, fbo);
```

3. We are not done yet. We have to satisfy the following conditions:

- We have to attach at least one buffer (color, depth or stencil buffer).
- There should be at least one color attachment.
- All attachments should be complete as well (reserved memory).
- Each buffer should have the same number of samples.

4. After all the requirements are satisfied, we can check the status:
```cpp
if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
```

5. All subsequent rendering operations will now render to the attachments of the currently bound framebuffer. Since our framebuffer is not the default framebuffer, the rendering commands will have no impact on the visual output of your window. For this reason it is called off-screen rendering when rendering to a different framebuffer. 

6. If you want all rendering operations to have a visual impact again on the main window we need to make the default framebuffer active by binding to 0: 
```cpp
glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

7. When we're done, we have to delete it.
```cpp
glDeleteFramebuffers(1, &fbo);
```

## Attachments
we need to attach one or more attachments to the framebuffer. An attachment is a memory location that can act as a buffer for the framebuffer, think of it as an image. When creating an attachment we have two options to take: textures or renderbuffer objects. 

### Texture attachments
When attaching a texture to a framebuffer, all rendering commands will write to the texture as if it was a normal color/depth or stencil buffer. The advantage of using textures is that the render output is stored inside the texture image that we can then easily use in our shaders.

Creating a texture for a framebuffer is roughly the same as creating a normal texture: 

```cpp
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
  
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  


```
The main differences here is that we set the dimensions equal to the screen size (although this is not required) and we pass NULL as the texture's data parameter. For this texture, we're only allocating memory and not actually filling it. Filling the texture will happen as soon as we render to the framebuffer. Also note that we do not care about any of the wrapping methods or mipmapping since we won't be needing those in most cases. 

Then we have to attach it to the framebuffer.
```cpp
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
```
>  Next to the color attachments we can also attach a depth and a stencil texture to the framebuffer object. To attach a depth attachment we specify the attachment type as GL_DEPTH_ATTACHMENT. Note that the texture's format and internalformat type should then become `GL_DEPTH_COMPONENT` to reflect the depth buffer's storage format. To attach a stencil buffer you use GL_STENCIL_ATTACHMENT as the second argument and specify the texture's formats as `GL_STENCIL_INDEX`. It is also possible to attach both a depth buffer and a stencil buffer as a single texture. Each 32 bit value of the texture then contains 24 bits of depth information and 8 bits of stencil information. To attach a depth and stencil buffer as one texture we use the `GL_DEPTH_STENCIL_ATTACHMENT` type and configure the texture's formats to contain combined depth and stencil values. An example of attaching a depth and stencil buffer as one texture to the framebuffer is given below: 

```cpp
glTexImage2D(
  GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, 
  GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
);

glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
```
### Renderbuffer objects
Read [this section](Objects.md/#Renderbuffer-Objects) for detail explanation of renderbuffer objects. After creating and (optional) defining it for depth and stencil storage, we can attach it to the framebuffer.

```cpp
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);  
```

## Post-processing effects can be achieved with Framebuffer objects
- Inversion
- Grayscale
- Kernel affects (Blur and edge detection)
