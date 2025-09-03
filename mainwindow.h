#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "objeto.h"
#include "camera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnDesenharPonto_clicked();
    void on_btnDesenharLinha_clicked();
    void on_btnDesenharPoligono_clicked();
    void on_btnTransladar_clicked();

    void on_btnEscalar_clicked();

    void on_btnRemover_clicked();

    void on_btnRotacionar_clicked();

    void on_btnDesenharCasa_clicked();
    void on_btnCarregarObj_clicked();
    void on_comboObjetos_currentIndexChanged(int index);
    void on_checkPerspectiva_toggled(bool checked);
    void on_spinFov_valueChanged(double arg1);
    void on_spinAspect_valueChanged(double arg1);
    void on_spinNear_valueChanged(double arg1);
    void on_spinFar_valueChanged(double arg1);
    void on_spinCamX_valueChanged(double arg1);
    void on_spinCamY_valueChanged(double arg1);
    void on_spinCamZ_valueChanged(double arg1);
    void on_spinModo_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QList<ObjetoGrafico*> displayFile;
    Camera camera;
    void atualizarComboBox();
};

#endif // MAINWINDOW_H
