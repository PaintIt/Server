#include <QStandardPaths>
#include <QDir>

#include "database.h"



DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

/* Методы для подключения к базе данных
 * */
void DataBase::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */

    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (!writeDir.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));

    const QString fileName = writeDir.absolutePath() + "/" + DATABASE_NAME;
    qDebug() << fileName;
    if(!QFile(fileName).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

/* Методы восстановления базы данных
 * */
bool DataBase::restoreDataBase()
{
    // Если база данных открылась ...
    if(this->openDataBase()){
        // Производим восстановление базы данных
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
}

/* Метод для открытия базы данных
 * */
bool DataBase::openDataBase()
{
    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (!writeDir.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));

    const QString fileName = writeDir.absolutePath() + "/" + DATABASE_NAME;


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase()
{
    db.close();
}

/* Метод для создания таблицы в базе данных
 * */
bool DataBase::createTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_NICKNAME     " VARCHAR(255)    NOT NULL,"
                            TABLE_PASSWORD     " TEXT            NOT NULL,"
                            TABLE_EMAIL        " VARCHAR(255)    NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

/* Метод для вставки записи в базу данных
 * */
bool DataBase::insertIntoTable(const QVariantList &data)
{

    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE " ( " TABLE_NICKNAME ", "
                                             TABLE_PASSWORD ", "
                                             TABLE_EMAIL " ) "
                  "VALUES (:nick, :password, :email)");
    query.bindValue(":nick",       data[0].toString());
    query.bindValue(":password",       data[1].toString());
    query.bindValue(":email",         data[2].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

/* Второй метод для вставки записи в базу данных
 * */
bool DataBase::insertIntoTable(const QString &nick, const QString &pass, const QString &email)
{
    QVariantList data;
    data.append(nick);
    data.append(pass);
    data.append(email);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}

/* Метод для удаления записи из таблицы
 * */
bool DataBase::removeRecord(const int id)
{
    // Удаление строки из базы данных будет производитсья с помощью SQL-запроса
    QSqlQuery query;

    // Удаление производим по id записи, который передается в качестве аргумента функции
    query.prepare("DELETE FROM " TABLE " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    // Выполняем удаление
    if(!query.exec()){
        qDebug() << "error delete row " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}
