#include <boost/program_options/errors.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/smart_ptr.hpp>

#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QGraphicsItem>
#include <QPainter>
#include <QString>

#include "canvas.h"
#include "word.h"
#include "wordlist.h"

namespace po = boost::program_options;

int main(int argc, char **argv)
{   
  std::string outfile, textfile;
  int width, height;
  po::options_description desc("Allowed options");
  std::vector<std::string> orbargs;
  desc.add_options()
    ("help,h", "produce help message")
    ("debug,d", "run in debug mode")
    ("text,t",
     po::value<std::string>(&textfile)->required(),
     "input file with words to use in cloud")
    ("output,o",
     po::value<std::string>(&outfile)->default_value("cloud.png"), 
     "filename for output image file")
    ("width,w",
     po::value<int>(&width)->default_value(800), 
     "width of output image file")
    ("height,h",
     po::value<int>(&height)->default_value(600), 
     "height of output image file");

  // parse options
  po::variables_map vmap;
  try
    {
      po::store(po::command_line_parser(argc, argv).
		options(desc).run(), vmap);
      po::notify(vmap);      
    }
  catch(po::unknown_option &e)
    {
      std::cout << e.what() << " - aborting!" << std::endl;
      std::cout << desc << std::endl;
      return 1;
    }
  catch(po::error &e) // other errors
    {  
      // did the user actually just ask for help?
      if (vmap.count("help")) goto show_help;
      std::cout << e.what() << " - aborting!" << std::endl;
      return 1;
    }
  
  // does our user need any help?
  if (vmap.count("help")) 
    {
    show_help:
      std::cout << desc << std::endl;
      return 0;
    }

  QApplication app(argc, argv);
  Canvas canvas(width, height);
  canvas.setBackgroundBrush(Qt::black);
  
  WordList wordlist;
  wordlist.fromTextFile(QString::fromStdString(textfile));
  canvas.setWordList(wordlist);
  canvas.createLayout();
  
   // create image
  QImage img(width, height, QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&img);
  canvas.render(&painter);  
  if (vmap.count("debug")) 
    {
      foreach (QGraphicsItem *i, canvas.items())
	{
	  painter.setPen(((Word*)i)->brush().color());
	  ((Word*)i)->setBoundingRegionGranularity(.4);
	  foreach (QRect rect, i->boundingRegion(i->sceneTransform()).rects())
	    painter.drawRect(rect);
	}

    }
  painter.end();
 
  // save image
  img.save(QString::fromStdString(outfile));  
  
  return 0;
}
