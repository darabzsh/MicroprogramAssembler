#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMap"
#include <QDebug>
#include <QMessageBox>
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

void MainWindow::run_micro(QString instruction)
{
    QString AC_tmp = ui->AC->text();
    QString AR_tmp = ui->AR->text();
    QString PC_tmp = ui->PC->text();
    QString DR_tmp = ui->DR->text();
    QString E_tmp = ui->E->text();

    // For F1
    if (ui->F1->text()=="001"){

    }


}

//QString MainWindow::toBinary(int dec)
//{
//    if (dec > 0){
//        return CompleteBits(QString::number(dec, 2), 16);
//    }
//    else{
//        QString PosBinary = QString::number(-dec+1, 2);
//        for (int i = 0 ; i < PosBinary.length() ; i++){
//            if (PosBinary[i]=='1')
//                PosBinary[i] = '0';
//            else
//                PosBinary[i] = '1';
//        }
//    }
//    return
//}


QString MainWindow::toHex(const QString& hexString)
{
    QString binaryString;
    bool ok;
    quint64 decimalValue = hexString.toULongLong(&ok, 16);

    if (ok) {
        binaryString = QString::number(decimalValue, 2);
        binaryString = binaryString.rightJustified(hexString.length() * 4, '0');
    } else {
        // Invalid hexadecimal string
        binaryString = "Invalid";
    }
    return binaryString;
}




QString MainWindow::CompleteBits(QString bits, int Length)
{
    int bitsLength = bits.length();
    for (int i = 0; i < Length - bitsLength; i++)
        bits.prepend('0');
    return bits;
}


QString MainWindow::toBinary(const QString &Hex)
{
    QString hexString;
    bool ok;
    quint64 decimalValue = Hex.toULongLong(&ok, 2);

    if (ok) {
        hexString = QString::number(decimalValue, 16);
        hexString = hexString.toUpper();
    } else {
        // Invalid binary string
        hexString = "Invalid";
    }
    return hexString; // 4 length hex returns 16 bits binary
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

