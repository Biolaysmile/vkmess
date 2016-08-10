#include "widget.h"
#include "ui_widget.h"

using namespace QtJson; //Для работы с парсером.


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->webView->load(QUrl("https://oauth.vk.com/authorize?client_id=5581233&display=wap&redirect_uri=https://oauth.vk.com/blank.html&scope=offline,messages,friends&response_type=token&v=5.53"));


    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(url_changed(QUrl)));
    connect(this, SIGNAL(auth_success()), this, SLOT(get_friends()));
    connect(this, SIGNAL(friends_loaded(bool)), ui->TEXT, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(friends_loaded(bool)), ui->SEND, SLOT(setEnabled(bool)));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::url_changed(QUrl url)
{
    if(!url.toString().contains("access_token"))
        return;
    url = url.toString().replace("#", "?");

    token = QUrlQuery(url).queryItemValue("access_token");
    emit auth_success();
}

void Widget::get_friends()
{
    QUrl current("https://api.vk.com/method/friends.get");
    QUrlQuery Param;

    Param.addQueryItem("user_id","33061210");
    Param.addQueryItem("v", "5.53");
    Param.addQueryItem("fields", "first_name, last_name");
    Param.addQueryItem("access_token", token);
    current.setQuery(Param);

    QByteArray answer = GET(current);

    //Проверяем получили пустой ответ или нет
    if(answer.isEmpty())
    {
        qDebug() << "Answer is Empty! Get Friends = NULL";
        exit(-1);
    }

    QVariantList List = parse(answer).toMap().value("response").toMap().value("items").toList();    //Получаем список объектов с интересующими нас полями.

    for(int i = 0; i < List.size(); i++)
    {
        QVariantMap current = List[i].toMap();
        QString tmp = current.value("first_name").toString() + " " + current.value("items").toString() +
                current.value("last_name").toString() + " " + current.value("items").toString();
        users[tmp] = current.value("id").toString();
    }

    for(User::iterator itr = users.begin(); itr!=users.end(); itr++)
        ui->LIST->addItem(itr.key());


    emit friends_loaded(true);
}

QByteArray Widget::GET(QUrl r)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest(r));
    QEventLoop wait; //Ожидание ответа от сервера
    connect(manager, SIGNAL(finished(QNetworkReply*)), &wait, SLOT(quit()));

    QTimer::singleShot(10000, &wait, SLOT(quit())); //Если ожидание ответа > 10 сек, заканчиваем сессию
    connect(manager, SIGNAL(finished(QNetworkReply*)), manager, SLOT(deleteLater()));
    wait.exec();

    QByteArray answer = reply->readAll();
    reply->deleteLater();
    return answer;

}


void Widget::on_SEND_clicked()
{
    QString text = ui->TEXT->text();
    QString id = users.value(ui->LIST->currentText());


    QUrl current("https://api.vk.com/method/messages.send");
    QUrlQuery Parametr;

    Parametr.addQueryItem("user_id", id);
    Parametr.addQueryItem("message", text);
    Parametr.addQueryItem("access_token", token);
    Parametr.addQueryItem("v","5.53");
    current.setQuery(Parametr);

    QByteArray answer = GET(current);

    if(answer.contains("response"))
    {
        QMessageBox::information(this, tr("Ура"), tr("Успешно отправили"), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning(this, tr("Error!"), tr("Не отправлено сообщение"), QMessageBox::Ok);
    }
}
