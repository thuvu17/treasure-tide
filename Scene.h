#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

struct GameState
{
    Map    *map;
    Entity *player;
    Entity *enemies;
    Entity *objects;
    Entity *chest;
    Entity *items;
    Entity *tanks;
    Entity *background;
    Entity *consumables;
    
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;
    
    int next_scene_id;
    static const int MAIN = 1,
                     SHOP = 2,
                     WIN  = 3,
                     LOSE = 4;
};

class Scene {
public:
    int m_number_of_enemies = 1;
    // Player stats
    float m_player_money;
    float m_player_speed;
    float m_max_o2_level;
//    float start_timer;
    
    GameState m_state;
    
    virtual void initialise(float max_o2_level, float player_speed, float player_money) = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    
    GameState const get_state() const { return m_state; }
};
