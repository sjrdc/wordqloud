#include "canvas.h"

void Canvas::addItem(Word *w)
{
  QGraphicsScene::addItem((QGraphicsItem*)w);
}
