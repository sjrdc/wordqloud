#include <math.h>
#include "colormap.h"

QVector<QColor> Colormap::springColormap(int n)
{
  QVector<QColor> colors;
  colors.resize(n);
    
  double spring;
  for (int i = 0; i < n; ++ i)
    {
      spring = i/(n - 1.);
      colors[i] = QColor(int(1.*MaxRGB), int(spring*MaxRGB), 
			 int((1 - spring)*MaxRGB), MaxRGB);
    }

  return colors;
}

QVector<QColor> Colormap::summerColormap(int n)
{ 
  QVector<QColor> colors;
  colors.resize(n);

  double summer;
  for (int i = 0; i < n; ++ i)
    {
      summer = i/(n - 1.);
      colors[i] = QColor(int(summer*MaxRGB), int(0.5*(1. + summer)*MaxRGB), 
			 int(0.4*MaxRGB), MaxRGB);
    }
  return colors;
}

QVector<QColor> Colormap::autumnColormap(int n)
{
  QVector<QColor> colors;
  colors.resize(n);

  for (int i = 0; i < n; ++ i)
    colors[i] = QColor(int(1.*MaxRGB), int(i/(n - 1)*MaxRGB ), 
		       int(0.), MaxRGB);
  return colors;
}

QVector<QColor> Colormap::winterColormap(int n)
{
  QVector<QColor> colors;
  colors.resize(n);

  double winter;
  for (int i = 0; i < n; ++ i)
    {
      winter = i/(n - 1.);
      colors[i] = QColor(int(0.), int(winter*MaxRGB),
			 int((1. - 0.5*winter)*MaxRGB), MaxRGB);
    }
  return colors;
}

QVector<QColor> Colormap::hotColormap(int l)
{
  QVector<QColor> colors;
  colors.resize(l);

    int n = 3 * l / 8;
    double r, g, b;
    for (int i = 0; i < l; i++)
      {
	if (i < n)
	  r = 1.*(i+1) / n;
	else
	  r = 1.;
	if (i < n)
	  g = 0.;
	else if (i >= n && i < 2 * n)
	  g = 1. * (i+1 - n) / n;
	else
	  g = 1.;
	if (i < 2 * n)
	  b = 0.;
	else
	  b = 1. * (i + 1 - 2 * n) / (l - 2 * n);

	colors[i] = QColor(int(r*MaxRGB), int(g*MaxRGB), 
			   int(b*MaxRGB), MaxRGB);
    }
  return colors;
}

QVector<QColor> Colormap::boneColormap(int l)
{
  QVector<QColor> colors;
  colors.resize(l);
  int n = 3 * l / 8;
  double r, g, b, grey;
  for (int i = 0; i < l; i++)
    {
      if (i < n)
	b = 1.*(i+1) / n;
      else
	b = 1.;
      if (i < n)
	g = 0.;
      else if (i >= n && i < 2 * n)
	g = 1. * (i+1 - n) / n;
      else
	g = 1.;
      if (i < 2 * n)
	r = 0.;
      else
	r = 1. * (i + 1 - 2 * n) / (l - 2 * n);

      grey = 7*i/(l - 1);

      colors[i] = QColor(int((grey + r)/8*MaxRGB), int((grey + g)/8*MaxRGB),
			 int((grey + b)/8*MaxRGB), MaxRGB);
    }
  return colors;
}

QVector<QColor> Colormap::copperColormap(int n)
{ 
  QVector<QColor> colors;
  colors.resize(n);

  double r, g, b, grey;
    
  for (int i = 0; i < n; ++ i)
    {
      grey = i/(n - 1.);

      r = std::min(1.2500 * grey, 1.);
      g = std::min(0.7812 * grey, 1.);
      b = std::min(0.4975 * grey, 1.);	

      colors[i] = QColor(int(r), int(g), int(b), MaxRGB);
    }
  return colors;
}

QVector<QColor> Colormap::coolColormap(int n)
{ 
  QVector<QColor> colors;
  colors.resize(n);

  float cool;
  for (int i = 0; i < n; ++ i)
    {
      cool = float(i)/(n - 1.);
      colors[i] = QColor(int(cool*MaxRGB), int((1. - cool)*MaxRGB), 
			 int(1.*MaxRGB), MaxRGB);
    }
  return colors;
}
