#ifndef WORD_H_
#define WORD_H_

#include <QColor>
#include <QFont>
#include <QRegion>
#include <QGraphicsSimpleTextItem>

#include <string>
#include "iareacomparable.h"
#include "istreamable.h"

class Word : public QGraphicsSimpleTextItem, 
	     public IAreaComparable, public IStreamable
{
public:
  Word(QString w, float b = 0.8);
  ~Word();

  float area();
  virtual QRectF boundingBox() const;
  void cacheCollision(Word *w);
  bool collidesWith(Word *w) const;
  bool collidesWithCashed() const;
  bool colourLocked() const;
  bool fontLocked() const;
  bool fontsizeLocked() const;
  QTextStream& fromStream(QTextStream&);
  int getFrequency() const;
  bool getPinned() const;
  float height();
  void lockColour(bool b = true);
  void lockFont(bool b = true);
  void lockFontsize(bool b = true);
  void lockOrientation(bool b = true);
  void moveBy(float x, float y);
  bool orientationLocked() const;
  void prepareCollisionDetection();
  void setColour(QColor c);
  void setPinned(bool p);
  void setFontName(QString fontname);
  void setFontsize(float s);
  void setFrequency(int f);
  void setRotation(float r);
  void showPinned(bool p);
  QTextStream& toStream(QTextStream&) const;
  void togglePinned();
  void toggleShowPinned();  
  void toggleManipulated();
  void updateCollisionDetection(QPointF delta);
  float width();

protected:


private:
  void setBrush(QBrush b);

  QRegion region;
  Word *cachedCollision;
  bool _pinned;
  bool _manipulated;
  bool _showBounding;
  bool _showPinnedState;
  bool _colourLocked;
  bool _fontLocked;  
  bool _orientationLocked;
  bool _fontsizeLocked;
  bool _regionInitialised;
  QColor cachedColor;
  int frequency;
};

inline QRectF Word::boundingBox() const
{
  QRectF b = ((QGraphicsSimpleTextItem*)this)->boundingRect();
  return ((QGraphicsSimpleTextItem*)this)->mapRectToScene(b);
}

// locks
inline bool Word::colourLocked() const { return _colourLocked; }
inline bool Word::fontLocked() const { return _fontLocked; }
inline bool Word::fontsizeLocked() const { return _fontsizeLocked; }
inline bool Word::orientationLocked() const { return _orientationLocked; }

inline void Word::lockColour(bool b) { _colourLocked = b; }
inline void Word::lockOrientation(bool b) { _orientationLocked = b; }
inline void Word::lockFontsize(bool b) { _fontsizeLocked = b; }
inline void Word::lockFont(bool b) { _fontLocked = b; }

// pinning
inline bool Word::getPinned() const { return _pinned; }
inline void Word::setPinned(bool p) { _pinned = p; }
inline void Word::togglePinned() { _pinned = !_pinned; }

inline void Word::showPinned(bool p)
{
  QColor c = this->brush().color();
  _showPinnedState = p;
  c.setAlpha(!p || _pinned ? 255 : 100);
  this->setBrush(c);
}

inline void Word::toggleShowPinned()
{
  QColor c = this->brush().color();
  if (!_showPinnedState)
    {
      _showPinnedState = true;
      c.setAlpha(_pinned ? 255 : 100);
    }
  else 
    {
      _showPinnedState = false;  
      c.setAlpha(255);
    }
  this->setBrush(c);
}

inline void Word::toggleManipulated()
{
  if (_manipulated)
    {
      _manipulated = false;
      this->setBrush(cachedColor);
    }
  else {
      _manipulated = true;
      this->cachedColor = this->brush().color();
      this->setBrush(Qt::red);
    }
}

// colouring
inline void Word::setBrush(QBrush b) { QGraphicsSimpleTextItem::setBrush(b); }
inline void Word::setColour(QColor c) { if (!_colourLocked) this->setBrush(c); }

inline void Word::setFrequency(int f) { frequency = f; }
inline int  Word::getFrequency() const { return frequency; }

#endif
