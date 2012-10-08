#include <QWheelEvent>
#include <QResizeEvent>

#include "canvas.h"
#include "view.h"

View::View(Canvas* canvas, QWidget *parent) : QGraphicsView(canvas, parent)
{
}

void View::setCenter(const QPointF& centerPoint) 
{
  // Get the rectangle of the visible area in scene coords
  QRectF visibleArea = mapToScene(rect()).boundingRect();
 
  // Get the scene area
  QRectF sceneBounds = sceneRect();
 
  double boundX = visibleArea.width() / 2.0;
  double boundY = visibleArea.height() / 2.0;
  double boundWidth = sceneBounds.width() - 2.0 * boundX;
  double boundHeight = sceneBounds.height() - 2.0 * boundY;
 
  // The max boundary that the centerPoint can be to
  QRectF bounds(boundX, boundY, boundWidth, boundHeight);
 
  if(bounds.contains(centerPoint))
    // We are within the bounds
    currentCenterPoint = centerPoint;
  else 
    {
      // We need to clamp or use the center of the screen
      if(visibleArea.contains(sceneBounds)) 
	// Use the center of scene ie. we can see the whole scene
	currentCenterPoint = sceneBounds.center();
      else {
	currentCenterPoint = centerPoint;
 
      // We need to clamp the center. The centerPoint is too large
      if(centerPoint.x() > bounds.x() + bounds.width())
	currentCenterPoint.setX(bounds.x() + bounds.width());
      else if(centerPoint.x() < bounds.x())
	currentCenterPoint.setX(bounds.x());
      if(centerPoint.y() > bounds.y() + bounds.height())
	currentCenterPoint.setY(bounds.y() + bounds.height());
      else if(centerPoint.y() < bounds.y())
	currentCenterPoint.setY(bounds.y());
    }
  }
 
  // Update the scrollbars
  centerOn(currentCenterPoint);
}

void View::resizeEvent(QResizeEvent* event) 
{
  // Get the rectangle of the visible area in scene coords
  QRectF visibleArea = mapToScene(rect()).boundingRect();
  setCenter(visibleArea.center());
 
  // Call the subclass resize so the scrollbars are updated correctly
  QGraphicsView::resizeEvent(event);
}

void View::wheelEvent(QWheelEvent *event)
{
    // Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));
 
    // Get the original screen centerpoint
    QPointF screenCenter = getCenter(); 
 
    // Scale the view ie. do the zoom
    double scaleFactor = 1.15; // How fast we zoom
    if(event->delta() > 0)
      // Zoom in
      scale(scaleFactor, scaleFactor);
    else
      // Zooming out
      scale(1.0 / scaleFactor, 1.0 / scaleFactor);
 
    // Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));
 
    // Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;
 
    // Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    setCenter(newCenter);
}
