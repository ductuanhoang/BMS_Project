#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QMessageBox>
#include <QJsonObject>
#include "parsermessage.h"
#include "simulate_message.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_serial(new QSerialPort(this))
{
    m_ui->setupUi(this);
    // set default com
    for (int var = 0; var < 100; ++var)
    {
        QString com = "COM";
        com.append(QString::number(var));
        m_ui->comboBox_COM->addItem(com);
    }

    for (size_t i = 0; i < 12; i++)
    {
        m_ui->comboBox_ID->addItem((QString::number(i)));
    }

    this->m_connected = false;
    m_ui->pushButton_Connect->setText("Open");
    m_ui->pushButton_Connect->setStyleSheet("background-color: red");
    connect(&m_serial, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    for (int var = 0; var < MAX_CELLS_SUPPORT; ++var)
    {
        m_battery_data.cell_voltage[var] = 0;
    }
    for (int var = 0; var < MAX_TEMPERATURE_SENSORS; ++var)
    {
        m_battery_data.temperature[var] = 0;
    }

    // simulate config
    m_ui->pushButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

bool MainWindow::SerialConnect(const QString &com)
{
    bool result = false;
    m_serial.setPortName(com);
    m_serial.setBaudRate(QSerialPort::BaudRate::Baud115200);
    m_serial.setDataBits(QSerialPort::DataBits::Data8);
    m_serial.setParity(QSerialPort::Parity::NoParity);
    m_serial.setStopBits(QSerialPort::StopBits::OneStop);
    m_serial.setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    // connect to the COM
    if (m_serial.open(QIODevice::ReadWrite))
    {
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    QMessageBox msg;
    if (this->m_connected == false)
    {
        // connect to COM
        qDebug() << "connect to " + m_ui->comboBox_COM->currentText();
        bool ret = SerialConnect(m_ui->comboBox_COM->currentText());
        if (ret == true)
        {
            this->m_connected = true;
            m_ui->comboBox_COM->setDisabled(true);
            m_ui->pushButton->setDisabled(false);
            m_ui->pushButton_Connect->setStyleSheet("background-color: green");
            m_ui->pushButton_Connect->setText("Close");
            qDebug() << "connected";
        }
        else
        {
            m_ui->pushButton->setDisabled(true);
            m_ui->pushButton_Connect->setText("Open");
            this->m_connected = false;
            qDebug() << "Can't connect to the " + m_ui->comboBox_COM->currentText();
            msg.setText("Can't connect to the " + m_ui->comboBox_COM->currentText());
            msg.exec();
        }
    }
    else
    {
        if (m_serial.isOpen())
            m_serial.close();
        m_ui->comboBox_COM->setDisabled(false);
        m_ui->pushButton_Connect->setText("Open");
        m_ui->pushButton_Connect->setStyleSheet("background-color: red");
        this->m_connected = false;
        battery_struct_t data;
        // call function to clear data label
        for (int var = 0; var < MAX_CELLS_SUPPORT; ++var)
        {
            data.cell_voltage[var] = 0;
        }
        for (int var = 0; var < MAX_TEMPERATURE_SENSORS; ++var)
        {
            data.temperature[var] = 0;
        }
        data.current = 0;
        data.total_voltage = 0;

        msg.setText("disconnect");
        msg.exec();
        for (int var = 0; var < 24; ++var)
        {
            data.pack_id = var;
            addLableMessage(data);
        }
    }
}

void MainWindow::on_widget_destroyed()
{
    QMessageBox msg;
    qDebug() << "close the program";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "close the program";
    if (m_serial.isOpen())
        m_serial.close();
}

void MainWindow::receiveMessage()
{
    QByteArray dataBA = m_serial.readAll();
    bool ret = false;
    ret = parserMessage(dataBA, &m_battery_data);
    if (ret)
        addLableMessage(m_battery_data);
}

void MainWindow::addLableMessage(battery_struct_t data)
{

    if (data.pack_id == 1)
    {
        if (data.status == 0)
            m_ui->label_bat1->setStyleSheet("background-color: red");
        else
            m_ui->label_bat1->setStyleSheet("background-color: green");
        m_ui->pushButton_bat1_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat1_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat1_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat1_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat1_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat1_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat1_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat1_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat1_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat1_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat1_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat1_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat1_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat1_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat1_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_1->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat1_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat1_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat1_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_1->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_1->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 2)
    {
        if (data.status == 0)
            m_ui->label_bat2->setStyleSheet("background-color: red");
        else
            m_ui->label_bat2->setStyleSheet("background-color: green");
        m_ui->pushButton_bat2_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat2_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat2_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat2_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat2_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat2_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat2_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat2_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat2_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat2_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat2_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat2_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat2_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat2_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat2_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_2->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat2_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat2_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat2_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_2->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_2->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 3)
    {
        if (data.status == 0)
            m_ui->label_bat3->setStyleSheet("background-color: red");
        else
            m_ui->label_bat3->setStyleSheet("background-color: green");
        m_ui->pushButton_bat3_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat3_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat3_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat3_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat3_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat3_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat3_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat3_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat3_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat3_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat3_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat3_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat3_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat3_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat3_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_3->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat3_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat3_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat3_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_3->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_3->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 4)
    {
        if (data.status == 0)
            m_ui->label_bat4->setStyleSheet("background-color: red");
        else
            m_ui->label_bat4->setStyleSheet("background-color: green");
        m_ui->pushButton_bat4_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat4_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat4_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat4_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat4_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat4_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat4_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat4_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat4_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat4_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat4_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat4_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat4_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat4_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat4_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_4->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat4_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat4_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat4_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_4->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_4->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 5)
    {
        if (data.status == 0)
            m_ui->label_bat5->setStyleSheet("background-color: red");
        else
            m_ui->label_bat5->setStyleSheet("background-color: green");
        m_ui->pushButton_bat5_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat5_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat5_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat5_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat5_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat5_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat5_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat5_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat5_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat5_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat5_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat5_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat5_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat5_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat5_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_5->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat5_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat5_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat5_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_5->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_5->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 6)
    {
        if (data.status == 0)
            m_ui->label_bat6->setStyleSheet("background-color: red");
        else
            m_ui->label_bat6->setStyleSheet("background-color: green");
        m_ui->pushButton_bat6_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat6_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat6_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat6_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat6_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat6_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat6_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat6_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat6_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat6_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat6_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat6_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat6_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat6_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat6_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_6->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat6_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat6_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat6_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_6->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_6->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 7)
    {
        if (data.status == 0)
            m_ui->label_bat7->setStyleSheet("background-color: red");
        else
            m_ui->label_bat7->setStyleSheet("background-color: green");
        m_ui->pushButton_bat7_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat7_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat7_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat7_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat7_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat7_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat7_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat7_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat7_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat7_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat7_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat7_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat7_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat7_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat7_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_7->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat7_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat7_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat7_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_7->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_7->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 8)
    {
        if (data.status == 0)
            m_ui->label_bat8->setStyleSheet("background-color: red");
        else
            m_ui->label_bat8->setStyleSheet("background-color: green");
        m_ui->pushButton_bat8_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat8_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat8_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat8_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat8_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat8_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat8_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat8_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat8_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat8_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat8_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat8_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat8_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat8_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat8_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_8->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat8_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat8_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat8_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_8->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_8->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 9)
    {
        if (data.status == 0)
            m_ui->label_bat9->setStyleSheet("background-color: red");
        else
            m_ui->label_bat9->setStyleSheet("background-color: green");
        m_ui->pushButton_bat9_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat9_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat9_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat9_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat9_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat9_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat9_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat9_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat9_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat9_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat9_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat9_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat9_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat9_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat9_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_9->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat9_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat9_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat9_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_9->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_9->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 10)
    {
        if (data.status == 0)
            m_ui->label_bat10->setStyleSheet("background-color: red");
        else
            m_ui->label_bat10->setStyleSheet("background-color: green");
        m_ui->pushButton_bat10_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat10_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat10_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat10_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat10_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat10_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat10_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat10_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat10_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat10_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat10_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat10_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat10_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat10_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat10_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_10->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat10_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat10_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat10_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_10->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_10->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 11)
    {
        if (data.status == 0)
            m_ui->label_bat11->setStyleSheet("background-color: red");
        else
            m_ui->label_bat11->setStyleSheet("background-color: green");
        m_ui->pushButton_bat11_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat11_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat11_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat11_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat11_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat11_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat11_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat11_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat11_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat11_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat11_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat11_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat11_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat11_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat11_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_11->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat11_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat11_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat11_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_11->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_11->setText(QString::number(data.soc / 100.0));
    }
    if (data.pack_id == 12)
    {
        if (data.status == 0)
            m_ui->label_bat12->setStyleSheet("background-color: red");
        else
            m_ui->label_bat12->setStyleSheet("background-color: green");

        m_ui->pushButton_bat12_cell_1->setText(QString::number(data.cell_voltage[0] / 100.0));
        m_ui->pushButton_bat12_cell_2->setText(QString::number(data.cell_voltage[1] / 100.0));
        m_ui->pushButton_bat12_cell_3->setText(QString::number(data.cell_voltage[2] / 100.0));
        m_ui->pushButton_bat12_cell_4->setText(QString::number(data.cell_voltage[3] / 100.0));
        m_ui->pushButton_bat12_cell_5->setText(QString::number(data.cell_voltage[4] / 100.0));
        m_ui->pushButton_bat12_cell_6->setText(QString::number(data.cell_voltage[5] / 100.0));
        m_ui->pushButton_bat12_cell_7->setText(QString::number(data.cell_voltage[6] / 100.0));
        m_ui->pushButton_bat12_cell_8->setText(QString::number(data.cell_voltage[7] / 100.0));
        m_ui->pushButton_bat12_cell_9->setText(QString::number(data.cell_voltage[8] / 100.0));
        m_ui->pushButton_bat12_cell_10->setText(QString::number(data.cell_voltage[9] / 100.0));
        m_ui->pushButton_bat12_cell_11->setText(QString::number(data.cell_voltage[10] / 100.0));
        m_ui->pushButton_bat12_cell_12->setText(QString::number(data.cell_voltage[11] / 100.0));
        m_ui->pushButton_bat12_cell_13->setText(QString::number(data.cell_voltage[12] / 100.0));
        m_ui->pushButton_bat12_cell_14->setText(QString::number(data.cell_voltage[13] / 100.0));
        m_ui->pushButton_bat12_cell_15->setText(QString::number(data.cell_voltage[14] / 100.0));

        m_ui->pushButton_total_votage_12->setText(QString::number(data.total_voltage / 100.0));

        m_ui->pushButton_bat12_temperature_1->setText(QString::number(data.temperature[0] / 100.0));
        m_ui->pushButton_bat12_temperature_2->setText(QString::number(data.temperature[1] / 100.0));
        m_ui->pushButton_bat12_temperature_3->setText(QString::number(data.temperature[2] / 100.0));

        m_ui->pushButton_total_current_12->setText(QString::number(data.current / 100.0));
        m_ui->pushButton_soc_12->setText(QString::number(data.soc / 100.0));
    }
}

void MainWindow::clearAllLabel(void)
{
}

/**
 * @brief button start simulation
 *
 */
void MainWindow::on_pushButton_clicked()
{
    simulate_message_t data;
    // get battery ID
    data.battery_id = (m_ui->comboBox_ID->currentText().toFloat());
    qDebug() << "battery_id =  " + m_ui->comboBox_ID->currentText();
    // get voltage
    data.cell_voltage[0] = m_ui->textEdit_cell_1->toPlainText().toFloat();
    data.cell_voltage[1] = m_ui->textEdit_cell_2->toPlainText().toFloat();
    data.cell_voltage[2] = m_ui->textEdit_cell_3->toPlainText().toFloat();
    data.cell_voltage[3] = m_ui->textEdit_cell_4->toPlainText().toFloat();
    data.cell_voltage[4] = m_ui->textEdit_cell_5->toPlainText().toFloat();
    data.cell_voltage[5] = m_ui->textEdit_cell_6->toPlainText().toFloat();
    data.cell_voltage[6] = m_ui->textEdit_cell_7->toPlainText().toFloat();
    data.cell_voltage[7] = m_ui->textEdit_cell_8->toPlainText().toFloat();
    data.cell_voltage[8] = m_ui->textEdit_cell_9->toPlainText().toFloat();
    data.cell_voltage[9] = m_ui->textEdit_cell_10->toPlainText().toFloat();
    data.cell_voltage[10] = m_ui->textEdit_cell_11->toPlainText().toFloat();
    data.cell_voltage[11] = m_ui->textEdit_cell_12->toPlainText().toFloat();
    data.cell_voltage[12] = m_ui->textEdit_cell_12->toPlainText().toFloat();
    data.cell_voltage[13] = m_ui->textEdit_cell_12->toPlainText().toFloat();
    data.cell_voltage[14] = m_ui->textEdit_cell_12->toPlainText().toFloat();
    for (size_t i = 0; i < 12; i++)
    {
        qDebug() << "cell " + QString::number(i + 1) + " = " + QString::number(data.cell_voltage[i]) + " volt";
    }
    // get current
    data.current = m_ui->textEdit_current->toPlainText().toFloat();
    // get temperature
    data.temperature[0] = m_ui->textEdit_temp_1->toPlainText().toFloat();
    data.temperature[1] = m_ui->textEdit_temp_2->toPlainText().toFloat();
    data.temperature[2] = m_ui->textEdit_temp_3->toPlainText().toFloat();
    // get soc
    data.soc = m_ui->textEdit_soc->toPlainText().toFloat();
    // QString jsonMessage = simulate_packet_message(data);
    QString jsonMessage = simulate_packet_non_json_message(data);
    jsonMessage = jsonMessage + "\r\n";
    // Sending data through QSerialPort
    qDebug() << "message: " + jsonMessage;
    m_serial.write(jsonMessage.toUtf8());
    m_serial.waitForBytesWritten();
}
