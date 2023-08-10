//
//  helper.cpp
//  SDLProject
//
//  Created by Sebastián Romero Cruz on 5/31/22.
//  Copyright © 2022 ctg. All rights reserved.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

// We might not need all these libraries yet, but I'll leave them there just in case
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Scene.h"
#include "Utility.h"
#include <stdio.h>

const char MAT_SEP = '\t';
const int FONTBANK_SIZE = 16;

/**
 * Prints a formatted representation of a size x size matrix onto
 * the user's console. The \t character is used for horizontal
 * separation.
 *
 * For example, printing a 4 x 4 identity matrix
 *
 *      print_matrix(glm::mat4(1.0f), 4);
 *
 * Prints:
 *
 *     1    0    0    0
 *     0    1    0    0
 *     0    0    1    0
 *     0    0    0    1
 *
 * @param matrix Reference to the matrix.
 * @param size The size of the matrix
 */

void DrawText(ShaderProgram *program, GLuint font_texture_id, std::string text, float screen_size, float spacing, glm::vec3 position)
{
    // Scale the size of the fontbank in the UV-plane
    // We will use this for spacing and positioning
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    // Instead of having a single pair of arrays, we'll have a series of pairs—one for each character
    // Don't forget to include <vector>!
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;

    // For every character...
    for (int i = 0; i < text.size(); i++) {
        // 1. Get their index in the spritesheet, as well as their offset (i.e. their position
        //    relative to the whole sentence)
        int spritesheet_index = (int) text[i];  // ascii value of character
        float offset = (screen_size + spacing) * i;
        
        // 2. Using the spritesheet index, we can calculate our U- and V-coordinates
        float u_coordinate = (float) (spritesheet_index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v_coordinate = (float) (spritesheet_index / FONTBANK_SIZE) / FONTBANK_SIZE;

        // 3. Inset the current pair in both vectors
        vertices.insert(vertices.end(), {
            offset + (-0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
        });

        texture_coordinates.insert(texture_coordinates.end(), {
            u_coordinate, v_coordinate,
            u_coordinate, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate + width, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate, v_coordinate + height,
        });
    }

    // 4. And render all of them using the pairs
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);
    
    program->SetModelMatrix(model_matrix);
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int) (text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void print_matrix(glm::mat4 &matrix, int size)
{
    for (auto row = 0 ; row < size ; row++)
    {
        for (auto col = 0 ; col < size ; col++)
        {
            // Print row
            std::cout << matrix[row][col] << MAT_SEP;
        }
        
        // Visually "move on" to the next row
        std::cout << "\n";
    }
}

void draw_background(GameState &m_state, ShaderProgram *program)
{
    // Initialise
    int entity_num = 7;
    m_state.background = new Entity[entity_num];
    for (int i = 0; i < entity_num; i++)
    {
        m_state.background[i].set_entity_type(BACKGROUND);
    }
    
    // Bottom layer
    m_state.background[0].set_width(35.0f);
    m_state.background[0].set_height(10.0f);
    m_state.background[0].m_texture_id = Utility::load_texture("assets/tileset/bottom.png");
    m_state.background[0].m_model_matrix = glm::translate(m_state.background[0].m_model_matrix, glm::vec3(17.5f, -17.0f, 1.0f));
    
    // Top layer
    m_state.background[1].set_width(35.0f);
    m_state.background[1].set_height(5.0f);
    m_state.background[1].m_texture_id = Utility::load_texture("assets/tileset/top.png");
    m_state.background[1].m_model_matrix = glm::translate(m_state.background[1].m_model_matrix, glm::vec3(17.5f, -9.5f, 1.0f));
    
    // Foreground
    m_state.background[2].set_width(30.0f);
    m_state.background[2].set_height(8.0);
    m_state.background[2].m_texture_id = Utility::load_texture("assets/tileset/full_bg.png");
    m_state.background[2].m_model_matrix = glm::translate(m_state.background[2].m_model_matrix, glm::vec3(15.0f, -18.0f, 1.0f));
    
    // Sky
    m_state.background[3].set_width(35.0f);
    m_state.background[3].set_height(7.0f);
    m_state.background[3].m_texture_id = Utility::load_texture("assets/tileset/sky.png");
    m_state.background[3].m_model_matrix = glm::translate(m_state.background[3].m_model_matrix, glm::vec3(17.5f, -3.5, 1.0f));
    
    // Cloud
    m_state.background[4].set_width(35.0f);
    m_state.background[4].set_height(1.0f);
    m_state.background[4].m_texture_id = Utility::load_texture("assets/tileset/cloud.png");
    m_state.background[4].m_model_matrix = glm::translate(m_state.background[4].m_model_matrix, glm::vec3(17.5f, -4.5, 1.0f));
    
    // Surface
    m_state.background[5].set_width(35.0f);
    m_state.background[5].set_height(1.0f);
    m_state.background[5].m_texture_id = Utility::load_texture("assets/tileset/surface.png");
    m_state.background[5].m_model_matrix = glm::translate(m_state.background[5].m_model_matrix, glm::vec3(17.5f, -7.0f, 1.0f));
    
    // Foreground2
    m_state.background[6].set_width(18.0f);
    m_state.background[6].set_height(8.0);
    m_state.background[6].m_texture_id = Utility::load_texture("assets/tileset/foreground-2.png");
    m_state.background[6].m_model_matrix = glm::translate(m_state.background[6].m_model_matrix, glm::vec3(39.0f, -18.0f, 1.0f));
    
    
    
    // Update
    for (int i = 0; i < entity_num; i++)
    {
        float width = m_state.background[i].get_width();
        float height = m_state.background[i].get_height();
        m_state.background[i].m_model_matrix = glm::scale(m_state.background[i].m_model_matrix, glm::vec3(width, height, 1.0f));
        m_state.background[i].render(program);
    }
}

void draw_homepage(GameState &m_state, ShaderProgram *program)
{
    // Initialise
    int entity_num = 5;
    std::vector<const char*> textures {
        "assets/homepage/sky.png",              // sky
        "assets/homepage/sea.png",              // sea
        "assets/homepage/left_island.png",      // left_island
        "assets/homepage/right_island.png",     // right_island
        "assets/homepage/cloud.png"             // cloud
    };
    std::vector<glm::vec3> positions {
        glm::vec3(0.0f, 2.0f, 1.0f),            // sky
        glm::vec3(0.0f, -4.0f, 1.0f),           // sea
        glm::vec3(-4.0f, -2.0f, 1.0f),          // left_island
        glm::vec3(5.0f, -2.0f, 1.0f),           // right_island
        glm::vec3(0.0f, 1.0f, 1.0f)             // cloud
    };
    std::vector<std::vector<float>> sizes {
        {20.0f, 8.0f},                          // sky
        {20.0f, 4.0f},                          // sea
        {13.0f, 6.0f},                          // left_island
        {10.0f, 5.0f},                          // right_island
        {20.0f, 10.0f}                          // cloud
    };
    m_state.background = new Entity[entity_num];
    for (int i = 0; i < entity_num; i++)
    {
        m_state.background[i].set_entity_type(BACKGROUND);
        m_state.background[i].m_texture_id = Utility::load_texture(textures[i]);
        m_state.background[i].m_model_matrix = glm::translate(m_state.background[i].m_model_matrix, positions[i]);
        float width = sizes[i][0];
        float height = sizes[i][1];
        m_state.background[i].m_model_matrix = glm::scale(m_state.background[i].m_model_matrix, glm::vec3(width, height, 1.0f));
        m_state.background[i].render(program);
    }
}

void draw_shop(GameState &m_state, ShaderProgram *program)
{
    // Initialise
    int entity_num = 4;
    std::vector<const char*> textures {
        "assets/tileset/shop_bg.png",           // background
        "assets/tileset/shop.png",              // shop
        "assets/tileset/sign.png",              // sign
        "assets/tileset/back_button.png",       // esc
    };
    std::vector<glm::vec3> positions {
        glm::vec3(0.0f, 0.0f, 0.0f),            // background
        glm::vec3(0.0f, 0.0f, 0.0f),            // shop
        glm::vec3(0.0f, 4.0f, 0.0f),            // sign
        glm::vec3(-8.0f, 5.5f, 0.0f)            // esc
    };
    std::vector<std::vector<float>> sizes {
        {20.0f, 12.0f},                         // background
        {10.0f, 10.0f},                         // shop
        {5.0f, 2.0f},                           // sign
        {3.5f, 1.0f}                            // sign
    };
    m_state.background = new Entity[entity_num];
    for (int i = 0; i < entity_num; i++)
    {
        m_state.background[i].set_entity_type(BACKGROUND);
        m_state.background[i].m_texture_id = Utility::load_texture(textures[i]);
        m_state.background[i].m_model_matrix = glm::translate(m_state.background[i].m_model_matrix, positions[i]);
        float width = sizes[i][0];
        float height = sizes[i][1];
        m_state.background[i].m_model_matrix = glm::scale(m_state.background[i].m_model_matrix, glm::vec3(width, height, 1.0f));
        m_state.background[i].render(program);
    }
}
