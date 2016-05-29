#define GL_GLEXT_PROTOTYPES
#include "timecomponent.h"
#include <QQuickFramebufferObject>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>
#include <iostream>

/*
 * Based on the tutorial at https://www.khronos.org/assets/uploads/books/openglr_es_20_programming_guide_sample.pdf.
 *
 */
class TimeRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
private:
    GLuint loadShader(GLenum type, const char* shaderSrc)
    {
        GLuint shader;
        GLint compiled;

        // Create the shader object
        shader = this->glCreateShader(type);

        if (shader == 0)
        {
            std::cout << "shader is 0" << std::endl;
            exit(1);
        }

        this->glShaderSource(shader, 1, &shaderSrc, nullptr);
        this->glCompileShader(shader);
        this->glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            std::cout << "shader failed to compile" << std::endl;
            exit(1);
        }
        std::cout << "shader compiled" << std::endl;

        return shader;
    }

public:
    TimeRenderer(const TimeComponent* tc)
    {
        this->tc = tc;
        this->initializeOpenGLFunctions();

        char vShaderStr[] =
                "attribute vec4 vPosition;      \n"
                "void main()                    \n"
                "{                              \n"
                "    gl_Position = vPosition;   \n"
                "}                              \n";

        char fShaderStr[] =
                "void main()                    \n"
                "{                              \n"
                "    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);   \n"
                "}                              \n";

        GLuint vertexShader = this->loadShader(GL_VERTEX_SHADER, vShaderStr);
        GLuint fragmentShader = this->loadShader(GL_FRAGMENT_SHADER, fShaderStr);
        programObject = glCreateProgram();

        if (programObject == 0) {
            std::cout << "Could not create program object" << std::endl;
            exit(1);
        }

        this->glAttachShader(programObject, vertexShader);
        this->glAttachShader(programObject, fragmentShader);

        this->glBindAttribLocation(programObject,  0, "vPosition");

        this->glLinkProgram(programObject);

        GLint linked;
        this->glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

        if (!linked) {
            std::cout << "Program was not linked" << std::endl;
            exit(1);
        }
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
    }

    void render()
    {
        this->initializeOpenGLFunctions();

        this->glUseProgram(this->programObject);

        int width = this->framebufferObject()->width();
        int height = this->framebufferObject()->height();

        QOpenGLContext* currContext = QOpenGLContext::currentContext();

        std::cout << currContext << std::endl;

        std::cout << "rendering " << width << " " << height << std::endl;

        int side = qMin(width, height);
        this->glViewport((width - side) / 2, (height - side) / 2, side, side);

        this->glClearColor(0.1f, 0.1f, 0.2f, 0.8f);
        this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f};

        this->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
        this->glEnableVertexAttribArray(0);

        this->glDrawArrays(GL_TRIANGLES, 0, 3);

        /*glLoadIdentity();
        glTranslatef(0, 0, -10);

        // Draw
        glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
        glEnd();*/

        this->tc->window()->resetOpenGLState();
    }

    const TimeComponent* tc;
    GLuint programObject;
};

TimeComponent::TimeComponent()
{
}

QQuickFramebufferObject::Renderer* TimeComponent::createRenderer() const
{
    return new TimeRenderer(this);
}
