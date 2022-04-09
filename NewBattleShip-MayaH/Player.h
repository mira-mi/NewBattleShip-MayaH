#pragma once
#include <string>
#ifndef Player_h
class Player {
public:
	Player() {}
	Player(int nID) { id = nID; }
	Player(std::string nType, int nID) { playerType = nType; id = nID; }
	Player(const Player& rhs);
	std::string getPlayerType() { return playerType; }
	void setPlayerType(std::string nType) { playerType = nType; }
	int getID() { return id; }
	void setID(int nID) { id = nID; }
	bool getTurn() { return turn; }
	void setTurn(bool nTurn) { turn = nTurn; }
	Board& getBoard() { return playerBoard; }
	Ship& operator()(int, int);
	Player& operator=(const Player& p);
	bool operator==(const Player& rhs);
	bool operator !=(const Player&);
	char p1;
	char p2;
private:
	Board playerBoard;
	std::string playerType = "";
	int id = -1;
	bool turn = false;

};
# endif // !Player_h



