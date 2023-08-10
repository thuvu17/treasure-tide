#include "Scene.h"

class Shop : public Scene {
public:
    int ENEMY_COUNT = 0;
    int item_number = 0;
    // Purchase variables
    bool not_enough_money = false;
    bool buy_flippers = false;
    bool buy_o2 = false;
    
    ~Shop();
    
    void back();
    void purchase(ShaderProgram *program);
    void initialise(float max_o2_level, float player_speed, float player_money) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};

