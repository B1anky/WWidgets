#include "mainwindow.h"

#include <QPushButton>
#include <QPointer>
#include <QVBoxLayout>
#include <QWidget>
#include <QFont>
#include <QDebug>
#include <QComboBox>
#include "WPushButton.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

    QPointer<QWidget> window = new QWidget;
    window->setFixedSize(500, 500);
    QPointer<QVBoxLayout> layout = new QVBoxLayout;
    window->setLayout(layout);
    QPointer<QPushButton> button = new QPushButton(tr("I am a QPushButton"));
    button->setFixedSize(150, 25);
    layout->addWidget(button);

    QPointer<QComboBox> combo = new QComboBox;
    combo->addItem("testing 1");
    combo->addItem("testing 2");
    layout->addWidget(combo);

    QPointer<WPushButton> wButton = new WPushButton;
    wButton->setText(tr("I am a WPushButton"));
    wButton->setFixedSize(150, 25);
    layout->addWidget(wButton);

    window->show();

}

MainWindow::~MainWindow(){

}
