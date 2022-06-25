#ifndef TALKMESSAGEMANY_H
#define TALKMESSAGEMANY_H
#include <QDialog>

class QPaintEvent;
class QPainter;
class Qlabel;


class TalkMessagMany : public QDialog
{
    Q_OBJECT

public:
    explicit TalkMessagMany(QWidget *parent = nullptr,QString iconid_other="girl",QString iconid_self="boy",QString id_self="11",QString id_other="12");
    ~TalkMessagMany();
    enum msg_type{//消息种类
        user_self,
        user_other,
        user_system,
        user_time
    };

    void sendTextSuccess();//发送消息成功时调用
    void sendText(QString text, QString time, QSize allSize, msg_type msgtype);
//发送消息，包含发送时间、内容、字体大小、消息种类

    QSize getMessageSize(QString src);//根据消息内容确定框的大小
    QSize fontRect(QString str);

    QString gettext() {return m_msg;}//获得消息内容
    QString gettime() {return m_sendtime;}//获得消息发送时间
    msg_type GetMsgType() {return m_type;}//获得消息种类
    void paintEvent(QPaintEvent *event);//使用QPainter绘出消息
private:
    msg_type m_type = user_system;

    bool m_isSending = false;//消息成功发送

    QString m_msg;//消息内容
    QString m_sendtime;//消息发送时间
    QString m_nowTime;//当前时间
    QString m_id_self;//自己的id
    QString m_id_other;//其他人的id

    QSize m_size;

    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    QRect m_textLeftRect;
    QRect m_textRightRect;
    QRect m_idLeftRect;
    QRect m_idRightRect;
    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;
    Qlabel* m_loading = Q_NULLPTR;
    QMovie* m_loadingMovie = Q_NULLPTR;

};

#endif
