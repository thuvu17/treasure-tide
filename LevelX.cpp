#include "LevelX.hpp"
#include "Utility.h"
#include "helper.hpp"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

LevelX::~LevelX()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelX::initialise(float max_o2_level, float player_speed, float player_money)
{
    m_state.next_scene_id = -1;
    m_player_money = player_money;
    m_max_o2_level = max_o2_level;
    m_player_speed = player_speed;
    
    /**
     BGM and SFX
     */
//    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
//    
//    m_state.bgm = Mix_LoadMUS("assets/sounds/bgm.mp3");
//    Mix_PlayMusic(m_state.bgm, -1);
//    Mix_VolumeMusic(2.0f);
}

void LevelX::update(float delta_time)
{
}

void LevelX::render(ShaderProgram *program)
{
    draw_homepage(m_state, program);
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::string("TREASURE TIDE"), 0.7f, 0.0f, glm::vec3(-3.25f, 2.0f, 0.0f));
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::string("Press enter to start"), 0.4f, 0.0f, glm::vec3(-3.0f, 0.0f, 0.0f));
}
