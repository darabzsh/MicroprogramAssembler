#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int var = 0; var < 128; ++var) {
        ui->Microprogram_Memory->insertRow(var);
        ui->Microprogram_Memory->verticalHeader()->setVisible(false);
        ui->Microprogram_Memory->setItem(var,0, new QTableWidgetItem(QString::number(var)));
        ui->Microprogram_Memory->item(var, 0)->setTextAlignment(Qt::AlignCenter);


        QString hexString = QString("%1").arg(var, 4, 16, QChar('0'));
        ui->Microprogram_Memory->setItem(var,1, new QTableWidgetItem(hexString));
        ui->Microprogram_Memory->item(var, 1)->setTextAlignment(Qt::AlignCenter);
    }
    for (int var = 0; var < 1024; ++var) {
        ui->Main_Memory->insertRow(var);
        ui->Main_Memory->verticalHeader()->setVisible(false);
        ui->Main_Memory->setItem(var,0, new QTableWidgetItem(QString::number(var)));
        ui->Main_Memory->item(var, 0)->setTextAlignment(Qt::AlignCenter);
        QString hexString = QString("%1").arg(var, 4, 16, QChar('0'));
        ui->Main_Memory->setItem(var,1, new QTableWidgetItem(hexString));
        ui->Main_Memory->item(var, 1)->setTextAlignment(Qt::AlignCenter);
    }
    ui->Microprogram_Memory->resizeRowsToContents();
    ui->Microprogram_Memory->resizeColumnsToContents();
    ui->Main_Memory->resizeRowsToContents();
    ui->Main_Memory->resizeColumnsToContents();
//    ui->Main_Memory-

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_MicroButton_clicked()
{
    QString micro = ui->microprogram->toPlainText();
    QStringList lines = micro.split("\n",Qt::SkipEmptyParts);
    QList<QStringList> wordList;

    foreach (const QString& line, lines) {
        QStringList lineWords = line.split(" ", Qt::SkipEmptyParts);
        wordList.append(lineWords);
    }
//    wordList.removeAll([](const QStringList& list) {
//        return list.isEmpty();
//    });

    //Conditions

    int count_end = 0;
    foreach (const QStringList& words, wordList) {
        count_end += words.count("END");
    }
    if(count_end != 1){
        QMessageBox::critical(nullptr, "Error", "MicroProgram just need one <END>...");
        return ;
    }
    // --------
    int currentline = 0;

//    qDebug() << wordList[0][1] ;

    ui->microprogram->clear();
    QStringList newline;
    foreach (const QStringList& words, wordList) {
        QString line = words.join(" ");
        newline.append(line);
    }

    ui->microprogram->append(newline.join("\n"));

    foreach (const QStringList& words, wordList) {
        if (words.at(0) == "END")
            break;
        if (words.at(0) == "ORG")
            currentline = words.at(1).toInt();
        if (words.at(0).endsWith(":")){
            qDebug() << words.at(0) ;
            QString text = words.at(0);
            text = text.chopped(1);
            ui->Microprogram_Memory->setItem(currentline-1,2,new QTableWidgetItem(text));
        }
        currentline ++;
    }

}

