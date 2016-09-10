#include "mainwindow.h"

#include<QDebug>
#include"aboutdialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    topLabelList.clear();
    isShotting=false;
    registerGlobalKey();
    initTray();
    aboutDialog =new AboutDialog;
    connect(this,SIGNAL(hotkeyShotBgPressed()),SLOT(hotkeyShotBgReceived()));
    trayIcon->showMessage(tr("截图置顶 v1.2.0"),"程序已启动,按\"SHIFT+ALT+Z\"后拖动鼠标！");
    trayIcon->setToolTip(tr("按\"SHIFT+ALT+Z\"后拖动鼠标"));


}

MainWindow::~MainWindow()
{

    if(UnregisterHotKey(HWND(this->winId()), hotkeyShotBgId))
    {
        qDebug("SHIFT+ALT+Z IS UNREGISTED");
    }

    clearShots();
}


void MainWindow::registerGlobalKey()
{
    hotkeyShotBgId =GlobalAddAtom(L"SsotGlobalHotKey") - 0xC000;

    if(RegisterHotKey((HWND(this->winId())), hotkeyShotBgId,MOD_SHIFT|MOD_ALT,0x5A))
    {
        qDebug("SHIFT+ALT+Z");
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
            if(fuModifiers==MOD_SHIFT|MOD_ALT && uVirtKey==0x5A)
            {
                emit hotkeyShotBgPressed();
                qDebug("SHIFT+ALT+Z is Pressed");

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

        CTopLabel* fullScreenLabel=new CTopLabel;

        topLabelList.append(fullScreenLabel);

        connect(fullScreenLabel, SIGNAL(destroyed(QObject*)), SLOT(removeLabel(QObject*)));
        //connect(fullScreenLabel,SIGNAL(meClosed(WId)),this,SLOT(clearShot(WId)));

        connect(fullScreenLabel,SIGNAL(shotted()),this,SLOT(allowShot()));

        fullScreenLabel->showFullScreen();
    }

    isShotting = true;

}

void MainWindow::initTray()
{
    trayIcon=new QSystemTrayIcon(this);

    trayIcon->setIcon(QIcon(":/icon/resource/icon.png"));

    QMenu* trayMenu=new QMenu;

    QAction* exitAction=new QAction(tr("退出 (&X)"),trayMenu);
    QAction* aboutAciton=new QAction(tr("关于 (&A)"),trayMenu);
    QAction* clearAciton=new QAction(tr("清除截图 (&C)"),trayMenu);

    trayMenu->addAction(clearAciton);

    connect(clearAciton,SIGNAL(triggered()),this,SLOT(clearShots()));

    trayMenu->addSeparator();

    trayMenu->addAction(aboutAciton);

    connect(aboutAciton,SIGNAL(triggered()),this,SLOT(doAboutAction()));

    trayMenu->addAction(exitAction);

    connect(exitAction,SIGNAL(triggered()),this,SLOT(quitApp()));

    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();
}

void MainWindow::quitApp(){
     trayIcon->hide();
     QApplication::quit();
}

void MainWindow::removeLabel(QObject *label)
{
   topLabelList.removeAll(static_cast<CTopLabel*>(label));
}

void MainWindow::clearShots()
{
    while(topLabelList.count()>0)
    {
        if(topLabelList.first()){
            topLabelList.first()->deleteLater();
        }

        topLabelList.removeFirst();
    }

}


void MainWindow::allowShot()
{
    isShotting=false;
}

void MainWindow::doAboutAction()
{
    qDebug()<<"about";


    aboutDialog->setText(":/icon/readme.txt",true);

    aboutDialog->setWindowTitle(tr("关于Ssot v1.2.0"));

    aboutDialog->setLogo(":/icon/resource/about-logo.png");

    aboutDialog->setInfo("<table border=\"0\"><tr height=\"22\"><td width=270 valign=\"middle\" ><b>Ssot v1.2.0</b></td><td><a href=\"https://github.com/pansinm/Ssot/\">查看源代码</a></td></tr><tr height=\"22\"><td width=300 valign=\"middle\">by pansinm</td><td>pansinm@163.com</td></tr></table>");

    aboutDialog->show();

}
