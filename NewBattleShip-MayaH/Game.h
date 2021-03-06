#pragma once
#ifndef Game_h
class Player;
class Game {
public:
	Game(Player& p1, Player& p2) : player1(p1), player2(p2) {
		Game::player1;
		Game::player2;
	}
	void Run();
	
private:
	Player& player1;
	Player& player2;
	Player& getPlayer(int id);
	void fire(Player& playerBeingAttacked, int attackX, int attackY);
	Game();
	Game(Player& p1, Player& p2);
	void run();
};
#endif // !Game_h
#define Game_h


