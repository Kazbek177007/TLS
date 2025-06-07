#include "percent.h"

Percent::Percent(QDomElement imports)
{
    importValue = imports.firstChildElement("Importlist")
        .firstChildElement("Import")
        .firstChildElement("Value").text();
    QString resultText = "Импортная пошлина: " + importValue + "\n\n";
    importValue.chop(1);

}
