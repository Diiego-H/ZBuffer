// Si no faig aquest import de GPUCamera.hh no em detecta
// glUniform1i.
#include "GPUConnections/GPUCamera.hh"
#include "ViewGL/GLWidget.hh"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    setFocusPolicy( Qt::StrongFocus );

}

GLWidget::GLWidget(const QGLFormat &glf, QWidget *parent) : QGLWidget(glf, parent) {

    setFocusPolicy( Qt::StrongFocus );
    auto cameraGPU = make_shared<GPUCamera>(this->size().width(), this->size().height());
    Controller::getInstance()->getSetUp()->setCamera(cameraGPU);
}


GLWidget::~GLWidget() {

}


/* Interacció amb GL i inicialització dels programes a la GPU */

/**
 * @brief GLWidget::initializeGL()
 */
void GLWidget::initializeGL() {

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_RGBA);
    glEnable(GL_DOUBLE);

    initShadersGPU();

    // Inicialitza el timer de l'exercici 2
    timerEx2 = new QTimer(this);
    connect(timerEx2, SIGNAL(timeout()), this, SLOT(updateTime()));

    // Les llums es llegeixen des de json

    // Configuració de la càmera
    configureCamera();

    // passar viewport
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();
    updateWindowSize(camera);
}

/**
 * @brief GLWidget::configureCamera()
 */
void GLWidget::configureCamera() {
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();
    auto scene = Controller::getInstance()->getScene();

    camera->init(this->size().width(), this->size().height(), scene->capsaMinima);
    emit ObsCameraChanged(camera);
    emit FrustumCameraChanged(camera);

    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);
}

/**
 * @brief GLWidget::paintGL()
 */
void GLWidget::paintGL() {

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();
    auto scene = Controller::getInstance()->getScene();

    camera->toGPU(program);
    scene->draw();

    // NOTE: Not changing lighting when varying GUI parameters
}

/**
 * @brief GLWidget::resizeGL()
 */
void GLWidget::resizeGL(int width, int height) {


    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->vp.a = width;
    camera->vp.h = height;
    auto scene = Controller::getInstance()->getScene();
    camera->CalculaWindow(scene->capsaMinima);
    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);
    emit FrustumCameraChanged(camera);
    updateWindowSize(camera);
    updateGL();
}

/**
 * @brief GLWidget::initShadersGPU
 */
void GLWidget::initShadersGPU(){
    GLShader *glshader = new GLShader("://resources/GPUshaders/vshader1.glsl", "://resources/GPUshaders/fshader1.glsl", program);
    if (glshader != nullptr) {
        program->link();
        program->bind();
    }
    programs[DEFAULT_SHADER_INDEX] = program;

    // color shader
    glshader = new GLShader("://resources/GPUshaders/vColorShader.glsl", "://resources/GPUshaders/fColorShader.glsl", programs[COLOR_SHADER_INDEX]);

    // normal shader
    glshader = new GLShader("://resources/GPUshaders/vNormalShader.glsl", "://resources/GPUshaders/fNormalShader.glsl", programs[NORMAL_SHADER_INDEX]);
    
    // gouraud shader
    glshader = new GLShader("://resources/GPUshaders/vGouraudShader.glsl", "://resources/GPUshaders/fGouraudShader.glsl", programs[GOURAUD_SHADER_INDEX]);

    // blinn-phong shader
    glshader = new GLShader("://resources/GPUshaders/vBlinnPhongShader.glsl", "://resources/GPUshaders/fBlinnPhongShader.glsl", programs[PHONG_SHADER_INDEX]);

    // cel shader
    glshader = new GLShader("://resources/GPUshaders/vCellShader.glsl", "://resources/GPUshaders/fCellShader.glsl", programs[CELL_SHADER_INDEX]);

    // exercici1 shader
    glshader = new GLShader("://resources/GPUshaders/vExercici1.glsl", "://resources/GPUshaders/fExercici1.glsl", programs[EXERCICI1_SHADER_INDEX]); 

    // exercici2 shader
    glshader = new GLShader("://resources/GPUshaders/vExercici2.glsl", "://resources/GPUshaders/fExercici2.glsl", programs[EXERCICI2_SHADER_INDEX]);

    // indirect texture mapping shader
    glshader = new GLShader("://resources/GPUshaders/vIndirectMapping.glsl", "://resources/GPUshaders/fIndirectMapping.glsl", programs[INDIRECT_TEXTURE_MAPPING_SHADER_INDEX]);

    // afegir aqui nous shaders ...
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

/** Gestio de les animacions i la gravació d'imatges ***/

void GLWidget::setCurrentFrame(){

    auto scene = Controller::getInstance()->getScene();
    scene->update(currentFrame);
    updateGL();
    this->saveFrame();
    currentFrame++;

    if (currentFrame == MAXFRAMES)
        timer->stop();

}

void GLWidget::updateTime() {
    time++;

    // Send the time to the GPU
    GLint timeId = program->uniformLocation("time");
    glUniform1i(timeId, time);
    updateGL();
}

void GLWidget::updateWindowSize(shared_ptr<GPUCamera> camera) {
    GLint windowId = program->uniformLocation("windowSize");
    vec2 vp(camera->vp.a, camera->vp.h);
    glUniform2fv(windowId, 1, vp);
    updateGL();
}

void GLWidget::saveFrame(){
    this->grabFrameBuffer().save("screen_output_"+QVariant(currentFrame).toString()+".jpg");
}

void GLWidget::saveImage(){
    this->grabFrameBuffer().save("screen_output_"+QVariant(currentImage).toString()+".jpg");
    currentImage++;
}



/** Metodes SLOTS que serveixen al builder per a actualitzar l'escena i els objectes */
void GLWidget::updateObject(shared_ptr<GPUMesh> obj) {

    obj->toGPU(program);
    updateGL();
}

void GLWidget::updateScene() {
    // Control de la nova escena a la GPU
    auto sc = Controller::getInstance()->getScene();
    sc->toGPU(program);
    Controller::getInstance()->setScene(sc);
    // Podeu utilitzar l'altre constructor de Camera per inicialitzar els
    // parametres de la camera.
    auto camera = make_shared<GPUCamera>(this->size().width(), this->size().height());
    Controller::getInstance()->getSetUp()->setCamera(camera);

    camera->actualitzaCamera(sc->capsaMinima);

    emit ObsCameraChanged(camera);
    emit FrustumCameraChanged(camera);

    updateGL();

}

// Actualització del SetUp
void GLWidget::updateSettings() {
    // Enviem les llums a la GPU
    Controller::getInstance()->getSetUp()->allLightsToGPU(program);

    // Configurem l'escena de nou (actualitza la càmera i refresca la imatge)
    updateScene();
}

// Informació a enviar a la GPU quan es canvia de shader
void GLWidget::switchShader(shared_ptr<QGLShaderProgram> p) {
    program = p;
    if (program != nullptr) {
        // Carreguem el nou shader
        program->link();
        program->bind();

        // Send information to GPU
        Controller::getInstance()->getScene()->toGPU(program);
        Controller::getInstance()->getSetUp()->allLightsToGPU(program);

        // Refresh visualization
        updateGL();
    } else {
        qDebug() << "Null Shader";
    }
}

/** Metodes que es criden des dels menús */

void GLWidget::saveAnimation() {
    // Comença el timer de l'animació
    timer = new QTimer(this);
    currentFrame=0;
    currentImage=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(setCurrentFrame()));
    timer->start(1000);
}

void GLWidget::activaColorShader() {
    timerEx2->stop();
    qDebug()<<"Estic a Color Shader";
    switchShader(programs[COLOR_SHADER_INDEX]);
}

void GLWidget::activaNormalShader() {
    timerEx2->stop();
    qDebug()<<"Estic a Normal Shader";
    switchShader(programs[NORMAL_SHADER_INDEX]);
}


void GLWidget::activaGouraudBlinnShader() {
    timerEx2->stop();
    qDebug()<<"Activant Gouraud shader";
    switchShader(programs[GOURAUD_SHADER_INDEX]);
}

void GLWidget::activaBlinnPhongShader() {
    timerEx2->stop();
    qDebug()<<"Estic a Blinn-Phong Shader";
    switchShader(programs[PHONG_SHADER_INDEX]);
}

void GLWidget::activaToonShader() {
    timerEx2->stop();
    qDebug()<<"Estic a Cell Shader";
    switchShader(programs[CELL_SHADER_INDEX]);
}


void GLWidget::activaReflection() {
   //TO DO: Pràctica 2:  implementar a la fase 2
     qDebug()<<"Estic a Reflection";
}

void GLWidget::activaEnvMapping() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Estic a Environmental Mapping";
}

void GLWidget::activaTransparency() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Estic a Transparencia";
}

void GLWidget::activaIndirectMapping(){
    timerEx2->stop();
    qDebug()<<"Estic a Indirect Texture Mapping";
    switchShader(programs[INDIRECT_TEXTURE_MAPPING_SHADER_INDEX]);
}

void GLWidget::activaExercici1() {
    qDebug()<<"Estic a Exercici 1";
    switchShader(programs[EXERCICI1_SHADER_INDEX]);
    updateWindowSize(Controller::getInstance()->getSetUp()->getCamera());

    // podem reutilitzar el mateix timer de ex2
    // Reiniciem el temps a zero
    time = 0;
    // 20 cops per segon (50ms * 20 = 1000ms = 1s)
    timerEx2->start(50);
}

void GLWidget::activaExercici2() {
    qDebug()<<"Estic a Exercici 2";
    switchShader(programs[EXERCICI2_SHADER_INDEX]);
    // Reiniciem el temps a zero
    time = 0;
    // 20 cops per segon (50ms * 20 = 1000ms = 1s)
    timerEx2->start(50);
}

void GLWidget::setPerspective(float nearPlane, float farPlane)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->setFrustum(camera->vfovRadians DEGREES, nearPlane, farPlane, TipProj::PERSPECTIVA);

    updateGL();
}

void GLWidget::setLookAt(const QVector3D &eye, const QVector3D &center, const QVector3D& up)
{
    vec4 lookfrom(eye[0], eye[1], eye[2], 1.0);
    vec4 lookat(center[0], center[1], center[2], 1.0);
    vec4 vup(up[0], up[1], up[2], 1.0);

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->setObservador(lookfrom, lookat, vup);

    updateGL();
}

void GLWidget::setLighting(const QVector3D &lightPos, const QVector3D &Ia, const QVector3D &Id,
                           const QVector3D &Is, const QVector3D &coefs)
{
    vec4 lightPosition(lightPos[0],lightPos[1], lightPos[2], 1.0) ;
    vec3 intensityA( Ia[0], Ia[1], Ia[2]);
    vec3 intensityD( Id[0], Id[1], Id[2]);
    vec3 intensityS( Is[0], Is[1], Is[2]);

    auto lights = Controller::getInstance()->getSetUp()->getLights();
    lights[0]->setIa(intensityA);
    lights[0]->setId(intensityD);
    lights[0]->setIs(intensityS);
    lights[0]->setLightPosition(lightPosition);
    updateGL();
}


/**  Mètodes d'interacció amb el ratolí */

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
           setXRotation(dy);
           setYRotation(dx);
        } else if(lastPos.y()!= event->y()) {// rotar la camera
            setXRotation(dy);
        } else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }

    } else if (event->buttons() & Qt::RightButton) {
       // Zoom: canviar la mida de la window
        if(lastPos.y()> event->y())
             Zoom(-1);
        else
             Zoom(1);
    }

    lastPos = event->pos();
}


void GLWidget::setXRotation(int angle)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    if (angle >0) {
        camera->angX += 5;
    } else if (angle<0)
        camera->angX -= 5;
    qNormalizeAngle(camera->angX);

    camera->rotaCamera();

    emit ObsCameraChanged(camera);

    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    if (angle >0) {
        camera->angY += 5;
    } else if (angle<0)
        camera->angY-= 5;
    qNormalizeAngle(camera->angY);

    camera->rotaCamera();

    emit ObsCameraChanged(camera);

    updateGL();
}

void GLWidget::qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360*16;
    while (angle > 360)
        angle -= 360*16;
}

void GLWidget::Zoom (int positiu) {

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->AmpliaWindow(positiu*(0.005));

    emit FrustumCameraChanged(camera);

    updateGL();
}

