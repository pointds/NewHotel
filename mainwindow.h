#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "Room.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class roomManager;
class roomDetail;
class roomList;
}
class RoomManager;
class RoomDetail;
class RoomList;

QT_END_NAMESPACE

extern map<string, Room> mRoomMap;

enum RoomDetailState
{
    NotUsed = -1,
    ForAdd = 0,
    ForUpdate,
    ForDelete
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadData(const string &filename);
    void storeData(const string &filename);

private slots:

    void on_checkBtn_released();

    void on_exitBtn_released();

    void on_manageBtn_released();

    void on_openRoomManage();
    void on_openRoomList();
private:
    Ui::MainWindow *ui;
    RoomManager *roomManagerWindow;
    RoomList *roomListWindow;
};
/////////////////////////////////////////////
class RoomManager : public QWidget
{
    Q_OBJECT

public:
    RoomManager(QWidget *parent = nullptr);
    ~RoomManager();

private slots:

    void on_openAddRoom();
    void on_openDelRoom();
    void on_openUpdateRoom();
private:
    Ui::roomManager *uiRoomManager;
    RoomDetail *roomDetailWindow;

};
/////////////////////////////////////////////
class RoomDetail : public QWidget
{
    Q_OBJECT

public:
    RoomDetail(QWidget *parent = nullptr);
    ~RoomDetail();

    void cleanUI();
    void getInfoFromUI();
    void setInfoToUI(const Room& iroom);

    void hideInfoLabel();
    void showInfoLabel();
    void setState(RoomDetailState state)
    {
        mState = state;
    };
    RoomDetailState getState()
    {
        return mState;
    };
    void setOperateLabel(const QString &str);
private slots:

    void on_loadBtn();
    void on_OKBtn();
    void on_CancelBtn();
private:
    Ui::roomDetail *uiRoomDetail;
    Room mRoom;

    RoomDetailState mState{NotUsed};
};

/////////////////////////////////////////////
class RoomList : public QWidget
{
    Q_OBJECT

public:
    RoomList(QWidget *parent = nullptr);
    ~RoomList();

    void refresh();
private slots:

    void on_refreshBtn();
private:
    Ui::roomList *uiRoomList;
};
#endif // MAINWINDOW_H
