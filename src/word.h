#ifndef WORD_H_
#define WORD_H_

#include <QColor>
#include <QFont>
#include <QRegion>
#include <QGraphicsSimpleTextItem>

#include <string>
#include "iareacomparable.h"

class Word : public QGraphicsSimpleTextItem, public IAreaComparable
{
public:
  Word(QString w, float b = 0.8);
  ~Word();

  virtual QRectF boundingBox() const;
  void cacheCollision(Word *w);
  bool collidesWith(Word *w);
  bool collidesWithCashed();
  bool getPinned();
  float height();

  void moveBy(float x, float y);
  void prepareCollisionDetection();
  void setPinned(bool p);
  void setFontName(QString fontname);
  void setFontSize(float s);
  void showPinned(bool p);
  void togglePinned();
  void toggleShowPinned();  
  void toggleManipulated();
  void updateCollisionDetection(QPointF delta);
  float width();
  void writeImage();

protected:


private:
    
  void initBitmap();

  QVector<QRect> regionRects;
  QRegion region;
  QImage *bitmap;
  Word *cachedCollision;
  bool pinned;
  bool manipulated;
  bool showBounding;
  bool showPinnedState;
  QColor cachedColor;
};

inline QRectF Word::boundingBox() const
{
  QRectF b = ((QGraphicsSimpleTextItem*)this)->boundingRect();
  return ((QGraphicsSimpleTextItem*)this)->mapRectToScene(b);
}

inline bool Word::getPinned() { return pinned; }

inline void Word::setPinned(bool p)
{
  pinned = p;
}

inline void Word::togglePinned()
{
  pinned = !pinned;
}

inline void Word::showPinned(bool p)
{
  QColor c = this->brush().color();
  showPinnedState = p;
  c.setAlpha(!p || pinned ? 255 : 100);
  this->setBrush(c);
}

inline void Word::toggleManipulated()
{
  if (manipulated)
    {
      manipulated = false;
      this->setBrush(cachedColor);
    }
  else {
      manipulated = true;
      this->cachedColor = this->brush().color();
      this->setBrush(Qt::red);
    }
}

inline void Word::toggleShowPinned()
{
  QColor c = this->brush().color();
  if (!showPinnedState)
    {
      showPinnedState = true;
      c.setAlpha(pinned ? 255 : 100);
    }
  else 
    {
      showPinnedState = false;  
      c.setAlpha(255);
    }
  this->setBrush(c);
}

#endif
