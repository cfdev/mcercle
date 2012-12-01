#ifndef DIALOGPROVIDERSEDIT_H
#define DIALOGPROVIDERSEDIT_H

#include <QDialog>
#include "product.h"

namespace Ui {
    class DialogProvidersEdit;
}

class DialogProvidersEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProvidersEdit(product *prod, unsigned char type,   QWidget *parent = 0);
    ~DialogProvidersEdit();

private:
    Ui::DialogProvidersEdit *ui;
    product *m_product;
    unsigned char m_type;

    void loadValuesFormProvider();
    void setTitle(QString val);

private slots:
    void checkConditions();
    void on_buttonBox_accepted();
};

enum{PROVIDER_ADD, PROVIDER_EDIT};

#endif // DIALOGPROVIDERSEDIT_H
