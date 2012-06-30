#include <QRectF>

template <typename T> class QuadNode;
template <typename T>

class QuadTree
{
public:
  void insert(T*);
  void setRootRectangle(QRectF r);
  
protected:
  QuadNode<T*> *rootnode;
};

template <typename T>
inline void QuadTree<T>::setRootRectangle(QRectF r) { rootnode->setNodeRectangle(r); }
