#include <QtGui>
#include <QtOpenGL>
#include <assert.h>
#include <math.h>
#include <QMutexLocker>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "renderwidget.h"

RenderWidget::RenderWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    m_mesh(nullptr)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);

    m_mouseX = 0;
    m_mouseY = 0;
    m_cameraAngleX = 30;
    m_cameraAngleY = -312;
    m_cameraDistance = 14.4;
}

RenderWidget::~RenderWidget(void) 
{
}

void RenderWidget::updateMesh(const simpleboolean::Mesh &mesh)
{
    simpleboolean::Mesh *newMesh = new simpleboolean::Mesh(mesh);
    {
        QMutexLocker lock(&m_meshMutex);
        delete m_mesh;
        m_mesh = newMesh;
    }
}

void RenderWidget::initializeGL() 
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

    qglClearColor(QWidget::palette().color(QWidget::backgroundRole()));
    glClearStencil(0);
    glClearDepth(1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat ambientLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat diffuseLight[] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat specularLight[] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    float lightDirection[4] = {0, 0, 1, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);

    glEnable(GL_LIGHT0);

    float shininess = 64.0f;
    float specularColor[] = {1.0, 1.0f, 1.0f, 1.0f};
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void RenderWidget::drawGrid(float size, float step)
{
    glDisable(GL_LIGHTING);

    // x z plane
    glBegin(GL_LINES);
    for (GLfloat i = -size; i <= size; i += step) {
        if (0 == i) {
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(i, 0, size); glVertex3f(i, 0, -size);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(size, 0, i); glVertex3f(-size, 0, i);
        } else {
            glColor3f(0.39, 0.39, 0.39);
            glVertex3f(i, 0, size); glVertex3f(i, 0, -size);
            glVertex3f(size, 0, i); glVertex3f(-size, 0, i);
        }
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void RenderWidget::paintGL() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(0, 0, -m_cameraDistance);
    glRotatef(m_cameraAngleX, 1, 0, 0);
    glRotatef(m_cameraAngleY, 0, 1, 0);

    glColor3f(0, 0, 0);
    
    drawGrid(10, 1);

    glPopMatrix();
}

void RenderWidget::resizeGL(int w, int h) 
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, w/h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RenderWidget::mousePressEvent(QMouseEvent *event) 
{
    m_mouseX = event->x();
    m_mouseY = event->y();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event) 
{
    m_cameraAngleY += (event->x() - m_mouseX);
    m_cameraAngleX += (event->y() - m_mouseY);
    update();
    m_mouseX = event->x();
    m_mouseY = event->y();
}

void RenderWidget::wheelEvent(QWheelEvent *event) 
{
    m_cameraDistance -= event->delta() * 0.01f;
}
