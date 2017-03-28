//
//  Gun.hpp
//  BreakOutGL
//
//  Created by Mac on 3/17/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef Gun_hpp
#define Gun_hpp

#include <stdio.h>
#include "GameObject.h"
#include "Texture.h"
#include "Bullets.h"
#include <vector>
#include "SpriteRenderer.h"

class Gun : public GameObject {
public:
    
    Gun(glm::vec2 pos, glm::vec2 size, Texture2D sprite, GLint ammoSize);
    std::vector<Bullet> bullets;
    void shoot(Texture2D sprite);
    void update(GLfloat dt);
    void reload();
    void clear();
    void drawBullets(SpriteRenderer &renderer);
    GLboolean ammoIsOut();
    GLint ammoLeft;
    
private:
    GLint ammoSize;
    GLint time = 11;
};

#endif /* Gun_hpp */
