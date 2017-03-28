//
//  Explosion.cpp
//  BreakOutGL
//
//  Created by Mac on 3/20/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#include "Explosion.h"

Explosion::Explosion(Shader shader, Texture2D texture): Position(glm::vec2(0,0)), Velocity(glm::vec2(0,0)), isCompleted(false), explosionShader(shader), texture(texture){
    this->init();
}

void Explosion::Draw() {
    if(isCompleted == false){
        this->explosionShader.Use();
        const int over = 90;
        if(this->time < over){
            GLint subTexture = (this->time * 36) / over;
            this->explosionShader.SetInteger("subTexture", subTexture);
            this->explosionShader.SetVector2f("offset", this->Position);
            
            glActiveTexture(GL_TEXTURE0);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
            
            this->time ++;
        }
        else {
            isCompleted = true;
        }
    }
    
}

void Explosion::init() {
    
    GLuint VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    GLfloat explosion_vertices[] = {
        //Vertex    //TextCoord
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(explosion_vertices), explosion_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    
}
