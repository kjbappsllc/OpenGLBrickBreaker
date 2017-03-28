//
//  Shader.hpp
//  BreakOutGL
//
//  Created by Mac on 3/12/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility
// functions for easy management.
class Shader
{
public:
    // State
    GLuint ID;
    // Constructor
    Shader() { }
    // Sets the current shader as active
    Shader Use();
    // Compiles the shader from given source code
    void    Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr); // Note: geometry source code is optional
    
    // Utility functions for setting the uniform
    // Utility functions
    void    SetFloat    (const GLchar *name, GLfloat value);
    void    SetInteger  (const GLchar *name, GLint value);
    void    SetVector2f (const GLchar *name, const glm::vec2 &value);
    void    SetVector3f (const GLchar *name, const glm::vec3 &value);
    void    SetVector4f (const GLchar *name, const glm::vec4 &value);
    void    SetMatrix4  (const GLchar *name, const glm::mat4 &matrix);
private:
    // Checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(GLuint object, std::string type);
};

#endif
