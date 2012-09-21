#ifndef WORDLIST_H_
#define WORDLIST_H_

#include <vector>
#include "word.h"

class WordList : public QList<Word*>
{
public:
  float area();
  void fromTextFile(QString filename, const QList<QColor> &colourlist);
  void fromWordFile(QString filename);
protected:
  void initBlackList();
  std::vector<QString> blacklist;
private:
};

#endif
