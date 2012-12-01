#ifndef DIALOGINTERVENTIONS_H
#define DIALOGINTERVENTIONS_H

#include <QDialog>

#include "interventions.h"

namespace Ui {
    class DialogInterventions;
}

class DialogInterventions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInterventions(intervention* inter, bool tax, QWidget *parent = 0);
    ~DialogInterventions();

    void setTitle(QString val);
    void loadValuesFromInter();

private:
    Ui::DialogInterventions *ui;
    intervention* m_inter;
    int m_idInter, m_idCustomer;

    void listInterCommToTable();

protected :
    void closeEvent(QCloseEvent* event);

private slots:
    void on_pushButton_edit_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_add_clicked();
    void on_tableWidget_Inter_itemSelectionChanged();
    void on_pushButton_more_clicked();
    void on_buttonBox_accepted();
    void checkConditions();
    void setIdCustomer();
};

#endif // DIALOGINTERVENTIONS_H
