#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void set_regs();
    void run_micro(QString instruction);

    QString toBinary(QString Hex);          // 4 character hex returns 16 bits binary, 5 returns 20
    QString toBinary(int dec);              // Returns 16 bits
    QString toHex(const QString& binary, int length = 16);
    QString CompleteBits(QString bits, int Length = 16);     // Add the first character (len - binary_len) times at the first string
    QString OneComplement(QString Binary);              // Inputs should be 16 bits
    QString TwoComplement(QString Binary);              // Inputs should be 16 bits
    QString SumWithCarry(const QString& binaryNumber1, const QString& binaryNumber2, bool mode = false);
    QString binaryToHex(QString binary);

    QString OR(QString binary1, QString binary2);       // inputs should be 16 bits
    QString AND(QString binary1, QString binary2);       // inputs should be 16 bits
    QString XOR(QString binary1, QString binary2);       // inputs should be 16 bits

    QString ShiftToLeft(QString binary);                // E will be assign too
    QString ShiftToRight(QString binary);                // E will be assign too
    ~MainWindow();

private slots:
    void on_MicroButton_clicked();
    void Fill_Micro_Table(QList<QStringList> wordList);
    void Clear_Micro();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
