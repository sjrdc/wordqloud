#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "word.h"

Word::Word(QString w)
  : QGraphicsSimpleTextItem(w)
{
  cachedCollision = NULL;
  this->setBoundingRegionGranularity(0.3);
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
  QVector<QRect> r = this->boundingRegion(this->sceneTransform()).rects();
  QVector<QRect> q = w->boundingRegion(w->sceneTransform()).rects();

  for (int i = 0; i < r.size(); ++i)
    for (int j = 0; j < q.size(); ++j)
      if (r[i].intersects(q[j])) return true;

  return false;

  // return collidesWithItem((QGraphicsSimpleTextItem*)w);
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
