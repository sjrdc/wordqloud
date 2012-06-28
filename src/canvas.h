#include <QGraphicsScene>

class Word;
class Canvas : public QGraphicsScene
{
public:
  void addItem(Word *w);
};
