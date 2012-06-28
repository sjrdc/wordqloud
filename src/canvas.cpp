#include "canvas.h"
#include "word.h"

void Canvas::addItem(QString s)
{
  Word *w = new Word(s);
  addItem(w);
}

void Canvas::addItem(Word *w)
{
  QGraphicsScene::addItem((QGraphicsItem*)w);
}
