#ifndef QUADNODE_H_
#define QUADNODE_H_

#include <QList>
#include <QRectF>
#include "iareacomparable.h"

class QPainter;
class QuadNode
{
public:
  void clearContents();
  void draw(QPainter &p);
  QRectF getNodeRectangle() const;
  void insert(IAreaComparable *item);
  int objectCount() const;
  void query(QRectF r, QList<IAreaComparable*> &l) const;
  void setNodeRectangle(QRectF r);
  
protected:  
  bool contains(IAreaComparable *item) const;
  bool contains(QRectF r) const;  
  void initBranches();
  bool intersects(QRectF r) const;
  
  QList<IAreaComparable*> contents;
  QList<QuadNode*> branches;
  QRectF nodeRectangle;

  static const float minimumNodeArea = 10.;
  static const short maxContents = 25;
};

inline void QuadNode::clearContents()
{
  contents.clear();
  foreach (QuadNode *branch, branches)
    branch->clearContents();
}

inline bool QuadNode::contains(QRectF r) const { return nodeRectangle.contains(r); }

inline QRectF QuadNode::getNodeRectangle() const { return nodeRectangle; }

inline bool QuadNode::intersects(QRectF r) const { return nodeRectangle.intersects(r); }

inline int QuadNode::objectCount() const { return contents.size(); }

inline void QuadNode::setNodeRectangle(QRectF r) { nodeRectangle = r; }


#endif
