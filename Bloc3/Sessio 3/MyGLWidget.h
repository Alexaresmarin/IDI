// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void initializeGL();
    virtual void iniMaterialTerra ();
    virtual void modelTransformPatricio();
    virtual void carregaShaders();

  private:
    int printOglError(const char file[], int line, const char func[]);

    GLuint posFocusELoc, posFocusCLoc, colFocusELoc, colFocusCLoc, colLlumAmbient, fixeLoc;

    glm::vec3 posFocus, colFocusE, colFocusC;

    float XFocus, YFocus;
    bool un, dos, b;
    float x, z;
};
