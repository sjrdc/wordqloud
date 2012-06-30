#ifndef QUADNODE_H_
#define QUADNODE_H_

#include <QList>
#include <QRectF>
#include "iareacomparable.h"
static const float minimumNodeArea = 10.;
static const short maxContents = 10;

class QuadNode
{
public:
  void insert(IAreaComparable *item);
  int objectCount() const;
  void query(QRectF r, QList<IAreaComparable*> &l) const;
  void setNodeRectangle(QRectF r);
  
protected:
  bool contains(IAreaComparable *item) const;
  void initBranches();
  bool intersects(QRectF r) const;
  
  QList<IAreaComparable*> contents;
  QList<QuadNode*> branches;
  QRectF nodeRectangle;
};

inline bool QuadNode::intersects(QRectF r) const { return nodeRectangle.intersects(r); }

inline int QuadNode::objectCount() const { return contents.size(); }

inline void QuadNode::setNodeRectangle(QRectF r) { nodeRectangle = r; }


#endif
