#include <QGraphicsScene>

class Word;
class Canvas : public QGraphicsScene
{
public:
  void addItem(QString s);
  void addItem(Word *w);
};
