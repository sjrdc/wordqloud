#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

#include <QGraphicsScene>
#include "quadtree.h"
#include "wordlist.h"

class QGraphicsSceneMouseEvent;
class Word;

class Canvas : public QGraphicsScene
{
public:
  /** 
   * Canvas constructor
   * 
   * @param w canvas gets width w
   * @param h canvas gets height h 
   * 
   * @return 
   */
  Canvas(float w = 800, float h = 600);
  ~Canvas();

  void createLayout();
  void randomiseWordColours(QVector<QColor> colours);
  void randomiseWordFontFamily(QVector<QString> fontfamilies);  
  void reCreateLayout();
  void setColors(QColor bcolor, QVector<QRgb> wcolors);
  void setWordList(WordList l);
  
protected:
  void layoutWord(Word *w);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);    
  
  WordList wordlist;
  QPointF centrepoint;
  QVector<QRgb> wordcolors;
  QuadTree quadtree;
  
  // some random generator stuff for coordinate generation
  boost::mt19937 rng;
  boost::normal_distribution<float> cxDistribution;
  boost::normal_distribution<float> cyDistribution; 
  boost::variate_generator<boost::mt19937&, 
                           boost::normal_distribution<float> > *cxvarnor;
  boost::variate_generator<boost::mt19937&, 
                           boost::normal_distribution<float> > *cyvarnor;
};

inline void Canvas::setWordList(WordList l) { wordlist = l; }
