#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

#include <QGraphicsScene>

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
  void addItem(QString s);
  void addItem(Word *w);

protected:
  QPointF centrepoint;
  boost::mt19937 rng;
  boost::normal_distribution<float> cxDistribution;
  boost::normal_distribution<float> cyDistribution; 
  boost::variate_generator<boost::mt19937&, 
                           boost::normal_distribution<float> > *cxvarnor;
  boost::variate_generator<boost::mt19937&, 
                           boost::normal_distribution<float> > *cyvarnor;
};
