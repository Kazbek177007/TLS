#ifndef CURRENCY_H
#define CURRENCY_H

#include <QDomElement>
#include <QObject>

class Currency : public QObject
{
    Q_OBJECT

    Currency();

public:
    static Currency& get();
    float euro() const;
    float dollar() const;
    float yuan() const;

private:
    QDomElement root;

};

#endif // CURRENCY_H
