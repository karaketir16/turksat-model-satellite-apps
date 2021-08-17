#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <stationtelemetryobject.h>

#include <QWebEngineView>

#define SAVE_FOLDER "/home/izzet/TURKSAT/"

class StationTelemetryObject;

struct PlotStruct {
    QwtPlot *       ptr;
    QwtPlotGrid *    grid;
    QwtPlotCurve *   curve;
    QwtSymbol *      symbol;
    QPolygonF        points;
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class WebEnginePage: public QWebEnginePage{
//    Q_OBJECT
//public:
//    WebEnginePage(QObject *parent = Q_NULLPTR):QWebEnginePage(parent){

//    }
//private Q_SLOTS:
//    void onFeaturePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature){

//        if(feature  == QWebEnginePage::MediaAudioCapture
//                || feature == QWebEnginePage::MediaVideoCapture
//                || feature == QWebEnginePage::MediaAudioVideoCapture)
//            setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
//        else
//            setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
//    }
//};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    PlotStruct heightStruct;
    PlotStruct tempStruct;
    PlotStruct longitudeStruct;
    PlotStruct latitudeStruct;
    PlotStruct altitudeStruct;
    PlotStruct speedStruct;
    PlotStruct voltageStruct;
    PlotStruct pressureStruct;

    QPixmap simpixmap;

    void plotConfig(PlotStruct &strc);

    StationTelemetryObject * stationTelemetryObject;

private slots:
    void on_fileSelect_clicked();
    void on_sendButton_clicked();
    void on_seperateCommand_clicked();
    void on_engineSlider_valueChanged(int value);
    void on_seperatorSlider_valueChanged(int value);
    void on_testThrust_clicked();
    void on_actionZemin_Belirle_triggered();
    void on_checkBox_stateChanged(int arg1);
    void on_actionStat_S_f_rla_triggered();
    void on_actionGrafikleri_Temizle_triggered();
    void on_actionPaket_Numaras_triggered();

    void on_csvSaveButton_clicked();

    void on_csvLineText_textChanged(const QString &arg1);

    void on_actionDVR_Ka_triggered();

    void downloadRequested(QWebEngineDownloadItem* download);

private:
    Ui::MainWindow *ui;
    QTimer tm;
    int b = 6, c = 7;

public slots:
        void newTelemetryData(Telemetry_Data data);
signals:
        void sendVideo(const char *);
        void commandSend(uint8_t, uint8_t);

        void startCSVsave(QString);
};
#endif // MAINWINDOW_H
