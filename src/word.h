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
  bool colourLocked();
  bool fontLocked();
  bool fontsizeLocked();
  bool getPinned();
  float height();
  void lockColour(bool b = true);
  void lockFont(bool b = true);
  void lockFontsize(bool b = true);
  void lockOrientation(bool b = true);
  void moveBy(float x, float y);
  bool orientationLocked();
  void prepareCollisionDetection();
  void setColour(QColor c);
  void setPinned(bool p);
  void setFontName(QString fontname);
  void setFontsize(float s);
  void setRotation(float r);
  void showPinned(bool p);
  void togglePinned();
  void toggleShowPinned();  
  void toggleManipulated();
  void updateCollisionDetection(QPointF delta);
  float width();
  void writeImage();

protected:


private:
  void setBrush(QBrush b);
  void initBitmap();

  QVector<QRect> regionRects;
  QRegion region;
  QImage *bitmap;
  Word *cachedCollision;
  bool pinned;
  bool manipulated;
  bool showBounding;
  bool showPinnedState;
  bool _colourLocked;
  bool _fontLocked;  
  bool _orientationLocked;
  bool _fontsizeLocked;
  QColor cachedColor;
};

inline QRectF Word::boundingBox() const
{
  QRectF b = ((QGraphicsSimpleTextItem*)this)->boundingRect();
  return ((QGraphicsSimpleTextItem*)this)->mapRectToScene(b);
}

// locks
inline bool Word::colourLocked() { return _colourLocked; }
inline bool Word::fontLocked() { return _fontLocked; }
inline bool Word::fontsizeLocked() { return _fontsizeLocked; }
inline bool Word::orientationLocked() { return _orientationLocked; }

inline void Word::lockColour(bool b) { _colourLocked = b; }
inline void Word::lockOrientation(bool b) { _orientationLocked = b; }
inline void Word::lockFontsize(bool b) { _fontsizeLocked = b; }
inline void Word::lockFont(bool b) { _fontLocked = b; }

// pinning
inline bool Word::getPinned() { return pinned; }
inline void Word::setPinned(bool p) { pinned = p; }
inline void Word::togglePinned() { pinned = !pinned; }
inline void Word::showPinned(bool p)
{
  QColor c = this->brush().color();
  showPinnedState = p;
  c.setAlpha(!p || pinned ? 255 : 100);
  this->setBrush(c);
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

// colouring
inline void Word::setBrush(QBrush b) { QGraphicsSimpleTextItem::setBrush(b); }
inline void Word::setColour(QColor c) { if (!_colourLocked) this->setBrush(c); }

#endif
