#include <QList>
#include <QRectF>

static const float minimumNodeArea = 10.;
static const short maxContents = 10;

template <typename T>
class QuadNode
{
public:
  void insert(T*);
  void setNodeRectangle(QRectF r);
  void query(QRectF r, QList<T*> &l);
protected:
  bool contains(T*);
  void initBranches();
  bool intersects(QRectF r) const;
  
  QList<T*> contents;
  QList<QuadNode<T>*> branches;
  QRectF nodeRectangle;
};

template <typename T>
inline void QuadNode<T>::setNodeRectangle(QRectF r) { nodeRectangle = r; }

template <typename T>
inline bool QuadNode<T>::intersects(QRectF r) const { return nodeRectangle.intersects(r); }
