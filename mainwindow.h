#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>

#include "mesh/mesh.h"
#include "settings.h"
#include "subdivision/subdivider.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class represents the main window.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

 private slots:
  void on_LoadOBJ_pressed();
  void on_MeshPresetComboBox_currentTextChanged(const QString &meshName);
  void on_SubdivSteps_valueChanged(int value);

  void on_phongShadingCheckBox_toggled(bool checked);

  void on_isophotesCheckBox_toggled(bool checked);

  void on_frequencySteps_valueChanged(int arg1);

  void on_colorStripesComboBox_currentTextChanged(const QString &arg1);

  void on_vertexSelectionCheckBox_toggled(bool checked);

private:
  void importOBJ(const QString &fileName);
  void importOBJVertexSelection(const QString &fileName);

  Ui::MainWindow *ui;
  Subdivider *subdivider;
  QVector<Mesh> meshes;
  Settings settings;
};

#endif  // MAINWINDOW_H
