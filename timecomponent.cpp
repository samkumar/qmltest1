#define GL_GLEXT_PROTOTYPES
#include "timecomponent.h"
#include <QQuickFramebufferObject>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <iostream>

class TimeRenderer : public QQuickFramebufferObject::Renderer
{
public:
    TimeRenderer(const TimeComponent* tc)
    {
        this->tc = tc;

        /*glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);*/
    }

    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size)
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject *item)
    {
        std::cout << "synchronizing" << std::endl;
        //this->tc->window()->resetOpenGLState();
    }

    void render()
    {
        /* Got these three lines from http://vtk.1045678.n5.nabble.com/Integrating-with-Qt-Quick-by-rendering-VTK-to-a-Framebuffer-Object-FBO-td5727165.html */
        glUseProgram(0);          // crucial for the cone to show up at all after the first render
        glDisable(GL_DEPTH_TEST); // depth buffer fighting between the cone and the background without this
        glDisable(GL_BLEND);      // doesn't seem crucial (?) but it is one of the differences that showed up in apitrace analysis


        glClearColor(0.1f, 0.1f, 0.2f, 0.8f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    #ifdef QT_OPENGL_ES_1
        glOrthof(-2, +2, -2, +2, 1.0, 15.0);
    #else
        glOrtho(-2, +2, -2, +2, 1.0, 15.0);
    #endif
        glMatrixMode(GL_MODELVIEW);

        static GLfloat lightPosition[4] = { 0, 0, 10, 1 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        int width = this->framebufferObject()->width();
        int height = this->framebufferObject()->height();

        QOpenGLContext* currContext = QOpenGLContext::currentContext();

        std::cout << currContext << std::endl;

        std::cout << "render " << width << " " << height << std::endl;

        int side = qMin(width, height);
        glViewport((width - side) / 2, (height - side) / 2, side, side);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0, 0, -10);

        // Draw
        glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
        glEnd();

        this->tc->window()->resetOpenGLState();

        /*int width = 1, height = 1;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0, 1.0, 0.0, 0.8);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(width, 0);
        glVertex2f(width, height);
        glVertex2f(0, height);
        glEnd();

        glLineWidth(2.5);
        glColor4f(0.0, 0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(width, height);
        glVertex2f(width, 0);
        glVertex2f(0, height);
        glEnd();

        update();*/
    }

    const TimeComponent* tc;
};

TimeComponent::TimeComponent()
{
}

QQuickFramebufferObject::Renderer* TimeComponent::createRenderer() const
{
    return new TimeRenderer(this);
}
