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

  void addWord(Word *w);
  void appendWordList(WordList l);
  void createLayout();
  void highlightPinned();
  void randomiseWordColours(QVector<QColor> colours);
  void randomiseWordFontFamily(QVector<QString> fontfamilies);  
  void reCreateLayout();
  void setColors(QColor bcolor, QVector<QRgb> wcolors);
  void setWordList(WordList l);
  void setWordFont(QFont font);

public slots:
  void randomiseOrientations();

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
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

inline void Canvas::addWord(Word *w)
{
  wordlist.push_back(w);
  layoutWord(w);
}

inline void Canvas::appendWordList(WordList l)
{
  wordlist.append(l);
  foreach (Word *word, l)
    layoutWord(word);
}

inline void Canvas::setWordList(WordList l) 
{
  quadtree.clearContents();
  this->clear();
  wordlist = l; 
}
