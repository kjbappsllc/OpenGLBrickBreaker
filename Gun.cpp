//
//  Gun.cpp
//  BreakOutGL
//
//  Created by Mac on 3/17/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#include "Gun.h"
#include "Texture.h"

Gun::Gun(glm::vec2 pos, glm::vec2 size, Texture2D sprite, GLint ammoSize) :
GameObject(pos,size,sprite, glm::vec3(1.0), glm::vec2(0.0,0.0)), ammoSize(ammoSize) {
    this->ammoLeft = 0;
}

void Gun::reload(){
    ammoLeft = ammoSize;
    time = 11;
}

GLboolean Gun::ammoIsOut(){
    return ammoLeft <= 0;
}

void Gun::shoot(Texture2D sprite){
    if(!ammoIsOut()){
        time++;
        if (time/12 == 1) {
            ammoLeft--;
            Bullet bullet;
            bullet.Position = this->Position;
            bullet.Position.y -= this->Size.y;
            bullet.Size = glm::vec2(16, 30);
            bullet.Velocity = glm::vec2(0, -450.0f);
            bullet.Sprite = sprite;
            bullets.push_back(bullet);
            time = 0;
        }
    }
}

void Gun::update(GLfloat dt){
    for(Bullet &bullet:bullets){
        bullet.Move(dt);
    }
    
    if(bullets.size() != 0){
        for (int i = 0; i < bullets.size(); i++) {
            if(bullets[i].isDestroyed){
                bullets.erase(bullets.begin() + i);
            }
        }
    }
}

void Gun::drawBullets(SpriteRenderer &renderer){
    for(Bullet &bullet:bullets){
        if(!bullet.isDestroyed){
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            bullet.Draw(renderer);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
}

void Gun::clear(){
    bullets.clear();
}
