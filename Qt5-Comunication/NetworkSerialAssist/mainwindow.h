#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class SerialGui;
//class NetworkGui;
class QTabWidget;
class udpGui;
class tcpClientGui;
class tcpServerGui;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    QTabWidget* m_tabWig;
    SerialGui*  m_serialGui;
    udpGui*     m_udpGui;
    tcpClientGui* m_tcpClientGui;
    tcpServerGui* m_tcpServerGui;
    //NetworkGui* m_networkGui;

};
#endif // MAINWINDOW_H
