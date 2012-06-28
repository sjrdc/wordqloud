#include <QList>
#include <QRectF>

static const float minimumNodeArea = 10.;

template <typename T>
class QuadNode
{
public:
  void insert(T*);
  
protected:
  bool contains(T*);
  void initLeafs();

  QList<T*> contents;
  QList<QuadNode<T*>*> leafs;
  QRectF nodeRectangle;
};


