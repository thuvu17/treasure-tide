#include "LevelA.h"
#include "Utility.h"
#include "helper.hpp"

#define LEVEL_WIDTH 35
#define LEVEL_HEIGHT 22
#define NUM_TANK 2
#define NUM_ENEMIES 3
#define NUM_OBJECTS 1
#define NUM_CONSUMABLES 3

unsigned int LEVELA_DATA[] = {};

// ––––– POSITIONS ––––– //
// tanks
glm::vec3 tank_0_pos = glm::vec3(2.0f, -21.5f, 0.0f),
          tank_1_pos = glm::vec3(20.0f, -21.5f, 0.0f);
// enemies
glm::vec3 enemy_0_pos = glm::vec3(5.0f, -13.0f, 0.0f),
          enemy_1_pos = glm::vec3(25.0f, -18.0f, 0.0f),
          enemy_2_pos = glm::vec3(12.0f, -9.5f, 0.0f);

// consumables
glm::vec3 consum_0_pos = glm::vec3(5.0f, -9.0f, 0.0f),
          consum_1_pos = glm::vec3(10.0f, -12.0f, 0.0f),
          consum_2_pos = glm::vec3(22.0f, -14.0f, 0.0f);

// objects
glm::vec3 chest_pos = glm::vec3(LEVEL_WIDTH - 2.5f, -LEVEL_HEIGHT + 1.0f, 0.0f),
          boatshop_pos = glm::vec3(3.5f, -6.0f, 0.0f);

LevelA::~LevelA()
{
    delete    m_state.enemies;
    delete    m_state.objects;
    delete    m_state.background;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelA::initialise(float max_o2_level, float player_speed, float player_money)
{
    std::cout<<"A init\n";
    m_state.next_scene_id = -1;
    m_player_money = player_money;
    m_max_o2_level = max_o2_level;
    m_player_speed = player_speed;
    
    GLuint map_texture_id = Utility::load_texture("assets/tileset/tiles.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 4, 1);

    // ––––– PLAYER ––––– //
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(1.0f, -7.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->m_speed = player_speed;
    m_state.player->m_money = player_money;
    m_state.player->m_oxygen_level = max_o2_level;
    m_state.player->m_max_oxygen = max_o2_level;
    m_state.player->set_acceleration(glm::vec3(0.0f, -4.905f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/entities/player.png");
    
    // Player animation
    m_state.player->m_walking[m_state.player->LEFT]  = new int[4] { 4,   5,  6,  7 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 8,   9, 10, 12 };
    m_state.player->m_walking[m_state.player->UP]    = new int[4] { 12, 13, 14, 15 };
    m_state.player->m_walking[m_state.player->DOWN]  = new int[4] { 0,   1,  2,  3 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index  = 0;
    m_state.player->m_animation_time   = 0.0f;
    m_state.player->m_animation_cols   = 4;
    m_state.player->m_animation_rows   = 4;
    
    m_state.player->set_width(1.0f);
    m_state.player->set_height(1.0f);
    
    // Jumping
    m_state.player->m_jumping_power = 5.0f;
    
    // ––––– ENEMIES ––––– //
    m_state.enemies = new Entity[NUM_ENEMIES];
    
    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        // Set entity and AI type
        m_state.enemies[i].set_entity_type(ENEMY);
        m_state.enemies[i].set_ai_state(IDLE);
    }
    
    // Walker 1
    m_state.enemies[0].set_position(enemy_0_pos);
    m_state.enemies[0].set_ai_type(WALKER);
    m_state.enemies[0].m_speed = 100.0f;
    m_state.enemies[0].set_width(5.0f);
    m_state.enemies[0].set_height(3.0f);
    
    m_state.enemies[0].m_texture_id = Utility::load_texture("assets/entities/whale.png");
    m_state.enemies[0].m_animation_frames = 10;
    m_state.enemies[0].m_animation_cols   = 10;
    m_state.enemies[0].m_animation_rows   = 2;
    m_state.enemies[0].m_walking[m_state.enemies[0].LEFT]  = new int[10] { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    m_state.enemies[0].m_walking[m_state.enemies[0].RIGHT] = new int[10] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    m_state.enemies[0].m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
    
    // Walker 2
    m_state.enemies[2].set_position(enemy_2_pos);
    m_state.enemies[2].set_ai_type(WALKER);
    m_state.enemies[2].m_speed = 150.0f;
    m_state.enemies[2].set_width(3.0f);
    m_state.enemies[2].set_height(3.0f);
    
    m_state.enemies[2].m_texture_id = Utility::load_texture("assets/entities/enemy.png");
    m_state.enemies[2].m_animation_frames = 4;
    m_state.enemies[2].m_animation_cols   = 4;
    m_state.enemies[2].m_animation_rows   = 2;
    m_state.enemies[2].m_walking[m_state.enemies[2].LEFT]  = new int[4] { 4, 5, 6, 7 };
    m_state.enemies[2].m_walking[m_state.enemies[2].RIGHT] = new int[4] { 0, 1, 2, 3};
    m_state.enemies[2].m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
    
    // Chaser
    m_state.enemies[1].set_position(enemy_1_pos);
    m_state.enemies[1].set_ai_type(GUARD);
    m_state.enemies[1].m_speed = 200.0f;
    m_state.enemies[1].set_width(4.0f);
    m_state.enemies[1].set_height(2.5f);
    
    m_state.enemies[1].m_texture_id = Utility::load_texture("assets/entities/swordfish.png");
    m_state.enemies[1].m_animation_frames = 6;
    m_state.enemies[1].m_animation_cols   = 6;
    m_state.enemies[1].m_animation_rows   = 2;
    m_state.enemies[1].m_walking[m_state.enemies[1].LEFT]  = new int[6] { 6, 7, 8, 9, 10, 11 };
    m_state.enemies[1].m_walking[m_state.enemies[1].RIGHT] = new int[6] { 0, 1, 2, 3, 4, 5 };
    m_state.enemies[1].m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
    
    // ––––– TANKS ––––– //
    m_state.tanks = new Entity[NUM_TANK];
    for (int i = 0; i < NUM_TANK; i++)
    {
        m_state.tanks[i].set_entity_type(TANK);
        m_state.tanks[i].m_texture_id = Utility::load_texture("assets/objects/o2_tank.png");
        m_state.tanks[i].set_width(1.75);
        m_state.tanks[i].set_height(1.0f);
    }
    m_state.tanks[0].set_position(tank_0_pos);
    m_state.tanks[1].set_position(tank_1_pos);
    
    // ––––– TREASURE CHEST ––––– //
    m_state.chest = new Entity();
    m_state.chest->set_entity_type(CHEST);
    m_state.chest->m_texture_id = Utility::load_texture("assets/objects/chest.png");
    m_state.chest->set_width(4.0f);
    m_state.chest->set_height(2.0f);
    m_state.chest->set_position(chest_pos);
    
    m_state.chest->m_chest_animation[m_state.chest->CLOSE]  = new int[5] { 20, 21, 22, 23, 24 };
    m_state.chest->m_chest_animation[m_state.chest->OPEN]   = new int[5] { 25, 26, 27, 28, 29 };

    m_state.chest->m_animation_indices = m_state.chest->m_chest_animation[m_state.chest->CLOSE];
    m_state.chest->m_animation_frames = 5;
    m_state.chest->m_animation_cols   = 5;
    m_state.chest->m_animation_rows   = 8;
    
    // ––––– OBJECTS ––––– //
    m_state.objects = new Entity[NUM_OBJECTS];
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        m_state.objects[i].set_entity_type(OBJECT);
    }
    
    // Boat shop
    m_state.objects[0].m_texture_id = Utility::load_texture("assets/objects/boat.png");
    m_state.objects[0].set_position(boatshop_pos);
    m_state.objects[0].set_width(3.0f);
    m_state.objects[0].set_height(2.0f);
    
    // ––––– CONSUMABLES ––––– //
    m_state.consumables = new Entity[NUM_CONSUMABLES];
    
    for (int i = 0; i < NUM_CONSUMABLES; i++)
    {
        // Set entity and AI type
        m_state.consumables[i].set_entity_type(CONSUMABLE);
        m_state.consumables[i].set_ai_type(WALKER);
        m_state.consumables[i].set_ai_state(IDLE);
        m_state.consumables[i].price = 10.0f;
        m_state.consumables[i].m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    
    // Positions
    m_state.consumables[0].set_position(consum_0_pos);
    m_state.consumables[1].set_position(consum_1_pos);
    m_state.consumables[2].set_position(consum_2_pos);
    
    // Consumable 0
    m_state.consumables[0].m_texture_id = Utility::load_texture("assets/entities/edible.png");
    m_state.consumables[0].m_animation_frames = 4;
    m_state.consumables[0].m_animation_cols   = 4;
    m_state.consumables[0].m_animation_rows   = 1;
    
    m_state.consumables[0].m_walking[m_state.consumables[0].LEFT]  = new int[4] { 0, 1, 2, 3 };
    m_state.consumables[0].m_walking[m_state.consumables[0].RIGHT] = new int[4] { 0, 1, 2, 3 };
    
    // Consumable 1
    m_state.consumables[1].m_texture_id = Utility::load_texture("assets/entities/jellyfish.png");
    m_state.consumables[1].m_animation_frames = 4;
    m_state.consumables[1].m_animation_cols   = 4;
    m_state.consumables[1].m_animation_rows   = 1;
    
    m_state.consumables[1].set_width(0.8f);
    m_state.consumables[1].set_height(1.0f);
    
    m_state.consumables[1].m_walking[m_state.consumables[1].LEFT]  = new int[4] { 0, 1, 2, 3 };
    m_state.consumables[1].m_walking[m_state.consumables[1].RIGHT] = new int[4] { 0, 1, 2, 3 };
    
    // Consumable 2
    m_state.consumables[2].m_texture_id = Utility::load_texture("assets/entities/octopus.png");
    m_state.consumables[2].m_animation_frames = 4;
    m_state.consumables[2].m_animation_cols   = 6;
    m_state.consumables[2].m_animation_rows   = 2;
    
    m_state.consumables[2].set_width(1.5f);
    m_state.consumables[2].set_height(1.5f);
    
    m_state.consumables[2].m_speed = 75.0f;
    m_state.consumables[2].m_walking[m_state.consumables[2].RIGHT]  = new int[6] { 0, 1, 2, 3, 4, 5 };
    m_state.consumables[2].m_walking[m_state.consumables[2].LEFT] = new int[6] { 6, 7, 8, 9, 10, 11 };
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_state.bgm = Mix_LoadMUS("assets/sounds/bgm.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    
}

void LevelA::update(float delta_time)
{
    std::cout<<"A update\n";
    // ––––– UPDATING ENTITIES ––––– //
    m_state.player->update(delta_time, m_state.player, m_state.enemies, NUM_ENEMIES, m_state.map);
    m_state.chest->update(delta_time, m_state.player, NULL, 0, m_state.map);
    for (int i = 0; i < NUM_CONSUMABLES; i ++) m_state.consumables[i].update(delta_time, m_state.player,m_state.player, 1, m_state.map);
    for (int i = 0; i < NUM_OBJECTS; i ++) m_state.objects[i].update(delta_time, m_state.player, NULL, 0, m_state.map);
    for (int i = 0; i < NUM_ENEMIES; i++) m_state.enemies[i].update(delta_time, m_state.player, NULL, 0, m_state.map);
    for (int i = 0; i < NUM_TANK; i ++) m_state.tanks[i].update(delta_time, m_state.player, NULL, 0, m_state.map);

    // ––––– OXYGEN STUFF ––––– //
    // If above sea level, gain oyxgen until full
    if (m_state.player->get_position().y >= -7.5f)
    {
        if (m_state.player->m_oxygen_level == m_state.player->m_max_oxygen) m_state.player->m_oxgyen_decay = 0.0f;
        else if (m_state.player->m_oxygen_level > m_state.player->m_max_oxygen) m_state.player->m_oxgyen_decay = 1.0f;
        else m_state.player->m_oxgyen_decay = -3.0f;
    }
    // If underwater, lose 1 unit every second unless at oxygen tank
    else
    {
        float player_x_pos = m_state.player->get_position().x;
        float player_y_pos = m_state.player->get_position().y;
        // If at any oxygen tank, increase by 1 unit /s
        for (int i = 0; i < NUM_TANK; i++)
        {
            float tank_x_pos = m_state.tanks[i].get_position().x;
            float tank_y_pos = m_state.tanks[i].get_position().y;
            if (player_x_pos >= tank_x_pos - 0.875f and
                player_x_pos <= tank_x_pos + 0.875f and
                player_y_pos <= tank_y_pos + 2.0f   and
                m_state.player->m_oxygen_level <= m_state.player->m_max_oxygen - 3.0f * delta_time)
            {
                m_state.player->m_oxgyen_decay = -3.0f;
                break;
            }
            else m_state.player->m_oxgyen_decay = 1.0f;
        }
    }
    m_state.player->m_oxygen_level -= m_state.player->m_oxgyen_decay * delta_time;
    
    // ––––– SWITCH SCENE CONDITIONS ––––– //
    // If player died
    if (m_state.player->m_oxygen_level <= 0.0f || !m_state.player->is_active())
    {
        m_state.next_scene_id = GameState::LOSE;
        return;
    }
    
    // If player found chest
    if (m_state.player->check_collision(m_state.chest))
    {
        m_state.next_scene_id = GameState::WIN;
        return;
    }
    
    // If player go to shop
    if (m_state.player->check_collision(&m_state.objects[0]))
    {
        m_state.next_scene_id = GameState::SHOP;
    }
    m_player_money = m_state.player->m_money;   // Update money
}

void LevelA::render(ShaderProgram *program)
{
    draw_background(m_state, program);
    m_state.objects[0].render(program);
    m_state.map->render(program);
    m_state.chest->render(program);
    for (int i = 0; i < NUM_TANK; i++) m_state.tanks[i].render(program);
    for (int i = 1; i < NUM_OBJECTS; i++) m_state.objects[i].render(program);
    for (int i = 0; i < NUM_ENEMIES; i++) m_state.enemies[i].render(program);
    for (int i = 0; i < NUM_CONSUMABLES; i++) m_state.consumables[i].render(program);
    m_state.player->render(program);
}
