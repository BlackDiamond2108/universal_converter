#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTextStream>

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
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openInputFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveOutputFile);
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
        ui->saveButton->setEnabled(false);
        ui->copyButton->setEnabled(false);
        setStatus(QString::fromUtf8(result.message.c_str()), true);
        return;
    }

    ui->outputEdit->setPlainText(QString::fromUtf8(result.output.c_str()));
    ui->saveButton->setEnabled(true);
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

void MainWindow::openInputFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть входные данные",
        QString(),
        "Text files (*.txt);;All files (*.*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream stream(&file);
    QString text = stream.readAll().trimmed();
    QStringList parts = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (parts.size() >= 3) {
        bool sourceOk = false;
        bool targetOk = false;
        int source = parts[0].toInt(&sourceOk);
        int target = parts[1].toInt(&targetOk);

        if (sourceOk && targetOk) {
            ui->sourceBaseBox->setValue(source);
            ui->targetBaseBox->setValue(target);
            ui->inputEdit->setText(parts[2]);
        } else {
            ui->inputEdit->setText(text);
        }
    } else {
        ui->inputEdit->setText(text);
    }

    ui->outputEdit->clear();
    ui->saveButton->setEnabled(false);
    ui->copyButton->setEnabled(false);
    setStatus("Файл загружен", false);
}

void MainWindow::saveOutputFile()
{
    if (ui->outputEdit->toPlainText().isEmpty()) {
        setStatus("Нет результата для сохранения", true);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить результат",
        "result.txt",
        "Text files (*.txt);;All files (*.*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QTextStream stream(&file);
    stream << ui->sourceBaseBox->value() << ' '
           << ui->targetBaseBox->value() << ' '
           << ui->inputEdit->text() << '\n';
    stream << ui->outputEdit->toPlainText() << '\n';

    setStatus("Результат сохранен", false);
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
