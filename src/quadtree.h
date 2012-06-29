template <typename T> class QuadNode;

template <typename T>
class QuadTree
{
public:
  void insert(T*);
  
protected:
  QuadNode<T*> *rootnode;
};
