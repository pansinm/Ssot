#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QtWidgets>
#include<QVector>
#include"aboutdialog.h"
#include"ctoplabel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    struct IDStruct
    {
        WId id;
        CTopLabel* pTopLabel;
    };

private:
    //托盘图标
    //注册热键
    void registerGlobalKey();

    //开始截图热键ID
    int hotkeyShotBgId;

    QVector<IDStruct> topLabelList;


    //初始化托盘;
    void initTray();

    //是否正在截图
    bool isShotting;

    //本地事件
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    AboutDialog* aboutDialog;

    QSystemTrayIcon* trayIcon;




private slots:

    //开始截图
    void hotkeyShotBgReceived();

    void clearShots();



public slots:
    //关闭
    void clearShot(WId i);

    //设置isShotting为false，允许再次截屏
    void allowShot();

    //关于
    void doAboutAction();


signals:

    //开始截屏热键按下
    void hotkeyShotBgPressed();

};

#endif // MAINWINDOW_H
