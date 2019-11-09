#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_NAME       "server.sqlite3"

#define TABLE                      "Users"              // Название таблицы
#define TABLE_NICKNAME             "NickName"           // Вторая колонка
#define TABLE_PASSWORD             "Password"           // Третья колонка
#define TABLE_EMAIL                "Email"              // Четвертая колонка

// Первая колонка содержит Autoincrement ID


class DataBase : public QObject
{
    Q_OBJECT


public:

    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void connectToDataBase();


private:

    QSqlDatabase db;

private:

    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание базы таблицы в базе данных


public slots:
    bool insertIntoTable(const QVariantList &data);      // Добавление записей в таблицу
    bool insertIntoTable(const QString &nick, const QString &pass, const QString &email);
    bool removeRecord(const int id); // Удаление записи из таблицы по её id
};

#endif // DATABASE_H
