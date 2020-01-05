#include "game.h"
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

using namespace std;
Game::Game() {

}
Game::Game(int num_of_player) {
    this -> num_of_player = num_of_player;
    for(int iter = 0; iter < num_of_player; iter++) {
        player.push_back(Player());
    }
	// player.resize(num_of_player);
	for (int card_iter = 0; card_iter < NUM_OF_OPPORTUNITY_CARD; card_iter++) {
		opportunity_fix.push_back(card_iter);
	}
	for (int card_iter = 0; card_iter < NUM_OF_FATE_CARD; card_iter++) {
		fate_fix.push_back(card_iter);
	}
	ShuffleFate();
	ShuffleOpportunity();
	goThrow = false;
	goWalk = false;
	getPoints = false;
	goTop = false;
	goSide = false;
	canThrow = false;
	response = false;
	movedone = false;
	getanswer = false;
	popout = false;
	keetsu = false;
	getcard = false;
	smove = false;
	updatemoney = false;
	money[0] = 15000;
	money[1] = 15000;
    // player.resize(num_of_player);
}
void Game::ShuffleFate() {
	fate_random = vector<int>(fate_fix);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(fate_random.begin(), fate_random.end(), default_random_engine(seed));
}
void Game::ShuffleOpportunity() {
	opportunity_random = vector<int>(opportunity_fix);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(opportunity_random.begin(), opportunity_random.end(), default_random_engine(seed));
}
void Game::ReturnAsset(int player_num) {
    for(int iter = 0; iter < player[player_num].grid_red.size(); iter++) {
        map.grid[player[player_num].grid_red[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_orange.size(); iter++) {
        map.grid[player[player_num].grid_orange[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_yellow.size(); iter++) {
        map.grid[player[player_num].grid_yellow[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_green.size(); iter++) {
        map.grid[player[player_num].grid_green[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_blue.size(); iter++) {
        map.grid[player[player_num].grid_blue[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_light.size(); iter++) {
        map.grid[player[player_num].grid_light[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_purple.size(); iter++) {
        map.grid[player[player_num].grid_purple[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_pink.size(); iter++) {
        map.grid[player[player_num].grid_pink[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_infrastructure.size(); iter++) {
        map.grid[player[player_num].grid_infrastructure[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_station.size(); iter++) {
        map.grid[player[player_num].grid_station[iter]].Init();
    }
}
void Game::Run() {
    while(!is_gameover) {
        for(int player_iter = 0; player_iter < num_of_player; player_iter++) {
			money[player_iter] = player[player_iter].GetMoney();
			doneidx = player_iter;
            if(!player[player_iter].GetBankRupt()) {
                cout << "\n\n\n";
                cout << "Turn for player" << player_iter << "\n";
                player[player_iter].Init();
                // count jail day
                if(player[player_iter].GetInJail()) {
                    player[player_iter].SetJailDay(player[player_iter].GetJailDay() + 1);
                    cout << "----------------" << "\n";
                    cout << "Player is in jail day " << player[player_iter].GetJailDay() << "\n";
                    cout << "----------------" << "\n";
                    if(player[player_iter].GetJailDay() > 3) {
                        player[player_iter].LeaveJail();
                    }
                }
				if (player[player_iter].GetIsRest()) {
					player[player_iter].SetRestDay(player[player_iter].GetRestDay() + 1);
					cout << "----------------" << "\n";
					cout << "Player is in rest day " << player[player_iter].GetRestDay() << "\n";
					cout << "----------------" << "\n";
					if (player[player_iter].GetRestDay() > 1) {
						player[player_iter].LeaveRest();
					}
				}
                while(player[player_iter].GetOneMore()) {
					getPoints = false;
					canThrow = false;
					getanswer = false;
					getcard = false;
					getcarddone = false;
					from = player[player_iter].GetPosition();
					if (player[player_iter].GetIsRest()) {
						// rest can't throw dice
						break;
					}
					//cout << "player" << player_iter << " position: " <<from << endl;
					while (!canThrow) {
						cout << "";
					}
					goThrow = true;
					while (!getPoints) {
						cout << "";
					}
					movedone = false;
					response = false;
					int num_of_move = player[player_iter].ThrowDice(point[0], point[1]);
					//cout << point << endl;
                    if(player[player_iter].GetInJail()) {
                        // do nothing
						response = true;
                        cout << "Player" << player_iter << " Position : " << player[player_iter].GetPosition() << "\n";
                    }
                    else {
						from = player[player_iter].GetPosition();
						goWalk = true;
						response = true;
                        player[player_iter].Move(num_of_move);
						while (!movedone) {
							if (updatemoney) {
								updatemoney = false;
								money[player_iter] = player[player_iter].GetMoney();
							}
							cout << "";
						}
                        cout << "Player" << player_iter << " Position : " << player[player_iter].GetPosition() << "\n";
                        Event(player[player_iter].GetPosition(), player_iter);
                        if(player[player_iter].GetMoney() < 0) {
                            // player[player_iter].GoBankrupt();
                            ReturnAsset(player_iter);
                            player[player_iter].SetBankRupt(true);
                            cout << "----------------" << "\n";
                            cout << "Player" << player_iter << " go bankrupt" << "\n";
                            cout << "----------------" << "\n";
                            int count_bankrupt = 0;
                            for(int iter = 0; iter < num_of_player; iter++) {
                                if(player[iter].GetBankRupt()) {
                                    count_bankrupt++;
                                }
                            }
                            if(count_bankrupt == num_of_player - 1) {
                                is_gameover = true;
                            }
                            break;
                        }
                    }
					money[player_iter] = player[player_iter].GetMoney();
					goTop = true;
                    //system("pause");
                }
                if(is_gameover) {
                    break;
                }
            }
			money[player_iter] = player[player_iter].GetMoney();
        }
    }
    GameOver();
}
void Game::GameOver() {
	money[0] = player[0].GetMoney();
	money[1] = player[1].GetMoney();
    cout << "GAMEOVER" << "\n";
}
void Game::Event(int position, int player_num) {
	string option;
	int toll = 0;
	int cost_build = 0; // cost to build house
	int card; // for opportunity and fate card
	int dice_result;
    switch (map.grid[position].GetLabel())
    {
    // land
    case red: case orange: case yellow: case green: case blue: case light: case purple: case pink:
        // buy land
        if(map.grid[position].GetEmpty()) { 
            if(player[player_num].GetMoney() >= map.grid[position].GetValue()) { // check whether player have enough money
                cout << "----------------" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "This land cost " << map.grid[position].GetValue() << " dollars" << "\n";
                cout << "yes to buy, no to not buy" << "\n";
                cout << "----------------" << "\n";
				popout = true;
				while (!getanswer) {
					cout << "";
				}
				if (answer == 1) {
					option = "yes";
				}
				else if (answer==2) {
					option = "no";
				}
				cout << option << endl;
                if(option == "yes") {
                    player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
                    map.grid[position].SetOwner(player_num);
                    map.grid[position].SetEmpty(false);
					tsuidx = position;
					tsuheight = 0;
					tsuowner = player_num;
					keetsu = true;
                    // update player land list
                    switch (map.grid[position].GetLabel())
                    {
                        case red:
                            player[player_num].grid_red.push_back(position);
                            break;
                        case orange:
                            player[player_num].grid_orange.push_back(position);
                            break;
                        case yellow:
                            player[player_num].grid_yellow.push_back(position);
                            break;
                        case green:
                            player[player_num].grid_green.push_back(position);
                            break;
                        case blue:
                            player[player_num].grid_blue.push_back(position);
                            break;
                        case light:
                            player[player_num].grid_light.push_back(position);
                            break;
                        case purple:
                            player[player_num].grid_purple.push_back(position);
                            break;
                        case pink:
                            player[player_num].grid_pink.push_back(position);
                            break;
                        default:
                            break;
                    }
                    cout << "----------------" << "\n";
                    cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "This land belong to player" << map.grid[position].GetOwner() << "\n";
                    cout << "----------------" << "\n";
                }
                else if(option == "no") {
                    
                }
            }
        }
        else {
            // pay toll
            if(player_num != map.grid[position].GetOwner()) { // not owner
                toll = map.grid[position].GetCost() + 0.2 * map.grid[position].GetValue() * map.grid[position].GetNumOfHouse();
                cout << "----------------" << "\n";
                cout << "This land belong to player" << map.grid[position].GetOwner() << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "Toll are " << toll << " dollars" << "\n";
                cout << "----------------" << "\n";
                
                player[player_num].SetMoney(player[player_num].GetMoney() - toll);
                player[map.grid[position].GetOwner()].SetMoney(player[map.grid[position].GetOwner()].GetMoney() + toll);

                cout << "----------------" << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "----------------" << "\n";
            }
            // build house
            else {
                cost_build = map.grid[position].GetValue() * 0.5;
                if(player[player_num].GetMoney() > cost_build) {
                    cout << "----------------" << "\n";
                    cout << "This land belong to you" << "\n";
                    cout << "You have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "Build house cost " << cost_build << " dollars" << "\n";
                    cout << "yes to bulid, no to not build" << "\n";
                    cout << "----------------" << "\n";
					popout = true;
					while (!getanswer) {
						cout << "";
					}
					if (answer == 1) {
						option = "yes";
					}
					else if (answer == 2) {
						option = "no";
					}
					cout << option << endl;
                    if(option == "yes") {
                        map.grid[position].SetNumOfHouse(map.grid[position].GetNumOfHouse() + 1);
                        player[player_num].SetMoney(player[player_num].GetMoney() - cost_build);
						tsuidx = position;
						tsuheight = map.grid[position].GetNumOfHouse();
						tsuowner = player_num;
						keetsu = true;
                    }
                    else if(option == "no") {

                    }

                    cout << "----------------" << "\n";
                    cout << "This land have " << map.grid[position].GetNumOfHouse() << " house" << "\n";
                    cout << "You have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "----------------" << "\n";
                }
            }
        }
        break;
    case station:
        // buy station
        if(map.grid[position].GetEmpty()) { 
            if(player[player_num].GetMoney() >= map.grid[position].GetValue()) { // check whether player have enough money
                cout << "----------------" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "This station cost " << map.grid[position].GetValue() << " dollars" << "\n";
                cout << "yes to buy, no to not buy" << "\n";
                cout << "----------------" << "\n";
				popout = true;
				while (!getanswer) {
					cout << "";
				}
				if (answer == 1) {
					option = "yes";
				}
				else if (answer == 2) {
					option = "no";
				}
				cout << option << endl;
                if(option == "yes") {
					tsuidx = position;
					tsuheight = 0;
					tsuowner = player_num;
					keetsu = true;
                    player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
                    map.grid[position].SetOwner(player_num);
                    map.grid[position].SetEmpty(false);
                    player[player_num].grid_station.push_back(position);
                    cout << "----------------" << "\n";
                    cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "This station belong to player" << map.grid[position].GetOwner() << "\n";
                    cout << "----------------" << "\n";
                }
                else if(option == "no") {
                    
                }
            }
        }
        // pay toll
        else {
            if(player_num != map.grid[position].GetOwner()) { // not owner
                toll = map.grid[position].GetCost() * pow(2, player[map.grid[position].GetOwner()].grid_station.size() - 1);
                cout << "----------------" << "\n";
                cout << "This station belong to player" << map.grid[position].GetOwner() << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "Toll are " << toll << " dollars" << "\n";
                cout << "----------------" << "\n";
                
                
                player[player_num].SetMoney(player[player_num].GetMoney() - toll);
                player[map.grid[position].GetOwner()].SetMoney(player[map.grid[position].GetOwner()].GetMoney() + toll);

                cout << "----------------" << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "----------------" << "\n";
            }
        }
        break;
    case infrastructure:
        // buy infrastructure
        if(map.grid[position].GetEmpty()) { 
            if(player[player_num].GetMoney() >= map.grid[position].GetValue()) { // check whether player have enough money
                cout << "----------------" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "This infrastructure cost " << map.grid[position].GetValue() << " dollars" << "\n";
                cout << "yes to buy, no to not buy" << "\n";
                cout << "----------------" << "\n";
				popout = true;
				while (!getanswer) {
					cout << "";
				}
				if (answer == 1) {
					option = "yes";
				}
				else if (answer == 2) {
					option = "no";
				}
				cout << option << endl;
                if(option == "yes") {
					tsuidx = position;
					tsuheight = 0;
					tsuowner = player_num;
					keetsu = true;
                    player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
                    map.grid[position].SetOwner(player_num);
                    map.grid[position].SetEmpty(false);
                    player[player_num].grid_infrastructure.push_back(position);
                    cout << "----------------" << "\n";
                    cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "This infrastructure belong to player" << map.grid[position].GetOwner() << "\n";
                    cout << "----------------" << "\n";
                }
                else if(option == "no") {
                    
                }
            }
        }
        // pay toll
        else {
            if(player_num != map.grid[position].GetOwner()) { // not owner
				//TODO
                int times = player[player_num].PureThrowDice(1, 1); // throw dice to how many times tolls are
                toll = map.grid[position].GetCost() * times; 

                cout << "----------------" << "\n";
                cout << "This infrastructure belong to player" << map.grid[position].GetOwner() << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "The result of dice is " << times << "\n";
                cout << "Toll are " << toll << " dollars" << "\n";
                cout << "----------------" << "\n";

                
                player[player_num].SetMoney(player[player_num].GetMoney() - toll);
                player[map.grid[position].GetOwner()].SetMoney(player[map.grid[position].GetOwner()].GetMoney() + toll);

                cout << "----------------" << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "----------------" << "\n";
            }
        }
        break;
	case opportunity:
		//TODO card
		card = opportunity_random.back();
		getcard = true;
		cardtype = 0;
		cardidx = card;
		goSide = true;
		opportunity_random.pop_back();
		while (!getcarddone) {
			cout << "";
		}
		switch (card)
		{
		case 0:
			player[player_num].SetMoney(player[player_num].GetMoney() + 600);
			break;
		case 1:
			player[player_num].SetMoney(player[player_num].GetMoney() + 500);
			break;
		case 2:
			player[player_num].SetMoney(player[player_num].GetMoney() + 200);
			break;
		case 3:
			if (position < 6) {
				smove = true;
				smoveidx = player_num;
				smovepos = 6;
				player[player_num].Move(6 - position);
			}
			else {
				smove = true;
				smoveidx = player_num;
				smovepos = 6;
				player[player_num].Move(6 + 40 - position);
			}
			break;
		case 4:
			if (position - 3 < 0) {
				smove = true;
				smoveidx = player_num;
				smovepos = position + 40 - 3;
				player[player_num].SetPosition(position + 40 - 3);
			}
			else {
				smove = true;
				smoveidx = player_num;
				smovepos = position - 3;
				player[player_num].SetPosition(position - 3);
			}
			break;
		case 5:
			player[player_num].SetMoney(player[player_num].GetMoney() + 100);
			break;
		case 6:
			player[player_num].SetMoney(player[player_num].GetMoney() + 600);
			break;
		case 7:
			player[player_num].SetMoney(player[player_num].GetMoney() + 300);
			break;
		case 8:
			player[player_num].Rest();
			break;
		case 9:
			player[player_num].SetMoney(player[player_num].GetMoney() - 500);
			break;
		case 10:
			player[player_num].SetMoney(player[player_num].GetMoney() - 300);
			break;
		case 11:
			player[player_num].SetMoney(player[player_num].GetMoney() - 1000);
			break;
		case 12:
			player[player_num].Rest();
			break;
		case 13:
			player[player_num].SetMoney(player[player_num].GetMoney() - 500);
			break;
		case 14:
			player[player_num].Rest();
			break;
		case 15:
			player[player_num].SetMoney(player[player_num].GetMoney() - 200);
			break;
		case 16:
			player[player_num].SetMoney(player[player_num].GetMoney() - 100);
			break;
		case 17:
			if (position < 15) {
				smove = true;
				smoveidx = player_num;
				smovepos = 15;
				player[player_num].Move(15 - position);
			}
			else {
				smove = true;
				smoveidx = player_num;
				smovepos = 15;
				player[player_num].Move(15 + 40 - position);
			}
			break;
		case 18:
			player[player_num].Move(3);
			break;
		case 19:
			smove = true;
			smoveidx = player_num;
			smovepos = 10;
			player[player_num].GoJail();
			break;
		default:
			break;
		}
		if (opportunity_random.empty()) {
			ShuffleOpportunity();
		}
		break;
	case fate:
		//TODO card
		card = fate_random.back();
		getcard = true;
		cardtype = 1;
		cardidx = card;
		goSide = true;
		fate_random.pop_back();
		while (!getcarddone) {
			cout << "";
		}
		switch (card)
		{
		case 0:
			player[player_num].SetMoney(player[player_num].GetMoney() + 200);
			break;
		case 1:
			player[player_num].SetMoney(player[player_num].GetMoney() + 200);
			break;
		case 2:
			player[player_num].SetMoney(player[player_num].GetMoney() + 100);
			break;
		case 3:
			player[player_num].SetMoney(player[player_num].GetMoney() + 1000);
			break;
		case 4:
			smove = true;
			smoveidx = player_num;
			smovepos = player[player_num].GetPosition() + 2;
			player[player_num].Move(2);
			break;
		case 5:
			player[player_num].SetMoney(player[player_num].GetMoney() + 500);
			break;
		case 6:
			smove = true;
			smoveidx = player_num;
			smovepos = player[player_num].GetPosition() + 3;
			player[player_num].Move(3);
			break;
		case 7:
			player[player_num].SetMoney(player[player_num].GetMoney() + 300);
			break;
		case 8:
			smove = true;
			smoveidx = player_num;
			smovepos = 0;
			player[player_num].SetPosition(0);
			break;
		case 9:
			player[player_num].Rest();
			break;
		case 10:
			player[player_num].SetMoney(player[player_num].GetMoney() - 200);
			break;
		case 11:
			if (position < 5) {
				smove = true;
				smoveidx = player_num;
				smovepos = 5;
				player[player_num].Move(5 - position);
			}
			else {
				smove = true;
				smoveidx = player_num;
				smovepos = 5;
				player[player_num].Move(5 + 40 - position);
			}
			break;
		case 12:
			player[player_num].Rest();
			break;
		case 13:
			player[player_num].SetMoney(player[player_num].GetMoney() - 500);
			break;
		case 14:
			player[player_num].Rest();
			break;
		case 15:
			player[player_num].SetMoney(player[player_num].GetMoney() - 100);
			break;
		case 16:
			player[player_num].SetMoney(player[player_num].GetMoney() + 1000);
			break;
		case 17:
			if (position - 1 < 0) {
				smove = true;
				smoveidx = player_num;
				smovepos = position + 40 - 1;
				player[player_num].SetPosition(position + 40 - 1);
			}
			else {
				smove = true;
				smoveidx = player_num;
				smovepos = position - 1;
				player[player_num].SetPosition(position - 1);
			}
			break;
		case 18:
			player[player_num].SetMoney(player[player_num].GetMoney() - 200);
			break;
		case 19:
			player[player_num].SetMoney(player[player_num].GetMoney() - 600);
			break;
		case 20:
			smove = true;
			smoveidx = player_num;
			smovepos = 10;
			player[player_num].GoJail();
		default:
			break;
		}
		if (fate_random.empty()) {
			ShuffleOpportunity();
		}
		break;
    case jail:
		smove = true;
		smoveidx = player_num;
		smovepos = 10;
        player[player_num].GoJail();
        break;
    case pay:
        switch (position)
        {
        case 4: 
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "1 to cost 2000, 2 to cost 10%" << "\n";
            cout << "----------------" << "\n";
			popout = true;
			while (!getanswer) {
				cout << "";
			}
			if (answer == 1) {
				option = "1";
			}
			else if (answer == 2) {
				option = "2";
			}
			cout << option << endl;
            if(option == "1") {
                player[player_num].SetMoney(player[player_num].GetMoney() - 2000);
            }
            else if(option == "2") {
                player[player_num].SetMoney(player[player_num].GetMoney() * 0.9);
            }
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            break;
        case 38:
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
        default:
            break;
        }
        break;
    case park:
        // do nothing
        break;
	case jail_visit:
		//TODO
		dice_result = player[player_num].ThrowDice(1,1);
		dice_result = rand();
		if (dice_result % 2 == 0) {
			smove = true;
			smoveidx = player_num;
			smovepos = 10;
			player[player_num].GoJail();
		}
		else if (dice_result % 2 == 1) {
			// do nothing
		}
		break;
    default:
        break;
    }
}
void Game::Show() {
    // cout << num_of_player << "\n";
    for(int player_iter = 0; player_iter < num_of_player; player_iter++){
        // cout << player[player_iter].GetMoney() << "\n";
        // cout << player[player_iter].GetPosition() << "\n";
    }
    for(int grid_iter = 0; grid_iter < NUM_OF_GRID; grid_iter++){
        // cout << map.GetGrid()[grid_iter].GetLabel() << "\n";
        cout << map.grid[grid_iter].GetValue() << "\n";
    }
}