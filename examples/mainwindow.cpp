#include <QtGui>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "renderwidget.h"

MainWindow::MainWindow(void) 
{
    m_renderWidget = new RenderWidget();
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
