#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>
#include <QToolButton>
#include <QWidget>

class ControlBar : public QWidget
{
    Q_OBJECT
public:
    explicit ControlBar(QWidget *parent = 0);

signals:
    void toSave();
    void toTurnLeft();
    void toTurnRight();
    void toClose();
public slots:
private:
    QToolButton *turnLeftBtn;
    QToolButton *turnRightBtn;
    QToolButton *saveBtn;
    QToolButton *closeBtn;
};

#endif // CONTROLBAR_H
