#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "parsermessage.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void DisplayMessage(void);
    bool SerialConnect(const QString &com);
private slots:
    void on_pushButton_Connect_clicked();

    void on_widget_destroyed();
    void closeEvent(QCloseEvent *event);
    void receiveMessage(void);
    void addLableMessage(battery_struct_t data);
    void clearAllLabel(void);
    void on_pushButton_clicked();

private:
    void clearLabelBat_1(void);
    void clearLabelBat_2(void);
    void clearLabelBat_3(void);
    void clearLabelBat_4(void);
    void clearLabelBat_5(void);
    void clearLabelBat_6(void);
    void clearLabelBat_7(void);
    void clearLabelBat_8(void);
    void clearLabelBat_9(void);
    void clearLabelBat_10(void);
    void clearLabelBat_11(void);
    void clearLabelBat_12(void);
    void clearLabelBat_13(void);
    void clearLabelBat_14(void);
    void clearLabelBat_15(void);
    Ui::MainWindow *m_ui;
    battery_struct_t m_battery_data;
    QSerialPort m_serial;
    bool m_connected;
    QString buffer;
};
#endif // MAINWINDOW_H
