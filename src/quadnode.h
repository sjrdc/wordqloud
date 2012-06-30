#include <QList>
#include <QRectF>

static const float minimumNodeArea = 10.;
static const float maxContents = 10.;

template <typename T>
class QuadNode
{
public:
  void insert(T*);
  void setNodeRectangle(QRectF r);
protected:
  bool contains(T*);
  void initBranches();

  QList<T*> contents;
  QList<QuadNode<T>*> branches;
  QRectF nodeRectangle;
};

template <typename T>
inline void QuadNode<T>::setNodeRectangle(QRectF r) { nodeRectangle = r; }
