#include <QDebug>
#include "word.h"

Word::Word(QString w)
  : QGraphicsSimpleTextItem(w)
{
  initBitmap();
  cachedCollision = NULL;
}

Word::~Word()
{
}

void Word::cacheCollision(Word *w)
{
  cachedCollision = w;
}

bool Word::collidesWith(Word *w)
{
  return collidesWithItem((QGraphicsItem*)w);
}

bool Word::collidesWithCashed()
{
  return (cachedCollision != NULL) && 
    this->collidesWithItem(cachedCollision);
}

void Word::initBitmap()
{
  
}

void Word::setFontSize(float p)
{
  QFont f = this->font();
  f.setPointSizeF(p);
  this->setFont(f);
}
