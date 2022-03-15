// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
  protected:
};
