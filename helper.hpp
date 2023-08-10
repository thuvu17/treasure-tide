// Helper function
#include "Map.h"
#include "Scene.h"

void print_matrix(glm::mat4 &matrix, int size);

void DrawText(ShaderProgram *program, GLuint font_texture_id, std::string text,
              float screen_size, float spacing, glm::vec3 position);

void draw_background(GameState &m_state, ShaderProgram *program);
void draw_homepage(GameState &m_state, ShaderProgram *program);
void draw_shop(GameState &m_state, ShaderProgram *program);
