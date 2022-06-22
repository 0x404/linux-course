#include "database.h"

//user表建立sql语句
const QString DataBase::CREATE_USER_SQL_STATEMENE = QString(
            "CREATE TABLE user (                                "
            "  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
            "  username TEXT(20) NOT NULL UNIQUE,               "
            "  password TEXT(12) NOT NULL DEFAULT 123456        "
            ");"
            );

//userinfo表建立语句
const QString DataBase::CREATE_USERINFO_SQL_STATEMENT = QString(
            "CREATE TABLE userinfo (                                    "
            "  id  INTEGER NOT NULL PRIMARY KEY ON CONFLICT ROLLBACK,   "
            "  username TEXT(20) NOT NULL,                              "
            "  motto TEXT(30) DEFAULT '',                               "
            "  otherinfo TEXT(100) DEFAULT '0'                           "
            ");                                                         "
            );

//relationship表建立sql语句
const QString DataBase::CREATE_RELATIONSHIP_SQL_STATEMENE=QString(
            "CREATE TABLE relationship(                         "
            "  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
            "  useid1 INTEGER NOT NULL,                         "
            "  useid2 INTEGER NOT NULL                          "
            ");                                                 "
            );

//request表建立sql语句
const QString DataBase::CREATE_REQUEST_SQL_STATEMENT=QString(
            "CREATE TABLE request (                             "
            "  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
            "  senduserid integer NOT NULL,                     "
            "  receiveuserid INTEGER NOT NULL,                  "
            "  status integer NOT NULL DEFAULT 0,               "
            "  sendtime TEXT(30) NOT NULL,                      "
            "  accepttime TEXT(30) DEFAULT '');                 "
            );

//chatHistory表建立语句
const QString DataBase::CREATE_CHATHISTORY_SQL_STATEMENT = QString(
            "CREATE TABLE chatHistory(                          "
            "  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
            "  sender INTEGER NOT NULL,                         "
            "  receiver INTEGER NOT NULL,                       "
            "  content TEXT(500) DEFAULT '',                    "
            "  time TEXT(20) NOT NULL                           "
            ");                                                 "
            );


const QString DataBase::CREATE_GROUPCHATHISTORY_SQL_STATEMENT = QString(
            "CREATE TABLE groupChatHistory(                         "
            " id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,        "
            " sender INTEGER NOT NULL,                              "
            " content TEXT(500) DEFAULT '',                          "
            " time TEXT(20) NOT NULL,                               "
            " senderInfo TEXT(20) NOT NULL                          "
            ");                                                     "
            );

//sql语句出现问题或者提前内部代码的问题会直接 “弹窗报错” ，返回false一般是表示不符合业务要求

// INSERT INTO userinfo3 (id,username,otherinfo) VALUES(102,'asd',DATETIME('now' ) ); 插入当前时间
DataBase* DataBase::dataBase = NULL;
DataBase::DataBase()
{

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

    /*
     *连接SQLITE数据库
     * */
    db.setHostName("127.0.0.1");                            //数据库服务器IP
    db.setDatabaseName("chat.db");                          //使用的数据库名字
    if(!db.open())
    {
        QMessageBox::warning(NULL,"失败","数据库开启失败",QMessageBox::No);
    }
    qDebug()<<"database: 数据库已经开启!";


    /*
     * 建立需要使用的表
     * */
    bool flagUser=createOrIsExistUserTable();                 //建立user表并且判断是否建立成功
    if(flagUser == false){
        QMessageBox::warning(nullptr,"error","user表未建立",QMessageBox::Yes);
    }

    bool flagUserInfo = createOrIsExistUserInfoTable();         //建立UserInfo表并且判断是否建立成功
    if(flagUserInfo == false){
        QMessageBox::warning(nullptr,"error","UserInfo表未建立",QMessageBox::Yes);
    }

    bool flagRelation = createOrIsExisRelationshipTable();      //建立relation表并且判断是否建立成功
    if(flagRelation == false){
        QMessageBox::warning(nullptr,"error","relationship表未建立,失败",QMessageBox::No);
    }

    bool flagRequest = createOrIsExistRequestTable();           //建立request表并且判断是否建立成功
    if(flagRequest == false){
        QMessageBox::warning(nullptr,"error","request表未建立，失败",QMessageBox::No);
    }

    bool flagChatHistory = createOrIsExistChatHistoryTable();   //建立chatHistory表并判断是否建立成功
    if (flagChatHistory == false)
    {
        QMessageBox::warning(nullptr, "error", "chatHistory表未建立", QMessageBox::Yes);
    }

    bool flagGroupChatHistory = createOrIsExistGroupChatHistory();
    if (flagGroupChatHistory == false)
    {
        QMessageBox::warning(nullptr, "error", "groupChatHistory表未建立", QMessageBox::Yes);
    }
}


//关闭数据库
DataBase::~DataBase()
{

    db.close();
}

//单例模式获取数据库对象
 DataBase *DataBase::getInstance()
{
    if(dataBase == NULL)
    {
        dataBase = new DataBase ;
    }

    return dataBase;
}

/***********************user表相关***************************/

//判断数据库user表是否建立完成
bool DataBase::createOrIsExistUserTable()
{
    QSqlQuery query;

    //判断数据表是否存在，不存在则创建
    if(!query.exec("select * from user"))
    {
        QString connectDB = CREATE_USER_SQL_STATEMENE;//建立USER表

        //创建数据表
        if(!query.exec(connectDB))//数据表创建失败
            {
            QSqlError lastError = query.lastError();
            QMessageBox::warning(NULL,"user数据表创建失败","Details:"+lastError.driverText(),QMessageBox::No);
            return false;
            }
    }
    qDebug()<<"database: user表创建成功!";
    return true;
}




bool DataBase::registerUser(QString username, QString psw)
{
    QSqlQuery query;
    query.prepare("insert into user (username,password) values(?,?)");
    query.addBindValue(username);
    query.addBindValue(psw);
    bool succeed = query.exec();

    if(!succeed)//
        {
        QSqlError lastError = query.lastError();
        qDebug() << "database:用户插入失败！,可能是因为用户已存在 details: " <<lastError.driverText();
        //QMessageBox::warning(NULL,"插入用户失败","Details:"+lastError.driverText(),QMessageBox::No);
        return false;
        }

    if(succeed){
        qDebug()<<"database:"<<username<<"注册完成！";

        insertIntoUserInfoByName(username);         //在user表注册完成之后，将user信息插入userinfo表之中
        return true;
    }

}

UserORM* DataBase::queryUserByName(QString username)
{
    QSqlQuery query ;
    UserORM* resUser = NULL;

    QString select_sql = "select id, username, password from user where username = :username";
    query.prepare(select_sql);
    query.bindValue(":username",username);

    if(!query.exec())
    {
        QMessageBox::warning(NULL,"error","执行语句失败！details: "+ query.lastError().text() ,QMessageBox::Yes);
    }
    else
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value("username").toString();
            QString psw = query.value("password").toString();
            resUser = new UserORM(id,name,psw);
            qDebug()<<"database: 已经在数据库内查询到用户"<<QString("id:%1    name:%2").arg(id).arg(name);
        }
    }

    return resUser; //如果没有查询到，返回空指针
}

UserORM *DataBase::queryUserById(int id)
{

    QSqlQuery query ;
    UserORM* resUser = NULL;

    QString select_sql = "select id, username, password from user where id = :id";
    query.prepare(select_sql);
    query.bindValue(":id",id);

    if(!query.exec())
    {
        QMessageBox::warning(NULL,"error","执行语句失败！"+ query.lastError().text() ,QMessageBox::No);
    }
    else
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value("username").toString();
            QString psw = query.value("password").toString();
            resUser = new UserORM(id,name,psw);
            qDebug()<<"database: 已经在数据库内查询到用户"<<QString("id:%1    name:%2").arg(id).arg(name); //成功输出
        }
    }

    return resUser; //如果没有查询到，返回空指针
}

bool DataBase::deleteUserByName(QString username)
{
    QSqlQuery query ;
    QString delete_sql = "delete from user where username = :username";
    query.prepare(delete_sql);
    query.bindValue(":username",username);

    if(!query.exec())
    {
        qDebug()<<"database: 执行失败! info:"<<query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"database: deleted!"<<username ;
        return true;
    }

}

bool DataBase::updateUserPasswordByName(QString username,QString psw)
{
    QSqlQuery query ;
    QString delete_sql = "UPDATE user set password = :password where username = :username ;";
    query.prepare(delete_sql);
    query.bindValue(":username",username);
    query.bindValue(":password",psw);

    if(!query.exec())
    {
        qDebug()<<"database:执行失败! info:"<<query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"database:update "<<username<<"\'s password";
        return true;
    }
}




/**---------userinfo表相关---------------------------------------------*/

// 建立userinfo表
bool DataBase::createOrIsExistUserInfoTable()
{
    QSqlQuery query;

    //判断数据表是否存在，不存在则创建
    if(!query.exec("select * from userinfo"))
    {
        QString connectDB = CREATE_USERINFO_SQL_STATEMENT;//建立UserInfo表

        //创建数据表
        if(!query.exec(connectDB))//数据表创建失败
            {
            QSqlError lastError = query.lastError();
            QMessageBox::warning(NULL,"userinfo数据表创建失败","Details: "+lastError.driverText(),QMessageBox::No);
            return false;
            }
    }
    qDebug()<<"database: userinfo表创建成功!";
    return true;

}

//该函数在仅仅在注册register时候调用
bool DataBase::insertIntoUserInfoByName(QString name)
{
    UserORM* user = queryUserByName(name);      //访问数据库获得Id
    int id = user->getQId();

    QSqlQuery query;
    query.prepare("insert into userinfo (id, username) values(?,?)");
    query.addBindValue(id);
    query.addBindValue(name);
    bool succeed = query.exec();

    if(!succeed)//
        {
        QSqlError lastError = query.lastError();
        qDebug() << "database: 数据写入userinfo失败！,可能是因为用户已在userinfo存在 details: " <<lastError.driverText();
        //QMessageBox::warning(NULL,"插入用户失败","Details:"+lastError.driverText(),QMessageBox::No);
        return false;
        }

    if(succeed){
        qDebug()<<"database: "<<name<<"已经插入到数据库userinfo之中！";
        return true;
    }
}


//设置个性签名
bool DataBase::setMottoByName(QString name, QString motto)
{
    QSqlQuery query ;
    UserORM* user = queryUserByName(name);
    if(user == NULL){
        qDebug()<<"database:更新个性签名失败! details:"<<"该用户不存在";
        return false;
    }

    QString delete_sql = "UPDATE userinfo set motto = :motto where username = :username ;";
    query.prepare(delete_sql);
    query.bindValue(":username",name);
    query.bindValue(":motto",motto);

    if(!query.exec())
    {
        QSqlError lastError = query.lastError();
        QMessageBox::warning(NULL,"更新个性签名失败!","Details: "+lastError.driverText(),QMessageBox::No);
        qDebug()<<"database: 更新个性签名失败! details:"<<query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"database: 更新 "<<name<<"\'s 个性签名成功";
        return true;
    }
}

//获取个性签名
QString DataBase::getMottoByName(QString name)
{
    QString motto;
    QSqlQuery query;
    QString find_sql="select motto from userinfo where username=:username";
    query.prepare(find_sql);
    query.bindValue(":username",name);

    if(!query.exec())
    {
        QSqlError lastError = query.lastError();
        QMessageBox::warning(NULL,"获取个性签名失败","Details: "+lastError.driverText(),QMessageBox::No);
        qDebug()<<"database: 查找个性签名失败!";
    }
    else {
        while(query.next())
        {
            motto = query.value("motto").toString();
        }
    }
    return motto;

}

//获取其他文件
QString DataBase::getOtherInfoByName(QString name)
{
    QString otherInfo;
    QSqlQuery query;
    QString find_sql="select otherinfo from userinfo where username=:username";
    query.prepare(find_sql);
    query.bindValue(":username",name);

    if(!query.exec())
    {
        qDebug()<<"database: 查找其他信息失败!";
    }
    else {
        while(query.next())
        {
            otherInfo = query.value("otherinfo").toString();
        }
    }
    return otherInfo;

}

bool DataBase::updateOhterInfoByName(QString name, QString info)
{
    QString update_sql = "UPDATE userinfo set otherinfo = :info where username=:username";
    QSqlQuery query;
    query.prepare(update_sql);
    query.bindValue(":info", info);
    query.bindValue(":username", name);
    if (!query.exec())
    {
        qDebug() << "database: 更新头像失败";
        return false;
    }
    return true;
}


UserInfoORM *DataBase::queryUserInfoByName(QString username)
{
    UserORM* user = queryUserByName(username);
    int id = user->getQId();
    QString motto = getMottoByName(username);
    QString otherInfo = getOtherInfoByName(username);

    std::vector<int> friendList =queryFriendByID(id);

    return new UserInfoORM(id, username, motto, friendList, otherInfo);
}


/***************************relationship表相关*******************************/

//判断数据库relationship表是否创建成功
bool DataBase::createOrIsExisRelationshipTable()
{
    QSqlQuery query;

    if(!query.exec("select * from relationship"))
    {
        QString connect_relation=CREATE_RELATIONSHIP_SQL_STATEMENE;
        if(!query.exec(connect_relation))
            {
            QSqlError lastError = query.lastError();
            QMessageBox::warning(NULL,"database: 数据表relationship创建失败","Details:"+lastError.driverText(),QMessageBox::No);
            return false;
            }
    }
    qDebug()<<"database: relationship创建成功!";
    return true;

}

bool DataBase::addFriendById(int useid1, int useid2)
{
    QSqlQuery query;
    if(useid1 == useid2 || isExistInRelationShipTable(useid1, useid2))
    {
        qDebug()<<"database: 两人已是好友或者传入的两个用户相同!" ;
        return false;
    }

    UserORM *resuser1=queryUserById(useid1);
    UserORM *resuser2=queryUserById(useid2);

    if(resuser1 != NULL && resuser2 != NULL)
    {
        QString addf_sql="insert into relationship(useid1,useid2) values (?,?)";
        query.prepare(addf_sql);
        QVariantList list1,list2;
        list1<<useid1<<useid2;
        list2<<useid2<<useid1;
        query.addBindValue(list1);
        query.addBindValue(list2);

        bool succeed =  query.execBatch();
        if(!succeed)
        {
            QSqlError lastError = query.lastError();
            qDebug()<<"database: 添加失败"<<lastError.text() ;
            return false;
        }
        else {
            qDebug()<<"database: 好友关系建立成功";
            return true;
        }
    }
    else {
        qDebug()<<"dataBase: 建立好友关系时，用户中存在无效id";
        return false;
    }

}

std::vector<int> DataBase::queryFriendByID(int userId)
{
    QSqlQuery query;
    std::vector<int> id2s;
    QString find_sql="select useid2 from relationship where useid1=:id1";
    query.prepare(find_sql);
    query.bindValue(":id1",userId);
    if(!query.exec())
    {
        qDebug()<<"查找好友列表失败";
    }
    else {
        while(query.next())
        {
            id2s.push_back(query.value("useid2").toInt());
        }
    }
    return id2s;
}

bool DataBase::deleteRelationship(int useid1, int useid2)
{
    if(!isExistInRelationShipTable(useid1, useid2)){
        qDebug()<< "database: 两者不是好友，拒绝删除！";
        return false;
    }

    UserORM* user1 = queryUserById(useid1);
    UserORM* user2 = queryUserById(useid2);
    QString user1Name = user1->getUserName();
    QString user2Name = user2->getUserName();

    QSqlQuery query;
    QString delete_sql="delete from relationship where useid1= :id1 and useid2= :id2";
    query.prepare(delete_sql);
    query.bindValue(":id1",useid1);
    query.bindValue(":id2",useid2);
    bool succed1=query.exec();

    query.prepare(delete_sql);
    query.bindValue(":id1",useid2);
    query.bindValue(":id2",useid1);
    bool succed2=query.exec();

    if(succed1&&succed2)
    {
        qDebug()<<"dataBase: 删除好友关系"<<user1Name<<" 与 "<< user2Name ;
        return true;
    }
    else {
        QSqlError lastError = query.lastError();
        qDebug()<<"database: 添加失败关系失败， details："<<lastError.text() ;
        return false;
    }
}


bool DataBase::isExistInRelationShipTable(int id1, int id2)
{
    QSqlQuery query;
    QString find_sql="select id from relationship where useid1=:id1 and useid2=:id2 ";
    query.prepare(find_sql);
    query.bindValue(":id1",id1);
    query.bindValue(":id2",id2);

    bool flag = false;
    if(!query.exec())
    {
        qDebug()<<"查找好友列表失败";
    }
    else {
        while(query.next())
        {
            flag = true;
            query.value("id").toInt();
        }
    }
    return flag;
}


/*-------------------request表相关操作-------------------------------------*/

bool DataBase::createOrIsExistRequestTable()
{
    QSqlQuery query;

    if(!query.exec("select * from request"))
    {
        QString connect_relation=CREATE_REQUEST_SQL_STATEMENT ;
        if(!query.exec(connect_relation))
            {
            QSqlError lastError = query.lastError();
            QMessageBox::warning(NULL,"database: 数据表request创建失败","Details:"+lastError.driverText(),QMessageBox::No);
            return false;
            }
    }
    qDebug()<<"database: request成功!";
    return true;
}

bool DataBase::sendRequestId1AddId2(int id1, int id2)
{
    QSqlQuery query;
    if(id1 == id2 || isExistInRelationShipTable(id1, id2))
    {
        qDebug()<<"database: 两人已是好友或者传入的两个用户相同!无需添加" ;
        return false;
    }

    QString addf_sql="insert into request(senduserid, receiveuserid, status, sendtime) values (:id1, :id2, 0,DATETIME('now' , '8 hours') );";  //状态为0表示还申请方发出申请，接收方还没有接受
    query.prepare(addf_sql);
    query.bindValue(":id1",id1);
    query.bindValue(":id2",id2);
    bool succeed =  query.exec();
    if(!succeed)
    {
        QSqlError lastError = query.lastError();
        qDebug()<<"database: 好友申请发送失败"<<lastError.text() ;
        QMessageBox::warning(NULL,"database: 好友申请发送失败","Details:"+lastError.driverText(),QMessageBox::No);
        return false;
    }
    else {
        qDebug()<<"database: 申请消息已发送";
        return true;
    }

}

bool DataBase::admitBecomeFriend(int id1, int id2)
{
    QSqlQuery query ;       //此处也可以执行删除操作，同意添加好友后可以删除好友申请的记录了
    QString update_sql = "UPDATE request set status = 1, accepttime = DATETIME('now' , '8 hours') where senduserid = :id1 and receiveuserid = :id2 ;";  //设置状态为1表示接受了申请
    query.prepare(update_sql);
    query.bindValue(":id1",id1);
    query.bindValue(":id2",id2);

    if(!query.exec())
    {
        qDebug()<<"database: 执行失败! info:"<<query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"database: id2接受了id1的申请";
        addFriendById(id1,id2);                         //接受申请后自动写入数据库
        return true;
    }
}

bool DataBase::removeFriendRequest(int useid1, int useid2)
{
    QSqlQuery query;
    QString delete_sql = "DELETE FROM request WHERE senduserid = :id1 and receiveuserid = :id2 and status = 0";
    query.prepare(delete_sql);
    query.bindValue(":id1", useid1);
    query.bindValue(":id2", useid2);

    if (!query.exec())
    {
        qDebug() << "database: 删除添加记录失败";
        return false;
    }
    else
    {
        qDebug() << "database: 删除添加记录成功";
        return true;
    }
}

//该函数返回request对象数组，表示 id对应的用户 接受的好友申请 以及 用户自身向别人申请好友 的情况
std::vector<RequestORM*> DataBase::getALLRequestUserId_ById(int id)
{
    QSqlQuery query;
    std::vector<RequestORM* > res;
    UserInfoORM* userSend;
    UserInfoORM* userReceive;
    int status;
    QString sendtime;
    QString accepttime;

    int sendUserId;
    int acceptUserId;

    QString find_sql="SELECT senduserid, receiveuserid, status, sendtime, accepttime FROM request where receiveuserid = :id ORDER BY id DESC ; ";
    query.prepare(find_sql);
    query.bindValue(":id",id);
    if(!query.exec())
    {
        qDebug()<<"database: 获取好友申请信息失败";
    }
    else {
        while(query.next())
        {
            sendUserId = query.value("senduserid").toInt();
            acceptUserId = query.value("receiveuserid").toInt();

            status = query.value("status").toInt();
            UserORM* userS = queryUserById(sendUserId);
            UserORM* userR = queryUserById(acceptUserId);
            userSend  = queryUserInfoByName(userS->getUserName());
            userReceive = queryUserInfoByName(userR->getUserName());

            sendtime = query.value("sendtime").toString();
            accepttime = query.value("status").toString();
            res.push_back(new RequestORM(userSend,userReceive,status,sendtime,accepttime));
        }

    }
    qDebug()<<"database: 获取好友申请信息成功";
    return res;
}

bool DataBase::createOrIsExistGroupChatHistory()
{
    QSqlQuery query;
    if (!query.exec("select * from groupChatHistory"))
    {
        QString connectDB = CREATE_GROUPCHATHISTORY_SQL_STATEMENT;
        if (!query.exec(connectDB))
        {
            qDebug() << "database: 创建groupChatHistory失败";
            return false;
        }
        return true;
    }
    qDebug() << "database: 创建groupChatHistory成功！";
    return true;
}



/*---------------------------chatHistory表相关---------------------------------------------------------------------------*/

bool DataBase::createOrIsExistChatHistoryTable()
{
    QSqlQuery query;

    if (!query.exec("select * from chatHistory"))
    {
        QString connectDB = CREATE_CHATHISTORY_SQL_STATEMENT;
        if (!query.exec(connectDB))
        {
            QSqlError lastError = query.lastError();
            QMessageBox::warning(nullptr, "chatHistory数据表创建失败", "Details:" + lastError.driverText(), QMessageBox::No);
            return false;
        }
    }
    qDebug() << "database: chatHistory表创建成功";
    return true;
}


bool DataBase::insertIntoGroupChatHistory(int senderId, QString content, QString info)
{
    QSqlQuery query;
    query.prepare("insert into groupChatHistory (sender, content, time, senderInfo) values(?, ?, DATETIME('now' , '8 hours'), ?)");
    query.addBindValue(senderId);
    query.addBindValue(content);
    query.addBindValue(info);
    bool succeed = query.exec();
    if (!succeed)
    {
        QSqlError lastError = query.lastError();
        qDebug() << "dataBase: 群聊聊天记录插入失败！ details: " << lastError.driverText();
        return  false;
    }
//    qDebug() << "dataBase: (" << senderId << " " << receiverId << " " << content << " )"<< " 聊天记录插入成功";
    return true;
}


bool DataBase::insertIntoChatHistory(int senderId, int receiverId, QString content)
{
    QSqlQuery query;
    query.prepare("insert into chatHistory (sender, receiver, content, time) values(?, ?, ?, DATETIME('now' , '8 hours'))");
    query.addBindValue(senderId);
    query.addBindValue(receiverId);
    query.addBindValue(content);
    bool succeed = query.exec();
    if (!succeed)
    {
        QSqlError lastError = query.lastError();
        qDebug() << "dataBase: 聊天记录插入失败！ details: " << lastError.driverText();
        return  false;
    }
    qDebug() << "dataBase: (" << senderId << " " << receiverId << " " << content << " )"<< " 聊天记录插入成功";
    return true;
}

std::vector<ChatORM*> DataBase::getAllGroupChatHistory()
{
    std::vector<ChatORM*> res;
    QString select_sql = "SELECT sender, content, time, senderInfo from groupChatHistory ORDER BY id DESC LIMIT 50";
    QSqlQuery query;
    query.prepare(select_sql);
    if (!query.exec())
    {
        qDebug() << "群聊记录查询失败";
    }
    else {
        while(query.next())
        {
            int senderId = query.value("sender").toInt();
            QString content = query.value("content").toString();
            QString time =  query.value("time").toString();
            QString info = query.value("senderInfo").toString();

            res.push_back(new ChatORM(senderId, content, time, info));
        }
    }
    return res;
}

std::vector<ChatORM *> DataBase::getALLChatHistory(int id1, int id2)
{
    std::vector<ChatORM *>  res;
    QString content;
    QString time;

    QSqlQuery query;
    QString find_sql="SELECT sender, receiver, content, time FROM chatHistory where (sender = :id1 and receiver = :id2) or (sender = :id2 and receiver = :id1) ORDER BY id DESC; ";
    query.prepare(find_sql);
    query.bindValue(":id1",id1);
    query.bindValue(":id2",id2);
    if(!query.exec())
    {
        qDebug()<<"database: 获取聊天记录信息失败";
    }
    else {
        while(query.next())
        {
            int senderId = query.value("sender").toInt();
            int receiverId = query.value("receiver").toInt();
            content = query.value("content").toString();
            time = query.value("time").toString();
            res.push_back(new ChatORM(senderId, receiverId, content,time));
        }

    }
    qDebug()<<"database: 获取聊天记录信息成功";
    return res;
}

