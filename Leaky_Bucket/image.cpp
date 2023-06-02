
#include <glut.h>
#include<GL/GL.h>
#include<gl/GLU.h>
#include <FreeImage.h>

GLuint textureID;

void loadTexture() {
    // Load the image using FreeImage library
    FIBITMAP* image = FreeImage_Load(FIF_JPEG, "image.jpg", 0);

    // Generate an OpenGL texture object
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the image data into the texture
    int width = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);
    GLvoid* data = FreeImage_GetBits(image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // Free the FreeImage bitmap
    FreeImage_Unload(image);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Render your object with the texture applied

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // Initialize OpenGL context and window using a library like GLFW or FreeGLUT
    
    loadTexture();
    
    glutDisplayFunc(display);
    glutMainLoop();
}