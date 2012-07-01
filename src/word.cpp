#include <cassert>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
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
  this->setAcceptDrops(true);
  pinned = false;
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
  return region.intersects(w->region);
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
  region = this->boundingRegion(this->sceneTransform());
}

void Word::setFontName(QString name)
{
  QFont f = this->font();
  f.setFamily(name);
  this->setFont(f);
}

void Word::setFontSize(float p)
{
  QFont f = this->font();
  f.setPointSizeF(p);
  this->setFont(f);
}

void Word::updateCollisionDetection(QPointF delta)
{
  region.translate(delta.toPoint());
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
