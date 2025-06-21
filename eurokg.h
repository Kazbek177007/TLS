#ifndef EUROKG_H
#define EUROKG_H

#include <QWidget>

namespace Ui {
class Eurokg;
}

class Eurokg : public QWidget
{
    Q_OBJECT

public:
    explicit Eurokg(QWidget *parent = nullptr);
    ~Eurokg();

private:
    Ui::Eurokg *ui;
};

#endif // EUROKG_H
