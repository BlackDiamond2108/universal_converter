#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "BaseConverter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BaseConverter converter;

    void connectActions();
    void convertNumber();
    void swapBases();
    void copyOutput();
    void setStatus(const QString& text, bool isError);
};

#endif // MAINWINDOW_H
