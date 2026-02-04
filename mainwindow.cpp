#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ponto.h"
#include "linha.h"
#include "poligono.h"
#include "casa.h"
#include "objetoobj.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboObjetos->clear();
    ui->spinSx->setMinimum(0.1);
    ui->spinSx->setMaximum(10.0);
    ui->spinSx->setValue(1.0);

    ui->spinSy->setMinimum(0.1);
    ui->spinSy->setMaximum(10.0);
    ui->spinSy->setValue(1.0);

    ui->spinFov->setValue(camera.getFov());
    ui->spinAspect->setValue(camera.getAspect());
    ui->spinNear->setValue(camera.getNear());
    ui->spinFar->setValue(camera.getFar());
    ui->checkPerspectiva->setChecked(camera.isPerspectiva());
    ui->canvas->setCamera(&camera);
    ui->spinCamX->setValue(camera.getPosition().x());
    ui->spinCamY->setValue(camera.getPosition().y());
    ui->spinCamZ->setValue(camera.getPosition().z());
    ui->spinModo->setValue(0);
    ui->canvas->setControlMode(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnDesenharPonto_clicked()
{
    displayFile.append(new Ponto(QPoint(100, 100)));
    atualizarComboBox();
    ui->canvas->update();
}

void MainWindow::on_btnDesenharLinha_clicked()
{
    displayFile.append(new Linha(QPoint(150, 150), QPoint(200, 200)));
    atualizarComboBox();
    ui->canvas->update();
}

void MainWindow::on_btnDesenharPoligono_clicked()
{
    QList<QPoint> pontos = {QPoint(250, 250), QPoint(300, 200), QPoint(350, 250)};
    displayFile.append(new Poligono(pontos));
    atualizarComboBox();
    ui->canvas->update();
}

void MainWindow::on_btnTransladar_clicked()
{
    int dx = ui->spinDx->value();
    int dy = ui->spinDy->value();
    double dz = ui->spinDz->value();
    int index = ui->comboObjetos->currentIndex();
    if (index >= 0 && index < displayFile.size()) {
        displayFile[index]->transladar(dx, dy);
        if (auto obj3d = dynamic_cast<ObjetoObj*>(displayFile[index])) {
            obj3d->transladarZ(dz);
        }
        ui->canvas->update();
    }
}

void MainWindow::atualizarComboBox()
{
    ui->comboObjetos->clear();
    for (int i = 0; i < displayFile.size(); ++i) {
        ui->comboObjetos->addItem(QString("Objeto %1").arg(i + 1));
    }
    if (!displayFile.isEmpty()) {
        ui->comboObjetos->setCurrentIndex(displayFile.size() - 1);
    }
    ui->canvas->setDisplayFile(&displayFile);
    ui->canvas->setSelectedIndex(ui->comboObjetos->currentIndex());
}

void MainWindow::on_btnEscalar_clicked() {
    int index = ui->comboObjetos->currentIndex();
    if (index < 0 || index >= displayFile.size()) return;

    float sx = ui->spinSx->value();
    float sy = ui->spinSy->value();

    displayFile[index]->escalar(sx, sy);
    ui->canvas->update();
}


void MainWindow::on_btnRemover_clicked()
{
    int index = ui->comboObjetos->currentIndex();
    if (index >= 0 && index < displayFile.size()) {
        delete displayFile[index];
        displayFile.removeAt(index);
        atualizarComboBox();
        ui->canvas->update();
    }
}

void MainWindow::on_btnRotacionar_clicked() {
    int index = ui->comboObjetos->currentIndex();
    if (index < 0 || index >= displayFile.size()) return;

    double angulo = ui->spinAngulo->value();
    int cx = ui->spinCentroX->value();
    int cy = ui->spinCentroY->value();

    displayFile[index]->rotacionar(angulo, cx, cy);
    ui->canvas->update();
}

void MainWindow::on_btnDesenharCasa_clicked() {
    displayFile.append(new Casa(QPoint(100, 250), 100, 100));
    atualizarComboBox();
    ui->canvas->update();
}

void MainWindow::on_btnCarregarObj_clicked() {
    QString file = QFileDialog::getOpenFileName(this, "Abrir OBJ", QString(), "OBJ Files (*.obj)");
    if (file.isEmpty()) return;
    ObjetoObj *obj = new ObjetoObj();
    if (!obj->carregar(file)) {
        delete obj;
        return;
    }
    displayFile.append(obj);
    atualizarComboBox();
    ui->canvas->update();
}

void MainWindow::on_comboObjetos_currentIndexChanged(int index) {
    ui->canvas->setSelectedIndex(index);
}

void MainWindow::on_checkPerspectiva_toggled(bool checked)
{
    camera.setPerspectiva(checked);
    ui->canvas->update();
}

void MainWindow::on_spinFov_valueChanged(double arg1)
{
    camera.setFov(arg1);
    ui->canvas->update();
}

void MainWindow::on_spinAspect_valueChanged(double arg1)
{
    camera.setAspect(arg1);
    ui->canvas->update();
}

void MainWindow::on_spinNear_valueChanged(double arg1)
{
    camera.setNear(arg1);
    ui->canvas->update();
}

void MainWindow::on_spinFar_valueChanged(double arg1)
{
    camera.setFar(arg1);
    ui->canvas->update();
}

void MainWindow::on_spinCamX_valueChanged(double)
{
    QVector3D pos(ui->spinCamX->value(), ui->spinCamY->value(), ui->spinCamZ->value());
    camera.setPosition(pos);
    ui->canvas->update();
}

void MainWindow::on_spinCamY_valueChanged(double)
{
    QVector3D pos(ui->spinCamX->value(), ui->spinCamY->value(), ui->spinCamZ->value());
    camera.setPosition(pos);
    ui->canvas->update();
}

void MainWindow::on_spinCamZ_valueChanged(double)
{
    QVector3D pos(ui->spinCamX->value(), ui->spinCamY->value(), ui->spinCamZ->value());
    camera.setPosition(pos);
    ui->canvas->update();
}

void MainWindow::on_spinModo_valueChanged(int arg1)
{
    ui->canvas->setControlMode(arg1 == 1);
}


