#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

#include <QFileDialog>

#include <QProcess>

#define VIDEO_PATH "../video/"

extern uint8_t RSSI;

void MainWindow::plotConfig(PlotStruct &strc){
    strc.grid = new QwtPlotGrid();
    strc.grid->attach(strc.ptr);

    strc.curve = new QwtPlotCurve();

    strc.curve->attach( strc.ptr );
    strc.curve->setPen( Qt::blue, 2 );
    strc.curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    strc.symbol = new QwtSymbol( QwtSymbol::Ellipse,
                             QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 2, 2 ) );
//    strc.curve->setSymbol( strc.symbol );
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->centralwidget->b`
//    ui->qwtPlot->ins

    simpixmap = *(ui->simulationTest->pixmap());

    heightStruct.ptr = ui->heightPlot;
    tempStruct.ptr = ui->tempPlot;
    longitudeStruct.ptr = ui->longitudePlot;
    latitudeStruct.ptr = ui->latitudePlot;
    altitudeStruct.ptr = ui->alttudePlot;
    speedStruct.ptr = ui->speedPlot;
    voltageStruct.ptr = ui->voltagePlot;
    pressureStruct.ptr = ui->pressurePlot;



    heightStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    heightStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Yükseklik(metre)");

    tempStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    tempStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Sıcaklık(°C)");

    longitudeStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    longitudeStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Enlem");

    latitudeStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    latitudeStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Boylam");

    altitudeStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    altitudeStruct.ptr->setAxisTitle( QwtPlot::yLeft, "GPS Rakım");

    speedStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    speedStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Hız(m/s)");

    voltageStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    voltageStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Gerilim(V)");

    pressureStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    pressureStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Basınç(hPa)");



    plotConfig(heightStruct);
    plotConfig(tempStruct);
    plotConfig(longitudeStruct);
    plotConfig(latitudeStruct);
    plotConfig(altitudeStruct);
    plotConfig(speedStruct);
    plotConfig(voltageStruct);
    plotConfig(pressureStruct);


}

float removePrec(float prec, float value){
    float scale = prec;  // i.e. round to nearest one-hundreth
    return (int)(value / scale) * scale;
}

void MainWindow::newTelemetryData(Telemetry_Data data){

//    Q_ASSERT(false);



    auto rePlot = [&](PlotStruct &strc){
        strc.curve->setSamples(strc.points);
        strc.ptr->replot();
    };

    ui->textRoll->setText(QString::number(data.roll));
    ui->textPitch->setText(QString::number(data.pitch));
    ui->textYaw->setText(QString::number(data.yaw));

    ui->textDate->setText(QString::number(data.day) + "/" + QString::number(data.month) + "/" + QString::number(data.year));
    ui->textTeamNo->setText(QString::number(data.team_no));
    ui->textTime->setText(QString::number(data.hour) + ":" + QString::number(data.minute) + ":" + QString::number(data.second));
    ui->textPackageNo->setText(QString::number(data.package_number));


    heightStruct.points << QPointF(data.package_number, data.height);
    rePlot(heightStruct);
    ui->textHeight->setText(QString::number(data.height));

    tempStruct.points << QPointF(data.package_number, data.temperature);
    rePlot(tempStruct);
    ui->textTemp->setText(QString::number(data.temperature, 'g', 2));

    if(data.GPS_fix){
        longitudeStruct.points << QPointF(data.package_number, removePrec(0.001, data.gps_longtitude));
        rePlot(longitudeStruct);
        ui->textLong->setText(QString::number(data.gps_longtitude, 'g', 10));


        latitudeStruct.points << QPointF(data.package_number, removePrec(0.001, data.gps_latitude));
        rePlot(latitudeStruct);
        ui->textLat->setText(QString::number(data.gps_latitude, 'g', 10));

        altitudeStruct.points << QPointF(data.package_number, removePrec(0.01, data.gps_altiude));
        rePlot(altitudeStruct);
//        ui->text->setText(QString::number(data.gps_latitude));
    }

    speedStruct.points << QPointF(data.package_number, data.speed);
    rePlot(speedStruct);
    ui->textSpeed->setText(QString::number(data.speed));


    voltageStruct.points << QPointF(data.package_number, data.voltage);
    rePlot(voltageStruct);
    ui->textVoltage->setText(QString::number(data.voltage));

    pressureStruct.points << QPointF(data.package_number, data.pressure);
    rePlot(pressureStruct);
    ui->textPressure->setText(QString::number(data.pressure));

    ui->progressBar->setValue( 100 * data.lastNotReceivedVideo / stationTelemetryObject->videoPartCount);

    ui->signalLevel->setText("-" + QString::number(RSSI) + "dBm");

    ui->textSatelliteStatus->setText(Status_Text[data.status]);

    ui->textRotation->setText(QString::number(data.rotation_count));


    QTransform trans;
    trans.rotate(data.roll);

////    label_image->setPixmap(pm.transformed(trans));
////    QMatrix rm;
////    rm.rotate(45);

//        auto pixmap = ui->simulationTest->pixmap();

    ui->simulationTest->setPixmap(simpixmap.transformed(trans));

//    ui->signalLevel->

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_fileSelect_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Dosya seçin"), "/home/karaketir16", tr("All Files (*)"));

    ui->videoFilePath->setText(fileName);

    if(fileName != QString(""))
        ui->sendButton->setEnabled(true);
}

void MainWindow::on_sendButton_clicked()
{
    auto tempPath = ui->videoFilePath->text();

    auto tempFileName =  VIDEO_PATH + QFileInfo(QFile(tempPath)).fileName();
    QFile::remove(tempFileName);
    if(tempFileName.right(4) == ".mp4"){
        QProcess::execute("ffmpeg -i " + tempPath + " -vcodec copy -acodec copy -movflags faststart " + tempFileName);
    }
    else{
        QProcess::execute("cp " + tempPath + " " + tempFileName);
    }


//    Q_ASSERT(false);

    stationTelemetryObject->videoPath = tempFileName;
    stationTelemetryObject->videoReady = true;
//    emit sendVideo(ui->videoFilePath->text().toStdString().c_str());

    ui->videosenderText->setText("Video Gönderimi Başladı");
    ui->videosenderText->setEnabled(true);
}


void MainWindow::on_seperateCommand_clicked()
{
    emit commandSend(Command_Enum::SEPERATE_CARRIER, 0);
}

void MainWindow::on_engineSlider_valueChanged(int value)
{
//    emit setEngineThrust(value);
    ui->engineThrustText->setText(QString::number(value));
    emit commandSend(Command_Enum::SET_THRUST, value);
//    ui->lcdNumber->set
}

void MainWindow::on_seperatorSlider_valueChanged(int value)
{
//    emit setSeperator(value);
    emit commandSend(Command_Enum::SET_SEPERATOR, value);
}


void MainWindow::on_testThrust_clicked()
{
//    emit testThrust(0);
    emit commandSend(Command_Enum::TEST_THRUST, 0);
}

void MainWindow::on_actionZemin_Belirle_triggered()
{
//    emit groundSet(0);
    emit commandSend(Command_Enum::ALTITUDE_CALIBRATE, 0);
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->engineSlider->setEnabled(arg1);
    ui->testThrust->setEnabled(arg1);
    if(arg1)
        ui->engineThrustText->setText(QString::number(ui->engineSlider->value()));
    else
        ui->engineThrustText->setText("");
}

void MainWindow::on_actionStat_S_f_rla_triggered()
{
//    emit resetStatus(0);
    emit commandSend(Command_Enum::RESET_SATELLITE_STATUS, 0);
}

void MainWindow::on_actionGrafikleri_Temizle_triggered()
{
    heightStruct.points.clear();
    tempStruct.points.clear();
    longitudeStruct.points.clear();
    latitudeStruct.points.clear();
    altitudeStruct.points.clear();
    speedStruct.points.clear();
    voltageStruct.points.clear();
    pressureStruct.points.clear();
}

void MainWindow::on_actionPaket_Numaras_triggered()
{
    //emit commandSend(Command_Enum::RESET_TELEMETRY_NUMBER, 0);
    emit commandSend(Command_Enum::RESET_PACKAGE_NUMBER, 0);
    QTimer::singleShot(1200, [&](){
        on_actionGrafikleri_Temizle_triggered();
    });
}



void MainWindow::on_csvSaveButton_clicked()
{
    emit startCSVsave(ui->csvLineText->text());
    ui->csvSaveText->setText("CSV Kaydı Başladı");
    ui->csvSaveText->setEnabled(true);
}

void MainWindow::on_csvLineText_textChanged(const QString &arg1)
{
    if(arg1 != QString("")){
        ui->csvSaveButton->setEnabled(true);
    }else{
        ui->csvSaveButton->setEnabled(false);
    }
}

void MainWindow::on_actionDVR_Ka_triggered()
{
    emit commandSend(Command_Enum::DVR_BUTTON, 0);
}
