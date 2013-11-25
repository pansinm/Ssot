#ifndef CTOPLABEL_H
#define CTOPLABEL_H

#include <QtWidgets>

class CTopLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CTopLabel(QWidget *parent = 0);

    //截图状态
    enum shotState{initShot,beginShot,finishShot};

    void setLabelId(WId id){labelId=id;}

    ~CTopLabel();

signals:
    //发送关闭信号
    void meClosed(WId);

    //完成截屏信号
    void shotted();



private:
    int labelId;

    //全屏Pixmap
    QPixmap fullScreenPixmap;

    //截图pixmap
    QPixmap resultPixmap;

    //截图状态
    shotState currentState;

    //鼠标起始点
    QPoint beginPoint;

    //终点
    QPoint endPoint;

    //鼠标拖动矩形
    QRect shotRect;

    //是否正在截图
    bool isShot;

    //左键是否按下
    bool isPressed;

    //拖动点
    QPoint dragPosition;


    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *);


};

#endif // CTOPLABEL_H
