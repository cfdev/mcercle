#ifndef DIALOGCUSTOMER_H
#define DIALOGCUSTOMER_H

#include <QDialog>
#include "customer.h"

namespace Ui {
    class DialogCustomer;
}

class DialogCustomer : public QDialog {
    Q_OBJECT
public:
    DialogCustomer(customer *c,QWidget *parent = 0);
    ~DialogCustomer();

    void loadValuesFormCustomer();
    void setTitle(QString val);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogCustomer *ui;
    QStringList customerValues;
    customer *m_customer;

private slots:
    void checkConditions();
    void on_buttonBox_accepted();
    void on_radioButton_compagny_clicked();
    void on_radioButton_people_clicked();
};

#endif // DIALOGCUSTOMER_H
