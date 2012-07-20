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

void Canvas::highlightPinned(bool highlight)
{
  foreach (Word *word, wordlist)
    word->showPinned(highlight);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Shift)
    highlightPinned(true);
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Shift)
    highlightPinned(false);
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
	  else if (!sceneRect().contains(w->boundingBox())) continue;
  
	  // check cashed collision first
	  if (w->collidesWithCashed()) continue;

	  /* see whether any of the items already on the canvas collides*/
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

void Canvas::randomiseOrientations(WordOrientation w)
{
  rng.seed(static_cast<unsigned int>(std::time(0)));
  if (w != AnyWordOrientation)
    {
      std::vector<short> angles;
      switch (w)
	{	
	case HorizontalWordOrientation:
	  angles.push_back(0);
	  break;
	case MostlyHorizontalWordOrientation:
	  angles.push_back(0);
	  angles.push_back(0);
	  angles.push_back(90);
	  break;
	case HalfAndHalfWordOrientation:
	  angles.push_back(0);
	  angles.push_back(90);
	  break;
	case MostlyVerticalWordOrientation:
	  angles.push_back(0);
	  angles.push_back(90);
	  angles.push_back(90);
	  break;
	case VerticalWordOrientation:
	  angles.push_back(90);
	  break;
	default: // unknown - treat as horizontal
	  angles.push_back(0);
	  break;
	}
      
      boost::uniform_int<> uni(0, angles.size() - 1);  
      boost::variate_generator<boost::mt19937, boost::uniform_int<> > 
	anglepicker(rng, uni);
      foreach (Word *word, wordlist)
	word->setRotation(angles[anglepicker()]);
    }
  else 
    {  
      boost::uniform_int<> uni(0, 359);
      boost::variate_generator<boost::mt19937, boost::uniform_int<> > 
	anglepicker(rng, uni);
      foreach (Word *word, wordlist)
       word->setRotation(anglepicker());
    }
}

void Canvas::randomiseWordColours(const QVector<QColor> &colourpalet)
{
  boost::mt19937 colourrng;
  colourrng.seed(static_cast<unsigned int>(std::time(0)));  

  boost::uniform_int<> uni(0, colourpalet.size()-1);
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > 
    colourpicker(colourrng, uni);

  foreach (Word *word, wordlist)
    word->setColour(colourpalet[colourpicker()]);
}

void Canvas::randomiseWordFontFamily(const QVector<QString> &fontfamilies)
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

void Canvas::setWordList(WordList l) 
{
  quadtree.clearContents();
  this->clear();
  wordlist = l;
  
  float wordArea = wordlist.area();
  float boundArea = 0.;
  if (boundingRegions.size() != 0)
    foreach (QRegion region, boundingRegions)
      {
	QVector<QRect> rects = region.rects();
	foreach (QRect rect, rects)
	  boundArea += rect.width()*rect.height();
      }
  else 
    {
      QRectF scene = sceneRect();
      boundArea = scene.width()*scene.height();
    }

  float scalefactor = 0.9/(wordArea/boundArea);

  qDebug() << wordArea << boundArea << scalefactor;
  foreach (Word *word, wordlist)  
    word->setScale(scalefactor);
}

void Canvas::unpinAll()
{
  foreach (Word *word, wordlist)
    word->setPinned(false);
}
