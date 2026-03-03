#ifndef ENEMY_H
#define ENEMY_H

#include <bn_random.h>

#include "bn_sprite_items_square.h"
#include "Player.h"

static constexpr bn::size ENEMY_SIZE = {8, 8};

class Enemy {
    public :
        Enemy(int starting_x, int starting_y, bn::fixed enemy_speed, bn::size enemy_size);

        /**
         * Update the position and bounding box of the enemy based on the player's position
         */
        void update(Player& player);

        void bounce(Enemy& enemy);

        bn::sprite_ptr sprite;
        bn::fixed speed;
        bn::size size;
        bn::rect bounding_box;
};

#endif