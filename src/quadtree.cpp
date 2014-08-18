#include <QPainter>

#include "quadtree.h"

QuadTree::QuadTree()
{
  rootnode = new QuadNode();
}

void QuadTree::draw(QPainter &p)
{
  rootnode->draw(p);
}
