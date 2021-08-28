# Camera

## Introduction
the view matrix transforms all the world coordinates into view coordinates that are relative to the camera's position and direction.

## What do we need for a camera?
1. Its position in the world space
2. The direction it is looking at
3. Two vectors pointing to the right and upwards from the camera.

This is actually a cartesian coordinate system assumes its origin at the camera's position.

### 1. Position in the world space
Position vector of the camera in the world space coordinate system.
```cpp
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
```
> Positive z direction is going through the screen so if you want to move camera backwards, the camera has to move along positive +z axis.

### 2. The direction from where the camera is looking at towards to the camera

```cpp
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
// Actually this is reverse direction of the direction vector where the camera is looking at so cameraDirection vector here is pointing from origin towards the camera.
```

### 3. Right vector from the camera
```cpp
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //helper
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
```

### 4. Up vector from the camera
```cpp
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
```

## A math fact
A great thing about matrices is that if you define a coordinate space using 3 perpendicular (or non-linear) axes you can create a matrix with those 3 axes plus a translation vector and you can transform any vector to that coordinate space by multiplying it with this matrix. ???

TODO: Look for the definition of Transformation matrix

## LookAt matrix
Where R is the right vector, U is the up vector, D is the direction vector and P is the camera's position vector. Note that the rotation (left matrix) and translation (right matrix) parts are inverted (transposed and negated respectively) since we want to rotate and translate the world in the opposite direction of where we want the camera to move. Using this LookAt matrix as our view matrix effectively transforms all the world coordinates to the view space we just defined. The LookAt matrix then does exactly what it says: it creates a view matrix that looks at a given target.

```cpp
glm::mat4 view = 
glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),    // CAmera position vector
  		   glm::vec3(0.0f, 0.0f, 0.0f),     // Camera's target position
  		   glm::vec3(0.0f, 1.0f, 0.0f));    // Helper up vector to calculate the camera's right vector
```

This is the `view matrix` that transforms the world coordinate space into the view space coordinates.

## References
- https://www.khronos.org/opengl/wiki/Coordinate_Transformations
- [https://learnopengl.com/Getting-started/Camera](https://learnopengl.com/Getting-started/Camera)
- https://openglcornellbox.wordpress.com/tutorials/cornell-box-part-5/
- file:///C:/Users/burca/AppData/Local/Temp/3.8%20Cameras%20in%20OpenGL.pdf
- https://inst.eecs.berkeley.edu/~cs184/fa09/resources/sec09_camera.pdf
- http://www.cs.kent.edu/~farrell/cg05/lectures/cg17.pdf
- https://www.cs.utexas.edu/~theshark/courses/cs354/lectures/cs354-10.pdf
- http://graphics.cs.cmu.edu/nsp/course/15-462/Spring04/syllabus.html
- http://graphics.cs.cmu.edu/nsp/course/15-462/Spring04/slides/02-openGL.pdf

- https://www.songho.ca/opengl/gl_camera.html#lookat
- https://www.tomdalling.com/blog/modern-opengl/04-cameras-vectors-and-input/
- 

- https://ogldev.org/
- https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix
- https://math.hws.edu/graphicsbook/c3/s3.html


