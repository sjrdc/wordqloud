#ifndef WORDLIST_H_
#define WORDLIST_H_

#include "word.h"

class WordList : public QList<Word*>
{
public:
  void fromTextFile(QString filename, const QList<QColor> &colourlist);
  void fromWordFile(QString filename);
protected:
private:
};

#endif
