#include "texture_utils.h"
#include <GL/glew.h> // Include this first
#include <iostream>

//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif


#include "stb_image.h"

namespace Utility::texture
{
    unsigned int load(std::string texture_path)
    {
        // 1. Load the corresponding image
        int width, height, nrChannels;
        unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
        if (!data)
            std::cerr << "Failed to load texture image!!!" << std::endl;

        bool is_png = texture_path.find(".png") != std::string::npos;

        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4 || is_png)
            format = GL_RGBA;

        // 2. Generate the texture object like any other OpenGL object
        GLuint texture_obj;
        glGenTextures(1, &texture_obj);

        // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
        // RULE: texture_obj should and can NOT be bound to any other target after this point
        glBindTexture(GL_TEXTURE_2D, texture_obj);
        // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

        // 4. Set the sampling parameters
        // Wrapping (edge value sampling)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 5. Get the storage for the texture
        // Mutable storage in this case
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // 6. Storage contents can now be altered after this point.
        // It is often a good practice to generate a mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        // 7. You can now free the loaded image data.
        stbi_image_free(data);

        return texture_obj;
    }

}