#include <ctime>
#include "canvas.h"
#include "word.h"

#include <QDebug>

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

void Canvas::addItem(QString s)
{
  Word *w = new Word(s);
  addItem(w);
}

void Canvas::addItem(Word *w)
{
  /* find out where to place the word */

  // initial location for word
  float tau = 0;
  float cx = -1, cy = -1;
  while (cx < 10 || cy < 10 || cx > width() - 10 || cy > height() - 10)
    {
      cx = cxvarnor->operator()();
      cy = cyvarnor->operator()();      
    }

  QRectF bbox = w->boundingRect();
  QPointF centre = QPointF(cx - 0.5*bbox.width(), cy - 0.5*bbox.height());
  
  bool done = false;
  do
    {
      // get a new location estimate
      tau += 0.10;
      float rho = tau;

      // place Word
      w->setPos(centre + QPointF(rho*cos(tau), rho*sin(tau)));

      // check cashed collision first
      if (w->collidesWithCashed())
	continue;

      // evaluate possible overlap between new word and all words placed so far
      done = true;
      w->prepareCollisionDetection();
      foreach (QGraphicsItem *i, items())
	{
	  Word* q = (Word*)i;

	  if (w->collidesWith(q)) 
	    {
	      w->cacheCollision(q);
	      done = false;
	      break;
	    }
	}
    }
  while (!done);

  /* finally add the word */
  QGraphicsScene::addItem((QGraphicsItem*)w);
  quadtree.insert(w);
}

void Canvas::setColors(QColor bcolor, QVector<QRgb> wcolors)
{
  setBackgroundBrush(bcolor);
  wordcolors = wcolors;
}
