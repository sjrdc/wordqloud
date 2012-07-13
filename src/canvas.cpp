#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include <ctime>
#include "canvas.h"
#include "word.h"

Canvas::Canvas(float w, float h) :   QGraphicsScene(0., 0., w, h)
{ 
  centrepoint = 0.5*QPointF(w, h);
  setBackgroundBrush(Qt::white);

  // // initialise random number generator
  rng.seed(static_cast<unsigned int>(std::time(0)));
  // rng.seed(static_cast<unsigned int>(0));
  cxDistribution = boost::normal_distribution<float>(centrepoint.x(), w*.1);
  cyDistribution = boost::normal_distribution<float>(centrepoint.y(), h*.1);

  cxvarnor = new
    boost::variate_generator<boost::mt19937&, 
  			     boost::normal_distribution<float> >(rng,
  							    cxDistribution);
  cyvarnor = new
    boost::variate_generator<boost::mt19937&, 
  			     boost::normal_distribution<float> >(rng,
								 cyDistribution);
  quadtree.setRootRectangle(sceneRect());
}

Canvas::~Canvas()
{
  delete cxvarnor;
  delete cyvarnor;
}

void Canvas::createLayout()
{
  foreach (Word* w, wordlist)
    layoutWord(w);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Shift)
    highlightPinned();
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Shift)
    highlightPinned();
}

void Canvas::layoutWord(Word *w)
{
  /* find out where to place the word */

  if (!w->getPinned())
    {
      // initial location for word
      float tau = 0;
      short cx = -1, cy = -1;
      while (cx < 10 || cy < 10 || cx > width() - 10 || cy > height() - 10)
	{
	  cx = short(cxvarnor->operator()());
	  cy = short(cyvarnor->operator()());
	}

      QRectF bbox = w->boundingBox();
      QPoint centre = QPoint(cx - bbox.width()/2, cy - bbox.height()/2);
      w->setPos(centre);
      QPoint oldpos(0, 0);
      w->prepareCollisionDetection();
      bool done = false;

      do
	{
	  // get a new location estimate
	  tau += 0.25;
	  float rho = tau;

	  // move Word to a new location
	  QPoint delta = (QPoint(rho*cos(tau), rho*sin(tau)) - oldpos);
	  w->moveBy(delta.x(), delta.y());
	  oldpos += delta;

	  if (!sceneRect().contains(w->boundingBox())) continue;
	  if (boundingRegions.size() > 0)
	    {
	      bool contains = false;
	      foreach(QRegion bound, boundingRegions)
		if (!bound.contains(w->boundingBox().toRect()))
		  {
		    contains = true;
		    break;
		  }
	      if (!contains) continue;
	    }
  
	  // check cashed collision first
	  if (w->collidesWithCashed()) continue;

	  done = true;
	  // query quadtree to find possibly overlapping items
	  QList<IAreaComparable*> l;
	  quadtree.query(w->boundingBox(), l);
	  foreach (IAreaComparable *i, l)
	  {
	      Word* q = (Word*)i;
	      if (q->collidesWith(w)) 
		{
		  w->cacheCollision(q);
		  done = false;
		  break;
		}
	    }	  

	}
      while (!done);
    }
  else w->prepareCollisionDetection();
  
  /* finally add the word */
  QGraphicsScene::addItem((QGraphicsItem*)w);

  // add it to the quadtree as well
  quadtree.insert(w);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    {
      QGraphicsItem *item = itemAt(event->scenePos());
      if (item != NULL)
	{
	  item->grabMouse();
	  ((Word*)item)->toggleManipulated();
	}
    }
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  QGraphicsItem *item = mouseGrabberItem();
  if (item != NULL)
    {
      QPoint delta = (mouseEvent->scenePos() - mouseEvent->lastScenePos()).toPoint();
      item->moveBy(delta.x(), delta.y());
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    {
      QGraphicsItem *item = mouseGrabberItem();  
      if (item != NULL)
	{
	  item->ungrabMouse();
      
	  Word *w = ((Word*)item);
	  ((Word*)item)->toggleManipulated();
	  if (event->modifiers() == Qt::ControlModifier)
	    {
	      w->setPinned(false);
	    }
	  else
	    {
	      w->setPinned(true);
	      wordlist.move(wordlist.indexOf(w), 0);
	    }
	}
    }
}

void Canvas::randomiseOrientations()
{
  boost::mt19937 rng;
  rng.seed(static_cast<unsigned int>(std::time(0)));  
  
  boost::uniform_int<> uni(0, 1);
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > 
    anglepicker(rng, uni);

  foreach (Word *word, wordlist)
    word->setRotation(90*anglepicker());
}

void Canvas::randomiseWordColours(QVector<QColor> colours)
{
  boost::mt19937 colourrng;
  colourrng.seed(static_cast<unsigned int>(std::time(0)));  
  
  boost::uniform_int<> uni(0, colours.size()-1);
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > 
    colourpicker(colourrng, uni);

  foreach (Word *word, wordlist)
    word->setBrush(colours[colourpicker()]);
}

void Canvas::randomiseWordFontFamily(QVector<QString> fontfamilies)
{
  boost::mt19937 colourrng;
  colourrng.seed(static_cast<unsigned int>(std::time(0)));  
  
  boost::uniform_int<> uni(0, fontfamilies.size()-1);
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > 
    colourpicker(colourrng, uni);

  foreach (Word *word, wordlist)
    word->setFontName(fontfamilies[colourpicker()]);
}

void Canvas::reCreateLayout()
{
  quadtree.clearContents();
  foreach (QGraphicsItem *item, items())
    removeItem(item);

  createLayout();
}

void Canvas::setBoundingRegions(QVector<QRegion> b)
{
  boundingRegions.clear();
  boundingRegions = b;

  reCreateLayout();
}

void Canvas::setColors(QColor bcolor, QVector<QRgb> wcolors)
{
  setBackgroundBrush(bcolor);
  wordcolors = wcolors;
}

void Canvas::setWordFont(QFont font)
{
  foreach (Word *word, wordlist)
    word->setFontName(font.family());
}

void Canvas::highlightPinned()
{
  foreach (Word *word, wordlist)
    word->toggleShowPinned();
}
