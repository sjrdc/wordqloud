#ifndef IAREACOMPARABLE_H_
#define IAREACOMPARABLE_H_

class IAreaComparable
{
public:
  virtual QRectF boundingBox() const = 0;
};

#endif
