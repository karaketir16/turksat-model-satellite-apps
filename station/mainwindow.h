#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <stationtelemetryobject.h>

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

private:
    Ui::MainWindow *ui;
    QTimer tm;
//    QPolygonF points;
//    QwtSymbol *symbol;
//    QwtPlotCurve *curve;
//    QwtPlotGrid *grid;
        int b = 6, c = 7;

public slots:
        void newTelemetryData(Telemetry_Data data);
signals:
        void sendVideo(const char *);
//        void seperateCarrier();
//        void setSeperator(uint8_t);
//        void setEngineThrust(uint8_t);
//        void testThrust(uint8_t);
//        void groundSet(uint8_t);
//        void resetStatus(uint8_t);
        void commandSend(uint8_t, uint8_t);
};
#endif // MAINWINDOW_H
