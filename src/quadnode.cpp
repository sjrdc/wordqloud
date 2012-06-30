#include <QDebug>
#include "quadnode.h"
#include "word.h"


bool QuadNode::contains(IAreaComparable *item) const
{
  return nodeRectangle.contains(item->boundingBox());
}

void QuadNode::insert(IAreaComparable *item)
{
  if (contents.size() >= maxContents && branches.isEmpty())
    initBranches();
  
  // try to insert in child nodes
  foreach (QuadNode *node, branches)
    if (node->contains(item))
      {
	node->insert(item);
	return;
      }

  // else... add item to our own collection
  contents.push_back(item);
}

void QuadNode::initBranches()
{
  QRectF r = this->nodeRectangle;
  r.setSize(0.5*r.size());
  
  QuadNode *lnw = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft());
  lnw->setNodeRectangle(r);
  branches.push_back(lnw);

  QuadNode *lne = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft() + QPointF(r.width(), 0.));
  lne->setNodeRectangle(r);
  branches.push_back(lne);

  QuadNode *lsw = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft() + QPointF(0., r.height()));
  lsw->setNodeRectangle(r);
  branches.push_back(lsw);

  QuadNode *lse = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft() + QPointF(r.width(), r.height()));
  lse->setNodeRectangle(r);
  branches.push_back(lse);

  QList<IAreaComparable*> content2Remove;
  foreach (IAreaComparable *item, contents)
    {
      foreach (QuadNode* node, branches)
	{
	  if (node->contains(item))
	    {
	      node->insert(item);
	      content2Remove.push_back(item);
	    }
	}
    }
  foreach (IAreaComparable *item, content2Remove)
    contents.removeOne(item);
}

void QuadNode::query(QRectF r, QList<IAreaComparable*>& l) const
{
  // if the area queried does not even intersect the node area, there
  // is nothing to add at all
  if (!this->intersects(r)) return;

  // query this nodes contents
  if (nodeRectangle.intersects(r))
    foreach (IAreaComparable* item, contents)
      l.push_back(item);

  // query contents of branches
  foreach (QuadNode *branch, branches)
    branch->query(r, l);    
}
