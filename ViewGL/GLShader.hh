#pragma once

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

#include <QGLShaderProgram>
#include <QGLShader>
#include <memory>

using namespace std;

class GLShader
{
public:
    GLShader(const char* vertexShaderFile, const char* fragmentShaderFile, shared_ptr<QGLShaderProgram> &program);
    void activateShader();
    void activateShaderTexture();
};
