#ifndef QUADTREE_H_
#define QUADTREE_H_
#include <QDebug>
#include <QRectF>
#include "quadnode.h"

class QPainter;
template <typename T> class QList;
class QuadTree
{
public:
  QuadTree();
  void clearContents();
  void draw(QPainter &p);
  void insert(IAreaComparable*);
  void setRootRectangle(QRectF r);
  void query(QRectF r, QList<IAreaComparable*> &l) const;
protected:
  QuadNode *rootnode;
};

inline void QuadTree::clearContents()
{
  rootnode->clearContents();
}

inline void QuadTree::setRootRectangle(QRectF r) 
{ 
  rootnode->setNodeRectangle(r); 
}

inline void QuadTree::insert(IAreaComparable *item)
{
  rootnode->insert(item);
}

inline void QuadTree::query(QRectF r, QList<IAreaComparable*> &l) const 
{
  rootnode->query(r, l); 
}

#endif
