#include "Player.h"

bn::rect create_bounding_box(bn::sprite_ptr sprite, bn::size box_size) {
    return bn::rect(sprite.x().round_integer(),
                    sprite.y().round_integer(),
                    box_size.width(),
                    box_size.height());
}

Player::Player(int starting_x, int starting_y, bn::fixed player_speed, bn::size player_size) :
    sprite(bn::sprite_items::dot.create_sprite(starting_x, starting_y)),
    speed(player_speed),
    size(player_size),
    bounding_box(create_bounding_box(sprite, size))
{}

void Player::update() {
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