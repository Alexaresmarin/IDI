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
    virtual void modelTransform();
    virtual void modelTransformTerra();
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void creaBuffers ();
    virtual void carregaShaders();
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
    float escala = 1, angle = 0;
    Model m;
  
  private:
    int printOglError(const char file[], int line, const char func[]);
};
