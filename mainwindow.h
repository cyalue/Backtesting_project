#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "qcustomplot.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void create_graph(QJsonValue jn, QJsonValue cl);
    QVector<QCPGraphData> moving_av( QVector<QCPGraphData> data, int par);

private slots:
    void on_pushButton_clicked();

    void on_comboBox_3_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_comboBox_5_currentTextChanged(const QString &arg1);

    void on_comboBox_7_currentTextChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    // Объявляем график
       // Объявляем графическое полотно
};
#endif // MAINWINDOW_H
