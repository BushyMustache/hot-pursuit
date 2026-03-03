#include <bn_core.h>
#include <bn_display.h>
#include <bn_keypad.h>
#include <bn_rect.h>
#include <bn_size.h>

#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_square.h"
#include <bn_random.h>
#include <bn_log.h>

#include <bn_music.h>
#include "bn_music_items.h"
#include "ScoreDisplay.h"


// Width and height of the the player bounding box
static constexpr bn::size PLAYER_SIZE = {8, 8};
static constexpr bn::size ENEMY_SIZE = {8, 8};

static constexpr int MIN_Y = -bn::display::height() / 2;
static constexpr int MAX_Y = bn::display::height() / 2;
static constexpr int MIN_X = -bn::display::width() / 2;
static constexpr int MAX_X = bn::display::width() / 2;

// Random x and y values for the Enemy
bn::random initial_x = bn::random();
bn::random initial_y = bn::random();

bn::random random_x = bn::random();
bn::random random_y = bn::random();

// Counter Variable
int counter = 120;


/**
 * Creates a rectangle centered at a sprite's location with a given size.
 * sprite the sprite to center the box around
 * box_size the dimensions of the bounding box
 */
bn::rect create_bounding_box(bn::sprite_ptr sprite, bn::size box_size)
{
    return bn::rect(sprite.x().round_integer(),
                    sprite.y().round_integer(),
                    box_size.width(),
                    box_size.height());
}



class Player {
    public:
        // Constructor
        // Bounding Box should be last
        Player(int starting_x, int starting_y, bn::fixed player_speed, bn::size player_size) :
            sprite(bn::sprite_items::dot.create_sprite(starting_x, starting_y)),
            speed(player_speed),
            size(player_size),
            bounding_box(create_bounding_box(sprite, size))
        {}

    /**
     * Update the position and bounding box of the player based on d-pad movement.
     */
    void update()
    {
        if (bn::keypad::right_held())
        {
            sprite.set_x(sprite.x() + speed);
        }
        if (bn::keypad::left_held())
        {
            sprite.set_x(sprite.x() - speed);
        }
        // TODO: Add logic for up and down
        if (bn::keypad::up_held())
        {
            sprite.set_y(sprite.y() - speed);
        }
        if (bn::keypad::down_held())
        {
            sprite.set_y(sprite.y() + speed);
        }

        bounding_box = create_bounding_box(sprite, size);

        if (sprite.x() < MIN_X) {
            sprite.set_x(MIN_X);
        }
        if (sprite.x() > MAX_X) {
            sprite.set_x(MAX_X);
        }
        if (sprite.y() < MIN_Y) {
            sprite.set_y(MIN_Y);
        }
        if (sprite.y() > MAX_Y) {
            sprite.set_y(MAX_Y);
        }
    }

    // Create the sprite. This will be moved to a constructor
    bn::sprite_ptr sprite;
    bn::fixed speed;       // The speed of the player
    bn::size size;         // The width and height of the sprite
    bn::rect bounding_box; // The rectangle around the sprite for checking collision
};

class Enemy {
    public :
        Enemy(int starting_x, int starting_y, bn::fixed enemy_speed, bn::size enemy_size):
            sprite(bn::sprite_items::square.create_sprite(starting_x, starting_y)),
            speed(enemy_speed),
            size(enemy_size),
            bounding_box(create_bounding_box(sprite, size))
    {}

    /**
     * Update the position and bounding box of the enemy based on the player's position
     */
    void update(Player& player) {
        
        if (player.sprite.x() > sprite.x()) {
            sprite.set_x(sprite.x() + speed);
        }
        if (player.sprite.x() < sprite.x()) {
            sprite.set_x(sprite.x() - speed);
        }
        // sets the enemy and user location to oppsite 
        if (player.sprite.y() > sprite.y()) {
            sprite.set_y(sprite.y() + speed);
        }
        if (player.sprite.y() < sprite.y()) {
            sprite.set_y(sprite.y() - speed);
        }

        bounding_box = create_bounding_box(sprite, size);

        if (bounding_box.intersects(player.bounding_box)) {
            while (player.sprite.x() == random_x.get_int(MIN_X, MAX_X) 
            && player.sprite.y() == random_x.get_int(MIN_Y, MAX_Y)) {
                random_x.update();
                random_y.update();
            }
            sprite.set_x(random_x.get_int(MIN_X, MAX_X));
            sprite.set_y(random_y.get_int(MIN_Y, MAX_Y));
        }

    }

    void bounce(Enemy& enemy) {

        // variables for calcualting x and y coordinates 
        bn::fixed dx = sprite.x() - enemy.sprite.x();
        bn::fixed dy = sprite.y() - enemy.sprite.y();
        // using pythagorean formula 
        bn::fixed distance = sqrt(dx*dx + dy*dy);

        if (distance < 10) {
            // left
            if (enemy.sprite.x() > sprite.x()) {
                sprite.set_x(sprite.x() - speed);
            }
            // right
            if (enemy.sprite.x() < sprite.x()) {
                sprite.set_x(sprite.x() + speed);
            }
            // up
            if (enemy.sprite.y() > sprite.y()) {
                sprite.set_y(sprite.y() - speed);
            }
            // down
            if (enemy.sprite.y() < sprite.y()) {
                sprite.set_y(sprite.y() + speed);
            }
        }
    }

    bn::sprite_ptr sprite;
    bn::fixed speed;
    bn::size size;
    bn::rect bounding_box;
};

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

    // Enemy enemyTwo = Enemy(-30, 10, 0.5, ENEMY_SIZE);

    // Enemy enemyThree = Enemy(-30, 32, 0.5, ENEMY_SIZE);

    enemies.push_back(initial_enemy);
    // enemies.push_back(enemyTwo);
    // enemies.push_back(enemyThree);

    // bn::sprite_ptr enemy_sprite = bn::sprite_items::square.create_sprite(-30, 22);
    // bn::rect enemy_bounding_box = create_bounding_box(enemy_sprite, ENEMY_SIZE);

    while (true) {

        if (counter == 0 && enemies.size() < enemies.max_size()) {
            Enemy new_enemy = Enemy(initial_x.get_int(MIN_X, MAX_X), initial_y.get_int(MIN_Y, MAX_Y), 0.7, ENEMY_SIZE);
            enemies.push_back(Enemy(new_enemy));

            initial_x.update();
            initial_y.update();
            counter = 120;
        } else {
            BN_LOG("Error: Couldn't generate new enemy");
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

        // if (enemy.bounding_box.intersects(player.bounding_box))
        // {
        //     scoreDisplay.resetScore();
        //     player.sprite.set_x(44);
        //     player.sprite.set_y(22);
        // }

        // Update the scores and display them
        scoreDisplay.update();

        random_x.update();
        random_y.update();

        bn::core::update();
    }
}