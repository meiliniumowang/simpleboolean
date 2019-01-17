#include <QtGui>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QMatrix4x4>
#include <simpleboolean/meshcombiner.h>
#include "mainwindow.h"
#include "testmesh.h"

MainWindow::MainWindow(void) 
{
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(70, QVector3D(0, 1, 1));

    TestMesh testMesh1;
    testMesh1.transform(rotation);
    
    TestMesh testMesh2;
    
    simpleboolean::MeshCombiner combiner;
    combiner.setMeshes(testMesh1.result(), testMesh2.result());
    combiner.combine(simpleboolean::Operation::Union);
    
    Model *model = new Model(combiner.m_debugFirstMeshReTriangulated);

    m_renderWidget = new GLWidget(model);
    int size = QDesktopWidget().availableGeometry(this).size().height() * 0.7;
    resize(QSize(size, size));
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_renderWidget);
    setLayout(mainLayout);
    setWindowTitle(tr("simpleboolean"));
}

void MainWindow::keyPressEvent(QKeyEvent *e) 
{
    if (e->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(e);
    }
}
