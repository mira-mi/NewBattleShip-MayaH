#include "battleship.h"
#include "Game.h"



Ship& Player::operator()(int x, int y) {
    return getBoard()(y, x);
}

Player& Player::operator=(const Player& rhs) {
    if (this != &rhs) {
        playerType = rhs.playerType;
        id = rhs.id;
        turn = rhs.turn;
        playerBoard = rhs.playerBoard;
    }
    return *this;
}

Player::Player(const Player& rhs) {
    playerType = rhs.playerType;
    id = rhs.id;
    turn = rhs.turn;
    playerBoard = rhs.playerBoard;
}

bool Player::operator==(const Player& rhs) {
    if ((playerBoard != rhs.playerBoard) || (playerType != rhs.playerType) || (id != rhs.id) || (turn != rhs.turn)) {
        return false;
    }
    return true;
}

bool Player::operator!=(const Player& rhs) {
    return !(*this == rhs);
}