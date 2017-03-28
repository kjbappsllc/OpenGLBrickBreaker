//
//  Explosion.hpp
//  BreakOutGL
//
//  Created by Mac on 3/20/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef Explosion_hpp
#define Explosion_hpp

#include <stdio.h>
#include "Shader.h"
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"

class Explosion: public GameObject {
public:
    Explosion(Shader shader, Texture2D texture);
    void Draw();
    GLboolean isCompleted;
    glm::vec2 Position;
    glm::vec2 Velocity;
    Texture2D texture;
    
private:
    GLint time = 0;
    GLuint VAO;
    Shader explosionShader;
    void init();
    
};
#endif /* Explosion_hpp */
