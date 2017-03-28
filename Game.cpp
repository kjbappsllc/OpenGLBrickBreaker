//
//  Game.cpp
//  BreakOutGL
//
//  Created by Mac on 3/12/17.
//  Copyright © 2017 KJB Apps LLC. All rights reserved.
//

#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Gun.h"
#include "Bullets.h"
#include "Shader.h"
#include <iostream>
#include "Texture.h"
#include <vector>
#include "ParticleGenerator.h"



// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;
Gun             *Gun1;
Gun             *Gun2;
ParticleGenerator   *particles;
BallObject          *Ball;

Game::Game(GLuint width, GLuint height)
: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete particles;
    delete Gun1;
    delete Gun2;
    
    for (int i = 0; i < explosions.size(); i ++) {
        delete explosions[i];
    }
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("res/shaders/texturedQuad.vs", "res/shaders/texturedQuad.frag", nullptr, "sprite");
    ResourceManager::LoadShader("res/shaders/particles.vs", "res/shaders/particles.frag", nullptr, "particle");
    ResourceManager::LoadShader("res/shaders/Animation.vs", "res/shaders/Animation.frag", nullptr, "animation");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("spriteColor", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    ResourceManager::GetShader("animation").Use().SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("res/images/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("res/images/block_normal.png", true, "block_normal");
    ResourceManager::LoadTexture("res/images/block_solid.png",true, "block_solid");
    ResourceManager::LoadTexture("res/images/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("res/images/ball.png", true, "ball");
    ResourceManager::LoadTexture("res/images/Star.png", true, "particle");
    ResourceManager::LoadTexture("res/images/Gun.png", true, "gun");
    ResourceManager::LoadTexture("res/images/Bullet.png", true, "bullet");
    ResourceManager::LoadTexture("res/images/boom.png", true, "explosion");
    ResourceManager::LoadTexture("res/images/Ammo.png", true, "ammo");
    ResourceManager::LoadTexture("res/images/PaddleDecrease.png", true, "padDecrease");
    ResourceManager::LoadTexture("res/images/PaddleIncrease.png", true, "padIncrease");
    ResourceManager::LoadTexture("res/images/PassThrough.png", true, "passThrough");
    ResourceManager::LoadTexture("res/images/speed.png", true, "speed");
    ResourceManager::LoadTexture("res/images/sticky.png", true, "sticky");
    // Set render-specific controls
    Shader spriteShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(spriteShader);
    Shader particlesShader = ResourceManager::GetShader("particle");
    Texture2D particleTexture = ResourceManager::GetTexture("particle");
    particles = new ParticleGenerator(particlesShader, particleTexture, 500);
    
    // Load levels
    GameLevel one; one.Load("res/levels/one.level", this->Width, this->Height * 0.4);
    GameLevel two; two.Load("res/levels/two.level", this->Width, this->Height * 0.4);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Level = 0;
    
    // Configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    glm::vec2 gun1Pos = playerPos + glm::vec2(PLAYER_SIZE.x / 5 - GUN_SIZE.x, -GUN_SIZE.y);
    glm::vec2 gun2Pos = playerPos + glm::vec2(PLAYER_SIZE.x - PLAYER_SIZE.x / 5, -GUN_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                          ResourceManager::GetTexture("ball"));
    
    Gun1 = new Gun(gun1Pos, GUN_SIZE, ResourceManager::GetTexture("gun"), 20);
    Gun2 = new Gun(gun2Pos, GUN_SIZE, ResourceManager::GetTexture("gun"), 20);
}

void Game::Update(GLfloat dt)
{
    // Update objects
    if(this->explosions.size() != 0){
        for (int i = 0; i < explosions.size(); i++) {
            if(explosions[i]->isCompleted){
                explosions.erase(explosions.begin() + i);
            }
        }
    }
    this->PowerUpdates(dt);
    Ball->Move(dt, this->Width);
    Gun1->update(dt);
    Gun2->update(dt);
    // Check for collisions
    this->DoCollisions();
    particles->Update(dt, *Ball, 2, glm::vec2(Ball->radius / 2));
    // Check loss condition
    if (Ball->Position.y >= this->Height) // Did ball reach bottom edge?
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // Move playerboard
        if (this->Keys[GLFW_KEY_A] || this-> Keys[GLFW_KEY_LEFT])
        {
            if (Player->Position.x >= 0)
            {
                Player->Position.x -= velocity;
                Gun1->Position.x -= velocity;
                Gun2->Position.x -= velocity;
                if (Ball->isStuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D] || this-> Keys[GLFW_KEY_RIGHT])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                Gun1->Position.x += velocity;
                Gun2->Position.x += velocity;
                if (Ball->isStuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            Ball->isStuck = false;
        
        if(this->Keys[GLFW_KEY_K] || this-> Keys[GLFW_KEY_UP]){
            if (!Ball->isStuck) {
                Gun1->shoot(ResourceManager::GetTexture("bullet"));
                Gun2->shoot(ResourceManager::GetTexture("bullet"));
            }
        }
        if(this->Keys[GLFW_KEY_R]){
            if(Gun1->ammoIsOut() && Gun2->ammoIsOut()){
                Gun1->reload();
                Gun2->reload();
            }
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("background");
        // Draw background
        Renderer->DrawSprite(myTexture, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        // Draw level
        GameLevel current = this->Levels[this->Level];
        current.Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);
        Gun1->Draw(*Renderer);
        Gun2->Draw(*Renderer);
        Gun1->drawBullets(*Renderer);
        Gun2->drawBullets(*Renderer);
        // Draw particles
        particles->Draw();
        //Draw Player
        Ball->Draw(*Renderer);
        
        for (PowerUp &powerUp : this->powerups){
            if (!powerUp.Destroyed)
            powerUp.Draw(*Renderer);
        }
        
        if(this->explosions.size() != 0) {
            for(Explosion * explode: explosions){
                explode->Draw();
            }
        }
    }
}

void Game::ResetLevel()
{
    Gun1->clear();
    Gun2->clear();
    if (this->Level == 0)this->Levels[0].Load("res/levels/one.level", this->Width, this->Height * 0.4f);
    else if (this->Level == 1)
        this->Levels[1].Load("res/levels/two.level", this->Width, this->Height * 0.4f);
  
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->Size = PLAYER_SIZE;
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player->Position = playerPos;
    Gun1->Position = glm::vec2(playerPos + glm::vec2(PLAYER_SIZE.x / 5 - GUN_SIZE.x, -GUN_SIZE.y));
    Gun2->Position = glm::vec2(playerPos + glm::vec2(PLAYER_SIZE.x - PLAYER_SIZE.x / 5, -GUN_SIZE.y));
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
    Ball->isStuck = GL_TRUE;
    Gun1->ammoLeft = 0;
    Gun2->ammoLeft = 0;
    Ball->PassThrough = Ball->Sticky = GL_FALSE;
    Player->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
    this->powerups.clear();
    this->explosions.clear();
}

GLboolean CheckCollisions(Bullet &one, GameObject &two);
GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // If collision is true
            {
                // Destroy block if not solid
                if (!box.IsSolid){
                    this->SpawnPowerUps(box);
                    box.Destroyed = GL_TRUE;
                }
                // Collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if(!(Ball->PassThrough && !box.IsSolid)){
                    if (dir == LEFT || dir == RIGHT) // Horizontal collision
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // Reverse horizontal velocity
                        // Relocate
                        GLfloat penetration = Ball->radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->Position.x += penetration; // Move ball to right
                        else
                            Ball->Position.x -= penetration; // Move ball to left;
                    }
                    else // Vertical collision
                    {
                        Ball->Velocity.y = -Ball->Velocity.y; // Reverse vertical velocity
                        // Relocate
                        GLfloat penetration = Ball->radius - std::abs(diff_vector.y);
                        if (dir == UP)
                            Ball->Position.y -= penetration; // Move ball bback up
                        else
                            Ball->Position.y += penetration; // Move ball back down
                    }
                }
            }
            
            for (Bullet &bullet:Gun1->bullets) {
                if (CheckCollisions(bullet, box)) {
                    if(!box.IsSolid){
                        Explosion * boom = new Explosion(ResourceManager::GetShader("animation"), ResourceManager::GetTexture("explosion"));
                        boom->Position = box.Position;
                        explosions.push_back(boom);
                        box.Destroyed = GL_TRUE;
                        this->SpawnPowerUps(box);
                    }
                    bullet.isDestroyed = true;
                }
            }
            
            for (Bullet &bullet:Gun2->bullets) {
                if (CheckCollisions(bullet, box)) {
                    if(!box.IsSolid){
                        Explosion * boom = new Explosion(ResourceManager::GetShader("animation"), ResourceManager::GetTexture("explosion"));
                        boom->Position = box.Position;
                        explosions.push_back(boom);
                        box.Destroyed = GL_TRUE;
                        this->SpawnPowerUps(box);
                    }
                    bullet.isDestroyed = true;
                }
            }
            
            for (PowerUp &powerUp : this->powerups)
            {
                if (!powerUp.Destroyed)
                {
                    if (powerUp.Position.y >= this->Height)
                    powerUp.Destroyed = GL_TRUE;
                    if (CheckCollision(*Player, powerUp))
                    {	// Collided with player, now activate powerup
                        ActivatePowerUp(powerUp);
                        powerUp.Destroyed = GL_TRUE;
                        powerUp.Activated = GL_TRUE;
                    }
                }
            }
            
        }
    }
    // Also check collisions for player pad (unless stuck)
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->isStuck && std::get<0>(result))
    {
        // Check where it hit the board, and change velocity based on where it hit the board
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        // Then move accordingly
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        // Fix sticky paddle
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        Ball->isStuck = Ball->Sticky;
    }
}

GLboolean CheckCollisions(Bullet &one, GameObject &two)
{
    // Collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
    two.Position.x + two.Size.x >= one.Position.x;
    // Collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
    two.Position.y + two.Size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

GLboolean CheckCollision(GameObject &one, GameObject &two)
{
    // Collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
    two.Position.x + two.Size.x >= one.Position.x;
    // Collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
    two.Position.y + two.Size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // Get center point circle first
    glm::vec2 center(one.Position + one.radius);
    // Calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // Get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // Now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;
    
    if (glm::length(difference) < one.radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

GLboolean Game::shouldSpawn(GLuint chance)
{
    GLuint random = rand() % chance;
    return random == 0;
}

void Game::SpawnPowerUps(GameObject &block)
{
    if (shouldSpawn(50)) // 1 in 75 chance
    this->powerups.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("speed")));
    if (shouldSpawn(50))
    this->powerups.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("sticky")));
    if (shouldSpawn(50))
    this->powerups.push_back(PowerUp("pass_through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("passThrough")));
    if (shouldSpawn(50))
    this->powerups.push_back(PowerUp("pad_increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("padIncrease")));
    if (shouldSpawn(50))
    this->powerups.push_back(PowerUp("ammo", glm::vec3(0.29f, 0.29f, 0.29f), 0.0f, block.Position, ResourceManager::GetTexture("ammo")));
    if (shouldSpawn(10))
    this->powerups.push_back(PowerUp("pad_decrease", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("padDecrease")));
 }

void Game::ActivatePowerUp(PowerUp &powerUp)
{
    // Initiate a powerup based type of powerup
    if (powerUp.Type == "speed")
    {
        Ball->Velocity *= 1.35;
    }
    else if (powerUp.Type == "sticky")
    {
        Ball->Sticky = GL_TRUE;
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass_through")
    {
        Ball->PassThrough = GL_TRUE;
        Ball->Color = glm::vec3(1.0f, 0.0f, 0.12f);
    }
    else if (powerUp.Type == "pad_increase")
    {
        if(Player->Size.x < 160)
            Player->Size.x += 50;
        
        glm::vec2 gun1Pos = Player->Position + glm::vec2(Player->Size.x/ 5 - GUN_SIZE.x, -GUN_SIZE.y);
        glm::vec2 gun2Pos = Player->Position + glm::vec2(Player->Size.x - Player->Size.x/ 5, -GUN_SIZE.y);
        Gun1->Position = gun1Pos;
        Gun2->Position = gun2Pos;
    }
    else if (powerUp.Type == "pad_decrease")
    {
        if(Player->Size.x > 70)
            Player->Size.x -= 50;
        glm::vec2 gun1Pos = Player->Position + glm::vec2(Player->Size.x / 5 - GUN_SIZE.x, -GUN_SIZE.y);
        glm::vec2 gun2Pos = Player->Position + glm::vec2(Player->Size.x - Player->Size.x / 5, -GUN_SIZE.y);
        Gun1->Position = gun1Pos;
        Gun2->Position = gun2Pos;
    }
    else if (powerUp.Type == "ammo")
    {
        Gun1->reload();
        Gun2->reload();
    }
}

GLboolean Game::IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.Activated)
        if (powerUp.Type == type)
        return GL_TRUE;
    }
    return GL_FALSE;
}

void Game::PowerUpdates(GLfloat dt)
{
    for (PowerUp &powerUp : this->powerups)
    {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;
            
            if (powerUp.Duration <= 0.0f)
            {
                // Remove powerup from list (will later be removed)
                powerUp.Activated = GL_FALSE;
                // Deactivate effects
                if (powerUp.Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->powerups, "sticky"))
                    {	// Only reset if no other PowerUp of type sticky is active
                        Ball->Sticky = GL_FALSE;
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass_through")
                {
                    if (!IsOtherPowerUpActive(this->powerups, "pass-through"))
                    {	// Only reset if no other PowerUp of type pass-through is active
                        Ball->PassThrough = GL_FALSE;
                        Ball->Color = glm::vec3(1.0f);
                    }
                }
                
                else if (powerUp.Type == "speed")
                {
                    if(!IsOtherPowerUpActive(this->powerups, "speed"))
                    {
                        Ball->Velocity = INITIAL_BALL_VELOCITY;
                    }
                }
            }
        }
    }
    this->powerups.erase(std::remove_if(this->powerups.begin(), this->powerups.end(),
                                        [](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
                                        ), this->powerups.end());
}
