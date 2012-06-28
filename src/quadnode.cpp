#include "quadnode.h"

template <typename T> 
void QuadNode<T>::insert(T* t)
{
  if (leafs.isEmpty())
    initLeafs();

  foreach (QuadNode<T> *node, leafs)
    if (node->contains(t))
      {
	node->insert(t);
	return;
      }

  // else...
  contents.push_back(t);
}

template <typename T>
void QuadNode<T>::initLeafs()
{
  if (nodeRectangle.width() * nodeRectangle.height() < minimumNodeArea)
    return;

  /// \todo further init leafs
}
