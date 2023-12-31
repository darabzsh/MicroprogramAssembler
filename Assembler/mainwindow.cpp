#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMap"
#include <QDebug>
#include <QMessageBox>
#include <QColor>
#include <QFile>
#include <QFileDialog>
QMap <QString, QString> F1,F2,F3,CD,BR;

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

    // Create tables
    for (int var = 0; var < 128; ++var) {
        ui->Microprogram_Memory->insertRow(var);
        ui->Microprogram_Memory->verticalHeader()->setVisible(false);
        ui->Microprogram_Memory->setItem(var,0, new QTableWidgetItem(QString::number(var)));
        ui->Microprogram_Memory->item(var, 0)->setTextAlignment(Qt::AlignCenter);


        QString hexString = QString("%1").arg(var, 4, 16, QChar('0'));
        ui->Microprogram_Memory->setItem(var,1, new QTableWidgetItem("0x"+hexString.toUpper()));
        ui->Microprogram_Memory->item(var, 1)->setTextAlignment(Qt::AlignCenter);
        ui->Microprogram_Memory->setItem(var,2, new QTableWidgetItem(QString("")));
        ui->Microprogram_Memory->setItem(var,3, new QTableWidgetItem(QString("")));
        ui->Microprogram_Memory->setItem(var,4, new QTableWidgetItem(QString("")));
    }
    for (int var = 0; var < 2048; ++var) {
        ui->Main_Memory->insertRow(var);
        ui->Main_Memory->verticalHeader()->setVisible(false);
        ui->Main_Memory->setItem(var,0, new QTableWidgetItem(QString::number(var)));
        ui->Main_Memory->item(var, 0)->setTextAlignment(Qt::AlignCenter);
        QString hexString = QString("%1").arg(var, 4, 16, QChar('0'));
        ui->Main_Memory->setItem(var,1, new QTableWidgetItem("0x"+hexString.toUpper()));
        ui->Main_Memory->item(var, 1)->setTextAlignment(Qt::AlignCenter);
        ui->Main_Memory->setItem(var,2, new QTableWidgetItem(QString("")));
        ui->Main_Memory->setItem(var,3, new QTableWidgetItem(QString("")));
        ui->Main_Memory->setItem(var,4, new QTableWidgetItem(QString("")));
    }
    ui->Microprogram_Memory->resizeColumnsToContents();
    ui->Main_Memory->resizeColumnsToContents();
}

QList<int> mic_color;
QList<int> main_color;

void MainWindow::reset_colors()
{
    for(int i=0 ; i< mic_color.length();i++){
        for (int column = 0; column < ui->Microprogram_Memory->columnCount(); ++column)
        {
            QTableWidgetItem *item = ui->Microprogram_Memory->item(mic_color[i], column);
            item->setBackground(QColor("#222b2e"));
        }
    }
    for(int i=0 ; i< main_color.length();i++){
        for (int column = 0; column < ui->Microprogram_Memory->columnCount(); ++column)
        {
            ui->Main_Memory->item(main_color[i],column)->setBackground(QColor("#222b2e"));
        }
    }
}


int scroll1 = 64;

void MainWindow::run_micro(QString instruction)
{
    QString AC_tmp = toBinary(ui->AC->text().mid(2, 4));   // 16 bits
    QString AR_tmp = ui->AR->text();                       // 11 bits
    QString PC_tmp = ui->PC->text();                       // 11 bits
    QString DR_tmp = toBinary(ui->DR->text().mid(2, 4));   // 16 bits

    QString F1 = instruction.mid(0, 3);
    QString F2 = instruction.mid(3, 3);
    QString F3 = instruction.mid(6, 3);
    QString CD = instruction.mid(9, 2);
    QString BR = instruction.mid(11, 2);
    QString AD = instruction.mid(13, 7);

    ui->F1->setText(F1);
    ui->F2->setText(F2);
    ui->F3->setText(F3);
    ui->CD->setText(CD);
    ui->BR->setText(BR);
    ui->AD->setText(AD);
    ui->I->setText(DR_tmp[0]);
    ui->OpCode->setText(DR_tmp.mid(1, 4));
    ui->ADDR->setText(DR_tmp.mid(5));

    // For F1
    if (F1 == "001")
        ui->AC->setText("0x" + binaryToHex(SumWithCarry(CompleteBits(AC_tmp), CompleteBits(DR_tmp), true))); // check
    else if (F1 == "010")
        ui->AC->setText("0x0000");
    else if (F1 == "011")
        ui->AC->setText("0x" + binaryToHex(SumWithCarry(CompleteBits(AC_tmp), CompleteBits("01"))));
    else if(F1 == "100")
        ui->AC->setText(ui->DR->text());
    else if(F1 == "101")
        ui->AR->setText(DR_tmp.mid(5));
    else if(F1 == "110")
        ui->AR->setText(PC_tmp);
    else if(ui->F1->text()=="111"){
        ui->Main_Memory->setItem(AR_tmp.toInt(nullptr, 2),4,new QTableWidgetItem("0x" + binaryToHex(CompleteBits(DR_tmp))));
        ui->Main_Memory->item(AR_tmp.toInt(nullptr, 2),4)->setTextAlignment(Qt::AlignCenter);
    }

    // For F2
    if (F2 == "001")
        ui->AC->setText("0x" + binaryToHex(SumWithCarry(CompleteBits(AC_tmp), TwoComplement((CompleteBits(DR_tmp))))));
    else if(F2 == "010")
        ui->AC->setText("0x" + binaryToHex(OR(AC_tmp, DR_tmp)));
    else if(F2 == "011")
        ui->AC->setText("0x" + binaryToHex(AND(AC_tmp, DR_tmp)));
    else if(F2 == "100")
        ui->DR->setText(ui->Main_Memory->item(AR_tmp.toInt(nullptr, 2), 4)->text());  // check
    else if(F2 == "101")
        ui->DR->setText("0x"+binaryToHex(AC_tmp)); // check
    else if(F2 == "110")
        ui->DR->setText(binaryToHex(SumWithCarry(DR_tmp, CompleteBits("01"))));
    else if(F2 == "111")
        ui->DR->setText(binaryToHex(DR_tmp.left(5) + PC_tmp)); // check


    // For F3
    if (F3 == "001")
        ui->AC->setText("0x" + binaryToHex(XOR(AC_tmp, DR_tmp)));
    else if (F3 == "010")
        ui->AC->setText("0x" + binaryToHex(OneComplement(AC_tmp)));
    else if (F3 == "011")
        ui->AC->setText("0x" + binaryToHex(ShiftToLeft(AC_tmp)));
    else if (F3 == "100")
        ui->AC->setText("0x" + binaryToHex(ShiftToRight(AC_tmp)));
    else if (F3 == "101")
        ui->PC->setText(SumWithCarry(PC_tmp, CompleteBits("01", 11)));
    else if (F3 == "110")
        ui->PC->setText(AR_tmp);
//    else if (ui->F3->text()=="111")  // RESERVED


    // For CD
    bool flag = false;
    if (CD == "00")
        flag = true;
    else if (CD == "01" && DR_tmp[0] == '1')
        flag = true;
    else if (CD == "10" && AC_tmp[0] == '1')
        flag = true;
    else if (CD == "11" && AC_tmp == "0x0000")
        flag = true;

    if (flag){
        if (BR == "00")                         // JMP
            ui->CAR->setText(AD);
        else if (BR == "01"){                   // CALL
            ui->SBR->setText(SumWithCarry(ui->CAR->text(), CompleteBits("01", 7))); // check
            ui->CAR->setText(AD);
        }
        else if (BR == "10")                    // RET
            ui->CAR->setText(ui->SBR->text());
        else {                                  // MAP
            ui->CAR->setText("0"+DR_tmp.mid(1, 4)+"00"); // check
        }
    }
    else{
        if(BR == "00" || BR == "01")
            ui->CAR->setText(SumWithCarry(ui->CAR->text(), CompleteBits("01", 7)));
    }


    // Scroll to next address
    scroll1 = ui->CAR->text().toInt(nullptr,2);
    ui->Microprogram_Memory->scrollToItem(ui->Microprogram_Memory->item(scroll1, 0));

    for (int column = 0; column < ui->Microprogram_Memory->columnCount(); ++column)
    {
        QTableWidgetItem *item = ui->Microprogram_Memory->item(scroll1, column);
        item->setBackground(QColor("#1f5748"));
        item->setTextAlignment(Qt::AlignCenter);
        mic_color.append(scroll1);
    }
}

QString MainWindow::toBinary(int dec)
{
    QString NormalBinary;
    int number = qAbs(dec);
    while (number > 0) {
        int remainder = number % 2;
        NormalBinary.prepend(QString::number(remainder));
        number /= 2;
    }
    NormalBinary.prepend("0");
    NormalBinary = CompleteBits(NormalBinary, 16);
    if (dec < 0)
        NormalBinary = TwoComplement(NormalBinary);

    return NormalBinary;
}


QString MainWindow::toHex(const QString& binary, int length)
{
    QString hexString;
    bool ok;
    quint64 decimalValue = binary.toULongLong(&ok, 2);
    if (ok) {
        hexString = QString::number(decimalValue, length);
        hexString = hexString.toUpper();
    } else {
        // Invalid binary string
        hexString = "Invalid";
    }

    return hexString;
}



QString MainWindow::CompleteBits(QString bits, int Length)
{
    int bitsLength = bits.length();
    bool neg = false;
    QString adder = "0";
    if (bits[0]=='1')
        adder = "1";
    else if (bits[0] == 'F')
        adder = "F";
    for (int i = 0; i < Length - bitsLength; i++){
        bits.prepend(adder);
    }
    return bits;
}

QString MainWindow::OneComplement(QString Binary)
{
    for (int i = 0 ; i < Binary.length(); i++){
        if (Binary[i] == '1')
            Binary[i] = '0';
        else
            Binary[i] = '1';
    }
    return Binary;
}

QString MainWindow::TwoComplement(QString Binary)
{
    Binary = OneComplement(Binary);
    Binary = SumWithCarry(CompleteBits(Binary), CompleteBits("01"));
    return Binary;
}

QString MainWindow::SumWithCarry(const QString &binaryNumber1, const QString &binaryNumber2, bool mode)
{
    QString sum;
    QString carry = "0";

    for (int i = binaryNumber1.length()-1; i >= 0; --i) {
        int bit1 = binaryNumber1.at(i).digitValue();
        int bit2 = binaryNumber2.at(i).digitValue();
        int currentSum = bit1 + bit2 + carry.toInt();

        sum.prepend(QString::number(currentSum % 2));
        carry = QString::number(currentSum / 2);
    }
    if (mode){
        ui->E->setText(carry);
    }
    return sum;
}


QString MainWindow::toBinary(QString hexString)
{
    bool ok;
    int intValue = hexString.toInt(&ok, 16);
    QString binaryString = QString::number(intValue, 2);
    int desiredLength = hexString.length() * 4;
    while (binaryString.length() < desiredLength) {
        binaryString = "0" + binaryString;
    }
    return binaryString;
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString MainWindow::binaryToHex(QString binary)
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

QString MainWindow::OR(QString binary1, QString binary2)
{
    QString result = "";
    for (int i = 0 ; i < binary1.length(); ++i)
        result.append(QString::number(binary1[i].digitValue() | binary2[i].digitValue()));
    return result;
}

QString MainWindow::AND(QString binary1, QString binary2)
{
    QString result = "";
    for (int i = 0 ; i < binary1.length(); ++i)
        result.append(QString::number(binary1[i].digitValue() & binary2[i].digitValue()));
    return result;
}

QString MainWindow::XOR(QString binary1, QString binary2)
{
    return OR(AND(binary1, OneComplement(binary2)), AND(OneComplement(binary1), binary2));
}

QString MainWindow::ShiftToLeft(QString binary)
{
    QString result = binary.mid(1) + ui->E->text();
    ui->E->setText(binary[0]);
    return result;
}

QString MainWindow::ShiftToRight(QString binary)
{
    QString result = ui->E->text() + binary.left(binary.length()-1);
    ui->E->setText(binary.at(binary.length() - 1));
    return result;
}

QMap <QString, int> Labels;
QMap <QString, int> var_labels;
void MainWindow::Fill_Micro_Table(QList<QStringList> wordList)
{
    int currentline = 0;
    ui->microprogram->clear();
    Clear_Micro();
    QStringList newline;
    // Clear additional spaces and lines
    foreach (const QStringList& words, wordList) {
        QString line = words.join(" ");
        newline.append(line);
    }

    ui->microprogram->append(newline.join("\n"));


    Labels.clear(); // Updating labels
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
            QString text = words.at(0);
            text = text.chopped(1);
            ui->Microprogram_Memory->setItem(currentline, 2, new QTableWidgetItem(text));
            ui->Microprogram_Memory->item(currentline, 2)->setTextAlignment(Qt::AlignCenter);
            instruct = words.mid(1).join(", ");
        }
        else
            instruct = words.mid(0).join(", ");
        ui->Microprogram_Memory->setItem(currentline,3,new QTableWidgetItem(instruct));
        ui->Microprogram_Memory->item(currentline, 3)->setTextAlignment(Qt::AlignCenter);

        QStringList cf1 = {} , cf2= {} , cf3 = {} ;
        QString cd = "" , br = "" ,addr = "0000000";
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

        ui->Microprogram_Memory->setItem(currentline,4,new QTableWidgetItem(hx));
        ui->Microprogram_Memory->item(currentline, 4)->setTextAlignment(Qt::AlignCenter);

        currentline ++;
    }
    ui->Microprogram_Memory->resizeColumnsToContents();
    ui->Main_Memory->resizeColumnsToContents();
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

void MainWindow::Clear_Main()
{
    int startColumn = 2;
    int endColumn = 4;

    int rowCount = ui->Main_Memory->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        for (int column = startColumn; column <= endColumn; ++column) {
            QTableWidgetItem* item = ui->Main_Memory->item(row, column);
            if (item) {
                item->setText("");
            }
        }
    }

}

void MainWindow::on_MicroButton_clicked()
{
    reset_colors();
    ui->PC->setText("00000000000");
    ui->CAR->setText("1000000");
    ui->AC->setText("0x0000");
    ui->DR->setText("0x0000");
    ui->AR->setText("00000000000");
    ui->SBR->setText("00000000000");
    ui->E->setText("0");

    QString micro = ui->microprogram->toPlainText();
    QStringList lines = micro.split("\n",Qt::SkipEmptyParts);
    QList<QStringList> wordList;

    foreach (QString line, lines) {
        line.remove(',');
        QStringList lineWords = line.split(" ", Qt::SkipEmptyParts);
        wordList.append(lineWords);
    }

    //Conditions
    int count_end = 0;
    foreach (const QStringList& words, wordList) {
        count_end += words.count("END");
    }
    if(count_end != 1){
        QMessageBox::critical(nullptr, "Error", "MicroProgram just need one <END>...");
        return ;
    }


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

QString binary16ToHex(const QString& binaryString) {
    bool ok;
    int integerValue = binaryString.toInt(&ok, 2);
    QString hexString = QString::number(integerValue, 16);

    if (hexString.length() < 4) {
        hexString = hexString.rightJustified(4, '0');
    }
    return hexString;
}

void MainWindow::Fill_Main_Table(QList<QStringList> wordList)
{
    int currentline = 0;
    ui->mainprogram->clear();
    Clear_Main();
    QStringList newline;
    foreach (const QStringList& words, wordList) {
        QString line = words.join(" ");
        newline.append(line);
    }


    ui->mainprogram->append(newline.join("\n"));
    foreach (const QStringList& words, wordList) {
        QString instruct = "";
        QString content = "";
        if (words.at(0).toUpper() == "END")
            break;
        if (words.at(0).toUpper() == "ORG"){
            currentline = words.at(1).toInt();
            continue;
        }

        //labels
        if (words.at(0).endsWith(",")){
            QString text = words.at(0);
            text = text.chopped(1);
            ui->Main_Memory->setItem(currentline,2,new QTableWidgetItem(text));
            ui->Main_Memory->item(currentline, 2)->setTextAlignment(Qt::AlignCenter);
            instruct = words.mid(1).join(", ");
        }
        else
            instruct = words.mid(0).join(", ");

        //instructs
        ui->Main_Memory->setItem(currentline,3,new QTableWidgetItem(instruct));
        ui->Main_Memory->item(currentline, 3)->setTextAlignment(Qt::AlignCenter);

        //contents
        QString mic_lable = "";
        QString hexed = "";
        QString I = "0" ;
        QString addr = "";
        bool b = 1;
        for (int i = 0; i < words.length(); ++i) {
            if(Labels.contains(words[i]))
                mic_lable = QString("%1").arg(Labels[words[i]]/4, 4, 2, QChar('0'));
            else if(words[i] == "HEX" )
            {
                hexed = I+ words[i+1].rightJustified(3, QChar('0'));
                ui->Main_Memory->setItem(currentline,4,new QTableWidgetItem("0x"+hexed.toUpper()));
                ui->Main_Memory->item(currentline, 4)->setTextAlignment(Qt::AlignCenter);
                b = 0;
                break;
            }
            else if(words[i] == "DEC")
            {
                //check
                hexed = binaryToHex(toBinary(words[i+1].toInt()));
                ui->Main_Memory->setItem(currentline,4,new QTableWidgetItem("0x"+hexed.toUpper()));
                ui->Main_Memory->item(currentline, 4)->setTextAlignment(Qt::AlignCenter);
                b = 0;
                break;
            }
            else if(words[i] == "I")
                I = "1";
            else if(var_labels.contains(words[i]))
            {
                addr = QString("%1").arg(var_labels[words[i]], 11, 2, QChar('0'));
            }
        }
        if(b)
        {
            content = I + mic_lable + hexed + addr;
            ui->Main_Memory->setItem(currentline,4,new QTableWidgetItem("0x"+binary16ToHex(content).toUpper()));
            ui->Main_Memory->item(currentline, 4)->setTextAlignment(Qt::AlignCenter);
        }
        currentline ++;
    }
    ui->Microprogram_Memory->resizeColumnsToContents();
    ui->Main_Memory->resizeColumnsToContents();
}

void MainWindow::on_MainButton_clicked()
{
    on_MicroButton_clicked();
    reset_colors();
    QString main_txt = ui->mainprogram->toPlainText();
    QStringList lines = main_txt.split("\n",Qt::SkipEmptyParts);
    QList<QStringList> wordList;
    var_labels.clear();
    bool endbool = true;
    foreach (QString line, lines) {
        QStringList lineWords = line.split(" ", Qt::SkipEmptyParts);
        wordList.append(lineWords);
        if(lineWords[0].toUpper() == "HLT")
            endbool = false;
    }
    if(endbool){
        ui->Compile->setDisabled(1);
        ui->debug->setDisabled(1);
        QMessageBox::critical(nullptr, "Error", "Main Program must have <HLT>");
        return;
    }
    int currentline = 0;
    foreach (QStringList x, wordList) {
        if(x[0].toUpper() == "HLT"){
            ui->Main_Memory->setItem(currentline,3,new QTableWidgetItem("HLT"));
            ui->Main_Memory->item(currentline, 3)->setTextAlignment(Qt::AlignCenter);
        }
        if(x[0].toUpper() == "ORG")
        {
            currentline = x[1].toInt();
            continue;
        }
        if(x[0].toUpper() == "HLT" || x[0].toUpper() == "HEX")
            continue;
        if(!x[0].endsWith(','))
            if(!Labels.contains(x[0])){
                QMessageBox::critical(nullptr, "Error", QString("MicroProgram doesn't have %1").arg(x[0]));
                return;
            }
        if(x[0].endsWith(','))
        {
            var_labels[x[0].chopped(1)] = currentline;
        }
        currentline ++ ;
    }
    ui->Compile->setEnabled(1);
    ui->debug->setEnabled(1);
    Fill_Main_Table(wordList);
}

QString MainWindow::CARtoContent(QString CAR)
{
    int row = CAR.toInt(nullptr,2);
    QString content = toBinary(ui->Microprogram_Memory->item(row,4)->text());
    ui->Microprogram_Memory->item(row, 4)->setTextAlignment(Qt::AlignCenter);

    return content;
}


void MainWindow::on_debug_clicked()
{

    reset_colors();
    for (int column = 0; column < ui->Microprogram_Memory->columnCount(); ++column)
    {
        ui->Main_Memory->item(ui->PC->text().toInt(nullptr,2),column)->setBackground(QColor("#1f5748"));
    }
    main_color.append(ui->PC->text().toInt(nullptr,2));
    if(ui->Main_Memory->item(ui->PC->text().toInt(nullptr,2),3)->text() == "HLT")
    {
        QMessageBox::warning(nullptr, "Finished", QString("The Program has reached the end... !"));
        return;
    }

    bool isPresent = true;
    int a = ui->PC->text().toInt(nullptr,2)-1;
    QList<int> valuesList = var_labels.values();
    for (const int value : valuesList) {
        if (value == a) {
            isPresent = false;
            break;
        }
    }
    if((ui->PC->text().toInt(nullptr,2) == 0)|| (isPresent && (ui->Main_Memory->item(ui->PC->text().toInt(nullptr,2)-1,4)->text() != "") ))
        run_micro(CARtoContent(ui->CAR->text()));
    else
        ui->PC->setText(SumWithCarry(ui->PC->text(),"00000000001"));
}

void MainWindow::on_Compile_clicked()
{
    while (ui->Main_Memory->item(ui->PC->text().toInt(nullptr,2),3)->text() != "HLT") {
        on_debug_clicked();
    }
}


void MainWindow::on_action_open_triggered()
{
    QFileDialog dialog(nullptr, "Select Text File");

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Text files (*.txt)");

    // Show the file dialog
    if (dialog.exec()) {
        QStringList selectedFiles = dialog.selectedFiles();
        QString filePath = selectedFiles.at(0);
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            file.close();

            if (!fileContent.contains('~')){
                QMessageBox::critical(nullptr, "Error", "The text file is not in a proper format.");
            }
            else{

                QStringList content = fileContent.split("\n~\n");
                ui->microprogram->setText(content[0]);
                ui->mainprogram->setText(content[1]);
            }
        }
        else {
            QMessageBox::critical(nullptr, "Error", "Failed to open the file.");
        }
    }
}

void MainWindow::on_action_save_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save File", ".txt", "Text Files (*.txt)");

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << ui->microprogram->toPlainText() + QString("\n~\n") + ui->mainprogram->toPlainText();
        file.close();
    }
}


void MainWindow::on_Exit_clicked()
{
    exit(0);
}

