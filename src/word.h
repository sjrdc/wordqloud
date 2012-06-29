#include <QColor>
#include <QFont>
#include <QGraphicsSimpleTextItem>

#include <string>

class Word : public QGraphicsSimpleTextItem
{
public:
  Word(QString w);
  ~Word();

  void cacheCollision(Word *w);
  bool collidesWith(Word *w);
  bool collidesWithCashed();
  bool getPinned();
  float height();
  void prepareCollisionDetection();
  void setPinned(bool p);
  void setFontSize(float s);
  void togglePinned();
  float width();
  void writeImage();
protected:
  
  
private:
    
  void initBitmap();

  QImage *bitmap;
  Word *cachedCollision;
  bool pinned;
  bool showBounding;
  
};

inline bool Word::getPinned() { return pinned; }

inline void Word::setPinned(bool p)
{
  pinned = p;
}

inline void Word::togglePinned()
{
  pinned = !pinned;
}
