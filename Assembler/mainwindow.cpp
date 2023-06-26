#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMap"
#include <QDebug>
#include <QMessageBox>
QMap <QString, QString> F1,F2,F3,CD,BR;

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

    //For CD
    CD["U"] = "00";
    CD["I"] = "01";
    CD["S"] = "10";
    CD["Z"] = "11";

    //For BR
    BR["JMP"] = "00";
    BR["CALL"] = "01";
    BR["RET"] = "10";
    BR["MAP"] = "11";

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


QString MainWindow::toHex(const QString& binary,int len){
    QString hexString;    bool ok;
    quint64 decimalValue = binary.toULongLong(&ok, 2);
    if (ok) {        hexString = QString::number(decimalValue, len);
        hexString = hexString.toUpper();    } else {
        // Invalid binary string        hexString = "Invalid";
    }
    return hexString;
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


QString binaryToHex(QString binary)
{
    QString hex;

    while (binary.length() % 4 != 0) {
        binary.prepend('0');
    }

    for (int i = 0; i < binary.length(); i += 4) {
        QString nibble = binary.mid(i, 4);
        bool ok;
        int decimal = nibble.toInt(&ok, 2);

        if (ok) {
            if (decimal < 10) {
                hex.append(QString::number(decimal));
            } else {
                hex.append(QChar('A' + decimal - 10));
            }
        } else {
            // Invalid binary input
            return QString();
        }
    }

    return hex;
}




void MainWindow::Fill_Micro_Table(QList<QStringList> wordList)
{
    int currentline = 0;
    ui->microprogram->clear();
    Clear_Micro();
    QStringList newline;
    foreach (const QStringList& words, wordList) {
        QString line = words.join(" ");
        newline.append(line);
    }

    ui->microprogram->append(newline.join("\n"));
    QMap <QString, int> Labels;
    Labels.clear();
    foreach (const QStringList& words, wordList){
        if (words.at(0) == "END")
            break;
        else if (words.at(0) == "ORG"){
            currentline = words.at(1).toInt();
            continue;
        }
        else if (words.at(0).endsWith(":")){
            Labels[words.at(0).left(words.at(0).size() - 1)] = currentline;
        }
        currentline ++;
    }

    currentline = 0;
    foreach (const QStringList& words, wordList) {
        QString instruct = "";
        QString content = "";

        if (words.at(0) == "END")
            break;
        if (words.at(0) == "ORG"){
            currentline = words.at(1).toInt();
            continue;
        }
        if (words.at(0).endsWith(":")){
            qDebug() << words.at(0) ;
            QString text = words.at(0);
            text = text.chopped(1);
            ui->Microprogram_Memory->setItem(currentline,2,new QTableWidgetItem(text));
//            Labels[words.at(0).left(words.at(0).size() - 1)] = currentline;
            instruct = words.mid(1).join(", ");
        }
        else
            instruct = words.mid(0).join(", ");
        ui->Microprogram_Memory->setItem(currentline,3,new QTableWidgetItem(instruct));
        QStringList cf1 = {} , cf2= {} , cf3 = {} ;
        QString cd = "" , br = "" ,addr = "";
        for(int i = 0;i < words.length();i++){
            if(F1.contains(words[i]))
                cf1.append(words[i]);
            else if(F2.contains(words[i]))
                cf2.append(words[i]);
            else if(F3.contains(words[i]))
                cf3.append(words[i]);
            else if(CD.contains(words[i]))
                cd = CD[words[i]];
            else if(BR.contains(words[i]))
                br = BR[words[i]];
            else if(words[i] == "NEXT")
                addr = QString::number(currentline+1, 2).rightJustified(7, '0');
            else if(Labels.contains(words[i]))
                addr = QString::number(Labels[words[i]], 2).rightJustified(7, '0');
        }
        if(cf1.length() == 0)
            content += "000";
        else
            content += F1[cf1[0]];
        if(cf2.length() == 0)
            content += "000";
        else
            content += F2[cf2[0]];
        if(cf3.length() == 0)
            content += "000";
        else
            content += F3[cf3[0]];
        content += cd;
        content += br;
        content += addr;
        QString hx = binaryToHex(content);
        qDebug() << content ;

        ui->Microprogram_Memory->setItem(currentline,4,new QTableWidgetItem(hx));
        currentline ++;
    }

}

void MainWindow::Clear_Micro()
{
    int startColumn = 2;
    int endColumn = 4;

    int rowCount = ui->Microprogram_Memory->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        for (int column = startColumn; column <= endColumn; ++column) {
            QTableWidgetItem* item = ui->Microprogram_Memory->item(row, column);
            if (item) {
                item->setText("");
            }
        }
    }
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


//    qDebug() << wordList[0][1] ;



    for(int i = 0 ; i< wordList.length();i++)
    {
        QStringList cf1 = {} , cf2= {} , cf3 = {};
        for(int j = 0 ; j< wordList[i].length();j++)
        {
            if(wordList[i][j].endsWith(":"))
                continue;
            if(F1.contains(wordList[i][j]))
                cf1.append(wordList[i][j]);
            else if(F2.contains(wordList[i][j]))
                cf2.append(wordList[i][j]);
            else if(F3.contains(wordList[i][j]))
                cf3.append(wordList[i][j]);
            QString message = "%1 has %2 , %3 both ...";
            if(cf1.length()>=2)
            {
                QMessageBox::critical(nullptr, "Error", message.arg("F1").arg(cf1[0]).arg(cf1[1]));
                return ;
            }
            if(cf2.length()>=2)
            {
                QMessageBox::critical(nullptr, "Error", message.arg("F2").arg(cf2[0]).arg(cf2[1]));
                return ;
            }
            if(cf3.length()>=2)
            {
                QMessageBox::critical(nullptr, "Error", message.arg("F2").arg(cf3[0]).arg(cf3[1]));
                return ;
            }
        }
    }
    Fill_Micro_Table(wordList);
}

