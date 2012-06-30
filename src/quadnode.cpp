#include "quadnode.h"

template <typename T> 
void QuadNode<T>::insert(T* t)
{
  if (contents.size() >= maxContents && branches.isEmpty())
    initBranches();

  // try to insert in child nodes
  foreach (QuadNode<T> *node, branches)
    if (node->contains(t))
      {
	node->insert(t);
	return;
      }

  // else... add item to our own collection
  contents.push_back(t);
}

template <typename T>
void QuadNode<T>::initBranches()
{
  QRectF r = this->nodeRectangle;
  r.setSize(0.5*r.size());
  
  QuadNode<T> *lnw = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft());
  lnw.setNodeRectangle(r);
  branches.push_back(lnw);

  QuadNode<T> *lne = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft() + QPointF(r.width(), 0.));
  lne.setNodeRectangle(r);
  branches.push_back(lne);

  QuadNode<T> *lsw = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft() + QPointF(0., r.height()));
  lsw.setNodeRectangle(r);
  branches.push_back(lsw);

  QuadNode<T> *lse = new QuadNode();
  r.moveTo(this->nodeRectangle.topLeft() + QPointF(r.width(), r.height()));
  lse.setNodeRectangle(r);
  branches.push_back(lse);

  QList<T*> content2Remove;
  foreach (T* item, contents)
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
  contents.remove(content2Remove);
}
