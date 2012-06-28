#include "canvas.h"
#include "word.h"

void Canvas::addItem(QString s)
{
  Word *w = new Word(s);
  addItem(w);
}

void Canvas::addItem(Word *w)
{
  // find out where to place the word

  // finally add the word
  QGraphicsScene::addItem((QGraphicsItem*)w);
}
