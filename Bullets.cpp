//
//  Bullets.cpp
//  BreakOutGL
//
//  Created by Mac on 3/18/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#include "Bullets.h"

Bullet::Bullet(): GameObject(), isDestroyed(false){}

void Bullet::Move(GLfloat dt){
    if (!isDestroyed) {
        this->Position += this->Velocity * dt;
        
        if(this->Position.y <= 0){
            this->isDestroyed = true;
        }
    }
    
}
