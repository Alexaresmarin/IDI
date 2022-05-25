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
    glEnable(GL_DEPTH_TEST);

    calculCapsModel();
    calculCapsEscena();
    calculCentreEscena();
    calculRadiEscena();

    iniCamera();
}

void MyGLWidget::paintGL () {
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Activem el VAO per a pintar la caseta 
    glBindVertexArray(VAO);
    modelTransform1();
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
    modelTransform2();
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
    modelTransform3();
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Terra);
    modelTransformTerra();
    // pintem
    glDrawArrays (GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
}

void MyGLWidget::iniCamera() {
    // distancia pot ser valor qualsevol que peremti veure tota lescena
    distancia = radi * 2.0;
    Znear = distancia - radi;
    Zfar = distancia + radi;
    raw = 1.0f;
    FOVini = 2.0 * asin(radi / distancia);
    FOV = FOVini;
    
    left = -radi;
    right = radi;
    bottom = -radi;
    top = radi;

    /*
    OBS = glm::vec3(0,2.0,distancia);
    VRP = centreEscena;
    UP = glm::vec3(0,1,0);
    */

    girTheta = 0.0;    
    girPsi = 0.0;

    projectTransform();
    viewTransform();
}

void MyGLWidget::resizeGL (int w, int h) 
{   
    float rav = float(w) / float(h);

    if (rav >= 1) {
        left = -float(radi*rav);
        right = float(radi*rav);
        bottom = -radi;
        top = radi;
    }
    else {
        bottom = -float(radi/rav);
        top = float(radi/rav);
        left = -radi;
        right = radi;
    }

    raw = rav;
    if (rav < 1.0) FOV = 2.0*atan(tan(FOVini/2.0)/rav);
	
	projectTransform ();
	glViewport(0, 0, w, h);
}

void MyGLWidget::modelTransform1(){
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::rotate(transform, angle, glm::vec3(0.0, 1.0, 0.0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(2., 0., 2.));
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::translate(transform, -centreBase);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform2(){
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::rotate(transform, angle+float(90*M_PI/180), glm::vec3(0.0, 1.0, 0.0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::translate(transform, -centreBase);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform3(){
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::rotate(transform, angle, glm::vec3(0.0, 1.0, 0.0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::translate(transform, glm::vec3(-2., 0., -2.));
    transform = glm::rotate(transform, angle+float(180*M_PI/180), glm::vec3(0.0, 1.0, 0.0));
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::translate(transform, -centreBase);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra() {
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_S: { // escalar a més gran
            scale += 0.05;
            break;
        }
        case Qt::Key_D: { // escalar a més petit
            if(scale > 0.05) scale -= 0.05;
            break;
        }
        case Qt::Key_R: {
            angle += float(M_PI)/4.0f;
            break;
        }
        case Qt::Key_O: {
            if (ortho) ortho = false;
            else ortho = true;
            projectTransform();
            break;
        }
        case Qt::Key_Z: {
            FOV -= 0.1;
            projectTransform();
            break;
        }
        case Qt::Key_X: {
            FOV += 0.1;
            projectTransform();
            break;
        }
        default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
	makeCurrent();
	
	if(e->x() > x_ant) girPsi -= 0.03;
	else if(e->x() < x_ant) girPsi += 0.03;
	
	if(e->y() > y_ant) girTheta -= 0.03;
	else if(e->y() < y_ant) girTheta += 0.03;
	
	x_ant = e->x();
	y_ant = e->y();
	viewTransform();
	update();
}

void MyGLWidget::creaBuffers() {
    // Terra
    glm::vec3 terra[6] = {
        glm::vec3( 2.5, 0.0, -2.5),
        glm::vec3(-2.5, 0.0, -2.5),
        glm::vec3(-2.5, 0.0,  2.5),
        glm::vec3( 2.5, 0.0, -2.5),
        glm::vec3(-2.5, 0.0,  2.5),
        glm::vec3( 2.5, 0.0,  2.5)
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
    m.load("../models/Patricio.obj");
    
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

void MyGLWidget::calculCapsModel() {
    // Càlcul capsa contenidora i valors transformacions inicials
    float minx, miny, minz, maxx, maxy, maxz;
    minx = maxx = m.vertices()[0];
    miny = maxy = m.vertices()[1];
    minz = maxz = m.vertices()[2];

    for (unsigned int i = 3; i < m.vertices().size(); i+=3) {
        if (m.vertices()[i+0] < minx) minx = m.vertices()[i+0];
        if (m.vertices()[i+0] > maxx) maxx = m.vertices()[i+0];
        if (m.vertices()[i+1] < miny) miny = m.vertices()[i+1];
        if (m.vertices()[i+1] > maxy) maxy = m.vertices()[i+1];
        if (m.vertices()[i+2] < minz) minz = m.vertices()[i+2];
        if (m.vertices()[i+2] > maxz) maxz = m.vertices()[i+2];
    }

    escala = 1.0/(maxy-miny);
    centreBase = glm::vec3((minx+maxx)/2, miny, (minz+maxz)/2);
}

void MyGLWidget::calculCapsEscena() {
    maxEscena.x = 2.5;
    maxEscena.y = 4;
    maxEscena.z = 2.5;
    
    minEscena = glm::vec3(-2.5, 0, -2.5); 
}

void MyGLWidget::calculCentreEscena() {
    centreEscena = (minEscena + maxEscena) / glm::vec3(2.0);
}

void MyGLWidget::calculRadiEscena() {
    radi = distance(minEscena, maxEscena)/2;
}

void MyGLWidget::projectTransform() {
    //glm::perspective(FOV, ra, znear, zfar)
    glm::mat4 Proj;
    if (ortho) Proj = glm::ortho(left, right, bottom, top, Znear, Zfar);
    else Proj = glm::perspective(FOV, raw, Znear, Zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
    //glm::lookAt(OBS, VRP, UP)
    /*
    glm::mat4 View = glm::lookAt(OBS, VRP, UP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
    */
    glm::mat4 View(1.0);
	View = glm::translate(View, glm::vec3(0., 0., -distancia));
	View = glm::rotate(View, girTheta, glm::vec3(1., 0., 0.));
	View = glm::rotate(View, -girPsi, glm::vec3(0., 1., 0.));
	View = glm::translate(View, -centreEscena);
	glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}