#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void ActivarCamera();
    void DesactivarCamera(); 
    void SetAltura(int);
  
  signals:
    void Activar();
    void Desactivar();
    void ActualitzarAltura(int);

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void initializeGL();
    virtual void modelTransformPatricio ();
    virtual void modelTransformAvio ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void enviaPosFocus ();
    virtual void iniMaterialTerra ();

  private:
    int printOglError(const char file[], int line, const char func[]);
    bool f;
    float girAvio;
    int altura;
};
