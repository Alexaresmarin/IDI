#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void activarOrtho();
    void activarPers();
    void AsignarPatCub(int);
  
  signals:
    void ActivarPers();
    void ActivarOrtho();
    void AsignarPat(int);

  protected:
    virtual void initializeGL();
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();

  private:
    int printOglError(const char file[], int line, const char func[]);
    int esrota;
    bool pintaPat, blanc;
    int rotacio [3];
    float angles [3];
    glm::vec3 colFocus;
};
