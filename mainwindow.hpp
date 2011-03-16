#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "showholder.hpp"

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ShowHolder m_shows;

private slots:
    void on_editName_returnPressed();
    void on_buttonSearch_clicked();
};

#endif // MAINWINDOW_HPP
