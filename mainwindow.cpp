#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QClipboard>

#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectActions();
    setStatus("Готово", false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectActions()
{
    connect(ui->convertButton, &QPushButton::clicked, this, &MainWindow::convertNumber);
    connect(ui->swapButton, &QPushButton::clicked, this, &MainWindow::swapBases);
    connect(ui->copyButton, &QPushButton::clicked, this, &MainWindow::copyOutput);
    connect(ui->inputEdit, &QLineEdit::returnPressed, this, &MainWindow::convertNumber);
}

void MainWindow::convertNumber()
{
    QString input = ui->inputEdit->text().trimmed();

    ConversionResult result = converter.convert(
        input.toUtf8().constData(),
        std::to_string(ui->sourceBaseBox->value()),
        std::to_string(ui->targetBaseBox->value())
    );

    if (!result.success) {
        ui->outputEdit->clear();
        ui->copyButton->setEnabled(false);
        setStatus(QString::fromUtf8(result.message.c_str()), true);
        return;
    }

    ui->outputEdit->setPlainText(QString::fromUtf8(result.output.c_str()));
    ui->copyButton->setEnabled(true);

    if (result.message.empty()) {
        setStatus("Конвертация выполнена", false);
    } else {
        setStatus(QString::fromUtf8(result.message.c_str()), false);
    }
}

void MainWindow::swapBases()
{
    int source = ui->sourceBaseBox->value();
    ui->sourceBaseBox->setValue(ui->targetBaseBox->value());
    ui->targetBaseBox->setValue(source);

    if (!ui->outputEdit->toPlainText().isEmpty()) {
        QString oldInput = ui->inputEdit->text();
        ui->inputEdit->setText(ui->outputEdit->toPlainText());
        ui->outputEdit->setPlainText(oldInput);
    }

    setStatus("Основания поменяны местами", false);
}

void MainWindow::copyOutput()
{
    QApplication::clipboard()->setText(ui->outputEdit->toPlainText());
    setStatus("Результат скопирован", false);
}

void MainWindow::setStatus(const QString& text, bool isError)
{
    ui->statusLabel->setText(text);
    ui->statusLabel->setStyleSheet(isError ? "color: #b3261e;" : "color: #25633a;");
}
