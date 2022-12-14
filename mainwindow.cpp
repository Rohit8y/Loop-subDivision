#include "mainwindow.h"

#include "initialization/meshinitializer.h"
#include "initialization/objfile.h"
#include "subdivision/loopsubdivider.h"
#include "ui_mainwindow.h"
#include "settings.h"


/**
 * @brief MainWindow::MainWindow Creates a new Main Window UI.
 * @param parent Qt parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->MeshGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
    ui->IsophotesGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
    ui->RendererGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
}

/**
 * @brief MainWindow::~MainWindow Deconstructs the main window.
 */
MainWindow::~MainWindow() {
    delete ui;
    meshes.clear();
    meshes.squeeze();
}

/**
 * @brief MainWindow::importOBJ Imports an obj file and adds the constructed
 * half-edge to the collection of meshes.
 * @param fileName Path of the .obj file.
 */
void MainWindow::importOBJ(const QString& fileName) {
  OBJFile newModel = OBJFile(fileName);
    meshes.clear();
    meshes.squeeze();

    if (newModel.loadedSuccessfully()) {
        MeshInitializer meshInitializer;
        meshes.append(meshInitializer.constructHalfEdgeMesh(newModel));
        ui->MainDisplay->updateBuffers(meshes[0]);
        ui->MainDisplay->settings.modelLoaded = true;
        ui->MainDisplay->settings.renderBasicModel = true;
        ui->MainDisplay->settings.selectedVertex = -1;
        ui->phongShadingCheckBox->setChecked(false);
        ui->MainDisplay->settings.phongShadingRender = false;
        ui->isophotesCheckBox->setChecked(false);
        ui->MainDisplay->settings.isophotesRender = false;
        ui->vertexSelectionCheckBox->setChecked(false);

    }
    else {
        ui->MainDisplay->settings.modelLoaded = false;
    }

    // handling enabled/disabled options
    if (ui->MainDisplay->settings.phongShadingRender || ui->MainDisplay->settings.isophotesRender){
        ui->MeshGroupBox->setEnabled(true);
    }
    if (ui->MainDisplay->settings.phongShadingRender ){
        ui->vertexSelectionCheckBox->setEnabled(true);
    }
    else{
        ui->vertexSelectionCheckBox->setEnabled(false);
    }

    ui->IsophotesGroupBox->setEnabled(ui->MainDisplay->settings.isophotesRender);
    ui->RendererGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
    ui->SubdivSteps->setValue(0);
    ui->frequencySteps->setValue(0);
    ui->MainDisplay->update();
}

/**
 * @brief MainWindow::importOBJ Imports an obj file and adds the constructed
 * half-edge to the collection of meshes.
 * @param fileName Path of the .obj file.
 */
void MainWindow::importOBJVertexSelection(const QString& fileName) {
  OBJFile newModel = OBJFile(fileName);
    meshes.clear();
    meshes.squeeze();

    if (newModel.loadedSuccessfully()) {
        MeshInitializer meshInitializer;
        meshes.append(meshInitializer.constructHalfEdgeMesh(newModel));
        ui->MainDisplay->updateBuffers(meshes[0]);
        ui->MainDisplay->settings.modelLoaded = true;
    }
    else {
        ui->MainDisplay->settings.modelLoaded = false;
    }
    ui->MainDisplay->update();
}

// Don't worry about adding documentation for the UI-related functions.

void MainWindow::on_LoadOBJ_pressed() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Import OBJ File", "../", tr("Obj Files (*.obj)"));
    importOBJ(filename);
}

void MainWindow::on_MeshPresetComboBox_currentTextChanged(
    const QString& meshName) {
    importOBJ(":/models/" + meshName + ".obj");
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
    Subdivider* subdivider = new LoopSubdivider();
    for (int k = meshes.size() - 1; k < value; k++) {
       meshes.append(subdivider->subdivide(meshes[k]));
    }

    QVector<Vertex> newVertex =  meshes[value].getVertices();
    QVector<QVector3D>newCoords ;
    for (int i =0;i<newVertex.count();i++)
    {
        newCoords.append(newVertex[i].coords);
    }
    ui->MainDisplay->updateCurrentMesh(newCoords);
    ui->MainDisplay->updateBuffers(meshes[value]);
    delete subdivider;
}

void MainWindow::on_phongShadingCheckBox_toggled(bool checkedPhong){

    ui->MainDisplay->settings.phongShadingRender = checkedPhong;

    if (ui->MainDisplay->settings.phongShadingRender || ui->MainDisplay->settings.isophotesRender){
        ui->MeshGroupBox->setEnabled(true);}
    else{
        ui->MeshGroupBox->setEnabled(false);}

    if (checkedPhong ){
        ui->isophotesCheckBox->setEnabled(false);
        ui->vertexSelectionCheckBox->setEnabled(true);}
    else{
        ui->vertexSelectionCheckBox->setChecked(false);
        ui->vertexSelectionCheckBox->setEnabled(false);
        importOBJ(":/models/" + ui->MeshPresetComboBox->currentText() + ".obj");
        ui->isophotesCheckBox->setEnabled(true);}

    update();
}


void MainWindow::on_isophotesCheckBox_toggled(bool checkedIsophote){
    ui->MainDisplay->settings.isophotesRender = checkedIsophote;
    ui->IsophotesGroupBox->setEnabled(checkedIsophote);

    if (ui->MainDisplay->settings.phongShadingRender || ui->MainDisplay->settings.isophotesRender){
        ui->MeshGroupBox->setEnabled(true);}
    else{
        ui->MeshGroupBox->setEnabled(false);}

    if (checkedIsophote){
            ui->phongShadingCheckBox->setEnabled(false);
            ui->MainDisplay->settings.uniformUpdateRequired = true;}
    else{
        ui->phongShadingCheckBox->setEnabled(true);
        importOBJ(":/models/" + ui->MeshPresetComboBox->currentText() + ".obj");}


    int valueSubDivision = ui->SubdivSteps->value();
    ui->MainDisplay->updateBuffers(meshes[valueSubDivision]);
    update();
}
void MainWindow::on_frequencySteps_valueChanged(int freq){
    ui->MainDisplay->settings.frequencyIsophotes = freq;
    ui->MainDisplay->settings.uniformUpdateRequired = true;
    int valueSubDivision = ui->SubdivSteps->value();
    ui->MainDisplay->updateBuffers(meshes[valueSubDivision]);
    update();
}
void MainWindow::on_colorStripesComboBox_currentTextChanged(
    const QString &colorStripes){
    if(colorStripes == "Black & White"){
       ui->MainDisplay->settings.colorStripeCode=0;
    }
    else if(colorStripes == "Red & White"){
        ui->MainDisplay->settings.colorStripeCode=1;
    }
    else if(colorStripes == "Blue & White"){
       ui->MainDisplay->settings.colorStripeCode=2;
    }
    ui->MainDisplay->settings.uniformUpdateRequired = true;
    int valueSubDivision = ui->SubdivSteps->value();
    ui->MainDisplay->updateBuffers(meshes[valueSubDivision]);
    update();
    update();

}
void MainWindow::on_vertexSelectionCheckBox_toggled(bool checked)
{
     ui->MainDisplay->settings.renderVertexSelection = checked;
    if (checked){
     // Render the model to original position
    ui->MainDisplay->scale =1.0f; // Init the scale
    QQuaternion initrotation; // Init rotation
    ui->MainDisplay->settings.wireframeMode = true;
    ui->MainDisplay->rotationQuaternion = initrotation;
    ui->MainDisplay->resizeGL(1031,750); // resize GL
    importOBJVertexSelection(":/models/" + ui->MeshPresetComboBox->currentText() + ".obj"); //Loade model

    int valueSubDivision = ui->SubdivSteps->value();
    on_SubdivSteps_valueChanged(valueSubDivision);

    Mesh newMesh = meshes[valueSubDivision];
    QVector<Vertex> newVertex =  newMesh.getVertices();
    QVector<QVector3D>newCoords ;
     for (int   i =0;i<newVertex.count();i++)
     {
         newCoords.append(newVertex[i].coords);
     }
     ui->MainDisplay->updateCurrentMesh(newCoords);
    }
    else{
       if (ui->MainDisplay->settings.phongShadingRender)
       {
       ui->MainDisplay->settings.selectedVertex = -1;
       int valueSubDivision = ui->SubdivSteps->value();
       ui->MainDisplay->updateBuffers(meshes[valueSubDivision]);
       }
    }
    ui->MainDisplay->paintGL();
    ui->MainDisplay->updateMatrices();
    update();
}

