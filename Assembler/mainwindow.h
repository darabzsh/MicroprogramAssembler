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
    QString toBinary(const QString& Hex);
//    QString toBinary(int dec);
    QString toHex(const QString& hexString);
    QString CompleteBits(QString bits, int Length);

    ~MainWindow();

private slots:
    void on_MicroButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
