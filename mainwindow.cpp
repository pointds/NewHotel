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

void deserializeMap(std::map<std::string, Room>& roomMap, const std::string& filename) {
    // Check if the file exists
    if (!std::filesystem::exists(filename)) {
       return; // File does not exist, return early
    }
    std::ifstream inFile(filename, std::ios::binary);
    if (inFile.is_open()) {
        size_t mapSize;
        inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
        for (size_t i = 0; i < mapSize; ++i) {
            size_t keyLength;
            inFile.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
            std::string key(keyLength, ' ');
            inFile.read(&key[0], keyLength);
            Room room;
            room.deserialize(inFile);
            roomMap[key] = room;
        }
        inFile.close();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , roomManagerWindow(new RoomManager(this))
    , roomListWindow(new RoomList(this))
{
    void keyPressEvent(QKeyEvent *event);
    ui->setupUi(this);
    roomManagerWindow->setWindowFlags(Qt::Window);
    roomManagerWindow->hide();

    roomListWindow->setWindowFlags(Qt::Window);
    roomListWindow->hide();

    loadData(ROOMFILE);

    connect(ui->manageBtn, &QPushButton::clicked, this, &MainWindow::on_openRoomManage);

    connect(ui->checkBtn, &QPushButton::clicked, this, &MainWindow::on_openRoomList);
}

MainWindow::~MainWindow()
{
    storeData(ROOMFILE);
    delete ui;
}

void MainWindow::loadData(const string &filename)
{
    try {
        deserializeMap(mRoomMap, filename);
        std::cout << "Map deserialized from " << filename << std::endl;

        // 输出反序列化结果
        for (const auto& pair : mRoomMap) {
            std::cout << "Key: " << pair.first << ", Room Number: " << pair.second.getRoomNumber() << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

}

void MainWindow::storeData(const string &filename)
{

    try {
        if(mRoomMap.size()<=0) return;

        serializeMap(mRoomMap, filename);
        std::cout << "Map serialized to " << filename << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void MainWindow::on_checkBtn_released()
{
    //ui->stackedWidget->setCurrentWidget(ui->checkpage);
}

void MainWindow::on_exitBtn_released()
{
    close();
}

void MainWindow::on_manageBtn_released()
{
    //ui->stackedWidget->setCurrentWidget(ui->managepage);
}

void MainWindow::on_openRoomManage()
{
    roomManagerWindow->show();
    roomManagerWindow->activateWindow(); // 激活窗口
    roomManagerWindow->raise(); // 提升窗口到最上层
}

void MainWindow::on_openRoomList()
{
    roomListWindow->refresh();
    roomListWindow->show();
    roomListWindow->activateWindow(); // 激活窗口
    roomListWindow->raise(); // 提升窗口到最上层
}

//RoomManager class
////////////////////////////////////////////////////
RoomManager::RoomManager(QWidget *parent)
    : QWidget(parent)
    , uiRoomManager(new Ui::roomManager)
    , roomDetailWindow(new RoomDetail(this))
{
    uiRoomManager->setupUi(this);
    roomDetailWindow->setWindowFlags(Qt::Window);
    roomDetailWindow->hide();
    connect(uiRoomManager->addRoomBtn, &QPushButton::clicked, this, &RoomManager::on_openAddRoom);
    connect(uiRoomManager->delRoomBtn, &QPushButton::clicked, this, &RoomManager::on_openDelRoom);
    connect(uiRoomManager->updateRoomBtn, &QPushButton::clicked, this, &RoomManager::on_openUpdateRoom);
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
void RoomManager::on_openUpdateRoom()
{
    roomDetailWindow->setState(ForUpdate);
    roomDetailWindow->setOperateLabel("更新");
    roomDetailWindow->cleanUI();
    roomDetailWindow->hideInfoLabel();
    roomDetailWindow->show();
}

//RoomDetail class
////////////////////////////////////////////////////
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

void RoomDetail::getInfoFromUI()
{
    mRoom.setRoomNumber(uiRoomDetail->roomNumL->text().toStdString());
    mRoom.setRoomArea(uiRoomDetail->roomSizeL->text().toInt());
    mRoom.setRoomPrice(uiRoomDetail->roomPriceL->text().toInt());
    mRoom.setBedNum(uiRoomDetail->benNumL->text().toInt());
    mRoom.setState(ROOM_STATE(uiRoomDetail->roomStateCBox->currentIndex()));

}

void RoomDetail::setInfoToUI(const Room& iroom)
{
    uiRoomDetail->roomNumL->setText(QString::fromStdString(iroom.getRoomNumber()));
    uiRoomDetail->roomSizeL->setText(QString::number(iroom.getRoomArea()));
    uiRoomDetail->roomPriceL->setText(QString::number(iroom.getRoomPrice()));
    uiRoomDetail->benNumL->setText(QString::number(iroom.getBedNum()));
    uiRoomDetail->roomStateCBox->setCurrentIndex(iroom.getState());
}

void RoomDetail::hideInfoLabel()
{
    uiRoomDetail->infoLabel->hide();
}
void RoomDetail::showInfoLabel()
{
    uiRoomDetail->infoLabel->show();
}
void RoomDetail::setOperateLabel(const QString &str)
{
    uiRoomDetail->operateLabel->setText(str);
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
void RoomDetail::on_CancelBtn()
{
    this->hide();
}

//RoomList class
////////////////////////////////////////////////////
RoomList::RoomList(QWidget *parent)
    : QWidget(parent)
    , uiRoomList(new Ui::roomList)
{
    uiRoomList->setupUi(this);
    uiRoomList->tableWidget->setSortingEnabled(true);
    // 设置列自适应
    uiRoomList->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    connect(uiRoomList->refreshBtn, &QPushButton::clicked, this, &RoomList::on_refreshBtn);
}

RoomList::~RoomList()
{
    delete uiRoomList;
}

void RoomList::refresh()
{
    on_refreshBtn();
}

void RoomList::on_refreshBtn()
{
    // 清空表格内容
    uiRoomList->tableWidget->clearContents();
    uiRoomList->tableWidget->setRowCount(0); // 重设行数

    for (const auto &pair : mRoomMap) {
        const Room &room = pair.second;
        int currentRow = uiRoomList->tableWidget->rowCount();
        uiRoomList->tableWidget->insertRow(currentRow);

        uiRoomList->tableWidget->setItem(currentRow, 0, new QTableWidgetItem(QString::fromStdString(room.getRoomNumber())));
        uiRoomList->tableWidget->setItem(currentRow, 1, new QTableWidgetItem(QString::number(room.getRoomPrice())));
        uiRoomList->tableWidget->setItem(currentRow, 2, new QTableWidgetItem(QString::number(room.getRoomArea())));
        uiRoomList->tableWidget->setItem(currentRow, 3, new QTableWidgetItem(QString::number(room.getBedNum())));

        QString stateStr = (room.getState() == FREE) ? "空闲" : "占用";
        uiRoomList->tableWidget->setItem(currentRow, 4, new QTableWidgetItem(stateStr));
    }
}


