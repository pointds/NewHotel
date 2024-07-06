#ifndef ROOM_H
#define ROOM_H
#include<iostream>
#include<string>
#include<map>
#include <fstream>

#endif // ROOM_H
using namespace std;
enum ROOM_STATE
{
    FREE = 0,
    OCCUPIED
};

class Room
{
public:
    Room()=default;
    Room(std::string roomNumber, int roomPrice, int roomArea, int bedNum, ROOM_STATE state);

    // Getters
    std::string getRoomNumber() const;
    int getRoomPrice() const;
    int getRoomArea() const;
    int getBedNum() const;
    ROOM_STATE getState() const;

    // Setters
    void setRoomNumber(const std::string &roomNumber);
    void setRoomPrice(int roomPrice);
    void setRoomArea(int roomArea);
    void setBedNum(int bedNum);
    void setState(ROOM_STATE state);

    // Serialization and Deserialization
    void serialize(std::ofstream &out) const;
    void deserialize(std::ifstream &in);
private:
    string mRoomNumber;
    int mRoomPrice{0};
    int mRoomArea{0};
    int mBedNum{1};
    enum ROOM_STATE mState{FREE};
};

