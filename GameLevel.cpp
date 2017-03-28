//
//  GameLevel.cpp
//  BreakOutGL
//
//  Created by Mac on 3/13/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#include "GameLevel.h"

#include <fstream>
#include <sstream>
#include <iostream>

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
    // Clear old data
    this->bricks.clear();
    // Load from file
    GLuint tileCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLuint>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> tileCode) // Read each word seperated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::init(std::vector<std::vector<GLuint> > tiledata, GLuint levelWidth, GLuint levelHeight){
    GLulong height = tiledata.size();
    GLulong width = tiledata[0].size();
    GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
    GLfloat unit_height = levelHeight / static_cast<GLfloat>(height);
    
    for (GLuint y = 0; y < height; ++y) {
        
        for(GLuint x = 0; x < width; ++x) {
            
            if (tiledata[y][x] == 1) {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                Texture2D brickSpriteSolid = ResourceManager::GetTexture("block_solid");
                GameObject obj(pos, size, brickSpriteSolid, glm::vec3(0.34f, 0.34f, 0.34f));
                obj.IsSolid = GL_TRUE;
                this->bricks.push_back(obj);
            }
            
            else if(tiledata[y][x] > 1){
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tiledata[y][x] == 2){
                    color = glm::vec3(0.81f, 0.0f, 0.11f);
                }
                else if (tiledata[y][x] == 3)
                    color = glm::vec3(1.0f, 0.62f, 0.0f);
                else if (tiledata[y][x] == 4)
                    color = glm::vec3(0.29f, 0.22f, 0.85f);
                else if (tiledata[y][x] == 5)
                    color = glm::vec3(0.13f, 0.46f, 1.0f);
                
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                Texture2D brickSpriteNormal = ResourceManager::GetTexture("block_normal");
                
                this->bricks.push_back(GameObject(pos, size, brickSpriteNormal, color));
            }
            
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->bricks){
        if (tile.Destroyed == GL_FALSE){
            tile.Draw(renderer);
        }
    }
}

GLboolean GameLevel::isGameCompleted()
{
    for (GameObject &tile : this->bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return GL_FALSE;
    return GL_TRUE;
}
