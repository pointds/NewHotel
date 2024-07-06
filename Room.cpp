#include "Room.h"

// Constructor implementation
Room::Room(std::string roomNumber, int roomPrice, int roomArea, int bedNum, ROOM_STATE state)
    : mRoomNumber(roomNumber), mRoomPrice(roomPrice), mRoomArea(roomArea), mBedNum(bedNum), mState(state)
{
}

// Getter implementations
std::string Room::getRoomNumber() const {
    return mRoomNumber;
}

int Room::getRoomPrice() const {
    return mRoomPrice;
}

int Room::getRoomArea() const {
    return mRoomArea;
}

int Room::getBedNum() const {
    return mBedNum;
}

ROOM_STATE Room::getState() const {
    return mState;
}

// Setter implementations
void Room::setRoomNumber(const std::string &roomNumber) {
    mRoomNumber = roomNumber;
}

void Room::setRoomPrice(int roomPrice) {
    mRoomPrice = roomPrice;
}

void Room::setRoomArea(int roomArea) {
    mRoomArea = roomArea;
}

void Room::setBedNum(int bedNum) {
    mBedNum = bedNum;
}

void Room::setState(ROOM_STATE state) {
    mState = state;
}


// Serialization
void Room::serialize(std::ofstream &out) const {
    size_t length = mRoomNumber.size();
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    out.write(mRoomNumber.c_str(), length);
    out.write(reinterpret_cast<const char*>(&mRoomPrice), sizeof(mRoomPrice));
    out.write(reinterpret_cast<const char*>(&mRoomArea), sizeof(mRoomArea));
    out.write(reinterpret_cast<const char*>(&mBedNum), sizeof(mBedNum));
    out.write(reinterpret_cast<const char*>(&mState), sizeof(mState));
}

void Room::deserialize(std::ifstream &in) {
    size_t length;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    mRoomNumber.resize(length);
    in.read(&mRoomNumber[0], length);
    in.read(reinterpret_cast<char*>(&mRoomPrice), sizeof(mRoomPrice));
    in.read(reinterpret_cast<char*>(&mRoomArea), sizeof(mRoomArea));
    in.read(reinterpret_cast<char*>(&mBedNum), sizeof(mBedNum));
    in.read(reinterpret_cast<char*>(&mState), sizeof(mState));
}
