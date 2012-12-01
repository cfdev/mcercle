#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QLocale>
#include <QTableWidgetItem>

#include "dbase.h"

namespace Ui {
    class board;
}



class board : public QWidget {
    Q_OBJECT
public:
    board(database *pdata, QLocale &lang, QWidget *parent = 0);
    ~board();

    void listStockAlertToTable();
    void listInvoiceAlertToTable();
    void listProposalAlertToTable();
    void listRevenuesToTable();
    void listYear();
    void calculYear();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::board *ui;

    database *m_data;
    QLocale m_lang;

private slots:

    void on_comboBox_yearsList_currentIndexChanged();
};

#endif // BOARD_H
