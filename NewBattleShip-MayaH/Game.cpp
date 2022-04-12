#include "battleship.h"
#include "Player.h"
#include "Game.h"
#include "Ship.h"
#include "Board.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <CodeAnalysis/sourceannotations.h>
#include <string>
#include "Player.cpp"
#include <chrono>
#include <thread>



using namespace std;
using namespace std::literals;


Player nullPlayer(-1);
const int speed1 = 0;
const int speed2 = 1;

void Game::Run()
{
}

Player& Game::getPlayer(int id) {
    if (id == 1) { return player1; }
    else if (id == 2) { return player2; }
    else {
        std::cout << "error getting player\n";
        return nullPlayer;
    }
}

void Game::fire(Player& playerBeingAttacked, int attackX, int attackY) {
    //referenced multiple times
    Ship* spaceBeingAttacked = &playerBeingAttacked(attackY, attackX);
    Ship* shipStoredInArray = &playerBeingAttacked.getBoard().getShip(playerBeingAttacked(attackY, attackX).getShipNumber());
    std::cout << border;
    //ensure valid target
    if (attackX >= 0 && attackX < BOARD_SIZE && attackY >= 0 && attackY < BOARD_SIZE) {
        //if already attacked, return error
        if (spaceBeingAttacked->getFiredUpon()) {
            std::cout << "error, already attacked\n";
            return;
        }
        //otherwise, attack
        else {
            spaceBeingAttacked->setFiredUpon(true);
            //sink ship, decrease board ships[] by 1
            if (spaceBeingAttacked->getStatus() == Ship::Afloat::afloat) {
                spaceBeingAttacked->setStatus(Ship::Afloat::sunk);
                //decrement ship in array
                shipStoredInArray->setSize(shipStoredInArray->getSize() - 1);
                std::cout << "HIT!\n";
                //sink board ship[] if all ship objects are sunk
                if (shipStoredInArray->getSize() == 0) {
                    shipStoredInArray->setStatus(Ship::Afloat::sunk);
                    std::cout << "Player " << playerBeingAttacked.getID() << "'s " << shipStoredInArray->getName() << " SUNK!\n";
                }
            }
            else if (shipStoredInArray->getSize() == 0.5) {
                std::cout << "NEAR MISS!\n";
            }
            else {
                std::cout << "MISS!\n";
            }
        }
    }
    else {
        std::cout << "invalid attack coordinate\n";
    }
}


Game::Game(Player& p1, Player& p2) : player1(p1), player2(p2) {

    int gamemode;
    Game::player1;
    Game::player2;
    std::cout << border;
    std::cout << "Gamemodes:\n1. Player vs. CPU\n2. CPU vs. CPU\n";
    std::cout << border;
    std::cout << "Please select a gamemode (1/2):\n: ";
    std::cin >> gamemode;

    player1.setID(1);
    player2.setID(2);
    player1.setTurn(true);
    player2.getBoard().randomizeFleet();
    player2.setPlayerType("cpu");

    std::cout << "---------------------------------------------------------\nGAME STARTED\n";
    std::string startLoc, endLoc;
    int x1, y1, x2, y2, shipNumber;
    char choice;

    if (gamemode == 1) { player1.setPlayerType("human"); }
    else if (gamemode == 2) { player1.setPlayerType("cpu"); }
    else { std::cout << "invalid gamemode\n"; return; }

    //place if human
    if (player1.getPlayerType() == "human") {
        //place ships
        std::cout << "Player 1, please place your ships.\n";
        std::cout << border;
        //while all ships aren't placed
        //give option to randomize
        std::cout << "Would you like to place or randomize your fleet (p/r)?\n: ";
        std::cin >> choice;
        std::cout << border;
        if (choice != 'r') {
            while (!player1.getBoard().allShipsPlaced()) {
                //print reamining ships
                //print board
                std::cout << "p1's current board:\n";
                player1.getBoard().printBoard();
                player1.getBoard().printRemainingShips();

                std::cout << "Please enter ship number to place:\nShip Number: ";
                std::cin >> shipNumber;
                std::cout << "Please enter range to place ship:\nStart Location (ie: a3): ";
                std::cin >> startLoc;
                std::cout << "End Location (ie: a6): ";
                std::cin >> endLoc;

                

                //place ship
                player1.getBoard().setShip(shipNumber, x1, y1, x2, y2);
            }
        }
        else if (choice == 'r') { player1.getBoard().randomizeFleet(); }
        else { std::cout << "invalid option\n"; return; }
    }
    //computer player
    else {
        rand(static_cast<unsigned int>(time(NULL)));
        ;
        player1.getBoard().randomizeFleet();
        std::cout << "CPU Player: Ships Randomly Placed\n";
        std::cout << border;
    }

    Player currPlayer = player1;
    Player nextPlayer = player2;
    Player tmpPlayer;
    int option;
    std::string attackCoord;
    if (currPlayer.getPlayerType() == "human") {
        while (!(currPlayer.getBoard().allShipsSunk() || nextPlayer.getBoard().allShipsSunk())) {
            //take turns
            std::cout << "Player " << currPlayer.getID() << "'s turn:\n";
            std::cout << "Please select a command.......\n";
            std::cout << border;
            std::cout << "1. Print My Board\n2. Attack Enemy\n3. Print Enemy Board\n: ";
            std::cin >> option;

            if (option == 1) {
                std::cout << "Player " << currPlayer.getID() << "'s board\n";
                currPlayer.getBoard().printBoard();
            }
            else if (option == 2) {
                std::cout << border;
                std::cout << "Enter coordinate to attack (ie: a3)\n: ";
                std::cin >> attackCoord;
                if (x1 >= 0 && y1 >= 0 && x1 < BOARD_SIZE && y1 < BOARD_SIZE && !nextPlayer.getBoard()(x1, y1).getFiredUpon()) {
                    fire(nextPlayer, x1, y1);
                    std::cout << border;
                    //swap players if successful fire
                    tmpPlayer = currPlayer;
                    currPlayer = nextPlayer;
                    nextPlayer = tmpPlayer;
                }
                else {
                    std::cout << border;
                    std::cout << "invalid target, try again\n";
                    std::cout << border;
                }
            }
            else if (option == 3) {
                std::cout << border;
                std::cout << "Player " << nextPlayer.getID() << "'s board\n";
                nextPlayer.getBoard().printBoard();
            }
            else {
                std::cout << border;
                std::cout << "invalid option, try again.\n";
                std::cout << border;
            }
        }

        //the last player to guess will have won and they will have ben reassigned to nextPlayer
        std::cout << "PLAYER " << nextPlayer.getID() << " WON!!!!\nCONGRATULATIONS!!!\n";
        std::cout << "play again (y/n)?\n: ";
        char again = 'q';
        std::cin >> again;
        do {
            std::cout << border;
            if (again == 'y') { Game(); }
            else if (again == 'n') { return; }
            else {
                std::cout << "invalid selection, try again.\nplay again (y/n)?\n: ";
                std::cin >> again;
            }
        } while (again != 'y' || again != 'n');
    }
    //if player 1 is a cpu
    else {

        //initialize array of random numbers 0-99 to attack ships randomly
        int p1RandomCoords[BOARD_SIZE * BOARD_SIZE];
        int p2RandomCoords[BOARD_SIZE * BOARD_SIZE];
        int tmpRandNumber1, tmpRandNumber2, tmpNumber;

        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            p1RandomCoords[i] = i;
            p2RandomCoords[i] = i;
        }

        rand(static_cast<unsigned int>(time(NULL)));

        for (int k = 0; k < BOARD_SIZE * BOARD_SIZE * BOARD_SIZE; k++) {
            tmpRandNumber1 = rand() % 100;
            tmpNumber = p1RandomCoords[tmpRandNumber1];
            p1RandomCoords[tmpRandNumber1] = p1RandomCoords[BOARD_SIZE * BOARD_SIZE - 1];
            p1RandomCoords[BOARD_SIZE * BOARD_SIZE - 1] = tmpNumber;

            tmpRandNumber2 = rand() % 100;
            tmpNumber = p2RandomCoords[tmpRandNumber2];
            p2RandomCoords[tmpRandNumber2] = p2RandomCoords[BOARD_SIZE * BOARD_SIZE - 1];
            p2RandomCoords[BOARD_SIZE * BOARD_SIZE - 1] = tmpNumber;
        }

        int attackX, attackY;
        attackX = rand() % BOARD_SIZE;
        attackY = rand() % BOARD_SIZE;

        std::cout << "Player " << currPlayer.getID() << "'s board\n";
        std::cout << border;
        currPlayer.getBoard().printBoard();
        std::cout << "Player " << nextPlayer.getID() << "'s board\n";
        std::cout << border;
        nextPlayer.getBoard().printBoard();

        int i = 0;
        int k = 0;
        //until one of player's has all ships sunk
        while (!(currPlayer.getBoard().allShipsSunk() || nextPlayer.getBoard().allShipsSunk())) {
            //generate random attack that hasn't already been guessed
            //pick from random numbers p1
            if (currPlayer.getID() == 1) {
                attackX = p1RandomCoords[i] / 10;
                attackY = p1RandomCoords[i] % 10;
                i++;
            }
            else {
                attackX = p2RandomCoords[i] / 10;
                attackY = p2RandomCoords[i] % 10;
                k++;
            }
            //take turns
            std::cout << "Player " << currPlayer.getID() << "'s turn.....\n";
            //quick pause (as if cpu is thinking)
            
            std::cout << border;
            std::cout << "Attacking player " << nextPlayer.getID() << "\nCoordinate's being attacked: (" << attackX << ',' << attackY << ")\n";
            
            fire(nextPlayer, attackX, attackY);
            std::cout << "Attack Successful\n";
            std::cout << border;
            //print boards
           ;
            std::cout << "Printing player 1's board:\n";
            std::cout << border;
            
            //print player 1's board
            std::cout << "Player 1's board\n";
            std::cout << border;
            if (currPlayer.getID() == 1) { currPlayer.getBoard().printBoard(); }
            else { nextPlayer.getBoard().printBoard(); }
            std::cout << "Printing player 2's board:\n";
            std::cout << border;
            
            //print player 2's board
            std::cout << "Player 2's board\n";
            std::cout << border;
            if (currPlayer.getID() == 2) { currPlayer.getBoard().printBoard(); }
            else { nextPlayer.getBoard().printBoard(); }
           
            //swap players on successful fire
            tmpPlayer = currPlayer;
            currPlayer = nextPlayer;
            nextPlayer = tmpPlayer;
        }

        //the last player to guess will have won and they will have ben reassigned to nextPlayer
        std::cout << border;
        std::cout << "PLAYER " << nextPlayer.getID() << " WON!!!!\nCONGRATULATIONS!!!\n";
        std::cout << "play again (y/n)?\n: ";
        char again = 'q';
        std::cin >> again;
        do {
            std::cout << border;
            if (again == 'y') { Game(); }
            else if (again == 'n') { return; }
            else {
                std::cout << "invalid selection, try again.\nplay again (y/n)?\n: ";
                std::cin >> again;
            }
        } while (again != 'y' || again != 'n');
    }
}

void Game::run()
{
}
