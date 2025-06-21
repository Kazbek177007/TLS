#pragma once

struct Fees
{
    float vat = 0;
    float price = 0;

//percent
// евро/кг 6307101000
// евро/пара 6403919300
// евро/м2 5703100000
// х%, но не менее евро/кг 0805102000
};




class ICustomFees
{
public:
    virtual float calculate(Fees fees) = 0;

};
