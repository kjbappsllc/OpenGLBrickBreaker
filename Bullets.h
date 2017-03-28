//
//  Bullets.hpp
//  BreakOutGL
//
//  Created by Mac on 3/18/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef Bullets_h
#define Bullets_h

#include <stdio.h>
#include "GameObject.h"
#include "Texture.h"

class Bullet : public GameObject {
public:
    Bullet();
    GLboolean isDestroyed;
    void Move(GLfloat dt);
    
};

#endif /* Bullets_hpp */
