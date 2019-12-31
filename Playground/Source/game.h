#ifndef _GAME_H_
#define _GAME_H_
#include <vector>
#include "player.h"
#include "logicmap.h"
class Game {
public:
	Game();
    Game(int);
    void Show();
    void Run();
    void Event(int, int);
    void ReturnAsset(int);
    void GameOver();
    int GetNumOfPlayer() {
        return num_of_player;
    }
    // void SetNumOfPlayer(int num_of_player) {
    //     this -> num_of_player = num_of_player;
    // }
	//go out
	bool goThrow;
	bool goWalk;
	bool goSide;
	int from;
	bool response;
	//go in
	bool canThrow;
	bool getPoints;
	bool movedone;
	int* point;
	int answer;
	bool getanswer;
    
private:
    vector<Player> player;
    LogicMap map;
    int num_of_player;
    bool is_gameover = false;
};
#endif