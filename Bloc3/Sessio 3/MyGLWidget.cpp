// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL() {
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();  

    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    glEnable(GL_DEPTH_TEST);
    BL3GLWidget::carregaShaders();
    carregaShaders();
    creaBuffersPatricio();
    creaBuffersTerraIParet();

    iniEscena();
    iniCamera();

    x = z = 0;
    un = dos = b = true;

    glm::vec3 posFocusC = glm::vec3(0);
    glUniform3fv(posFocusCLoc, 1, &posFocusC[0]);

    posFocus = glm::vec3(1, 0.5, 1);
    glUniform3fv(posFocusELoc, 1, &posFocus[0]);
    
    glm::vec3 colFocus = glm::vec3(0.8, 0.8, 0.8);
    glUniform3fv(colFocusELoc, 1, &colFocus[0]);
    glUniform3fv(colFocusCLoc, 1, &colFocus[0]);
    
    glm::vec3 llumAmbient = glm::vec3(0.2, 0.2, 0.2);
    glUniform3fv(colLlumAmbient, 1, &llumAmbient[0]);
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_K: {
            posFocus.x -= 0.1;
            glUniform3fv(posFocusELoc, 1, &posFocus[0]);
            break;
        }
        case Qt::Key_L: {
            posFocus.x += 0.1;
            glUniform3fv(posFocusELoc, 1, &posFocus[0]);
            break;
        }
        case Qt::Key_Left: {
            x -= 0.1;
            posFocus.x -= 0.1;
            glUniform3fv(posFocusELoc, 1, &posFocus[0]);
            break;
        }
        case Qt::Key_Right: {
            x += 0.1;
            posFocus.x += 0.1;
            glUniform3fv(posFocusELoc, 1, &posFocus[0]);
            break;
        }
        case Qt::Key_Up: {
            z += 0.1;
            posFocus.z += 0.1;
            glUniform3fv(posFocusELoc, 1, &posFocus[0]);
            break;
        }
        case Qt::Key_Down: {
            z -= 0.1;
            posFocus.z -= 0.1;
            glUniform3fv(posFocusELoc, 1, &posFocus[0]);
            break;
        }
        case Qt::Key_1: {
            if (un) colFocusE = glm::vec3(0.0,0.0,0.0);
            else colFocusE = glm::vec3(0.8,0.8,0.8);

            un = !un;
            glUniform3fv(colFocusELoc,1,&colFocusE[0]); 
            break;
        }
        case Qt::Key_2: {
            if (dos) colFocusC = glm::vec3(0.0,0.0,0.0);
            else colFocusC = glm::vec3(0.8,0.8,0.8);

            dos = !dos;
            glUniform3fv(colFocusCLoc,1,&colFocusC[0]); 
            break;
        }
        case Qt::Key_B: {
            if (b) glDisable(GL_CULL_FACE);
            else glEnable(GL_CULL_FACE);

            b = !b;
            break;
        }
        default: BL3GLWidget::keyPressEvent(event); break;
    }
    update();
}

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::translate(glm::mat4(1.f), glm::vec3(x,-0.85,z));
  TG = glm::scale(TG, glm::vec3(0.15*escala,0.15*escala,0.15*escala));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::iniMaterialTerra()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.2,0,0.2);
  diff = glm::vec3(0,0,1);
  spec = glm::vec3(1,1,1);
  shin = 100;
}

void MyGLWidget::carregaShaders() {
    posFocusELoc = glGetUniformLocation (program->programId(), "posFocusEscena");
    posFocusCLoc = glGetUniformLocation (program->programId(), "posFocusCamera");
    colFocusELoc = glGetUniformLocation (program->programId(), "colorFocusEscena");
    colFocusCLoc = glGetUniformLocation (program->programId(), "colorFocusCamera");
    colLlumAmbient = glGetUniformLocation (program->programId(), "llumAmbient");
}