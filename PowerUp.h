//
//  PowerUp.hpp
//  BreakOutGL
//
//  Created by Mac on 3/23/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef PowerUp_hpp
#define PowerUp_hpp
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Gameobject.h"
#include <stdio.h>

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
    // PowerUp State
    std::string Type;
    GLfloat     Duration;
    GLboolean   Activated;
    // Constructor
    PowerUp(std::string type, glm::vec3 color, GLfloat duration,
            glm::vec2 position, Texture2D texture)
    : GameObject(position, SIZE, texture, color, VELOCITY),
    Type(type), Duration(duration), Activated()
    { }
};

#endif /* PowerUp_hpp */
