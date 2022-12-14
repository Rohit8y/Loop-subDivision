#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include "mesh/mesh.h"
#include "renderers/meshrenderer.h"
#include "subdivision/loopsubdivider.h"
#include "mainwindow.h";


/**
 * @brief The MainView class represents the main view of the UI. It handles and
 * orchestrates the different renderers.
 */
class MainView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
  Q_OBJECT

 public:
  MainView(QWidget* Parent = nullptr);
  ~MainView() override;

  void updateMatrices();
  void updateUniforms();
  void updateBuffers(Mesh& mesh);
  float angleBetweenVectors(const QVector2D& vec1, const QVector2D& vec2);
  int findClosest(const QVector3D& p, const float maxDist);
  QVector<QVector3D> currentVertices;
  void updateCurrentMesh(QVector<QVector3D> newVertice);
  void resizeGL(int newWidth, int newHeight);



 protected:
  void initializeGL() override;
  void paintGL() override;

  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

 private:
  QVector2D toNormalizedScreenCoordinates(float x, float y);
  QVector3D toNormalizedDeviceCoordinates(int mouse_x, int mouse_y);

  QOpenGLDebugLogger debugLogger;

  // for mouse interactions:
  float scale;
  QVector3D oldVec;
  QQuaternion rotationQuaternion;
  bool dragging;

  MeshRenderer meshRenderer;

  Settings settings;

  LoopSubdivider loopSubdivider;

  // we make mainwindow a friend so it can access settings
  friend class MainWindow;
 private slots:
  void onMessageLogged(QOpenGLDebugMessage Message);
};

#endif  // MAINVIEW_H
