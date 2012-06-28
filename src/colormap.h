#include <QVector>
#include <QColor>

const static int MaxRGB = 255;

class Colormap
{
public:
  static QVector<QColor> springColormap(int n);
  static QVector<QColor> summerColormap(int n);
  static QVector<QColor> autumnColormap(int n);
  static QVector<QColor> winterColormap(int n);
  
  static QVector<QColor> boneColormap(int n);
  static QVector<QColor> copperColormap(int n);
  static QVector<QColor> hotColormap(int n);
  static QVector<QColor> coolColormap(int n);
  
};
