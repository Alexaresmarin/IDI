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
    BL2GLWidget::initializeGL ();
    creaBuffers();
    projectTransform();
    viewTransform();
    glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL () {
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO);
    modelTransform();
    // pintem
    glDrawArrays (GL_TRIANGLES, 0, m.faces().size()*3);

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Terra);
    modelTransformTerra();
    // pintem
    glDrawArrays (GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
}

void MyGLWidget::modelTransform(){
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::rotate (transform, angle, glm::vec3 (0.0, 1.0, 0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra() {
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_S: { // escalar a més gran
            escala += 0.05;
            break;
        }
        case Qt::Key_D: { // escalar a més petit
            if(escala > 0.05) escala -= 0.05;
            break;
        }
        case Qt::Key_R: {
            angle += float(M_PI)/4.0f;
            break;
        }
        default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::creaBuffers() {
    // Terra
    glm::vec3 terra[6] = {
        glm::vec3( 2.0,  -1.0,  -2.0),
        glm::vec3(-2.0,  -1.0,  -2.0),
        glm::vec3(-2.0, -1.0,  2.0),
        glm::vec3( 2.0,  -1.0,  -2.0),
        glm::vec3(-2.0, -1.0,  2.0),
        glm::vec3( 2.0, -1.0,  2.0)
    };

    glm::vec3 colorTerra[6] = {
        glm::vec3(1,0,0),
        glm::vec3(0,0,1),
        glm::vec3(0,1,0),
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1)
    };

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Terra);
    glBindVertexArray(VAO_Terra);

    GLuint VBOT_pos;
    glGenBuffers(1, &VBOT_pos);

    glBindBuffer(GL_ARRAY_BUFFER, VBOT_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terra), terra, GL_STATIC_DRAW);
    // Activem l'atribut vterraLoc
    glVertexAttribPointer(vterraLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vterraLoc);

    GLuint VBOT_col;
    glGenBuffers(1, &VBOT_col);

    glBindBuffer(GL_ARRAY_BUFFER, VBOT_col);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorTerra), colorTerra, GL_STATIC_DRAW);
    // Activem l'atribut cterraLoc
    glVertexAttribPointer(cterraLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(cterraLoc);


    // Homer
    m.load("../models/HomerProves.obj");
    
    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    GLuint VBO_pos;
    glGenBuffers(1, &VBO_pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, 
                    m.VBO_vertices(), GL_STATIC_DRAW);
    //Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);
    
    GLuint VBO_col;
    glGenBuffers(1, &VBO_col);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_col);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*m.faces().size()*3*3, 
                    m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);	

    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders() {    
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");
    vertexLoc = glGetAttribLocation(program->programId(), "vertex");
    colorLoc = glGetAttribLocation(program->programId(), "color");
    vterraLoc = glGetAttribLocation (program->programId(), "vertex");
    cterraLoc = glGetAttribLocation (program->programId(), "color");
    
}

void MyGLWidget::projectTransform() {
    //glm::perspective(FOV, ra, znear, zfar)
    glm::mat4 Proj = glm::perspective(float(M_PI)/2.0f, 1.0f, 0.2f, 3.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
    //glm::lookAt(OBS, VRP, UP)
    glm::mat4 View = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}