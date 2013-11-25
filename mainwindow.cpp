#include "mainwindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    isShotting=false;
    registerGlobalKey();
    initTray();
    connect(this,SIGNAL(hotkeyShotBgPressed()),SLOT(hotkeyShotBgReceived()));
}

MainWindow::~MainWindow()
{
    if(UnregisterHotKey(HWND(this->winId()), hotkeyShotBgId))
    {
        qDebug("SHIFT+Z IS UNREGISTED");
    }

    clearShots();
}


void MainWindow::registerGlobalKey()
{
    hotkeyShotBgId =GlobalAddAtom(L"hotkeyShotBg") - 0xC000;
    if(RegisterHotKey((HWND(this->winId())), hotkeyShotBgId,MOD_SHIFT,0x5A))
    {
        qDebug("SHIFT+Z IS REGISTER");
    }
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(eventType=="windows_generic_MSG")
    {
        //qDebug("This is windows MSG");
        MSG* msg=static_cast<MSG*>(message);
        if(msg->message==WM_HOTKEY)
        {
            UINT fuModifiers = (UINT) LOWORD(msg->lParam);  // key-modifier flags
            UINT uVirtKey = (UINT) HIWORD(msg->lParam);     // virtual-key code

           // qDebug("This is HotKey!");
            if(fuModifiers==MOD_SHIFT&&uVirtKey==0x5A)
            {
                emit hotkeyShotBgPressed();
                qDebug("SHIFT+Z is Pressed");

            }

            return true;

         }
    }
 return  false;

}

void MainWindow::hotkeyShotBgReceived()
{
    if(!isShotting)
    {
        //如果正在截图，则不处理，否则进入截图状态，将list位置传给CTopLabel

        CTopLabel* fullScreenLabel=new CTopLabel(topLabelList.count());

        topLabelList.append(fullScreenLabel);

        connect(fullScreenLabel,SIGNAL(closeMe(int)),this,SLOT(clearShot(int)));

        connect(fullScreenLabel,SIGNAL(shotted()),this,SLOT(allowShot()));

        fullScreenLabel->showFullScreen();
    }

    isShotting=true;

}

void MainWindow::initTray()
{
    trayIcon=new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icon/resource/icon.png"));

    QMenu* trayMenu=new QMenu;
    QAction* exitAction=new QAction(tr("退出 (&X)"),trayMenu);
    QAction* aboutAciton=new QAction(tr("&About"),trayMenu);
    QAction* clearAciton=new QAction(tr("清除 (&C)"),trayMenu);

    trayMenu->addAction(clearAciton);
    connect(clearAciton,SIGNAL(triggered()),this,SLOT(clearShots()));

    trayMenu->addSeparator();
    trayMenu->addAction(aboutAciton);

    trayMenu->addAction(exitAction);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void MainWindow::clearShots()
{
    //删除指针,清除所有截图
    if(topLabelList.count()>0)
    {
        for(int i=topLabelList.count()-1;i>=0;i--)
        {
            delete topLabelList.at(i);
        }
        topLabelList.clear();
    }
}

void MainWindow::clearShot(int i)
{
    qDebug()<<"clearShot"<<i;
    QList<CTopLabel*>::iterator iterator;
    if(i>=0&&i<topLabelList.count())
    {
        iterator=topLabelList.begin();

        delete(*(iterator+i));
        topLabelList.erase(iterator+i);

    }
}

void MainWindow::allowShot()
{
    isShotting=false;
}
