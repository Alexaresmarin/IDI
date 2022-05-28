// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void initializeGL();
    virtual void carregaShaders();
    virtual void modelTransformPatricio();
    void modelTransformFocus();
  private:
    int printOglError(const char file[], int line, const char func[]);
    float angle;
    bool camera, escena, pat;
    GLuint colFocusCam;
    GLuint posFocusPat, colFocusPat;
    GLuint posFocusEsc, colFocusEsc;
    GLuint transFocusLoc;
    glm::mat4 TGaux;
};
