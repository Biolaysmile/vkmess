#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrlQuery>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include "json.h"
#include <QMap>
#include <QMessageBox>

typedef QMap<QString, QString> User;  //Тип User - <Имя/Фамилия, id>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void auth_success();
    void friends_loaded(bool);


public slots:
    void url_changed(QUrl); //Для смены страницы
    void get_friends(); //Получаем список друзей


private slots:
    void on_SEND_clicked();

private:
    Ui::Widget *ui;
    QString token;
    User users;
    QByteArray GET(QUrl);
};



#endif // WIDGET_H
