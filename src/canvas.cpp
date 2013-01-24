#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QTextStream>

#include <ctime>
#include "canvas.moc"
#include "word.h"
#include "wordpropertydialog.h"

Canvas::Canvas(float w, float h) :   QGraphicsScene(0., 0., w, h)
{
  setBackgroundBrush(Qt::white);
  setLayoutPath(CircularPath);
  
  // // initialise random number generator
  rng.seed(static_cast<unsigned int>(std::time(0)));
  angleIncrement = boost::uniform_int<>(1, 25);
  
  centrepoint = 0.5*QPointF(w, h);
  cxDistribution = boost::normal_distribution<float>(centrepoint.x(), w*.5);
  cyDistribution = boost::normal_distribution<float>(centrepoint.y(), h*.5);
  
  cxvarnor = new
    boost::variate_generator<boost::mt19937&, 
  			     boost::normal_distribution<float> >(rng,
								 cxDistribution);
  cyvarnor = new
    boost::variate_generator<boost::mt19937&, 
  			     boost::normal_distribution<float> >(rng,
								 cyDistribution);
  avarnor = new
    boost::variate_generator<boost::mt19937&,
			     boost::uniform_int<> >(rng, angleIncrement);
  
  quadtree.setRootRectangle(sceneRect());

  layoutBusy = false;
  groupSelectMode = false;
}

Canvas::~Canvas()
{
  delete cxvarnor;
  delete cyvarnor;
  delete avarnor;
}

void Canvas::addWordList(WordList l)
{
  wordlist.append(l);
  scaleSceneRect();
}

void Canvas::clearWordList()
{
  quadtree.clearContents();
  this->clear();
  wordlist.clear();
}

void Canvas::createLayout()
{
  quadtree.clearContents();
  foreach (QGraphicsItem *item, items())
    removeItem(item);

  int words = 0;
  int c = 0;
  foreach (Word* w, wordlist)
    {
      boost::this_thread::interruption_point();
      if (layoutWord(w)) ++words;
      emit layoutProgress(c++, wordlist.size());
    }
  
  emit layoutProgress(c++, wordlist.size());
  emit layoutEnded();
  layoutBusy = false;
  qDebug() << wordlist.size() << words;
}

void Canvas::hAlignSelectedWords()
{
  qDebug() << __PRETTY_FUNCTION__;
}

void Canvas::highlightPinned(bool highlight)
{
  foreach (Word *word, wordlist)
    word->showPinned(highlight);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
  if (!layoutBusy)
    {
      if (event->key() == pinKey)
	{
	  highlightPinned(true);
	  update();
	}
      else if (event->key() == groupKey)
	{
	  groupSelectMode = true;
	  selectGroup.clear();
	}
    }
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
  if (!layoutBusy)
    {
      switch (event->key())
	{
	case Qt::Key_P:
	  highlightPinned(false);
	  update();
	  break;
	case Qt::Key_Shift:
	  groupSelectMode = false;
	  break;
	}
    }
}

bool Canvas::layoutWord(Word *w)
{
  int attempts = 0;
  int maxattempts = 25;
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

    startlayout:
      QPoint oldpos(0, 0);
      bool done = false;
      w->setPos(centre);
      w->prepareCollisionDetection();

      do
	{
	  // get a new location estimate
	  float rho;
	  switch (path)
	    {
	    case RectangularPath:
	      {
		tau += avarnor->operator()()*0.05;
		rho = 3*round(tau/M_PI*0.5);
		break;
	      }
	    case CircularPath:
	    default:
	      {
		tau += avarnor->operator()()*0.05;
		rho = tau;
		break;
	      }
	    }

	  // move Word to a new location
	  QPoint delta = (QPoint(rho*cos(tau), rho*sin(tau)) - oldpos);
	  w->moveBy(delta.x(), delta.y());
	  oldpos += delta;

	  if (attempts >= maxattempts) break;
	  if (!sceneRect().contains(w->boundingBox())) { attempts++; goto startlayout; }
          if (boundingRegions.size() > 0)
            {
              bool contains = false;
	      for (int i = 0; i < boundingRegions.size() && !contains; ++i)
		if (!boundingRegions[i].contains(w->boundingBox().toRect()))
		  {
		    contains = true;
		  }
              if (!contains) continue;
            }
  
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
      while (!done && attempts < maxattempts);
    }
  else w->prepareCollisionDetection();

  if (attempts >= maxattempts) return false;
  
  /* finally add the word */
  QGraphicsScene::addItem((QGraphicsItem*)w);
  this->update(w->boundingRect());

  // add it to the quadtree as well
  quadtree.insert(w);

  return true;
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (!layoutBusy)
    {
      QGraphicsItem *item = itemAt(event->scenePos());
      if (item != NULL)
	{
	  item->grabMouse();
	  if (event->button() == Qt::LeftButton)
	    {
	      if (groupSelectMode && !selectGroup.contains((Word*)item))
		selectGroup.push_back((Word*)item);
	      else
		((Word*)item)->toggleManipulated();		  
	    }
	}
    }
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if (groupSelectMode)
    {
      QPoint delta = (mouseEvent->scenePos() - mouseEvent->lastScenePos()).toPoint();      
      foreach (Word* w, selectGroup)
	{
	  w->moveBy(delta.x(), delta.y());
	  this->update(w->boundingRect());	      
	}
    }
  else
    {
      QGraphicsItem *item = mouseGrabberItem();
      if (item != NULL && !layoutBusy)
	{
	  QPoint delta = (mouseEvent->scenePos() - mouseEvent->lastScenePos()).toPoint();
	  ((Word*)item)->moveBy(delta.x(), delta.y());
	  this->update(item->boundingRect());	  
	}
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (!layoutBusy)
    {
      if (groupSelectMode)
	{
	}
      else
	{
	  QGraphicsItem *item = mouseGrabberItem(); 
	  if (item != NULL)
	    {
	      item->ungrabMouse();
	      if (event->button() == Qt::LeftButton)
		{
      
		  Word *w = ((Word*)item);
		  w->toggleManipulated();
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
	      else
		{
		  WordPropertyDialog *wpd = new WordPropertyDialog((Word*)item);
		  wpd->exec();
		  delete wpd;
		}
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
    picker(colourrng, uni);

  foreach (Word *word, wordlist)
    word->setColour(colourpalet[picker()]);
}

void Canvas::randomiseWordFontFamily(const QVector<QString> &fontfamilies)
{
  boost::mt19937 colourrng;
  colourrng.seed(static_cast<unsigned int>(std::time(0)));  
  
  boost::uniform_int<> uni(0, fontfamilies.size()-1);
  boost::variate_generator<boost::mt19937, boost::uniform_int<> > 
    picker(colourrng, uni);

  foreach (Word *word, wordlist)
    word->setFontName(fontfamilies[picker()]);
}

void Canvas::setBoundingRegions(QVector<QRegion> b)
{
  // find bounding rectangle for union of all bounding regions
  QRegion masterBound;
  foreach (QRegion region, b)
    masterBound += region;

  QRect r = masterBound.boundingRect();
  this->setSceneRect(r);
  
  // adapt random distributions to new bounding regions
  centrepoint = 0.5*QPointF(r.width(), r.height());
  cxDistribution = boost::normal_distribution<float>(centrepoint.x(), r.width()*0.5);
  cyDistribution = boost::normal_distribution<float>(centrepoint.y(), r.height()*0.5);
  
  /*
    QRect srect = this->sceneRect().toRect();

    // find out whether bounds within scene
    if (!srect.contains(brect))
    {
    // is the area of the scene large enough to contain the bounds?
    int boundArea = brect.width()*brect.height();
    int sceneArea = srect.width()*srect.height();
    if (scene > boundArea)
	
    }
  */
  
  boundingRegions.clear();
  boundingRegions = b;
}

void Canvas::saveWordcloud(QString filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text)) 
    {
      qDebug() << "Could not save wordcloud to file " << filename << ".";
      return;
    }

  QTextStream o(&file);
  foreach (QGraphicsItem *item, items())
    o << *(Word*)item << "\n"; 
    
  file.close();
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

QRectF Canvas::setWordList(WordList l) 
{
  this->clearWordList();
  wordlist = l;
  return scaleSceneRect();
}

QRectF Canvas::scaleSceneRect()
{
  if (wordlist.size() > 1)
    {
      float wordArea = wordlist.area();
      QSizeF maximumWordSize = wordlist.maximumWordSize();

      float boundArea = 0.;
      if (boundingRegions.size() != 0)
	{
	  foreach (QRegion region, boundingRegions)
	    {
	      QVector<QRect> rects = region.rects();
	      foreach (QRect rect, rects)
		boundArea += rect.width()*rect.height();
	    }
	}
      else 
	{
	  QRectF scene = sceneRect();
	  boundArea = scene.width()*scene.height();
	}

      std::cout << boundArea << " " << wordArea << " " 
		<< (wordArea/boundArea) << std::endl;
      return scaleSceneRectArea(1.6*wordArea/boundArea, maximumWordSize);
    }
  else return QRectF();
}

QRectF Canvas::scaleSceneRectArea(float factor, QSizeF maximumWordSize)
{
  QRectF sceneRect = this->sceneRect();
  sceneRect.setWidth(sceneRect.width()*sqrt(factor));
  sceneRect.setHeight(sceneRect.height()*sqrt(factor));

  if (sceneRect.width() < maximumWordSize.width())
    {
      float r = sceneRect.width()/sceneRect.height();
      sceneRect.setWidth(maximumWordSize.width()*1.2);
      sceneRect.setHeight(maximumWordSize.width()*1.2/r);
    }
  if (sceneRect.height() < maximumWordSize.height())
    {
      float r = sceneRect.height()/sceneRect.width();
      sceneRect.setHeight(maximumWordSize.height()*1.2);
      sceneRect.setWidth(maximumWordSize.height()*1.2/r);
    }
        
  this->setSceneRect(sceneRect);
  quadtree.setRootRectangle(sceneRect);
  centrepoint = 0.5*QPointF(sceneRect.width(), sceneRect.height());
  cxDistribution = boost::normal_distribution<float>(centrepoint.x(),
						     sceneRect.width()*.5);
  cyDistribution = boost::normal_distribution<float>(centrepoint.y(), 
						     sceneRect.height()*.5);
  delete cxvarnor, cyvarnor;
  cxvarnor = new
    boost::variate_generator<boost::mt19937&, 
			     boost::normal_distribution<float> >(rng,
								 cxDistribution);
  cyvarnor = new
    boost::variate_generator<boost::mt19937&, 
			     boost::normal_distribution<float> >(rng,
								 cyDistribution);
  return sceneRect;
}

void Canvas::startLayout()
{
  // if (layoutThread != NULL) 
  //   {
  //     layoutThread->interrupt();
  //     layoutThread->join();
  //   }
  // layoutThread.reset(new boost::thread(boost::bind(&Canvas::createLayout, this)));
  // layoutBusy = true;
  // this->update();
  emit layoutStarted();
  createLayout();
}

void Canvas::stopLayout()
{
  if (layoutThread != NULL) 
    {
      layoutThread->interrupt();
      layoutThread->join();
    }
  layoutBusy = false;
  this->update();
  emit layoutEnded();
}

void Canvas::unpinAll()
{
  foreach (Word *word, wordlist)
    word->setPinned(false);
}

void Canvas::vAlignSelectedWords()
{
  qDebug() << __PRETTY_FUNCTION__;
}
