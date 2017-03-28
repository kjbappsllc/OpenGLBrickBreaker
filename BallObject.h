//
//  BallObject.hpp
//  BreakOutGL
//
//  Created by Mac on 3/14/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef BallObject_h
#define BallObject_h

#include <stdio.h>
#include "GameObject.h"
#include "Texture.h"

class BallObject : public GameObject {
    
public:
    GLfloat radius;
    GLboolean isStuck;
    GLboolean Sticky, PassThrough;
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
    
    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void      Reset(glm::vec2 position, glm::vec2 velocity);
    
};
#endif /* BallObject_hpp */
