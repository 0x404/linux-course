#ifndef ADD_FRIEND_H
#define ADD_FRIEND_H

#include <QDialog>

namespace Ui {
class add_friend;
}

class add_friend : public QDialog
{
    Q_OBJECT

public:
    explicit add_friend(QWidget *parent = nullptr);
    ~add_friend();

private slots:
    void on_requestList_clicked();

private:
    Ui::add_friend *ui;
};

#endif // ADD_FRIEND_H
