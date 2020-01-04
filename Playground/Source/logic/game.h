#ifndef _GAME_H_
#define _GAME_H_
#define NUM_OF_OPPORTUNITY_CARD 20
#define NUM_OF_FATE_CARD 20
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
	void ShuffleOpportunity();
	void ShuffleFate();
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
	bool goTop;
	int from;
	bool response;
	bool popout;
	bool getcard;
	int cardtype;
	int cardidx;
	int doneidx;
	int throwidx;
	int money[2];
	bool keetsu;
	int tsuidx;
	int tsuheight;
	int tsuowner;
	bool smove;
	int smoveidx;
	int smovepos;
	//go in
	bool canThrow;
	bool getPoints;
	bool movedone;
	int* point;
	int answer;
	bool getanswer;
	bool getcarddone;
	bool updatemoney;
	bool is_gameover = false;
    
private:
    vector<Player> player;
    LogicMap map;
    int num_of_player;
	vector<int> opportunity_fix;
	vector<int> opportunity_random;
	vector<int> fate_fix;
	vector<int> fate_random;
};
#endif