#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/thread.hpp>

#include <QGraphicsScene>

#include "enumerations.h"
#include "quadtree.h"
#include "wordlist.h"

class QGraphicsSceneMouseEvent;
class Word;

class Canvas : public QGraphicsScene
{
  Q_OBJECT
public:
  /** 
   * Canvas constructor
   * 
   * @param w canvas initially gets width w
   * @param h canvas initially gets height h 
   * 
   * @return 
   */
  Canvas(float w = 800, float h = 600);
  ~Canvas();

  void addWord(Word *w);
  void addWordList(WordList l);
  void clearWordList();
  void createLayout();
  void highlightPinned(bool h);
  void randomiseWordColours(const QVector<QColor> &colours);
  void randomiseWordFontFamily(const QVector<QString> &fontfamilies);  
  // void reCreateLayout(); 
  void saveWordcloud(QString filename);
  void startLayout() ;
  QRectF scaleSceneRect();
  void setColors(QColor bcolor, QVector<QRgb> wcolors);
  void setBoundingRegions(QVector<QRegion> b);
  void setLayoutBound(LayoutBound l);
  void setLayoutPath(LayoutPath p);
  QRectF setWordList(WordList l);
  void setWordFont(QFont font);

signals:
  void layoutProgress(int, int);
  void layoutEnded();
  void layoutStarted();

public slots:
  void randomiseOrientations(WordOrientation w);
  void stopLayout();
  void unpinAll();

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  bool layoutWord(Word *w);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);    
  QRectF scaleSceneRectArea(float factor);
  
  WordList wordlist;
  LayoutBound layoutBound;
  QPointF centrepoint;
  QVector<QRgb> wordcolors;
  QuadTree quadtree;
  QVector<QRegion> boundingRegions;
  LayoutPath path;
  bool layoutBusy;

  // some random generator stuff for coordinate generation
  boost::mt19937 rng;
  boost::normal_distribution<float> cxDistribution;
  boost::normal_distribution<float> cyDistribution;
  boost::uniform_int<> angleIncrement;   
  boost::variate_generator<boost::mt19937&, 
                           boost::normal_distribution<float> > *cxvarnor;
  boost::variate_generator<boost::mt19937&, 
                           boost::normal_distribution<float> > *cyvarnor;
  boost::variate_generator<boost::mt19937&,
			   boost::uniform_int<> > *avarnor;    

  boost::scoped_ptr<boost::thread> layoutThread;
};

inline void Canvas::addWord(Word *w)
{
  wordlist.push_back(w);
  layoutWord(w);
}

inline void Canvas::setLayoutBound(LayoutBound l)
{
  layoutBound = l;
}

inline void Canvas::setLayoutPath(LayoutPath p)
{
  path = p;
}
