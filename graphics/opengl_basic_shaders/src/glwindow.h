#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL/glew.h>

#include <glm/vec3.hpp>

#include "geometry.h"
#include "entity.h"
#include "light.h"

class OpenGLWindow
{
public:
    OpenGLWindow();

    void initGL();
    void render();
    bool handleEvent(SDL_Event e);
    void cleanup();
    void loadTexture(const std::string &filename);

private:
    SDL_Window* sdlWin;
    
    GLuint vao;
    GLuint shader;
    GLuint vertexBuffer;

    Entity parentEntity;
    Entity childEntity;

    Light light1;
    Light light2;

    GeometryData geometry;

    int colorIndex;
    int translateDirection;
    int rotateDirection;
    int scaleDirection;
};

#endif
