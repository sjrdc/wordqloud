#include <QBrush>
#include <QDebug>
#include <QFile>
#include <QGraphicsItem>
#include <QPen>
#include <QTextStream>

#include <math.h>

#include "colormap.h"
#include "word.h"
#include "wordlist.h"

void WordList::fromTextFile(QString filename)
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

  QVector<QColor> colormap = Colormap::coolColormap(10);

  int counter = 0;
  for (int i = 0; i < stringlist.size(); ++i)
    {
      QString s = stringlist[i];
      if (!s.isEmpty())
	{
	  Word *w = new Word(s);
	  w->setFontSize(10 + 20*exp(-counter/5+1));
	  w->setBrush(colormap[counter % 10]);
	  this->push_back(w);
	  counter++;
	}
    }
}
