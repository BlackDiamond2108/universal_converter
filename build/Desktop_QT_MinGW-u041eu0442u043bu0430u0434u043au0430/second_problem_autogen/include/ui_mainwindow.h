/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QGroupBox *inputGroup;
    QVBoxLayout *inputLayout;
    QHBoxLayout *baseLayout;
    QLabel *sourceBaseLabel;
    QSpinBox *sourceBaseBox;
    QSpacerItem *baseSpacer;
    QLabel *targetBaseLabel;
    QSpinBox *targetBaseBox;
    QPushButton *swapButton;
    QSpacerItem *baseStretch;
    QLabel *inputLabel;
    QLineEdit *inputEdit;
    QHBoxLayout *buttonLayout;
    QPushButton *convertButton;
    QPushButton *copyButton;
    QSpacerItem *buttonStretch;
    QGroupBox *outputGroup;
    QVBoxLayout *outputLayout;
    QTextEdit *outputEdit;
    QLabel *statusLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(920, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setSpacing(12);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(18, 18, 18, 14);
        inputGroup = new QGroupBox(centralwidget);
        inputGroup->setObjectName("inputGroup");
        inputLayout = new QVBoxLayout(inputGroup);
        inputLayout->setObjectName("inputLayout");
        baseLayout = new QHBoxLayout();
        baseLayout->setObjectName("baseLayout");
        sourceBaseLabel = new QLabel(inputGroup);
        sourceBaseLabel->setObjectName("sourceBaseLabel");

        baseLayout->addWidget(sourceBaseLabel);

        sourceBaseBox = new QSpinBox(inputGroup);
        sourceBaseBox->setObjectName("sourceBaseBox");
        sourceBaseBox->setMinimumSize(QSize(110, 0));
        sourceBaseBox->setMinimum(2);
        sourceBaseBox->setMaximum(500);
        sourceBaseBox->setValue(10);

        baseLayout->addWidget(sourceBaseBox);

        baseSpacer = new QSpacerItem(12, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        baseLayout->addItem(baseSpacer);

        targetBaseLabel = new QLabel(inputGroup);
        targetBaseLabel->setObjectName("targetBaseLabel");

        baseLayout->addWidget(targetBaseLabel);

        targetBaseBox = new QSpinBox(inputGroup);
        targetBaseBox->setObjectName("targetBaseBox");
        targetBaseBox->setMinimumSize(QSize(110, 0));
        targetBaseBox->setMinimum(2);
        targetBaseBox->setMaximum(500);
        targetBaseBox->setValue(2);

        baseLayout->addWidget(targetBaseBox);

        swapButton = new QPushButton(inputGroup);
        swapButton->setObjectName("swapButton");

        baseLayout->addWidget(swapButton);

        baseStretch = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        baseLayout->addItem(baseStretch);


        inputLayout->addLayout(baseLayout);

        inputLabel = new QLabel(inputGroup);
        inputLabel->setObjectName("inputLabel");

        inputLayout->addWidget(inputLabel);

        inputEdit = new QLineEdit(inputGroup);
        inputEdit->setObjectName("inputEdit");
        inputEdit->setClearButtonEnabled(true);

        inputLayout->addWidget(inputEdit);


        mainLayout->addWidget(inputGroup);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        convertButton = new QPushButton(centralwidget);
        convertButton->setObjectName("convertButton");

        buttonLayout->addWidget(convertButton);

        copyButton = new QPushButton(centralwidget);
        copyButton->setObjectName("copyButton");
        copyButton->setEnabled(false);

        buttonLayout->addWidget(copyButton);

        buttonStretch = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(buttonStretch);


        mainLayout->addLayout(buttonLayout);

        outputGroup = new QGroupBox(centralwidget);
        outputGroup->setObjectName("outputGroup");
        outputLayout = new QVBoxLayout(outputGroup);
        outputLayout->setObjectName("outputLayout");
        outputEdit = new QTextEdit(outputGroup);
        outputEdit->setObjectName("outputEdit");
        outputEdit->setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);
        outputEdit->setReadOnly(true);

        outputLayout->addWidget(outputEdit);


        mainLayout->addWidget(outputGroup);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);

        mainLayout->addWidget(statusLabel);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        convertButton->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Universal Converter", nullptr));
        inputGroup->setTitle(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\276\320\264", nullptr));
        sourceBaseLabel->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267 \320\276\321\201\320\275\320\276\320\262\320\260\320\275\320\270\321\217", nullptr));
        targetBaseLabel->setText(QCoreApplication::translate("MainWindow", "\320\222 \320\276\321\201\320\275\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
#if QT_CONFIG(tooltip)
        swapButton->setToolTip(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\274\320\265\320\275\321\217\321\202\321\214 \320\276\321\201\320\275\320\276\320\262\320\260\320\275\320\270\321\217 \320\274\320\265\321\201\321\202\320\260\320\274\320\270", nullptr));
#endif // QT_CONFIG(tooltip)
        swapButton->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\274\320\265\320\275\321\217\321\202\321\214", nullptr));
        inputLabel->setText(QCoreApplication::translate("MainWindow", "\320\247\320\270\321\201\320\273\320\276", nullptr));
        inputEdit->setPlaceholderText(QString());
        convertButton->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\275\320\262\320\265\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        copyButton->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\277\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        outputGroup->setTitle(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        outputEdit->setPlaceholderText(QString());
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
