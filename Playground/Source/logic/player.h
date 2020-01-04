#ifndef _PLAYER_H_
#define _PLAYER_H_
using namespace std;
#include <vector>
class Player {
public:
	Player();
	int GetMoney() {
		return money;
	}
	void SetMoney(int money) {
		this->money = money;
	}
	int GetPosition() {
		return position;
	}
	void SetPosition(int position) {
		this->position = position;
	}
	void GoJail();
	void LeaveJail();
	// void GoBankrupt();
	bool GetOneMore() {
		return one_more;
	}
	bool GetInJail() {
		return in_jail;
	}
	int GetJailDay() {
		return jail_day;
	}
	void SetJailDay(int jail_day) {
		this->jail_day = jail_day;
	}
	void SetInJail(bool in_jail) {
		this->in_jail = in_jail;
	}
	void SetIsRest(bool is_rest) {
		this->is_rest = is_rest;
	}
	bool GetIsRest() {
		return is_rest;
	}
	int GetRestDay() {
		return rest_day;
	}
	void SetRestDay(int rest_day) {
		this->rest_day = rest_day;
	}
	bool GetBankRupt() {
		return is_bankrupt;
	}
	void SetBankRupt(bool is_bankrupt) {
		this->is_bankrupt = is_bankrupt;
	}
	// void SetOneMore(bool one_more) {
	//     this -> one_more = one_more;
	// }
	void Init();
	int ThrowDice(int ont, int two);
	void Move(int);
	void Rest();
	void LeaveRest();
	int PureThrowDice(int one, int two);
	vector<int> grid_red;
	vector<int> grid_orange;
	vector<int> grid_yellow;
	vector<int> grid_green;
	vector<int> grid_blue;
	vector<int> grid_light;
	vector<int> grid_purple;
	vector<int> grid_pink;
	vector<int> grid_infrastructure;
	vector<int> grid_station;
private:
	int money = 15000;
	int position = 0;
	bool one_more = false;
	int continuous_move = 0;
	bool in_jail = false;
	int jail_day = 0;
	bool is_bankrupt = false;
	bool is_rest = false;
	int rest_day = 0;
};
#endif