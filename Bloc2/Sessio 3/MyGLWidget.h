// MyGLWidget.h
#include "BL2GLWidget.h"
#include "../Model/model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();
  
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
    GLuint VAO, VAO_Terra;
    // Internal vars
    bool ortho = false;
    float  scale = 1, escala, angle = 0, FOV, FOVini, raw, Znear, Zfar, left, right, bottom, top;
    float radi, distancia, girPsi, girTheta, x_ant, y_ant;
    glm::vec3 OBS, VRP, UP;
    glm::vec3 minEscena, maxEscena, centreEscena, centreBase;
    Model m;
  
  private:
    int printOglError(const char file[], int line, const char func[]);
};
