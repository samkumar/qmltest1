#ifndef TIMECOMPONENT_H
#define TIMECOMPONENT_H

#include <QQuickFramebufferObject>

class TimeComponent : public QQuickFramebufferObject
{
    Q_OBJECT

public:
    TimeComponent();
    QQuickFramebufferObject::Renderer* createRenderer() const;
    /*void touchEvent(QTouchEvent* event);
    void mouseMoveEvent(QMouseEvent* event);*/

    float zoom = 1.0f;
    float oldzoom = 1.0f;
    int x = 0;

signals:

public slots:
    void updatePosition(int x, int y);
    void updateZoom(double magnitude, bool commit);
};

#endif // TIMECOMPONENT_H
