#include <QColor>
#include <QFont>
#include <QGraphicsSimpleTextItem>

#include <string>

class Word : public QGraphicsSimpleTextItem
{
public:
  Word(QString w);
  ~Word();

  bool getPinned();
  void setPinned(bool p);
  void setFontSize(float s);
  void togglePinned();

protected:
  
  
private:

  void initBitmap();
  
  bool pinned;
  bool showbounding;
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
