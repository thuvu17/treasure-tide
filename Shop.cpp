#include "Shop.hpp"
#include "Utility.h"
#include "helper.hpp"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8
#define NUM_ITEMS 2
#define NUM_GUI_STUFF 1


Shop::~Shop()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Shop::purchase(ShaderProgram *program)
{
    float price = m_state.items[item_number].price;
    std::cout << price << " " << m_player_money;
    if (m_player_money < price)
    {
        not_enough_money = true;
        buy_flippers = false;
        buy_o2 = false;
    }
    else
    {
        m_player_money -= price;
        // If purchased flippers, +1 speed
        if (item_number == 0)
        {
            m_player_speed += 10.0f;
            buy_flippers = true;
            not_enough_money = false;
            buy_o2 = false;
        }
        // If purchased O2, +5s O2
        else if (item_number == 1)
        {
            m_max_o2_level += 10.0f;
            buy_o2 = true;
            not_enough_money = false;
            buy_flippers = false;
        }
    }
}

void Shop::initialise(float max_o2_level, float player_speed, float player_money)
{
    m_state.next_scene_id = -1;
    m_player_money = player_money;
    m_max_o2_level = max_o2_level;
    m_player_speed = player_speed;
    
    // ––––– ITEMS ––––– //
    m_state.items = new Entity[NUM_ITEMS];
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        m_state.items[i].set_entity_type(ITEM);
        m_state.items[i].set_width(1.25f);
        m_state.items[i].set_height(1.25f);
    }
    // Flippers
    m_state.items[0].m_texture_id = Utility::load_texture("assets/objects/flippers.png");
    m_state.items[0].set_position(glm::vec3(-2.0f, 0.0f, 0.0f));
    m_state.items[0].price = 20.0f;
    
    // O2 tanks
    m_state.items[1].m_texture_id = Utility::load_texture("assets/objects/extra_o2.png");
    m_state.items[1].set_position(glm::vec3(2.25f, 0.0f, 0.0f));
    m_state.items[1].price = 10.0f;
    
    // ––––– GUI STUFF ––––– //
    m_state.objects = new Entity[NUM_GUI_STUFF];
    for (int i = 0; i < NUM_GUI_STUFF; i++)
    {
        m_state.objects[i].set_entity_type(OBJECT);
        m_state.objects[i].set_width(2.0f);
        m_state.objects[i].set_height(2.0f);
    }
    // Selector
    m_state.objects[0].set_entity_type(SELECTOR);
    m_state.objects[0].m_texture_id = Utility::load_texture("assets/objects/select.png");
    m_state.objects[0].set_position(m_state.items[item_number].get_position());
    m_state.objects[0].m_speed = 50.0f;
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/sounds/shop.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
}

void Shop::back()
{
    // Reset bool
    buy_o2 = false;
    buy_flippers = false;
    not_enough_money = false;
    
    m_state.next_scene_id = GameState::MAIN;
}

void Shop::update(float delta_time)
{
    // Moving selector
    float target_x = m_state.items[item_number].get_position().x;
    if (abs(m_state.objects[0].get_position().x - target_x) >= 0.5f)
    {
        if (m_state.objects[0].get_position().x < target_x)
        {
            m_state.objects[0].m_movement.x = 1.0f;
        }
        else if (m_state.objects[0].get_position().x > target_x)
        {
            m_state.objects[0].m_movement.x = -1.0f;
        }
    }
    else m_state.objects[0].m_movement.x = 0.0f;
    
    
    for (int i = 0; i < NUM_ITEMS; i++) m_state.items[i].update(delta_time, NULL, NULL, 0, NULL);
    for (int i = 0; i < NUM_GUI_STUFF; i++) m_state.objects[i].update(delta_time, NULL, NULL, 0, NULL);
}

void Shop::render(ShaderProgram *program)
{
    draw_shop(m_state, program);
    // Go back
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::string("GO BACK [ESC]"), 0.25f, 0.0f, glm::vec3(-9.5f, 5.5f, 0.0f));
    
    // Item names
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::string("FLIPPERS"), 0.35f, 0.0f, glm::vec3(-3.0f, 1.5f, 0.0f));
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::string("O2 TANK"), 0.35f, 0.0f, glm::vec3(1.5f, 1.5f, 0.0f));
    
    // Price tags
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::string("$20"), 0.25f, 0.0f, glm::vec3(-2.2f, -1.25f, 0.0f));
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::string("$10"), 0.25f, 0.0f, glm::vec3(2.1f, -1.25f, 0.0f));
    
    
    // Render entities
    for (int i = 0; i < NUM_GUI_STUFF; i++) m_state.objects[i].render(program);
    for (int i = 0; i < NUM_ITEMS; i++) m_state.items[i].render(program);
    
    // Purchase confirmation
    if (not_enough_money)
    {
        Utility::draw_text(program, Utility::load_texture("assets/font1.png"),
                           std::string("Not enough money!"), 0.5f, 0.0f, glm::vec3(-3.5f, -3.0f, 0.0f));
    }
    else if (buy_flippers)
    {
        Utility::draw_text(program, Utility::load_texture("assets/font1.png"),
                           std::string("+10 SPEED"), 0.5f, 0.0f, glm::vec3(-2.0f, -3.0f, 0.0f));
    }
    else if (buy_o2)
    {
        Utility::draw_text(program, Utility::load_texture("assets/font1.png"),
                           std::string("+10 O2"), 0.5f, 0.0f, glm::vec3(-2.0f, -3.0f, 0.0f));
    }
}
