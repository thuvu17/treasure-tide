#include "Scene.h"

class LevelX : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    ~LevelX();
    
    void initialise(float max_o2_level, float player_speed, float player_money) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
