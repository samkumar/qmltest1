#ifndef TIMECOMPONENT_H
#define TIMECOMPONENT_H

#include <QQuickFramebufferObject>

class TimeComponent : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    TimeComponent();
    Renderer* createRenderer() const;

signals:

public slots:
};

#endif // TIMECOMPONENT_H
