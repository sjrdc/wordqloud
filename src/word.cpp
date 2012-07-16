#include <cassert>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <math.h>

#include "word.h"

Word::Word(QString string, float b)
  : QGraphicsSimpleTextItem(string)
{
  assert (b >= 0 && b <= 1);
  this->setBrush(Qt::black);
  this->setBoundingRegionGranularity(b);
  cachedCollision = NULL;
  this->setAcceptDrops(true);

  showPinnedState = false;
  pinned = false;
  manipulated = false; 

  _colourLocked = false;
  _fontLocked = false;
  _orientationLocked = false;
  _fontsizeLocked = false;
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

void Word::moveBy(float x, float y)
{
  QGraphicsSimpleTextItem::moveBy(round(x), round(y));
  region.translate(QPoint(round(x), round(y)));
}

void Word::prepareCollisionDetection()
{
  region = this->boundingRegion(this->sceneTransform());
}

void Word::setFontName(QString name)
{
  if (!_fontLocked)
    {
      QFont f = this->font();
      f.setFamily(name);
      this->setFont(f);
    }
}

void Word::setFontsize(float p)
{
  if (!_fontsizeLocked)
    {
      QFont f = this->font();
      f.setPointSizeF(p);
      this->setFont(f);
    }
}

void Word::setRotation(float r)
{
  if (!_orientationLocked)
    QGraphicsSimpleTextItem::setRotation(r);
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
