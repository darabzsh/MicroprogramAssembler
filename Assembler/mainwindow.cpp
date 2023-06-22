#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMap"
QMap <QString, QString> F1,F2,F3;
QString f1 = "-",
    f2 = "-",
    f3 = "-",
    cd = "-",
    br = "-",
    ad = "-",
    sbr = "-",
    car = "-",
    ar = "-",
    ac = "-",
    pc = "-",
    dr = "-",
    addr = "-",
    i = "-",
    e = "-",
    opcode = "-";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // For F1
    F1["NOP"] = "000";
    F1["ADD"] = "001";
    F1["CLRAC"] = "010";
    F1["INCAC"] = "011";
    F1["DRTAC"] = "100";
    F1["DRTAR"] = "101";
    F1["PCTAR"] = "110";
    F1["WRITE"] = "111";

    // For F2
    F2["NOP"] = "000";
    F2["SUB"] = "001";
    F2["OR"] = "010";
    F2["AND"] = "011";
    F2["READ"] = "100";
    F2["ACTDR"] = "101";
    F2["INCDR"] = "110";
    F2["PCTDR"] = "111";

    // For F3
    F3["NOP"] = "000";
    F3["XOR"] = "001";
    F3["COM"] = "010";
    F3["SHL"] = "011";
    F3["SHR"] = "100";
    F3["INCPC"] = "101";
    F3["ARTPC"] = "110";
    F3[""] = "RESERVED";
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

void MainWindow::set_regs()
{
    if (f1 != "-") ui->F1->setText(f1);
    if (f2 != "-") ui->F1->setText(f2);
    if (f3 != "-") ui->F1->setText(f3);
    if (cd != "-") ui->F1->setText(cd);
    if (br != "-") ui->F1->setText(br);
    if (ad != "-") ui->F1->setText(ad);
    if (sbr != "-") ui->F1->setText(sbr);
    if (car != "-") ui->F1->setText(car);
    if (ar != "-") ui->F1->setText(ar);
    if (ac != "-") ui->F1->setText(ac);
    if (pc != "-") ui->F1->setText(pc);
    if (dr != "-") ui->F1->setText(dr);
    if (addr != "-") ui->F1->setText(addr);
    if (i != "-") ui->F1->setText(i);
    if (e != "-") ui->F1->setText(e);
    if (opcode != "-") ui->F1->setText(opcode);
}

MainWindow::~MainWindow()
{
    delete ui;
}


