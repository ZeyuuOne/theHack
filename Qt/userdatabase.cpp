#include "userdatabase.h"
#include "QFile"
#include "QTextStream"
#include "QDateTime"
#include "QDebug"

historyEntry::historyEntry(int _rubbishType,int _legality,int _time){
    rubbishType=_rubbishType;
    legality=_legality;
    time=_time;
}

userDataBase::userDataBase(){
    filePath="user.data";
    initializeFromFile();
}

user::user(QString _id,int _point){
    id=_id;
    point=_point;
}

void user::printHistoryTo(QTableWidget *tW){
    int row=0;
    if (rubbishHistory.empty()) return;
    QString mapRubbishType[4]={QStringLiteral("干垃圾"),QStringLiteral("湿垃圾"),QStringLiteral("可回收物"),QStringLiteral("有害垃圾")};
    QString mapLegality[2]={QStringLiteral("失败"),QStringLiteral("成功")};
    QString mapPoint[2]={QStringLiteral("-5"),QStringLiteral("+1")};
    for (auto i=rubbishHistory.rbegin();i!=rubbishHistory.rend();i++,row++){
        tW->setItem(row,0,new QTableWidgetItem(mapRubbishType[i->rubbishType]));
        tW->setItem(row,1,new QTableWidgetItem(mapLegality[i->legality]));
        tW->setItem(row,2,new QTableWidgetItem(mapPoint[i->legality]));
        QDateTime time = QDateTime::fromTime_t((uint)i->time);
        tW->setItem(row,3,new QTableWidgetItem(time.toString("yyyy/M/d h:m")));
    }
}

void userDataBase::initializeFromFile(){
    QFile file(filePath);
    if (!file.exists()) {
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.close();
            return;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file.atEnd()) {
        file.close();
        return;
    }
    QTextStream in(&file);
    adminPassword = in.readLine();
    int userNum=in.readLine().toInt();
    for (int i=0;i<userNum;i++){
        user t;
        t.id=in.readLine();
        t.point=in.readLine().toInt();
        int historyNum=in.readLine().toInt();
        for (int j=0;j<historyNum;j++){
            historyEntry hE;
            hE.rubbishType=in.readLine().toInt();
            hE.legality=in.readLine().toInt();
            hE.time=in.readLine().toInt();
            t.rubbishHistory.push_back(hE);
        }
        insert(t.id,t);
    }
    file.close();
}

void userDataBase::writeIntoFile(){
    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out<<adminPassword<<'\n';
    out<<size()<<'\n';
    for (auto i=begin();i!=end();i++){
        out<<i->id<<'\n';
        out<<i->point<<'\n';
        out<<i->rubbishHistory.size()<<'\n';
        for (auto j=i->rubbishHistory.begin();j!=i->rubbishHistory.end();j++){
            out<<j->rubbishType<<'\n';
            out<<j->legality<<'\n';
            out<<j->time<<'\n';
        }
    }
    file.close();
}

bool userDataBase::isEmpty() const{
    if (adminPassword=="") return 1;
    return 0;
}

bool userDataBase::adminPasswordIs(QString &password) const{
    if (adminPassword==password) return 1;
    return 0;
}

void userDataBase::setAdminPassword(QString password){
    adminPassword=password;
    writeIntoFile();
}

void userDataBase::addUser(QString id){
    insert(id,user(id));
    writeIntoFile();
}

void userDataBase::addHistoryEntry(QString id, historyEntry hE){
    this->find(id).value().rubbishHistory.push_back(hE);
    int mapPoint[2]={-5,1};
    this->find(id).value().point+=mapPoint[hE.legality];
    writeIntoFile();
}

void userDataBase::printUserListTo(QTableWidget *tW){
    int row=0;
    if (empty()) return;
    for (auto i=begin();i!=end();i++,row++){
        tW->setItem(row,0,new QTableWidgetItem(i.value().id));
        tW->setItem(row,1,new QTableWidgetItem(QString::number(i.value().point)));
    }
}
