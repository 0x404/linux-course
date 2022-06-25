#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void getData(); // 监听服务器发送数据，并完成对应事件解析
    void on_registerBtn_clicked();      // 当用户点击注册按钮
    void on_registerBtn_2_clicked();    // 当用户点击退出按钮

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
