#include "controlbar.h"
#include<QHBoxLayout>
#include<QSize>
#include<QIcon>
ControlBar::ControlBar(QWidget *parent) : QWidget(parent)
{
    //设置无边框，任务栏无图标，顶置
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->setFixedWidth(160);
    this->setFixedHeight(40);

    QSize buttonSize = QSize(20, 20);
    turnLeftBtn = new QToolButton(this);
    turnLeftBtn->setFixedSize(buttonSize);
    turnLeftBtn->setIcon(QIcon(":/icon/resource/turn-left.png"));
    connect(turnLeftBtn,SIGNAL(clicked()),this,SIGNAL(toTurnLeft()));

    turnRightBtn = new QToolButton(this);
    turnRightBtn->setFixedSize(buttonSize);
    turnRightBtn->setIcon(QIcon(":/icon/resource/turn-right.png"));
    connect(turnRightBtn,SIGNAL(clicked()),this,SIGNAL(toTurnRight()));

    saveBtn = new QToolButton(this);
    saveBtn->setFixedSize(buttonSize);
    saveBtn->setIcon(QIcon(":/icon/resource/save.png"));
    connect(saveBtn,SIGNAL(clicked()),this,SIGNAL(toSave()));

    closeBtn= new QToolButton(this);
    closeBtn->setFixedSize(buttonSize);
    closeBtn->setIcon(QIcon(":/icon/resource/close.png"));
    connect(closeBtn,SIGNAL(clicked()),this,SIGNAL(toClose()));

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(turnLeftBtn);
    hLayout->addWidget(turnRightBtn);
    hLayout->addWidget(saveBtn);
    hLayout->addWidget(closeBtn);
    this->setLayout(hLayout);
}

