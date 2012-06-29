#include "quadtree.h"

template <typename T>
void QuadTree<T>::insert(T* t)
{
  rootnode->insert(t);
}
