#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , firstOperand(0)
    , waitingForOperand(true)
    , errorState(false)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    ui->display->setFocusPolicy(Qt::NoFocus);

    ui->display->setText("0");
    ui->display->setReadOnly(true);
    ui->display->setAlignment(Qt::AlignRight);

    const QList<QPushButton *> buttons = findChildren<QPushButton *>();

    for (QPushButton *button : buttons) {
        button->setFocusPolicy(Qt::NoFocus);

        connect(button, &QPushButton::clicked, this, [this, button]() {
            handleButton(button->text());
        });
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton(const QString &text)
{
    if (text >= "0" && text <= "9") {
        inputDigit(text);
    } else if (text == ".") {
        inputDecimal();
    } else if (text == "+" || text == "-" || text == "*" || text == "/") {
        chooseOperator(text);
    } else if (text == "=") {
        calculate();
    } else if (text == "退格") {
        backspace();
    } else if (text == "清空") {
        clearAll();
    }
}

void MainWindow::inputDigit(const QString &digit)
{
    if (errorState) {
        clearAll();
    }

    if (waitingForOperand || ui->display->text() == "0") {
        ui->display->setText(digit);
        waitingForOperand = false;
    } else {
        ui->display->setText(ui->display->text() + digit);
    }
}

void MainWindow::inputDecimal()
{
    if (errorState) {
        clearAll();
    }

    if (waitingForOperand) {
        ui->display->setText("0.");
        waitingForOperand = false;
        return;
    }

    if (!ui->display->text().contains(".")) {
        ui->display->setText(ui->display->text() + ".");
    }
}

void MainWindow::chooseOperator(const QString &op)
{
    if (errorState) {
        clearAll();
        return;
    }

    if (!waitingForOperand && !pendingOperator.isEmpty()) {
        calculate();

        if (errorState) {
            return;
        }
    }

    firstOperand = ui->display->text().toDouble();
    pendingOperator = op;
    waitingForOperand = true;
}

void MainWindow::calculate()
{
    if (pendingOperator.isEmpty() || waitingForOperand || errorState) {
        return;
    }

    double secondOperand = ui->display->text().toDouble();
    double result = 0;

    if (pendingOperator == "+") {
        result = firstOperand + secondOperand;
    } else if (pendingOperator == "-") {
        result = firstOperand - secondOperand;
    } else if (pendingOperator == "*") {
        result = firstOperand * secondOperand;
    } else if (pendingOperator == "/") {
        if (secondOperand == 0) {
            showError("错误：除数不能为0");
            return;
        }
        result = firstOperand / secondOperand;
    }

    ui->display->setText(QString::number(result, 'g', 12));
    firstOperand = result;
    pendingOperator.clear();
    waitingForOperand = true;
}

void MainWindow::backspace()
{
    if (errorState) {
        clearAll();
        return;
    }

    if (waitingForOperand) {
        return;
    }

    QString text = ui->display->text();
    text.chop(1);

    if (text.isEmpty() || text == "-") {
        ui->display->setText("0");
        waitingForOperand = true;
    } else {
        ui->display->setText(text);
    }
}

void MainWindow::clearAll()
{
    ui->display->setText("0");
    firstOperand = 0;
    pendingOperator.clear();
    waitingForOperand = true;
    errorState = false;
}

void MainWindow::showError(const QString &message)
{
    ui->display->setText(message);
    pendingOperator.clear();
    waitingForOperand = true;
    errorState = true;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const QString text = event->text();

    if (text >= "0" && text <= "9") {
        inputDigit(text);
    } else if (text == ".") {
        inputDecimal();
    } else if (text == "+" || text == "-" || text == "*" || text == "/") {
        chooseOperator(text);
    } else if (event->key() == Qt::Key_Return ||
               event->key() == Qt::Key_Enter ||
               event->key() == Qt::Key_Equal) {
        calculate();
    } else if (event->key() == Qt::Key_Backspace) {
        backspace();
    } else if (event->key() == Qt::Key_Escape) {
        clearAll();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}