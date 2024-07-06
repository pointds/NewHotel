#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_roomManage.h"
#include "ui_roomDetail.h"
#include "ui_roomList.h"
#include <QKeyEvent>
#include <QFile>
#include<QTextStream>
#include <QDialog>
#include <QMessageBox>
#include <map>
#include <string>
#include <fstream>

const static string ROOMFILE = "rooms.dat";
map<string, Room> mRoomMap;
void serializeMap(const std::map<std::string, Room>& roomMap, const std::string& filename) {
    // 删除源文件
    std::filesystem::remove(filename);

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Could not open file for writing.");
    }

    if (outFile.is_open()) {
        size_t mapSize = roomMap.size();
        outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
        for (const auto& pair : roomMap) {
            size_t keyLength = pair.first.size();
            outFile.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
            outFile.write(pair.first.c_str(), keyLength);
            pair.second.serialize(outFile);
        }
        outFile.close();
    }
}

MainWindow::~MainWindow()
{
    storeData(ROOMFILE);
    delete ui;
}

void MainWindow::on_exitBtn_released()
{
    close();
}

RoomManager::~RoomManager()
{
    delete uiRoomManager;
}
void RoomManager::on_openAddRoom()
{
    roomDetailWindow->setState(ForAdd);
    roomDetailWindow->setOperateLabel("添加");
    roomDetailWindow->cleanUI();
    roomDetailWindow->hideInfoLabel();
    roomDetailWindow->show();
}
void RoomManager::on_openDelRoom()
{
    roomDetailWindow->setState(ForDelete);
    roomDetailWindow->setOperateLabel("删除");
    roomDetailWindow->cleanUI();
    roomDetailWindow->showInfoLabel();
    roomDetailWindow->show();
}

RoomDetail::RoomDetail(QWidget *parent)
    : QWidget(parent)
    , uiRoomDetail(new Ui::roomDetail)
{
    uiRoomDetail->setupUi(this);


    connect(uiRoomDetail->loadBtn, &QPushButton::clicked, this, &RoomDetail::on_loadBtn);
    connect(uiRoomDetail->okBtn, &QPushButton::clicked, this, &RoomDetail::on_OKBtn);
    connect(uiRoomDetail->cancelBtn, &QPushButton::clicked, this, &RoomDetail::on_CancelBtn);

}

RoomDetail::~RoomDetail()
{
    delete uiRoomDetail;
}

void RoomDetail::cleanUI()
{
    uiRoomDetail->roomNumL->clear();
    uiRoomDetail->roomSizeL->clear();
    uiRoomDetail->roomPriceL->clear();
    uiRoomDetail->benNumL->clear();
    uiRoomDetail->roomStateCBox->clearEditText();
}



void RoomDetail::on_loadBtn()
{
    string roomNum = uiRoomDetail->roomNumL->text().toStdString();

    cleanUI();

    auto it = mRoomMap.find(roomNum);
    if(it == mRoomMap.end())
    {
        QMessageBox msgBox;
        QString roomNumber = QString::fromStdString(roomNum); // 将 std::string 转换为 QString

        QString msg = "房间号" + roomNumber + "不存在！";

        msgBox.setText(msg);
        msgBox.setWindowTitle("提示");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.addButton("确认", QMessageBox::AcceptRole);
        msgBox.exec();
        uiRoomDetail->roomNumL->setText(roomNumber);
    }
    else
    {
        setInfoToUI(it->second);
    }
}
void RoomDetail::on_OKBtn()
{
    switch (mState)
    {
    case ForAdd:
        getInfoFromUI();
        if(mRoomMap.find(mRoom.getRoomNumber()) != mRoomMap.end())
        {
            QMessageBox msgBox;
            std::string roomNumberStr = mRoom.getRoomNumber();
            QString roomNumber = QString::fromStdString(roomNumberStr); // 将 std::string 转换为 QString

            QString msg = "房间号" + roomNumber + "已经存在！";

            msgBox.setText(msg);
            msgBox.setWindowTitle("提示");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.addButton("确认", QMessageBox::AcceptRole);
            msgBox.exec();
        }
        else
        {
            mRoomMap[mRoom.getRoomNumber()] = mRoom;
            this->hide();
        }
        break;
    case ForDelete:
        {
            mRoom.setRoomNumber(uiRoomDetail->roomNumL->text().toStdString());
            auto it = mRoomMap.find(mRoom.getRoomNumber());
            if(it == mRoomMap.end())
            {
                QMessageBox msgBox;
                std::string roomNumberStr = mRoom.getRoomNumber();
                QString roomNumber = QString::fromStdString(roomNumberStr); // 将 std::string 转换为 QString

                QString msg = "房间号" + roomNumber + "不存在！";

                msgBox.setText(msg);
                msgBox.setWindowTitle("提示");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.addButton("确认", QMessageBox::AcceptRole);
                msgBox.exec();
            }
            else
            {
                mRoomMap.erase(it);
                this->hide();
            }
        }
        break;
    case ForUpdate:
        {
            mRoom.setRoomNumber(uiRoomDetail->roomNumL->text().toStdString());
            auto it = mRoomMap.find(mRoom.getRoomNumber());
            if(it == mRoomMap.end())
            {
                QMessageBox msgBox;
                std::string roomNumberStr = mRoom.getRoomNumber();
                QString roomNumber = QString::fromStdString(roomNumberStr); // 将 std::string 转换为 QString

                QString msg = "房间号" + roomNumber + "不存在！";

                msgBox.setText(msg);
                msgBox.setWindowTitle("提示");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.addButton("确认", QMessageBox::AcceptRole);
                msgBox.exec();
            }
            else
            {
                getInfoFromUI();
                mRoomMap[mRoom.getRoomNumber()] = mRoom;
                this->hide();
            }
        }
        break;
    default:
        break;
    }

}

RoomList::~RoomList()
{
    delete uiRoomList;
}



