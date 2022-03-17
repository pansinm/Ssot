#include <QTransform>
#include <QFileDialog>
#include <QString>
#include <QCursor>
#include<QScreen>
#include "ctoplabel.h"

CTopLabel::CTopLabel(QWidget *parent) :
    QLabel(parent)
{
    controlBar = new ControlBar(this);

    //设置后才能响应不按按键的mouseMoveEventX
    this->setMouseTracking(true);

    //鼠标初始形状设置为十字
    this->setCursor(QCursor(Qt::CrossCursor));

    setLabelId(this->winId());

    setAutoFillBackground(true);

    //当前状态设置为初始化
    currentState=initShot;

    //设置为正在截图
    isShot=true;

    //设置无边框，任务栏无图标，顶置
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);

    QPoint globalCursorPos = QCursor::pos();
    QScreen* screen = QGuiApplication::screenAt(globalCursorPos);

    //抓取屏幕
    fullScreenPixmap = screen->grabWindow(0);

    //初始化
    beginPoint=QPoint(0,0);
    endPoint=QPoint(0,0);
    shotRect=QRect(0,0,0,0);

    //左键按下状态
    isPressed=false;

    //初始化窗口拖动点
    dragPosition=QPoint(-1,-1);

    //连接旋转信号
    connect(controlBar,SIGNAL(toTurnLeft()),this,SLOT(turnLeft()));

    connect(controlBar,SIGNAL(toTurnRight()),this,SLOT(turnRight()));

    //保存
    connect(controlBar,SIGNAL(toSave()),this,SLOT(savePic()));

    //连接关闭
    connect(controlBar,SIGNAL(toClose()),this,SLOT(deleteLater()));

}
CTopLabel::~CTopLabel()
{
    controlBar->deleteLater();
}

void CTopLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {

        isPressed=true;
        if(isShot)
        {
            //正在截图时，改变状态及beginPoint
            beginPoint=ev->pos();
            qDebug()<<beginPoint.x();

        }
        else
        {
            //截图完毕时
            dragPosition=ev->globalPos()-this->pos();
            controlBar->setFocus();
        }
    }
}
void CTopLabel::mouseMoveEvent(QMouseEvent *ev)
{

    update();

    if(isPressed)
    {
        if(isShot)
        {
            currentState=beginShot;
            //截屏拖动时
            //qDebug()<<"isShot";
            endPoint=ev->pos();


            //根据拖动位置，计算截图区域

            //左上->右下
            if(beginPoint.x()<endPoint.x()&&beginPoint.y()<endPoint.y())
            {
                shotRect=QRect(beginPoint,endPoint);
            }

            //左下->右上
            else if(beginPoint.x()<endPoint.x()&&beginPoint.y()>endPoint.y())
            {
                shotRect=QRect(beginPoint.x(),endPoint.y(),endPoint.x()-beginPoint.x(),beginPoint.y()-endPoint.y());
            }

            //右上->左下
            else if(beginPoint.x()>endPoint.x()&&beginPoint.y()<endPoint.y())
            {
                shotRect=QRect(endPoint.x(),beginPoint.y(),beginPoint.x()-endPoint.x(),endPoint.y()-beginPoint.y());
            }

            //右下->左上
            else
            {
                shotRect=QRect(endPoint,beginPoint);
            }

            update();

        }
        else
        {
            //窗口移动
            move(ev->globalPos()-dragPosition);

            setContralBarPos();
        }

    }
}

void CTopLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        endPoint=ev->pos();

        if(isShot)
        {
            if(currentState==beginShot)
            {

                isShot = false;

                //改变状态
                currentState = finishShot;

                //截图区域图像
                resultPixmap = fullScreenPixmap.copy(shotRect.x() * this->devicePixelRatio(),
                                                     shotRect.y() * this->devicePixelRatio(),
                                                     shotRect.width() * this->devicePixelRatio(),
                                                     shotRect.height() * this->devicePixelRatio());

                //结果复制到剪切板
                QClipboard *clipboard=QApplication::clipboard();

                clipboard->clear(QClipboard::Clipboard);
                clipboard->setPixmap(resultPixmap, QClipboard::Clipboard);

                fullScreenPixmap.loadFromData(NULL);

                this->repaint();

                QPoint point = this->screen()->geometry().topLeft() + shotRect.topLeft();
                setGeometry(point.x(), point.y(), shotRect.width(), shotRect.height());

                setContralBarPos();

                controlBar->show();

                emit shotted();

                this->setCursor(QCursor(Qt::SizeAllCursor));

            }

        }
        else
        {
            move(ev->globalPos()-dragPosition);

        }
    }

    isPressed=false;
}



void CTopLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        //关闭当前
        this->deleteLater();
    }
}

void CTopLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setFont(QFont("arial",12));
    QPoint pos=QCursor::pos();

    pointColor = fullScreenPixmap
            .toImage()
            .pixelColor(
                (pos - this->geometry().topLeft()) * this->devicePixelRatio()
             );
    switch(currentState)
    {
    case initShot:
    {
        painter.drawPixmap(0,0,fullScreenPixmap);
        painter.setPen(QPen(Qt::blue,1,Qt::SolidLine,Qt::FlatCap));//设置画笔
        QString s=QString("(x:%1,y:%2,%3)")
                .arg(pos.x())
                .arg(pos.y())
                .arg(pointColor.name());
        painter.setPen(QPen(Qt::red,1,Qt::SolidLine,Qt::FlatCap));//设置画笔
        painter.drawText(pos - this->geometry().topLeft(),s);
        break;
    }
    case beginShot:
    {
        painter.drawPixmap(0,0,fullScreenPixmap);
        painter.setPen(QPen(Qt::blue,1,Qt::SolidLine,Qt::FlatCap));//设置画笔
        painter.drawRect(shotRect);
        QString s=QString("(W:%1,H:%2,%3)")
                .arg(shotRect.width())
                .arg(shotRect.height())
                .arg(pointColor.name());
        painter.setPen(QPen(Qt::red,1,Qt::SolidLine,Qt::FlatCap));//设置画笔
        painter.drawText(pos - this->geometry().topLeft(),s);
        break;
    }

    case finishShot:
        //绘制结果窗口
        painter.drawPixmap(0, 0, this->width(), this->height(), resultPixmap);
        painter.setPen(QPen(Qt::blue,1,Qt::SolidLine,Qt::FlatCap));//设置画笔
        painter.drawRect(0, 0, this->width() - 1, this->height() - 1);
        break;
    }

}

//键盘事件
void CTopLabel::keyPressEvent(QKeyEvent *ev){
    if(ev->key()==Qt::Key_Escape){
        emit shotted();
        this->deleteLater();
    }

    if (ev->key() == Qt::Key_C) {
        QClipboard *clipboard=QApplication::clipboard();
        clipboard->setText(pointColor.name());
    }
}

void CTopLabel::leaveEvent(QEvent *event) {
    if (this->currentState == initShot) {
        qDebug() << "receive leave event" << event;
        QTimer::singleShot(100, [this]{
            QPoint globalCursorPos = QCursor::pos();
            QScreen* screen = QGuiApplication::screenAt(globalCursorPos);
            this->fullScreenPixmap = screen->grabWindow(0);
            this->setGeometry(screen->geometry().x(),
                              screen->geometry().y(),
                              screen->geometry().width() / screen->devicePixelRatio(),
                              screen->geometry().height() / screen->devicePixelRatio());
        });
    }
}

//设置控制条位置
void CTopLabel::setContralBarPos(){

    int controlBarX=this->geometry().right()-controlBar->width();

    int controlBarY=this->geometry().bottom()+5;

    controlBar->setGeometry(controlBarX,controlBarY,controlBar->width(),controlBar->height());
}

//逆时针旋转
void CTopLabel::turnLeft(){

    QTransform leftTransform;

    leftTransform.rotate(-90);

    resultPixmap = resultPixmap.transformed(leftTransform);

    int x = this->geometry().right() - resultPixmap.width()+1;

    int y = this->geometry().bottom() - resultPixmap.height()+1;

    this->setGeometry(x,y,resultPixmap.width(),resultPixmap.height());

    this->setContralBarPos();

    this->repaint();
}

//顺时针旋转
void CTopLabel::turnRight(){

    qDebug()<<"turnRight";

    QTransform rightTransform;

    rightTransform.rotate(90);

    resultPixmap = resultPixmap.transformed(rightTransform);

    int x = this->geometry().right() - resultPixmap.width()+1;

    int y = this->geometry().bottom() - resultPixmap.height()+1;

    this->setGeometry(x,y,resultPixmap.width(),resultPixmap.height());

    this->setContralBarPos();

    this->repaint();
}

void CTopLabel::savePic(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"),
                                                    "%DESKTOP%/untitled.png",
                                                    tr("Images (*.png *.xpm *.jpg)"));

    resultPixmap.save(fileName);

}


