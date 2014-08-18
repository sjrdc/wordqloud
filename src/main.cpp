#include <QApplication>
#include "wordqloud.h"

int main(int argc, char **argv)
{   
  QApplication app(argc, argv);
  WordQloud wordqloud;
  wordqloud.show();
  return app.exec();
}
