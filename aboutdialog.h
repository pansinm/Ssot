#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include<QLabel>
#include<QTextEdit>
#include<QString>
#include<QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);

    //如果为true，则text为文件名，文本框中加载文件内容（文本文件）
    void setText(const QString str,bool isFileName=false);

    //370x45图片
    void setLogo(const QString filename);
    void setInfo(const QString info);
    void setBackground(const QString styleSheet="default");
private:
    QLabel* label_Logo;
    QLabel* label_Info;
    QTextEdit* textEdit;
    QPushButton* btn_confirm;

    void init();
signals:

public slots:
    void openUrl(QString);

};

#endif // ABOUTDIALOG_H
