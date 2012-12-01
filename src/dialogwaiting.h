#ifndef DIALOGWAITING_H
#define DIALOGWAITING_H

#include <QDialog>

namespace Ui {
    class DialogWaiting;
}

class DialogWaiting : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWaiting(QWidget *parent = 0);
    ~DialogWaiting();

    void setProgressBar(int val);
    void setProgressBarRange(int min, int max);
    void setTitle(const QString& val);
    void setDetail(const QString& val);

private:
    Ui::DialogWaiting *ui;
};

#endif // DIALOGWAITING_H
