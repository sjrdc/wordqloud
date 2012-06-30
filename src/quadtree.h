#include <QRectF>

template <typename T> class QList;
template <typename T> class QuadNode;

template <typename T>
class QuadTree
{
public:
  void insert(T*);
  void setRootRectangle(QRectF r);
  void query(QRectF r, QList<T*> &l) const;
protected:
  QuadNode<T> *rootnode;
};

template <typename T>
inline void QuadTree<T>::setRootRectangle(QRectF r) 
{ 
  rootnode->setNodeRectangle(r); 
}

template <typename T>
inline void QuadTree<T>::query(QRectF r, QList<T*> &l) const 
{
  rootnode->query(r, l); 
}

