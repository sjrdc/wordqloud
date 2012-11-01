#include <QBrush>
#include <QDebug>
#include <QFile>
#include <QGraphicsItem>
#include <QPen>
#include <QTextStream>

#include <math.h>
#include <iostream>
#include <map>

#include "colormap.h"
#include "word.h"
#include "wordlist.h"

float WordList::area()
{
  float a = .0;
  foreach (Word *word, *this)
    a += word->area();

  return a;
}

bool compareWords(std::pair<QString, int> a, std::pair<QString, int> b)
{
  return a.second > b.second;
}

void WordList::initBlackList()
{
  QFile file("../blacklist.txt");
  
  if (!file.open(QIODevice::ReadOnly)) 
    qDebug() << "Could not read from file ../blacklist.txt";
  QTextStream stream(&file);
  while (!stream.atEnd() && stream.status() == QTextStream::Ok)
    {
      QString line = stream.readLine();
      QStringList stringlist = line.split(' ');
      foreach (QString s, stringlist)  
	blacklist.push_back(s.toLower());
    }

  file.close();
  std::sort(blacklist.begin(), blacklist.end());
}

void WordList::fromTextFile(QString filename, const QList<QColor> &colourlist)
{
  if (blacklist.size() == 0) initBlackList();    

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) 
    qDebug() << "Could not read from file " << filename << ".";
  
  QTextStream stream(&file); 
  std::map<QString, int> map;
  while (!stream.atEnd() && stream.status() == QTextStream::Ok)
    {
      QString line = stream.readLine();
      QStringList stringlist = line.split(' ');
      foreach (QString s, stringlist)  
	{
	  s = s.remove(QRegExp("[().,:;`\'!?0-9]")).trimmed();
	  if (s.toStdString().find_first_of("\\{}$~%") == std::string::npos
	      && s.length() > 1)
	    map[s]++;
	}
    }
  file.close();

  std::vector<std::pair<QString, int> > histogram(map.size());
  std::copy(map.begin(), map.end(), histogram.begin());
  map.clear();
  std::sort(histogram.begin(), histogram.end(), compareWords);
  int counter = 0;
  for (std::vector<std::pair<QString, int> >::const_iterator i = histogram.begin();
       i != histogram.end(); ++i)
    {
      if (std::find(blacklist.begin(), blacklist.end(), i->first.toLower()) == blacklist.end())
	{
	  Word *w = new Word(i->first);
	  float f = log10(i->second);
	  w->setFontsize(10*(f > 0 ? 4*f : 1));
	  w->setFrequency(i->second);
	  w->setColour(colourlist[counter % colourlist.size()]);
	  this->push_back(w);
	  counter ++;
	}
    }
}

void WordList::fromWordFile(QString filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) 
    qDebug() << "Could not read from file " << filename << ".";
  
  QTextStream stream(&file); 
  QStringList stringlist;
  while (!stream.atEnd() && stream.status() == QTextStream::Ok)
    {
      Word *w = new Word;
      w->fromStream(stream);
      this->push_back(w);
    }
  file.close();
}

QSizeF WordList::maximumWordSize()
{
  float maxwidth = 0, maxheight = 0;

  foreach(Word *word, *this)
    {
      QRectF r = word->boundingRect();
      if (r.width() > maxwidth) maxwidth = r.width();
      if (r.height() > maxheight) maxheight = r.height();
    }
  
  return QSizeF(maxwidth, maxheight);
}

