#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();


  protected:
    virtual void paintGL();
    virtual void patrTransform();
    virtual void viewTransform();
    virtual void projectTransform();
    virtual void iniCamera();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void rebotaParets();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void creaBuffersPil();
    virtual void tractamentGol();




    float d;
    float FOVini;
    float eX, eY;
    float radiPil;
    glm::vec3 Vmin;
    glm::vec3 Vmax; 
    bool GOL;
    bool cameraPers;


  private:
    int printOglError(const char file[], int line, const char func[]);


    void paret1Transform();
    void paret2Transform();
    void paret3Transform();


};
