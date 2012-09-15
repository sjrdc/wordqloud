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

void WordList::fromTextFile(QString filename, const QList<QColor> &colourlist)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) 
    qDebug() << "Could not read from file " << filename << ".";
  
  QTextStream stream(&file); 
  QStringList stringlist;
  while (!stream.atEnd() && stream.status() == QTextStream::Ok)
    {
      QString line = stream.readLine();
      stringlist.append(line.split(' '));
    }
  file.close();

  std::map<QString, int> histogram;
  foreach (QString s, stringlist)
    histogram[s]++;

  int counter = 0;
  // for (int i = 0; i < stringlist.size(); ++i)
  //   {
  //     QString s = stringlist[i];
  //     if (!s.isEmpty())
  // 	{
  // 	  Word *w = new Word(s);
  // 	  w->setFontsize(12);
  // 	  w->setColour(colourlist[counter % colourlist.size()]);
  // 	  this->push_back(w);
  // 	  counter++;
  // 	}
  //   }

  for (std::map<QString, int>::const_iterator i = histogram.begin();
       i != histogram.end(); ++i)
    {
      if (i->first.size() > 0 &&i->first.size() < 50)
	{
	  qDebug() << i->first << i->second;
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
