#ifndef USERDATABASE_H
#define USERDATABASE_H

#include "QVector"
#include "QMap"
#include "QTableWidget"

class historyEntry{
public:
    int rubbishType,legality,time;
    historyEntry(int _rubbishType=-1,int _legality=-1,int _time=-1);
};

class history:public QVector<historyEntry>{

};

class user{
public:
    QString id;
    int point;
    history rubbishHistory;
    user(QString _id="",int _point=0);
    void printHistoryTo(QTableWidget* tW);
};

class userDataBase:public QMap<QString,user>{
    QString filePath,adminPassword;
public:
    userDataBase();
    void initializeFromFile();
    void writeIntoFile();
    bool isEmpty()const;
    bool adminPasswordIs(QString &password)const;
    void setAdminPassword(QString password);
    void addUser(QString id);
    void addHistoryEntry(QString id,historyEntry hE);
    void printUserListTo(QTableWidget* tW);
};

#endif // USERDATABASE_H
