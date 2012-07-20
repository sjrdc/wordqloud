#ifndef VIEW_H_
#define VIEW_H_

#include <QGraphicsView>
class Canvas;
class QWheelEvent;

class View : public QGraphicsView
{
Q_OBJECT
public:
View(Canvas *canvas, QWidget *parent = 0);

QPointF getCenter() const;
void setCenter(const QPointF& centerPoint);
 
protected:
virtual void wheelEvent(QWheelEvent* event);
virtual void resizeEvent(QResizeEvent* event);

private:
QPointF currentCenterPoint;
};

inline QPointF View::getCenter() const { return currentCenterPoint; }
 
#endif
