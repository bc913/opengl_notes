# Lighting

## Introduction
In real world, an object's seen color is defined as the color that is reflected from the object under a light source. Light source (i.e. sun) has too many colors as a source and the object absorbs a large portion of it and reflects the colors that represent object's color.

### How this works in OpenGL?
```cpp
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
glm::vec3 reflected = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);

// ex2
glm::vec3 lightColor(0.0f, 1.0f, 0.0f); // Only green light source
glm::vec3 toyColor(1.0f, 0.5f, 0.31f); // absorbs half of the green 
glm::vec3 result = lightColor * toyColor; // = (0.0f, 0.5f, 0.0f);
```

Since the real world light physics is quite complex, it is impossible to simulate real-world lighting phenomena in graphics world. Therefore, some lighting models (based on some approximations) are offered to simulate the real world lighting and its effects on the objects. One of the major lighting model offered is `Phong lighting model` and it basically acts as a combination of three lighting components:
- Ambient Lighting
- Diffuse Lighting
- Specular Lighting

## Lighting Components
### Ambient Lighting
Even it is dark, there is still always a light on the objects from an obvious or unobvious source(s) of light. The ambient light is modeled as light that has no origin, no direction and has an equal effect on all objects in the scene. Ambient lighting can be assumed as the lighting effect on an object with one or multiple sources come from all directions.

Even though there are several complex algorithms available (i.e. `global illumination`), the ambient lighting on an object can be basically approximated as follows:
```cpp
// object's fragment shader
float ambientStrength = 0.1;
vec3 ambient = ambientStrength * lightColor;

vec3 result = ambient * objectColor;
FragColor = vec4(result, 1.0);
```
Check extra [Extra](#Extra) section for more explanation.

### Diffuse Lighting
Diffuse Lighting can be considered as the directional lighting cast by a determined light source. Diffuse light can be described as the light that has a position in space and comes from a single direction. As the angle of the light ray gets closer to 90 degrees, more brightness shows up on the object. If the light ray hits the object surface with 90 degrees, it has the greatest impact.

TODO: Add explanatory image

In other words, if the angle between the light ray vector and the object's vertex(surface) normal is **0**, `cos(0) = 1`,  the light has the greatest impact on the object. The larger the angle, the less brightness on the object.

> REMEMBER: The dot product of two unit vectors results in the cosine of the angle between the unit vectors.

Check extra [Extra](#Extra) section for more explanation.

Therefore, we need two things to calculate diffuse lighting:
1. Normal vector perpendicular to the vertex' surface.
2. The directed light ray: a direction vector that is the difference vector between the light's position and the fragment's position. To calculate this light ray we need the light's position vector and the fragment's position vector.
3. As a result of 1. and 2., the diffuse impact will be calculated.


#### Normal Vector
Since a vertex by itself has no surface (it's just a single point in space) we retrieve a normal vector by using its surrounding vertices to figure out the surface of the vertex. We can use a little trick to calculate the normal vectors for all the cube's vertices by using the cross product, but since a 3D cube is not a complicated shape we can simply manually add them to the vertex data.  

We can have the vertex normals data through VBO and since it is a vertex data, it can be acessed through the vertex shader.
```cpp
// Vertex shader
//...
layout(location = 1) in vec3 vertex_normal;
//...
```
#### Directed light Ray
 - *Light source position vector*  comes from the client side in our example and can be accessed in the `fragment shader` using uniform keyword.
 ```cpp
 // Client
 glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
 // Fragment shader
uniform vec3 lightPos;
 ```
 - *Fragment's position vector*: We're going to do all the lighting calculations in world space so we want a vertex position that is in world space first. We can accomplish this by multiplying the vertex position attribute with the model matrix only (not the view and projection matrix) to transform it to world space coordinates. This can easily be accomplished in the vertex shader so let's declare an output variable and calculate its world space coordinates: 
 ```cpp
 // Vertex shader
 out vec3 frag_position;
 void main()
 {
    frag_position = vec3(model_matrix * vec4(vertex_position, 1.0));
 }

 // Fragment shader
 in vec3 frag_position;
 /*
This in variable will be interpolated from the 3 world position vectors of the triangle to form the FragPos vector that is the per-fragment world position. Now that all the required variables are set we can start the lighting calculations.
  */
 ```
Now we have the required `light source's position vector` and `fragment's position vector` components to calculate the `direction vector from the light source's position to the fragment position`. That is basically the difference between them.
> IMPORTANT: We usually do NOT care about the magnitude of the vector during lighting calculations. We care about the directions so normalizing the operand vectors before using them is required and better practice. 

```cpp
// Fragment shader
void main()
{
    vec3 normalized_frag_normal = normalize(frag_normal);
    vec3 dir_vector_from_light_source_to_fragment = normalize(lightPos - frag_position);
}
```
#### Diffuse impact
The diffuse impact depends on the angle between `light source's position vector` and `fragment's position vector`.
```cpp
// Fragment shader
void main()
{
    /*...*/

    // since both of them are normalized, the result is cosine of the angle
    float cosine_angle = dot(normalized_frag_normal, dir_vector_from_light_source_to_fragment);
    // the angle might be greater than 90 so dot product might result in negative value.
    // This will cause negative diffuse value so we have to avoid it.
    cosine_angle = max(cosine_angle, 0.0); // take the max

    // Diffuse diffuse impact
    vec3 diffuse = cosine_angle * lightColor;
    // Since we have also ambient impact, include it
    vec3 resulting_color = (diffuse + ambient) * objectColor;
    frag_color = vec4(resulting_color, 1.0);
}
```

#### Sum-up
The vertex and fragment shaders for the object (not the light source) will be as follows:
- Vertex shader

```cpp
#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
out vec3 frag_position;
out vec3 frag_normal;
void main()
{
   frag_position = vec3(model_matrix * vec4(vertex_position, 1.0));
   frag_normal = vertex_normal;
   gl_Position = projection_matrix * view_matrix * vec4(frag_position, 1.0);
}
```
- Fragment Shader
```cpp
#version 330 core
in vec3 frag_position;
in vec3 frag_normal;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;
out vec4 frag_color;
void main()
{
   vec3 normalized_frag_normal = normalize(frag_normal);
   vec3 dir_vector_from_light_source_to_fragment = normalize(lightPos - frag_position);
   float cosine_angle = dot(normalized_frag_normal, dir_vector_from_light_source_to_fragment);
   cosine_angle = max(cosine_angle, 0.0);
   vec3 diffuse = cosine_angle * lightColor;
   vec3 ambient = ambientStrength * lightColor;
   vec3 resulting_color = (ambient + diffuse) * objectColor;
   frag_color = vec4(resulting_color, 1.0);
}
```
#### **One last thing**

in the previous section we passed the normal vector directly from the vertex shader to the fragment shader. However, the calculations in the fragment shader are all done in world space, so shouldn't we transform the normal vectors to world space coordinates as well? Basically yes, but it's not as simple as simply multiplying it with a model matrix.

First of all, normal vectors are only direction vectors and do not represent a specific position in space. Second, normal vectors do not have a homogeneous coordinate (the w component of a vertex position). This means that translations should not have any effect on the normal vectors. So if we want to multiply the normal vectors with a model matrix we want to remove the translation part of the matrix by taking the upper-left 3x3 matrix of the model matrix (note that we could also set the w component of a normal vector to 0 and multiply with the 4x4 matrix).

Second, if the model matrix would perform a non-uniform scale, the vertices would be changed in such a way that the normal vector is not perpendicular to the surface anymore. The following image shows the effect such a model matrix (with non-uniform scaling) has on a normal vector: 

 Whenever we apply a non-uniform scale (note: a uniform scale only changes the normal's magnitude, not its direction, which is easily fixed by normalizing it) the normal vectors are not perpendicular to the corresponding surface anymore which distorts the lighting.

The trick of fixing this behavior is to use a different model matrix specifically tailored for normal vectors. This matrix is called the normal matrix and uses a few linear algebraic operations to remove the effect of wrongly scaling the normal vectors. If you want to know how this matrix is calculated I suggest the following article.

The normal matrix is defined as 'the transpose of the inverse of the upper-left 3x3 part of the model matrix'. Phew, that's a mouthful and if you don't really understand what that means, don't worry; we haven't discussed inverse and transpose matrices yet. Note that most resources define the normal matrix as derived from the model-view matrix, but since we're working in world space (and not in view space) we will derive it from the model matrix.

In the vertex shader we can generate the normal matrix by using the inverse and transpose functions in the vertex shader that work on any matrix type. Note that we cast the matrix to a 3x3 matrix to ensure it loses its translation properties and that it can multiply with the vec3 normal vector: 

### Specular Lighting
Similar to diffuse lighting, specular lighting depends on the `normal vector of the objects vertex' surface` and the `direction of the light ray`. In addition to these, the specular lighting is based on `the view direction e.g. from what direction the viewer is looking at the fragment`. `Specular lighting` is based on the reflective properties of the surfaces and therefore we need to somehow get the `reflection vector` to calculate the angle between the view direction and the reflection vector. The smaller the angle, the greater impact of the specular light.

Therefore, we need three things to calculate diffuse lighting:
1. Normal vector perpendicular to the vertex' surface.
2. The directed light ray: a direction vector that is the difference vector between the light's position and the fragment's position. To calculate this light ray we need the light's position vector and the fragment's position vector.
3. The direction vector from the viewer's (camera) point to the fragment's position. (Viewer Direction)
4. The reflection vector: This is calculated by reflecting the light direction around the normal vector. 
5. Then, the angle between the reflection vector and the viewer direction is calculated.
6. Specular impact using the angle between the reflection vector and the viewer direction vector.

#### Normal vector and directed light ray
Please follow the instructions in the `Diffuse Lighting` section.
#### Direction vector from the viewer's point to the fragment's position (From camera's position to the fragment's position)
We have to know the camera's position and it can be provided from the client side using `Camera` class' `Position` field. Also, fragment shader requires this info so it is required to mark it as `uniform` in the object's fragment shader.
```cpp
// Client side
object_cube_shader.setVec3("camera_position", camera.Position);

// Object's Fragment shader
uniform vec3 camera_position;
vec3 dir_vec_from_camera_pos_to_fragment = normalize(camera_position - frag_position);
```
#### Reflection vector
```cpp
// Object's fragment shader
vec3 reflection_vec = reflect(-dir_vector_from_light_source_to_fragment, normalized_frag_normal);

```

#### Angle between the angle between the reflection vector and the viewer direction vector.
```cpp
// Object's fragment shader
float cosine_angle2 = dot(dir_vec_from_camera_pos_to_fragment, reflection_vec);
cosine_angle2 = max(cosine_angle2, 0.0);
```
#### Specular impact
```cpp
// Object's fragment shader
int shininess = 32;
float specular_scalar = pow(cosine_angle2, shininess);
vec3 specular = specularStrength * specular_scalar * lightColor;
```


## Lighting Maps
An object might consist of different materials and each material might have different lighting/color characteristics so each of them responds/reflects the light differently. Therefore, we need more fine settings not at the object level but at the fragment level. Per fragment adjustments for the lighting are required.

### Diffuse Maps
We can index unique color(diffuse) values per fragment.

### Specular Maps
Similar logic works here too. Each material's reflective (specular) property might differ and it should be handled in fragment level.

From learnopengl.com:
>  The intensity of the specular highlight comes from the brightness of each pixel in the image. Each pixel of the specular map can be displayed as a color vector where black represents the color vector vec3(0.0) and gray the color vector vec3(0.5) for example. In the fragment shader we then sample the corresponding color value and multiply this value with the light's specular intensity. The more 'white' a pixel is, the higher the result of the multiplication and thus the brighter the specular component of an object becomes. 

> Because the container mostly consists of wood, and wood as a material should have no specular highlights, the entire wooden section of the diffuse texture was converted to black: black sections do not have any specular highlight. The steel border of the container has varying specular intensities with the steel itself being relatively susceptible to specular highlights while the cracks are not. 
## Extra
### Ambient Lighting
Some additional explanations for ambient lighting from different references given in the references section.
> Ambient illumination is light that's been scattered so much by the environment that its direction is impossible to determine - it seems to come from all directions. Backlighting in a room has a large ambient component, since most of the light that reaches your eye has first bounced off many surfaces. A spotlight outdoors has a tiny ambient component; most of the light travels in the same direction, and since you're outdoors, very little of the light reaches your eye after bouncing off other objects. When ambient light strikes a surface, it's scattered equally in all directions.

>  Ambient light is the type of light you see when you go outside in a usual sunny day. Even though the sun is traveling across the sky and its light rays hit the world at different angles in different parts of the day, most of the stuff will be visible, even if it is in shadow. Since light bounces off everything it eventually hits everything so objects that are not in the direct path of the sun are also lit. Even a light bulb in a room behaves like the sun in that sense and spreads ambient light because if the room is not too big everything is lit equally. The ambient light is modeled as light that has no origin, no direction and has an equal effect on all objects in the scene.

> Ambient lighting: even when it is dark there is usually still some light somewhere in the world (the moon, a distant light) so objects are almost never completely dark. To simulate this we use an ambient lighting constant that always gives the object some color.

> A 3D sphere lit by AMBIENT LIGHT only; appears to look 2D. Ambient light is the average volume of light that is created by emission of light from all of the light sources surrounding (or located inside of) the lit area. When sun rays pass through the window of a room they hit the walls and are reflected and scattered into all different directions which averagely brightens up the whole room. This visual quality is described by ambient light. Ambient light alone cannot communicate the complete representation of an object set in 3D space because all vertices are evenly lit by the same color and the object appears to be 2-dimensional as seen in the image above. Although the object displayed is actually a 3D sphere, it appears to be flat on the screen, when lit only by ambient light.

### Diffuse Lighting
> Diffuse lighting emphasizes the fact that the angle by which the light hits the surface effects the brightness by which the object is lit. When light hits an object on one side that side is brighter than the other side (the side not directly infront of the light source). We just saw that the sun spreads ambient light which has no specific direction. However, the sun also has diffuse properties in its light. When it hits a tall building you can usually see that one side of the building is lighter than the other side. The most important property of diffuse light is its direction.

### Specular Lighting
> Specular lighting is more a property of the object, rather than the light itself. This is what makes parts of things shine when light hits them at a very specific angle and the viewer is positioned at a specific point. Metalic objects often have some kind of specular property. For example, a car in a bright sunny day can sometimes shine off its edges. Calculating specular lighting must take into consideration both the direction the light hits (and bounces off) as well as the position of the viewer.

> Specular reflection (or specular highlight) is displayed here in addition to the Ambient and Diffuse layers of light. You can observe how the object's 3D representation is greatly augmented by specular light properties. It's as if we can feel the material the object is made from. This could be a balloon, something plastic, with a smooth surface. The smoother the object is, the more defined this specular highlight will be. Just like diffuse light, specular light is a directional type of light. It comes from one particular direction. The difference between the two is that specular light reflects off the surface in a sharp and uniform way. The rendering of specular light relies on the angle between the viewer and the light source.

> specular light comes from a particular direction, and it tends to bounce off the surface in a preferred direction. A well-collimated laser beam bouncing off a high-quality mirror produces almost 100 percent specular reflection. Shiny metal or plastic has a high specular component, and chalk or carpet has almost none. You can think of specularity as shininess. 

## References
- [https://learnopengl.com/Lighting/Basic-Lighting](https://learnopengl.com/Lighting/Basic-Lighting)
- [https://www.falloutsoftware.com/tutorials/gl/gl8.htm](https://www.falloutsoftware.com/tutorials/gl/gl8.htm)
- [https://ogldev.org/www/tutorial17/tutorial17.html](https://ogldev.org/www/tutorial17/tutorial17.html)
- [https://www.glprogramming.com/red/chapter05.html](https://www.glprogramming.com/red/chapter05.html)
- [https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-15-lightmaps/](https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-15-lightmaps/)
- [https://my.eng.utah.edu/~cs5610/lectures/Lightmaps%20Multitexture%202010.pdf](https://my.eng.utah.edu/~cs5610/lectures/Lightmaps%20Multitexture%202010.pdf)
- [https://www.opengl.org/archives/resources/code/samples/advanced/advanced97/notes/node70.html](https://www.opengl.org/archives/resources/code/samples/advanced/advanced97/notes/node70.html)