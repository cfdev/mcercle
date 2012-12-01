#ifndef DIALOGPRODUCT_H
#define DIALOGPRODUCT_H

#include <QDialog>
#include <QLocale>
#include "product.h"

namespace Ui {
    class DialogProduct;
}

class DialogProduct : public QDialog {
    Q_OBJECT
public:
    DialogProduct(QLocale &lang, product *p, bool tax, unsigned char type, QWidget *parent = 0);
    ~DialogProduct();
    void setTitle(QString val);
    void loadValuesFormProduct();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogProduct *ui;
    product *m_product;
    QLocale m_lang;
    unsigned char m_type;

    void loadProviderList();
    void loadCategoryList();
    void clearEdits();

private slots:
    void on_comboBox_categories_currentIndexChanged(QString name);
    void on_toolButton_editCategorie_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_add_edit_clicked();
    void on_comboBox_providers_currentIndexChanged(QString name);
    void on_toolButton_editProvider_clicked();
    void checkConditions();

signals:
    void askRefreshList();
};

enum{PRODUCT_ADD,PRODUCT_EDIT};
#endif // DIALOGPRODUCT_H
