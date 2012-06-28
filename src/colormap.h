#include <QVector>
#include <QColor>

const static int MaxRGB = 255;

class Colormap
{
  QVector<QRgb> springColormap(int n);
  QVector<QRgb> summerColormap(int n);
  QVector<QRgb> autumnColormap(int n);
  QVector<QRgb> winterColormap(int n);
  
  QVector<QRgb> boneColormap(int n);
  QVector<QRgb> copperColormap(int n);
  QVector<QRgb> hotColormap(int n);
  QVector<QRgb> coolColormap(int n);
  
};
