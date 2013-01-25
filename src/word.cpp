#include <cassert>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <math.h>

#include "word.h"
#include "wordpropertydialog.h"

Word::Word(QString string, float b)
  : QGraphicsSimpleTextItem()
{
  assert (b >= 0 && b <= 1);

  setFlags(ItemIsSelectable | ItemIsMovable);
  setAcceptsHoverEvents(true);
 
  this->setText(string);
  this->setBrush(Qt::black);
  this->setBoundingRegionGranularity(b);
  cachedCollision = NULL;
  this->setAcceptDrops(true);

  _showPinnedState = false;
  _pinned = false;
  _manipulated = false; 

  _colourLocked = false;
  _fontLocked = false;
  _orientationLocked = false;
  _fontsizeLocked = false;
  _regionInitialised = false;
}

Word::~Word()
{
}

float Word::area()
{
  if (!_regionInitialised) prepareCollisionDetection();

  float a = 0.;  
  QVector<QRect> regionRects = region.rects();
  foreach (QRect rect, regionRects)
    a += rect.width()*rect.height();

  return a;
}

void Word::cacheCollision(Word *w)
{
  cachedCollision = w;
}

bool Word::collidesWith(Word *w) const
{
  return region.intersects(w->region);
}

bool Word::collidesWithCashed() const
{
  return (cachedCollision != NULL) && 
    this->collidesWith(cachedCollision);
}

QTextStream& Word::fromStream(QTextStream& i)
{
  QString line = i.readLine();
  if (!line.isEmpty())
    {
      // find possible modifiers
      QStringList modifiers = line.split(':', QString::SkipEmptyParts);
      QString word = modifiers[0];
      modifiers.pop_front();
      this->setText(word.replace("\\n", QString(QChar::LineSeparator)));
      foreach (QString modifier, modifiers)
	{
	  char key = modifier[0].toAscii();
	  switch(key)
	    {
	    case '!':
	      {
		this->setFontName(modifier.right(modifier.size() - 1));
		this->lockFont();
		break;
	      }
	    case '@':
	      {
		double angle = modifier.right(modifier.size() - 1).toDouble();
		this->setRotation(angle);
		this->lockOrientation();
		break;
	      }
	    case '#':
	      {
		if (modifier.size() == 7)
		  {
		    this->setColour(QColor(modifier));
		    this->lockColour();
		  }
		break;
	      }
	    case '$':
	      {
		QStringList coordinates = modifier.right(modifier.size() - 1).split(',');
		float x = coordinates[0].toFloat();
		float y = coordinates[1].toFloat();
		this->setPos(QPointF(x, y));
		this->setPinned(true);
		break;
	      }
	    case '%':
	      {
		float size = modifier.right(modifier.size() - 1).toFloat();
		this->setFontsize(size);
		this->lockFontsize();
		break;
	      }
	    default: 
	      {
		break;
	      }
	    }
	}
    }
  return i;
}

QFont Word::getFont() const
{
  return this->font();
}

QString Word::getFontName() const
{
  QFont f = this->font();
  return f.family();
}
void Word::moveBy(float x, float y)
{
  QGraphicsSimpleTextItem::moveBy(round(x), round(y));
  region.translate(QPoint(round(x), round(y)));
}

void Word::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  this->setManipulated(true);
  QGraphicsSimpleTextItem::mousePressEvent(event);
  update();
}

void Word::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QPoint delta = (event->scenePos() - event->lastScenePos()).toPoint();  
  qDebug() << __PRETTY_FUNCTION__ << delta;
  moveBy(delta.x(), delta.y());
  QGraphicsSimpleTextItem::mouseMoveEvent(event);
}

void Word::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  this->setManipulated(false);
  qDebug() << __PRETTY_FUNCTION__;
  QGraphicsSimpleTextItem::mouseReleaseEvent(event);
  update();

  if (event->button() == Qt::RightButton)
    {
      WordPropertyDialog *wpd = new WordPropertyDialog(this);
      wpd->exec();
      delete wpd;
    }
}

void Word::prepareCollisionDetection()
{
  region = this->boundingRegion(this->sceneTransform());
  _regionInitialised = true;
}

void Word::setFontName(QString name)
{
  if (!_fontLocked)
    {
      QFont f = this->font();
      f.setFamily(name);
      this->setFont(f);
      _regionInitialised = false;
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

QTextStream& Word::toStream(QTextStream& o) const
{
  QPointF f = this->pos();
  return  o << this->text() << ":"
	    << "%" << this->font().pointSize() << ":"
	    << this->brush().color().name() << ":"
	    << "@" << this->rotation() << ":"
	    << "!" << this->font().family() << ":"
	    << "$" << f.x() << "," << f.y();
}

void Word::updateCollisionDetection(QPointF delta)
{
  region.translate(delta.toPoint());
  _regionInitialised = true;
}
