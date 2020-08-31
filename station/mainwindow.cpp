#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

#include <QFileDialog>

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
    altitudeStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Yükseklik");

    speedStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    speedStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Hız(m/s)");

    voltageStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    voltageStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Gerilim(V)");

    pressureStruct.ptr->setAxisTitle( QwtPlot::xBottom, "Zaman(saniye)");
    pressureStruct.ptr->setAxisTitle( QwtPlot::yLeft, "Basınç(hPa)");


//    grid = new QwtPlotGrid();
//    grid->attach( ui->heightPlot );

//    curve = new QwtPlotCurve();
//    curve->setTitle( "Some Points" );
//    curve->setPen( Qt::blue, 2 );
//    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );

//    symbol = new QwtSymbol( QwtSymbol::Ellipse,
//        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 2, 2 ) );
//    curve->setSymbol( symbol );



//    QCamera *camera = new QCamera(this);

//    auto viewfinder = new QCameraViewfinder();
//    viewfinder->show();

//    camera->setViewfinder(viewfinder);

//    auto imageCapture = new QCameraImageCapture(camera);

//    camera->setCaptureMode(QCamera::CaptureStillImage);
//    camera->start();
//    QGraphicsScene scene;
//    QPixmap pixmap(":/nevfeza.png");
//    scene.addPixmap(pixmap);
//    //ui->viewer->setScene(&scene);

//    // add here
//    ui->graphicsView->setScene(&scene);
//    ui->openGLWidget->whatsThis()



//    points << QPointF( 0.0, 4.4 ) << QPointF( 1.0, 3.0 )
//        << QPointF( 2.0, 4.5 ) << QPointF( 3.0, 6.8 )
//        << QPointF( 4.0, 7.9 ) << QPointF( 5.0, 7.1 );
//    curve->setSamples( points );
//    curve->attach( ui->heightPlot );

    plotConfig(heightStruct);
    plotConfig(tempStruct);
    plotConfig(longitudeStruct);
    plotConfig(latitudeStruct);
    plotConfig(altitudeStruct);
    plotConfig(speedStruct);
    plotConfig(voltageStruct);
    plotConfig(pressureStruct);


//    heightStruct.ptr = ui->heightPlot;
//    tempStruct.ptr = ui->tempPlot;
//    longitudeStruct.ptr = ui->longitudePlot;
//    latitudeStruct.ptr = ui->latitudePlot;
//    altitudeStruct.ptr = ui->alttudePlot;
//    speedStruct.ptr = ui->speedPlot;
//    voltageStruct.ptr = ui->voltagePlot;
//    pressureStruct.ptr = ui->pressurePlot;

//    tm.setInterval(1000);

//    connect(&tm, &QTimer::timeout, [&](){
//        heightStruct.points << QPointF( b++, c++);
//        b += rand() % 2 + 1;
////        curve->setSamples(points);
//        heightStruct.curve->setSamples(heightStruct.points);
////        curve->attach(&plot);
//        ui->heightPlot->replot();
////        ui->qwtPlot->show();
//        qDebug() << "TEST";

//    });
//    tm.start();
//    QPixmap pm;
//    ...

//    ui->Image->setPixmap(pixmap);
//     ui->simulationTest->setPixmap(pm.transformed(trans));
}

void MainWindow::newTelemetryData(Telemetry_Data data){
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
    ui->textTemp->setText(QString::number(data.temperature));

    if(data.GPS_fix){
        longitudeStruct.points << QPointF(data.package_number, data.gps_longtitude);
        rePlot(longitudeStruct);
        ui->textLong->setText(QString::number(data.gps_longtitude));


        latitudeStruct.points << QPointF(data.package_number, data.gps_latitude);
        rePlot(latitudeStruct);
        ui->textLat->setText(QString::number(data.gps_latitude));

        altitudeStruct.points << QPointF(data.package_number, data.gps_altiude);
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

    ui->progressBar->setValue(data.videoPercent);

    ui->signalLevel->setText("-" + QString::number(RSSI) + "dBm");


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
        tr("Dosya seçin"), "/home/karaketir16", tr("Video (*.mp4 *.avi)"));

    ui->videoFilePath->setText(fileName);
    ui->sendButton->setEnabled(true);
}

void MainWindow::on_sendButton_clicked()
{
    stationTelemetryObject->videoPath = ui->videoFilePath->text();
    stationTelemetryObject->videoReady = true;
//    emit sendVideo(ui->videoFilePath->text().toStdString().c_str());
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_seperateCommand_clicked()
{
    emit seperateCarrier();
}
