//
//  Game.hpp
//  BreakOutGL
//
//  Created by Mac on 3/12/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "BallObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include <tuple>
#include <random>


// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Represents the ball direction in a given collision
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(110, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -475.0f);

// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

//Initial Size of the Guns
const glm::vec2 GUN_SIZE(12, 16);
const glm::vec2 playerPos = glm::vec2(800 / 2 - PLAYER_SIZE.x / 2, 600 - PLAYER_SIZE.y);
const glm::vec2 gun1Pos = playerPos + glm::vec2(PLAYER_SIZE.x / 5 - GUN_SIZE.x, -GUN_SIZE.y);
const glm::vec2 gun2Pos = playerPos + glm::vec2(PLAYER_SIZE.x - PLAYER_SIZE.x / 5, -GUN_SIZE.y);
// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState              State;
    GLboolean              Keys[1024];
    GLuint                 Width, Height;
    std::vector<GameLevel> Levels;
    std::vector<Explosion*> explosions;
    std::vector<PowerUp>    powerups;
    GLuint                 Level;

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    void DoCollisions();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    
    //REset
    void ResetLevel();
    void ResetPlayer();
    
    //Powerups
    void SpawnPowerUps(GameObject &block);
    void UpdatePowerUps(GLfloat dt);
    
private:
    GLboolean shouldSpawn(GLuint chance);
    void ActivatePowerUp(PowerUp &powerUp);
    GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);
    void PowerUpdates(GLfloat dt);
};
#endif
