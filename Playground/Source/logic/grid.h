#ifndef _GRID_H_
#define _GRID_H_
using namespace std;
enum Label
{
    start,
    red,
    orange,
    yellow,
    green,
    blue,
    light,
    purple,
    pink,
    station,
    infrastructure,
    opportunity,
    fate,
    jail,
    pay,
    park,
    jail_visit
};
class Grid{
public:
    Grid();
    void SetLabel(Label label) {
        this -> label = label;
    };
    Label GetLabel() {
        return label;
    }
    void SetValue(int value) {
        this -> value = value;
    }
    int GetValue() {
        return value;
    }
    bool GetEmpty() {
        return is_empty;
    }
    void SetEmpty(bool is_empty) {
        this -> is_empty = is_empty;
    }
    void SetOwner(int owner) {
        this -> owner = owner;
    }
    int GetOwner() {
        return owner;
    }
    int GetCost() {
        return cost;
    }
    void SetCost(int cost) {
        this -> cost = cost;
    }
    void SetBuild(int build) {
        this -> build = build;
    }
    int GetNumOfHouse() {
        return num_of_house;
    }
    void SetNumOfHouse(int num_of_house) {
        this -> num_of_house = num_of_house;
    }
    void Init();
private:
    Label label;
    int value;
    bool is_empty = true; // to check land is bougut or not
    int owner = -1;
    int cost = 0; // pay for owner
    int num_of_house = 0;
    int build = 0;
};
#endif