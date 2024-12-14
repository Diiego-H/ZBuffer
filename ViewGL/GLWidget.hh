#pragma once

#define GLWIDGET_H

#include <math.h>

#include <QtWidgets>
#include <QGLWidget>
#include <QtOpenGL>
#include <QTime>
#include <QtDebug>
#include <QString>

#include "Controller.hh"

#include "ViewGL/GLShader.hh"

#include "GPUConnections/GPUScene.hh"
#include "GPUConnections/GPUCamera.hh"

#include "VW/Lights/LightFactory.hh"
#include "TemporalVW/Animation.hh"

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

class QGLShaderProgram;


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    GLWidget(const QGLFormat &glf, QWidget *parent=0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:

    void updateObject(shared_ptr<GPUMesh> obj);
    void updateScene();
    void updateSettings();
    void saveImage();
    void saveAnimation();

    void activaColorShader();
    void activaNormalShader();

    void activaGouraudBlinnShader();
    void activaBlinnPhongShader();
    void activaToonShader();

    void activaEnvMapping();
    void activaReflection();
    void activaTransparency();
    void activaIndirectMapping();

    void activaExercici1();
    void activaExercici2();

    void setPerspective(float nearPlane, float farPlane);
    void setLookAt(const QVector3D &eye, const QVector3D &center, const QVector3D& up);
    void setLighting(const QVector3D &lightPos, const QVector3D &Ia, const QVector3D &Id,
                    const QVector3D &Is, const QVector3D &coefs);

    // Acció per activar a cada update del timer
    void setCurrentFrame();

    // Acció per actualitzar el temps del timer
    void updateTime();

    // Acció per actualitzar el tamany del viewport a GPU
    void updateWindowSize(shared_ptr<GPUCamera> camera);


signals:
    void ObsCameraChanged(shared_ptr<GPUCamera> cam);
    void FrustumCameraChanged(shared_ptr<GPUCamera> cam);


protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void setXRotation(int angle);
    void setYRotation(int angle);

    void qNormalizeAngle(double &angle);

    void Zoom (int positiu);
    void saveFrame();
private:

    QTimer *timer;
    QTimer *timerEx2;
    int time;
    unsigned int currentFrame;
    unsigned int currentImage;

    QPoint lastPos;   // per interactuar amb la camera

    // TO DO: Pràctica 2: Fase 2: Per ara nomes es té un parell vertex-fragment
    // i cal estendre-ho a tenir mé parells
    shared_ptr<QGLShaderProgram> program;

    #define NUM_SHADERS 9

    #define DEFAULT_SHADER_INDEX 0
    #define COLOR_SHADER_INDEX 1
    #define NORMAL_SHADER_INDEX 2
    #define GOURAUD_SHADER_INDEX 3
    #define PHONG_SHADER_INDEX 4
    #define CELL_SHADER_INDEX 5
    #define EXERCICI1_SHADER_INDEX 6
    #define EXERCICI2_SHADER_INDEX 7
    #define INDIRECT_TEXTURE_MAPPING_SHADER_INDEX 8

    shared_ptr<QGLShaderProgram> programs[NUM_SHADERS];

    void initShadersGPU();

    void configureCamera();
    void switchShader(shared_ptr<QGLShaderProgram> p);
};

