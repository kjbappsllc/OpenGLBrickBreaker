//
//  GameObject.cpp
//  BreakOutGL
//
//  Created by Mac on 3/13/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
: Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) :
    Position(pos), Size(size), Sprite(sprite), Color(color), Velocity(velocity), IsSolid(GL_FALSE), Destroyed(GL_FALSE){}

void GameObject::Draw(SpriteRenderer &renderer){
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
