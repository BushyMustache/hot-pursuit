#include "Enemy.h"

// Random x and y values for the first Enemy
bn::random random_x = bn::random();
bn::random random_y = bn::random();

Enemy::Enemy(int starting_x, int starting_y, bn::fixed enemy_speed, bn::size enemy_size) :
    sprite(bn::sprite_items::square.create_sprite(starting_x, starting_y)),
    speed(enemy_speed),
    size(enemy_size),
    bounding_box(create_bounding_box(sprite, size))
{}

void Enemy::update(Player& player) {
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

void Enemy::bounce(Enemy& enemy) {
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