#include <cassert>
#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "word.h"

Word::Word(QString w, float a, float b)
  : QGraphicsSimpleTextItem(w)
{
  assert (b >= 0 && b <= 1);
  cachedCollision = NULL;
  this->setBoundingRegionGranularity(b);
  this->setRotation(a);
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
  // if collision based on bounding box...
  if (collidesWithItem((QGraphicsSimpleTextItem*)w))
    {
      // find out whether there is collision based on region rectangles
      for (int i = 0; i < regionRects.size(); ++i)
	for (int j = 0; j < w->regionRects.size(); ++j)
	  if (regionRects[i].intersects(w->regionRects[j])) return true;
    }

  return false;
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
  regionRects = this->boundingRegion(this->sceneTransform()).rects();
}

void Word::setFontSize(float p)
{
  QFont f = this->font();
  f.setPointSizeF(p);
  this->setFont(f);
}

void Word::updateCollisionDetection(QPointF delta)
{
  foreach (QRect rect, regionRects)
    moveBy(delta.x(), delta.y());
}

void Word::writeImage()
{
  QImage gimg(boundingRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
  QStyleOptionGraphicsItem opt;
  opt.state = QStyle::State_None;
  QPainter p;
  p.begin(&gimg);
  p.setCompositionMode( QPainter::CompositionMode_Clear);
  p.fillRect(boundingRect(), QBrush( QColor( 0, 0, 0, 255)));
  p.setCompositionMode( QPainter::CompositionMode_Source );
  this->paint(&p, &opt, 0);

  p.end();
  gimg.save(text() + ".png");
}
