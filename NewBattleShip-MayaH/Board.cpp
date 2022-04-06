#include <cstdio>
#include "battleship.h"
//#include "Board.h"
//#include "Ship.h"
//#include "Game.h"
//#include <ctime>

Board::Board(const Board& rhs) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int k = 0; k < BOARD_SIZE; k++) {
            gameBoard[i][k] = rhs.gameBoard[i][k];
        }
    }
    for (int i = 0; i < numOfShips; i++) {
        Ships[i] = rhs.Ships[i];
    }
}

//x = hit ship
//o = ship on board
//m = empty space
void Board::printBoard() {
    std::cout << "x = hit ship\no = ship on board\nm = empty space\n";
    std::cout << border;
    std::cout << "   ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << '|' << alphabet[i] << "|";
        if (i != BOARD_SIZE - 1) { std::cout << ' '; }
    }
    std::cout << "\n";
    for (int j = 0; j < BOARD_SIZE; j++) {
        std::cout << j << " - ";
        for (int k = 0; k < BOARD_SIZE; k++) {
            if (gameBoard[j][k].getShipNumber() != -1 && gameBoard[j][k].getFiredUpon()) {
                std::cout << 'x';
            }
            else if (!gameBoard[j][k].getStatus()) { std::cout << 'm'; } //empty spaces
            else { std::cout << 'o'; } //ships that are afloat
            if (k != BOARD_SIZE - 1) { std::cout << "   "; }
        }
        std::cout << "\n";
    }
    std::cout << border;
}

Board::Board() {
    Ships[0] = Ship("Carrier", 5, Ship::Afloat::sunk, 0);
    Ships[1] = Ship("Battleship", 4, Ship::Afloat::sunk, 1);
    Ships[2] = Ship("Cruiser", 3, Ship::Afloat::sunk, 2);
    Ships[3] = Ship("Submarine", 3, Ship::Afloat::sunk, 3);
    Ships[4] = Ship("Destroyer", 2, Ship::Afloat::sunk, 4);
}

Ship& Board::operator()(int x, int y) {
    return gameBoard[y][x];
}

//returns bool based on if coords are valid direction
bool Board::isValidDirection(int x1, int y1, int x2, int y2) {
    if (x1 == x2 || y1 == y2) { return true; }
    return false;
}

//returns bool based on if range is occupied or not
bool Board::rangeIsOccupied(int x1, int y1, int x2, int y2) {
    //if horizontal
    if (y1 == y2) {
        while (x1 != x2) {
            if (gameBoard[y1][x1].getStatus()) {
                return true;
            }
            if (x1 > x2) {
                x1--;
            }
            else {
                x1++;
            }
        }
        if (gameBoard[y1][x1].getStatus()) {
            return true;
        }
    }

    //if vertical
    else {
        while (y1 != y2) {
            if (gameBoard[y1][x1].getStatus()) {
                return true;
            }
            if (y1 > y2) {
                y1--;
            }
            else {
                y1++;
            }
        }
        if (gameBoard[y1][x1].getStatus()) {
            return true;
        }

    }
    return false;
}

bool Board::fitsOnBoard(int x1, int y1, int x2, int y2) {
    if ((x1 >= 0 && x1 < BOARD_SIZE) && (x2 >= 0 && x2 < BOARD_SIZE) && (y1 >= 0 && y1 < BOARD_SIZE) && (y2 >= 0 && y2 < BOARD_SIZE)) { return true; }
    return false;
}

bool Board::isSunk(int shipNumber) {
    return !Ships[shipNumber].getStatus();
}

bool Board::allShipsPlaced() {
    for (int i = 0; i < numOfShips; i++) {
        if (!Ships[i].getStatus()) {
            return false;
        }
    }
    return true;
}

bool Board::allShipsSunk() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int k = 0; k < BOARD_SIZE; k++) {
            if (gameBoard[i][k].getStatus()) {
                return false;
            }
        }
    }
    return true;
}

bool Board::operator==(const Board& rhs) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int k = 0; k < BOARD_SIZE; k++) {
            if (gameBoard[i][k] != rhs.gameBoard[i][k]) {
                return false;
            }
        }
    }
    for (int r = 0; r < numOfShips; r++) {
        if (Ships[r] != rhs.Ships[r]) {
            return false;
        }
    }
    return true;
}

bool Board::operator!=(const Board& rhs) {
    return !(*this == rhs);
}

Board& Board::operator=(const Board& rhs) {
    if (this != &rhs) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int k = 0; k < BOARD_SIZE; k++) {
                gameBoard[i][k] = rhs.gameBoard[i][k];
            }
        }
        for (int i = 0; i < numOfShips; i++) {
            Ships[i] = rhs.Ships[i];
        }
    }
    return *this;
}

void Board::setShip(int shipNumber, int x1, int y1, int x2, int y2) {
    //checks if valid placement first
    if (!rangeIsOccupied(x1, y1, x2, y2) && fitsOnBoard(x1, y1, x2, y2) && isValidDirection(x1, y1, x2, y2)) {
        Ship shipToPlace = Ship(Ships[shipNumber].getName(), Ships[shipNumber].getSize(), Ship::Afloat::afloat, shipNumber);
        //place ship
        while ((x1 != x2 || y1 != y2)) {
            gameBoard[y1][x1] = shipToPlace;
            gameBoard[y1][x1].setShipNumber(shipNumber);
            if (x1 > x2) {
                x1--;
            }
            else if (x2 > x1) {
                x1++;
            }
            else if (y1 > y2) {
                y1--;
            }
            else if (y2 > y1) {
                y1++;
            }
        }
        gameBoard[y1][x1] = shipToPlace;
        Ships[shipNumber].setStatus(Ship::Afloat::afloat);
    }
    else {
        return;
    }
}

void Board::randomizeFleet() {
    srand(static_cast<unsigned int>(time(NULL)));
    for (int i = 0; i < numOfShips; i++) {
        int startX = rand() % BOARD_SIZE;
        int startY = rand() % BOARD_SIZE;
        int endX = startX;
        int endY = startY;

        //go vert
        if (rand() % 2 == 0) {
            //go up
            if (rand() % 2 == 0) {
                endX -= (Ships[i].getSize() - 1);
            }
            //go down
            else {
                endX += (Ships[i].getSize() - 1);
            }
        }
        //go horiz
        else {
            //go left
            if (rand() % 2 == 0) {
                endY -= (Ships[i].getSize() - 1);
            }
            //go right
            else {
                endY += (Ships[i].getSize() - 1);
            }
        }
        setShip(i, startX, startY, endX, endY);
        //don't incrment i unless ship is placed
        if (!Ships[i].getStatus()) {
            i--;
        }
    }
}

void Board::printRemainingShips() {
    for (int i = 0; i < numOfShips; i++) {
        if (!Ships[i].getStatus()) {
            std::cout << Ships[i].getShipNumber() << ". " << Ships[i].getName() << " | Size = " << Ships[i].getSize() << "\n";
        }
    }
}
Ship.h: