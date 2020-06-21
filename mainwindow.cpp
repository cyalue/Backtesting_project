#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QJsonDocument>

#include <QVariant>
#include <QDebug>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        ui->comboBox->addItem("ETHUSD");
        ui->comboBox->addItem("XBTUSD");
        for(int i = 1; i <= 31; i++){
            QString num;
           if(i < 10){
          num = "0" + QString::number(i);
           }
           else{
               num =  QString::number(i);
           }
            ui->comboBox_4->addItem(num);
            ui->comboBox_6->addItem(num);
        }



        ui->widget_2->addGraph();
        ui->widget_2->addGraph();
        ui->widget_2->addGraph();
        ui->widget_2->addGraph();
        ui->widget_2->addGraph();

        ui->widget_2->setBackground(QBrush(QColor("#e9edf5")));

        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("yyyy-MM-dd");
        ui->widget_2->xAxis->setTicker(dateTicker);

        QPen GridPen;
        GridPen.setColor(QColor("#ffffff"));
        GridPen.setStyle(Qt::DotLine);
        GridPen.setWidthF(1);
        ui->widget_2->xAxis->grid()->setPen(GridPen);
        ui->widget_2->yAxis->grid()->setPen(GridPen);

        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9);

        ui->widget_2->yAxis->setTickLabelFont(legendFont);
        ui->widget_2->yAxis->setLabelFont(legendFont);
        ui->widget_2->xAxis->setTickLabelFont(legendFont);
        ui->widget_2->xAxis->setLabelFont(legendFont);

        QPen DotPen;
        DotPen.setColor(QColor("#b0c3d4"));
        DotPen.setStyle(Qt::DotLine);
        DotPen.setWidthF(3);
        ui->widget_2->graph(4)->setPen(DotPen);

        DotPen.setColor(QColor(180, 180, 100, 110));
         ui->widget_2->graph(3)->setPen(DotPen);

         DotPen.setStyle(Qt::SolidLine);
         DotPen.setWidthF(3);
         DotPen.setColor(QColor(50, 50, 120, 200));
         ui->widget_2->graph(2)->setPen(DotPen);

         DotPen.setWidthF(2);

          ui->widget_2->xAxis->setBasePen(DotPen);
          ui->widget_2->xAxis->setTickPen(DotPen);
          ui->widget_2->xAxis->setSubTickPen(Qt::NoPen);
          ui->widget_2->yAxis->setBasePen(DotPen);
          ui->widget_2->yAxis->setTickPen(DotPen);
          ui->widget_2->yAxis->setSubTickPen(Qt::NoPen);

          ui->widget_2->addLayer("front");
          ui->widget_2->graph(2)->setLayer("front");
          ui->widget_2->graph(1)->setLayer("front");
          ui->widget_2->graph(0)->setLayer("front");

          ui->widget_2->graph(1)->setLineStyle(QCPGraph::lsNone);
          ui->widget_2->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::red, 10));
          ui->widget_2->graph(0)->setLineStyle(QCPGraph::lsNone);
          ui->widget_2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::cyan, Qt::cyan, 10));

          ui->widget_2->xAxis->setLabel("Date, d");
          ui->widget_2->yAxis->setLabel("Price, USD");

          ui->widget_2->graph(0)->setName("Sell");
          ui->widget_2->graph(1)->setName("Buy");
          ui->widget_2->graph(2)->setName("Price 1d");
          ui->widget_2->graph(3)->setName("MA 10");
          ui->widget_2->graph(4)->setName("MA 20");

          ui->widget_2->legend->setVisible(true);
          ui->widget_2->legend->setBrush(QBrush(QColor(255,255,255,150)));
          ui->widget_2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

          ui->widget_2->legend->setFont(legendFont);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<QCPGraphData> MainWindow::moving_av( QVector<QCPGraphData> data, int par){

    QVector<QCPGraphData> ma;
    for(int i = 0; i < data.size() - par; i++){
        double mean = 0;
        for(int j = 0; j < par; j++){
           mean += data[i + j].value;
        }
        mean /= par;
        QCPGraphData tt;
        tt.key = data[i+par].key;
        tt.value = mean;
        ma.push_back(tt);
    }
    return ma;

}

void MainWindow::create_graph(QJsonValue jn, QJsonValue cl){

    QString aa;
    QString first_date = 0;
    QVector<QCPGraphData> timeData;
    QVector<QCPGraphData> timeData1;
    QVector<QCPGraphData> timeData2;

    if(jn.isArray()){

        QJsonArray ja = jn.toArray(); qDebug() << ja.count();

        for(int i = 0; i < ja.count(); i++){

            QJsonObject subtree = ja.at(i).toObject();
            QString status = subtree.value("title").toString();
            QString date = subtree.value("date").toString();

           // if(i == 0) first_date = date;
           // if(i != 0 && date == first_date) break;

            if(status == "BUY EXECUTED" ) continue;

            aa = subtree.value("price").toString();
            if(aa.isNull()){
                timeData1.pop_back();
                continue;
            }

            double k = aa.toDouble();

            QDateTime dt = QDateTime::fromString(subtree.value("date").toString(), "yyyy-MM-dd");
            QCPGraphData tt;
            tt.key = dt.toTime_t();
            tt.value = k;

            if(status == "SELL EXECUTED"){
                timeData.push_back(tt);
            }
            else{
                timeData1.push_back(tt);
            }

        }
    }

    if(cl.isArray()){
        QJsonArray ja = cl.toArray(); qDebug() << ja.count();

    for(int i = 0; i < ja.count(); i++){

        QJsonObject subtree = ja.at(i).toObject();
        QString date = subtree.value("date").toString();
        aa = subtree.value("price").toString();
        double k = aa.toDouble();
        QDateTime dt = QDateTime::fromString(subtree.value("date").toString(), "yyyy-MM-dd");
        QCPGraphData tt;
        tt.key = dt.toTime_t();
        tt.value = k;
        timeData2.push_back(tt);

    }
}

    ui->widget_2->graph(0)->data()->set(timeData);
    ui->widget_2->graph(1)->data()->set(timeData1);
    ui->widget_2->graph(2)->data()->set(timeData2);
    ui->widget_2->graph(3)->data()->set(moving_av( timeData2, 10));
    ui->widget_2->graph(4)->data()->set(moving_av( timeData2, 20));

    ui->widget_2->xAxis->setRange(timeData2[0].key, timeData2[timeData2.size() - 1].key);

    int min = 999999;
    int max = -1;
    for(int i = 0; i < timeData2.size(); i++){
        if(min > timeData2[i].value) min = timeData2[i].value;
        if(max < timeData2[i].value) max = timeData2[i].value;
    }
    ui->widget_2->yAxis->setRange(min - (max - min)*0.05, max + (max - min)*0.2);

    ui->widget_2->replot();
}

void MainWindow::on_pushButton_clicked()
{
    QString start = ui->comboBox_2->currentText() + "-" + ui->comboBox_3->currentText() + "-" + ui->comboBox_4->currentText();
    QString end = ui->comboBox_5->currentText() + "-" + ui->comboBox_7->currentText() + "-" + ui->comboBox_6->currentText();
    qDebug() << start << end;
    QDateTime dstart = QDateTime::fromString(start, "yyyy-MM-dd");
    QDateTime dend = QDateTime::fromString(end, "yyyy-MM-dd");
    qDebug() << dend.toTime_t() << dstart.toTime_t();
    long long int delta = (int)(dend.toTime_t() - dstart.toTime_t())/3600/24;
    qDebug() << delta;
    if(delta <= 120 && delta >= 50 ){
        QNetworkRequest request(QUrl("http://194.58.120.139:8000/backtest/"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonObject body;

        body.insert("startTime", start);
        body.insert("endTime", end);
        body.insert("symbol", ui->comboBox->currentText());
        body.insert("binSize", "1d");
        body.insert("strategy", "Macrossover");

        QNetworkAccessManager nam;
        QNetworkReply *reply = nam.post(request, QJsonDocument(body).toJson());
        while (!reply->isFinished())
        {
            qApp->processEvents();
        }
       QByteArray response_data = reply->readAll();

       QJsonDocument json = QJsonDocument::fromJson(response_data);
       qDebug() << json;
       if(json.isEmpty()){
           qDebug() << "suckk";
       }

       qDebug() << reply->error();

       QJsonObject jobj = json.object();
       QJsonValue jval = jobj.value("trades");
       QJsonValue closes = jobj.value("closes");
       QString pr = "Profit: " + jobj.value("percent").toString() + "%";
       ui->label_3->setText(pr);
       create_graph(jval, closes);

       delete(reply);
    }

    else if(delta <= 0){
        (new QErrorMessage(this))->showMessage("Дата начала не может быть после даты конца");

    }
    else{
        (new QErrorMessage(this))->showMessage("Интервал времени должен длиться от 50 до 120 дней");
    }

    }








void MainWindow::on_comboBox_3_currentTextChanged(const QString &arg1)
{

    int days = 0;
     switch(arg1.toInt()){
     case 1:
         days = 31;
         break;
     case 2:
         if(ui->comboBox_2->currentText().toInt() % 4 == 0){
             days = 29;
         }
         else {
             days = 28;
         }
         break;
     case 3:
         days = 31;
         break;
     case 4:
         days = 30;
         break;
     case 5:
         days = 31;
         break;
     case 6:
         days = 30;
         break;
     case 7:
         days = 31;
         break;
     case 8:
         days = 31;
         break;
     case 9:
         days = 30;
         break;
     case 10:
         days = 31;
         break;
     case 11:
         days = 30;
         break;
     case 12:
         days = 31;
         break;
     }
     ui->comboBox_4->clear();
     for(int i = 1; i <= days; i++){
         QString num;
        if(i < 10){
       num = "0" + QString::number(i);
        }
        else{
            num =  QString::number(i);
        }
         ui->comboBox_4->addItem(num);
     }

}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox_2->currentText() == "2020"){
        qDebug() << "signal worked";
        for(int i = 12; i > 4; --i) ui->comboBox_3->removeItem(i);
    }
    else{
        ui->comboBox_3->clear();
        for(int i = 1; i <= 12; i++){
             QString num;
            if(i < 10){
           num = "0" + QString::number(i);
            }
            else{
                num =  QString::number(i);
            }
            ui->comboBox_3->addItem(num);
        }
    }
}

void MainWindow::on_comboBox_5_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox_5->currentText() == "2020"){
        qDebug() << "signal worked";
         ui->comboBox_7->clear();
         for(int i = 1; i <= 5; i++){
             QString num;
            if(i < 10){
           num = "0" + QString::number(i);
            }
            else{
                num =  QString::number(i);
            }
             ui->comboBox_7->addItem(num);
         }
    }
    else{
        ui->comboBox_7->clear();
        for(int i = 1; i <= 12; i++){
            QString num;
           if(i < 10){
          num = "0" + QString::number(i);
           }
           else{
               num =  QString::number(i);
           }
            ui->comboBox_7->addItem(num);
        }
    }
}

void MainWindow::on_comboBox_7_currentTextChanged(const QString &arg1)
{
    int days = 0;
     switch(arg1.toInt()){
     case 1:
         days = 31;
         break;
     case 2:
         if(ui->comboBox_5->currentText().toInt() % 4 == 0){
             days = 29;
         }
         else {
             days = 28;
         }
         break;
     case 3:
         days = 31;
         break;
     case 4:
         days = 30;
         break;
     case 5:
         days = 31;
         break;
     case 6:
         days = 30;
         break;
     case 7:
         days = 31;
         break;
     case 8:
         days = 31;
         break;
     case 9:
         days = 30;
         break;
     case 10:
         days = 31;
         break;
     case 11:
         days = 30;
         break;
     case 12:
         days = 31;
         break;
     }
     ui->comboBox_6->clear();
     for(int i = 1; i <= days; i++){
         QString num;
        if(i < 10){
       num = "0" + QString::number(i);
        }
        else{
            num =  QString::number(i);
        }
         ui->comboBox_6->addItem(num);
     }
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox->currentText() == "XBTUSD"){
        qDebug() << "signal worked";
         ui->comboBox_2->clear();
         ui->comboBox_5->clear();
         for(int i = 2016; i <= 2020; i++){
             QString num;
                num =  QString::number(i);
             ui->comboBox_2->addItem(num);
             ui->comboBox_5->addItem(num);
         }
    }
    else if(ui->comboBox->currentText() == "ETHUSD"){
        qDebug() << "signal worked";
         ui->comboBox_2->clear();
         ui->comboBox_5->clear();
         for(int i = 2019; i <= 2020; i++){
             QString num;
                num =  QString::number(i);
                ui->comboBox_2->addItem(num);
                ui->comboBox_5->addItem(num);
         }
    }

}
