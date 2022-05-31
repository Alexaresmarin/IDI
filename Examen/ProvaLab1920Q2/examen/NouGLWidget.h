#include "MyGLWidget.h"

class NouGLWidget : public MyGLWidget
{
 Q_OBJECT
	   
 public:
  NouGLWidget (QWidget *parent=NULL);
  ~NouGLWidget ();

 public slots:
    void Ortho();
    void Pers();
 signals:
    void ActivarOrtho();
    void ActivarPers();

 protected:
  void initializeGL();
  void paintGL ();
  void iniCamera ();
  void iniEscena ();
  void viewTransform ();
  void projectTransform ();
  void modelTransformPatricio ();
  void modelTransformPatricio2 ();
  void modelTransformPatricio3 ();

  
  // keyPressEvent - Es cridat quan es prem una tecla
  void keyPressEvent (QKeyEvent *event);
  
 private:
    float movx;
    GLuint colFocusLoc;
    glm::vec3 colFocus;
    bool magenta, ortho;
};
