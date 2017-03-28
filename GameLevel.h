//
//  GameLevel.hpp
//  BreakOutGL
//
//  Created by Mac on 3/13/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"

class GameLevel {
public:
    
    std::vector<GameObject> bricks;
    GameLevel(){ };
    void Load(const GLchar * file, GLuint levelWidth, GLuint levelHeight);
    void Draw(SpriteRenderer &Render);
    GLboolean isGameCompleted();
    
private:
    void init(std::vector< std::vector<GLuint>> tiledata, GLuint levelWidth, GLuint levelHeight);
    
    
};

#endif /* GameLevel_hpp */
