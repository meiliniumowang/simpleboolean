#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H
#include <QGLWidget>
#include <QMutex>
#include <simpleboolean/meshdatatype.h>

class RenderWidget : public QGLWidget {
    Q_OBJECT

public:
    RenderWidget(QWidget *parent = 0);
    ~RenderWidget(void);
    void updateMesh(const simpleboolean::Mesh &mesh);

protected:
    void initializeGL(void);
    void paintGL(void);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void drawGrid(float size, float step);

    int m_mouseX;
    int m_mouseY;
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraDistance;
    simpleboolean::Mesh *m_mesh;
    QMutex m_meshMutex;
};

#endif
