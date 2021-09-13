#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

#include <QFileDialog>

#include <QProcess>

#define VIDEO_PATH "../video/"

#include <QWebEngineProfile>

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

    strc.ptr->setAxisMaxMajor(QwtPlot::xBottom, 4);
    strc.ptr->setAxisMaxMinor(QwtPlot::xBottom, 0);
}

void MainWindow::downloadRequested(QWebEngineDownloadItem* download) {
        if (download->savePageFormat() != QWebEngineDownloadItem::UnknownSaveFormat)  {
            qDebug() << "Format: " <<  download->savePageFormat();
            qDebug() << "Path: " << download->path();
//            // If you want to modify something like the default path or the format
//            download->setSavePageFormat(...);
//            download->setPath(...);
            // Check your url to accept/reject the download
            download->accept();
        }
        else {
            auto date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd_MM_yyyy_hh_mm_ss_");
            download->setPath(SAVE_FOLDER + formattedTime + QString("video_kayit.webm"));
            qDebug() << "Path: " << download->path();
            download->accept();
        }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->centralwidget->b`
//    ui->qwtPlot->ins

    auto view = new QWebEngineView(this);

    view->setUrl(QUrl("http://localhost:4001/"));

    auto page = view->page();
    auto profile = page->profile();

    connect(profile, &QWebEngineProfile::downloadRequested,
                    this, &MainWindow::downloadRequested);

    connect(page, &QWebEnginePage::featurePermissionRequested, [page](const QUrl &securityOrigin, QWebEnginePage::Feature feature){

        if(feature  == QWebEnginePage::MediaAudioCapture
                || feature == QWebEnginePage::MediaVideoCapture
                || feature == QWebEnginePage::MediaAudioVideoCapture)
            page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
        else
            page->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
    });

    ui->webly->addWidget(view);


    simpixmap = *(ui->simulationTest->pixmap());

    heightStruct.ptr = ui->heightPlot;
    tempStruct.ptr = ui->tempPlot;
    speedStruct.ptr = ui->speedPlot;
    voltageStruct.ptr = ui->voltagePlot;
    pressureStruct.ptr = ui->pressurePlot;

    heightStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    heightStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Yükseklik(metre)");

    tempStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    tempStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Sıcaklık(°C)");

    speedStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    speedStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Hız(m/s)");

    voltageStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    voltageStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Gerilim(V)");
    voltageStruct.ptr->setAxisMaxMajor(QwtPlot::yLeft, 5);
    voltageStruct.ptr->setAxisMaxMinor(QwtPlot::yLeft, 2);

    pressureStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    pressureStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Basınç(hPa)");


    plotConfig(heightStruct);
    plotConfig(tempStruct);
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
        qDebug() << "FIX";


        ui->textLong->setText(QString::number(data.gps_longtitude, 'g', 10));

        ui->textLat->setText(QString::number(data.gps_latitude, 'g', 10));

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


    if( data.video_check ){
        ui->videosenderText->setText("Video Gönderimi Tamamlandi");
    }




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
        tr("Dosya seçin"), "/home/burhan", tr("All Files (*)"));

    ui->videoFilePath->setText(fileName);

    if(fileName != QString(""))
        ui->sendButton->setEnabled(true);
}

void MainWindow::on_sendButton_clicked()
{
    ui->sendButton->setEnabled(false);

    auto tempPath = ui->videoFilePath->text();

    auto basename = QFileInfo(QFile(tempPath)).baseName();

    auto tempFileName =  VIDEO_PATH + basename + ".mp4" ;

    auto tmptmp = VIDEO_PATH + QString("tmp.mp4");


    QFile::remove(tmptmp);
    QProcess::execute("ffmpeg -i " + tempPath + " -vcodec libx265 -crf 30 " + tmptmp);
    QFile::remove(tempFileName);
    QProcess::execute("ffmpeg -i " + tmptmp + " -vcodec copy -acodec copy -movflags faststart " + tempFileName);


//    auto tempFileName =  VIDEO_PATH + QFileInfo(QFile(tempPath)).fileName();
//    QFile::remove(tempFileName);
//    if(tempFileName.right(4) == ".mp4"){
//        qDebug() << "ffmpeg -i " + tempPath + " -vcodec copy -acodec copy -movflags faststart " + tempFileName;
//        QProcess::execute("ffmpeg -i " + tempPath + " -vcodec copy -acodec copy -movflags faststart " + tempFileName);
//    }
//    else{
//        QProcess::execute("cp " + tempPath + " " + tempFileName);
//    }


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
