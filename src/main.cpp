#include <bn_core.h>
#include <bn_display.h>
#include <bn_random.h>

#include <bn_music.h>
#include "bn_music_items.h"
#include "ScoreDisplay.h"
#include "Player.h"
#include "Enemy.h"

// Random initial x and y values for the generated Enemies
bn::random initial_x = bn::random();
bn::random initial_y = bn::random();

// Counter Variable
int counter = 120;

int main()
{
    bn::core::init();

    // plays music
    bn::music_items::background.play();
    bn::music::set_volume(0.1);

    // Create a new score display
    ScoreDisplay scoreDisplay = ScoreDisplay();

    // Create a player and initialize it
    // TODO: we will move the initialization logic to a constructor.
    Player player = Player(44, 22, 1.5, PLAYER_SIZE);

    bn::vector<Enemy, 10> enemies;

    // Create a enemy and initialize it
    Enemy initial_enemy = Enemy(-30, 22, 0.7, ENEMY_SIZE);
    enemies.push_back(initial_enemy);
    

    while (true) {

        if (counter == 0 && enemies.size() < enemies.max_size()) {
            Enemy new_enemy = Enemy(initial_x.get_int(MIN_X, MAX_X), initial_y.get_int(MIN_Y, MAX_Y), 0.7, ENEMY_SIZE);
            enemies.push_back(Enemy(new_enemy));

            initial_x.update();
            initial_y.update();
            counter = 120;
        }

        if (counter > 0) {
            counter -= 1;
        }

        player.update();

        bn::vector<Enemy, 10> trashBin;
        bool hitOnce = true;

        // for each loop to access enemies and updates
        for (Enemy opponent : enemies) {
            opponent.update(player);

            for (Enemy reference : enemies) {
                opponent.bounce(reference);
            }

            // Reset the current score and player position if the player collides with enemy
            if (opponent.bounding_box.intersects(player.bounding_box)) {
                scoreDisplay.resetScore();

                player.sprite.set_x(44);
                player.sprite.set_y(22);

                if (hitOnce == true) {
                    for (Enemy removed : enemies) {
                        trashBin.push_back(removed);
                    }
                    hitOnce = false;
                }

                counter = 120;
            }
        }

        if (trashBin.size() > 0) {
            for (int i = 0; i < trashBin.size(); i++) {
                enemies.pop_back();
            }
            enemies.push_back(initial_enemy);
        }

        // Update the scores and display them
        scoreDisplay.update();

        bn::core::update();
    }
}