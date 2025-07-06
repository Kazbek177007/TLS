#include "widget.h"
#include "ui_widget.h"
#include "percent.h"
#include "eurokg.h"
#include "currency.h"

#include <QComboBox>
#include <QDomDocument>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    auto vbox = new QVBoxLayout;
    setLayout(vbox);
    auto hbox = new QHBoxLayout;
    vbox->addLayout(hbox);
    auto confirm = new QPushButton("Confirm");
    vbox->addWidget(confirm);
    customsFees = new QLabel("Customs fees");
    vbox->addWidget(customsFees);
    vatVariants = new QVBoxLayout;
    vbox->addLayout(vatVariants);
    auto calculate = new QPushButton("Calculate");
    vbox->addWidget(calculate);
    calculation = new QLabel;
    calculation->setText("formula");
    vbox->addWidget(calculation);
    codeuser = new QLineEdit;
    hbox->addWidget(codeuser);
    hbox->addWidget(new QComboBox);
    price = new QLineEdit;
    hbox->addWidget(price);
    networkManager = new QNetworkAccessManager;
    connect(confirm,&QPushButton::clicked,
            this, &Widget::onClicked);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &Widget::onFinished);
    connect(calculate, &QPushButton::clicked,
            this, &Widget::onCalculate);
}

Widget::~Widget()
{
    delete ui;
    delete networkManager;
}

void Widget::onClicked()
{
    QString code = codeuser->text();
    if (code.isEmpty())
        return;
    QString login = "sa58789";
    QString pass = "p2L9S2MU";
    QString password = QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Md5).toHex();
    qDebug() << password;
    QString secret = QCryptographicHash::hash((code+":"+login+":"+password).toUtf8(), QCryptographicHash::Md5).toHex();
    qDebug() << secret;
    QString countrycode = "156";

    const QUrl url("https://www.alta.ru/tnved/xml/?tncode="+code+"&login="+login+"&secret="+secret);
    qDebug() << url;
    QNetworkRequest request(url);
    networkManager->get(request);

    codeuser->clear();
    qDebug () << Currency::get().dollar();
}

enum class CustomsFees
{
    percent,
    euroKg
};
QDomElement xmlParser(const QDomNode& node, const QString& tagName)
{
    QDomNode child = node.firstChild();
    while (!child.isNull()) {
        if (child.isElement())
        {
            QDomElement el = child.toElement();
            if(el.tagName()==tagName)
                return el;

            QDomElement found = xmlParser(child, tagName);
            if(!found.isNull())
                return found;
        }
        else
        {
            QDomElement found = xmlParser(child, tagName);
            if(!found.isNull())
                return found;
        }
        child = child.nextSibling();
    }
    return QDomElement();
}

CustomsFees recognize(const QDomElement& root)
{
    if (xmlParser(xmlParser(root, "Import"), "ValueUnit").text()=="кг")
    {
        return CustomsFees::euroKg;
    }
    return CustomsFees::percent;
}

void Widget::onFinished(QNetworkReply *reply)
{
    if(reply->error()!=QNetworkReply::NoError)
    {
        qDebug()<< "Error:" << reply->errorString();
        return;
    }
    QByteArray data = reply->readAll();

    QDomDocument doc;
    doc.setContent(data);

    QDomElement root = doc.documentElement();
    QString resultText;

    // 1. Code
    QString code = root.firstChildElement("Code").text();
    resultText += "Код: " + code + "\n";

    // 2. Name
    QString name = root.firstChildElement("Name").text();
    resultText += "Наименование: " + name + "\n\n";

    // 3. Import->Value
    if (recognize(root)==CustomsFees::euroKg)
    {
        auto euroKg = new Eurokg(root.firstChildElement("Importlist").firstChildElement("Import"));
        import = euroKg;
        vatVariants->addWidget(euroKg);
    }
    else
    {
        auto percent = new Percent(root.firstChildElement("Importlist").firstChildElement("Import"));
        import = percent;
        vatVariants->addWidget(percent);
    }


    // 4. VAT data
    QDomNodeList vatNodes = root.firstChildElement("VATlist").elementsByTagName("VAT");
    customsFees->setText(resultText);

    for (int i = 0; i < vatNodes.size(); ++i) {
        QDomElement vatElement = vatNodes.at(i).toElement();
        QString vatValue = vatElement.firstChildElement("Value").text();
        QString vatCondition = vatElement.firstChildElement("Condition").text();
        QRadioButton* button = new QRadioButton(vatValue+vatCondition);
        vatVariants->addWidget(button);
    }

    reply->deleteLater();
}

void Widget::onCalculate()
{
    QRadioButton* selectedButton = nullptr;
    for (int i = 0; i<vatVariants->count(); i++)
    {
        QWidget* widget = vatVariants->itemAt(i)->widget();
        QRadioButton* ptrbutton = dynamic_cast<QRadioButton*>(widget);
        if (ptrbutton and ptrbutton->isChecked())
        {
            selectedButton = ptrbutton;
            break;
        }

    }
    if (!selectedButton)
    {
        return;
    }
    QString selectedVat = selectedButton->text();
    selectedVat = selectedVat.split('%')[0];
    float vat = selectedVat.toFloat();
    float result = import->calculate({vat,price->text().toFloat()});
    calculation->setText(QString::number(result));

}
