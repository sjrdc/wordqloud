#include <boost/program_options/errors.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/smart_ptr.hpp>

#include <iostream>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

namespace po = boost::program_options;

int main(int argc, char **argv)
{
  std::cout << "Hello, world!" << std::endl;
   
  std::string outfile, textfile;
  po::options_description desc("Allowed options");
  std::vector<std::string> orbargs;
  desc.add_options()
    ("help,h", "produce help message")
    ("words,w",
     po::value<std::string>(&textfile)->required(),
     "input file with words to use in cloud")
    ("output,o",
     po::value<std::string>(&outfile)->default_value("cloud.png"), 
     "filename for output image file");

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

  QFile file(QString::fromStdString(textfile));
  if (!file.open(QIODevice::ReadOnly)) 
    std::cerr << "Could not read from file" << textfile << "." << std::endl;

  QTextStream stream(&file); 
  QStringList wordlist;
  while (!stream.atEnd() && stream.status() == QTextStream::Ok)
    {
      QString line = stream.readLine();
      wordlist.append(line.split(' '));
    }

  file.close();
  
  return 0;
}
