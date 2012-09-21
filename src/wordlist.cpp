#include <QBrush>
#include <QDebug>
#include <QFile>
#include <QGraphicsItem>
#include <QPen>
#include <QTextStream>

#include <math.h>
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

void WordList::fromTextFile(QString filename, const QList<QColor> &colourlist)
{
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
	map[s.remove(QRegExp("[().,:;`\'!?0-9]")).trimmed()]++;
    }
  file.close();

  std::vector<std::pair<QString, int> > histogram(map.size());
  std::copy(map.begin(), map.end(), histogram.begin());
  map.clear();
  std::sort(histogram.begin(), histogram.end(), compareWords);

  int counter = 0;
  char blacklistedCharacters [6] = {'\\', '{', '}', '$', '~', '%'};
  for (std::vector<std::pair<QString, int> >::const_iterator i = histogram.begin();
       i != histogram.end(); ++i)
    {
      
      if (!(i->first.toStdString().find_first_of(blacklistedCharacters) < std::string::npos) && i->first.size() > 1)
	{
	  qDebug() << i->first << i->first.toStdString().find_first_of(blacklistedCharacters) << i->second;
	  Word *w = new Word(i->first);
	  float f = log10(i->second);
	  w->setFontsize(10*(f > 0 ? 2*f : 1));
	  w->setColour(colourlist[counter % colourlist.size()]);
	  this->push_back(w);
	  counter++;
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
      QString line = stream.readLine();
      QStringList words = line.split(' ');

      foreach (QString string, words)
	if (!string.isEmpty())
	  {
	    // find possible modifiers
	    QStringList modifiers = string.split(':', QString::SkipEmptyParts);
	    string = modifiers[0];
	    modifiers.pop_front();
	    Word *word = new Word(string);
	    foreach (QString modifier, modifiers)
	      {
		char key = modifier[0].toAscii();
		switch(key)
		  {
		  case '#':
		    {
		      if (modifier.size() == 7)
			{
			  word->setColour(QColor(modifier));
			  word->lockColour();
			}
		      break;
		    }
		  case '@':
		    {
		      double angle = modifier.right(1).toDouble();
		      word->setRotation(angle);
		      word->lockOrientation();
		      break;
		    }
		  case '!':
		    {
		      word->setFontName(modifier.right(1));
		      word->lockFont();
		      break;
		    }
		  default: 
		    {
		      int fsize = modifier.toInt();
		      word->setFontsize(fsize);
		      word->lockFontsize();
		      break;
		    }
		  }
	      }
	    this->push_back(word);
	  }
    }
  file.close();
}
