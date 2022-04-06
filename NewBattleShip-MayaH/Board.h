#pragma once
#ifndef Board_h
const int BOARD_SIZE = 8;
const int numOfShips = 5;
class Board {
public: 
	Board();
	Board(const Board& rhs);
	~Board() {}
	void printBoard();
    void setShip(int shipNumber, int x1, int y1, int x2, int y2);
    bool isValidDirection(int x1, int y1, int x2, int y2);
    bool rangeIsOccupied(int x1, int y1, int x2, int y2);
    bool fitsOnBoard(int x1, int y1, int x2, int y2);
    bool isSunk(int shipNumber);
    void randomizeFleet();
    bool allShipsPlaced();
    bool allShipsSunk();
    void printRemainingShips();
    Ship& operator()(int, int);
    Ship& getShip(int n) { return Ships[n]; }
    Board& operator=(const Board& rhs);
    bool operator==(const Board&);
    bool operator!=(const Board&);
private:
    Ship gameBoard[BOARD_SIZE][BOARD_SIZE];
    Ship Ships[numOfShips];
};

#endif //!Board_h
