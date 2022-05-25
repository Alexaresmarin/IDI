// MyGLWidget.h
#include "BL2GLWidget.h"
#include "../Model/model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();
  
  signals:
    void FOVcanviado(int n);
    void CanvioEscala(int n);
    void CanvioPsi(int n);
    void CanvioTheta(int n);
  
  public slots:
    void RecogerValor(int n);
    void RecogerEscalado(int n);
    void RecogerPsi(int n);
    void RecogerTheta(int n);
    void activarLego();
    void activarOrtho();
    void RecogerRGB(float fr, float fg, float fb);

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void iniCamera();
    virtual void resizeGL(int w, int h);
    virtual void modelTransform1();
    virtual void modelTransform2();
    virtual void modelTransform3();
    virtual void modelTransformTerra();
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void creaBuffers();
    virtual void carregaShaders();
    virtual void calculCapsModel();
    virtual void calculCapsEscena();
    virtual void calculCentreEscena();
    virtual void calculRadiEscena();
    virtual void viewTransform();
    virtual void projectTransform();
    virtual void ColorTerra();

    // attribute locations
    GLuint vertexLoc;
    GLuint vterraLoc;
    GLuint colorLoc;
    GLuint cterraLoc;
    // uniform locations
    GLuint transLoc;
    GLuint projLoc;
    GLuint viewLoc;
    // VAO i VBO names
    GLuint VAO, VAO_Terra, VAO_lego;
    // Internal vars
    bool ortho = false, patricio = true;
    float scale, escala, angle, FOV, FOVini, raw, Znear, Zfar, left, right, bottom, top;
    float radi, distancia, girPsi, girTheta, x_ant, y_ant, r = 0, g = 0, b = 0;
    glm::vec3 OBS, VRP, UP;
    glm::vec3 minEscena, maxEscena, centreEscena, centreBase;
    Model pat, lego;
  
  private:
    int printOglError(const char file[], int line, const char func[]);
};
