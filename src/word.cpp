#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "word.h"

Word::Word(QString w)
  : QGraphicsSimpleTextItem(w)
{
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
  return collidesWithItem((QGraphicsSimpleTextItem*)w);
}

bool Word::collidesWithCashed()
{
  return (cachedCollision != NULL) && 
    this->collidesWith(cachedCollision);
}

void Word::initBitmap()
{
  bitmap = new QImage(boundingRect().size().toSize(), QImage::Format_Mono);

  QStyleOptionGraphicsItem opt;
  opt.state = QStyle::State_None;
  QPainter p;
  p.begin(bitmap);
  p.setCompositionMode( QPainter::CompositionMode_Clear);
  p.fillRect(boundingRect(), QBrush( QColor( 0, 0, 0, 255)));
  p.setCompositionMode( QPainter::CompositionMode_Source );
  this->paint(&p, &opt, 0);
  p.end();
}

void Word::prepareCollisionDetection()
{
  initBitmap();
}

void Word::setFontSize(float p)
{
  QFont f = this->font();
  f.setPointSizeF(p);
  this->setFont(f);
}

void Word::writeImage()
{
  QImage gimg(boundingRect().size().toSize(), QImage::Format_Mono);
  QStyleOptionGraphicsItem opt;
  opt.state = QStyle::State_None;
  QPainter p3;
  p3.begin(&gimg);
  p3.setCompositionMode( QPainter::CompositionMode_Clear);
  p3.fillRect(boundingRect(), QBrush( QColor( 0, 0, 0, 255)));
  p3.setCompositionMode( QPainter::CompositionMode_Source );
  this->paint(&p3, &opt, 0);
  p3.end();

  gimg.save(text() + ".png");
}
