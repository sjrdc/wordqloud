#include "word.h"

Word::Word(QString w)
  : QGraphicsSimpleTextItem(w)
{
  initBitmap();
}

Word::~Word()
{
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
