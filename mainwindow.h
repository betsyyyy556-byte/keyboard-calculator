#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QKeyEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    double firstOperand;
    QString pendingOperator;
    bool waitingForOperand;
    bool errorState;

    void handleButton(const QString &text);
    void inputDigit(const QString &digit);
    void inputDecimal();
    void chooseOperator(const QString &op);
    void calculate();
    void backspace();
    void clearAll();
    void showError(const QString &message);
};

#endif // MAINWINDOW_H