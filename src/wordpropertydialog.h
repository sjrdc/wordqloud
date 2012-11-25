#ifndef WORDPROPERTYDIALOG_H_
#define WORDPROPERTYDIALOG_H_

#include <QDialog>

class Word;
class QLabel;
class QPushButton;
class QDialogButtonBox;

class WordPropertyDialog : public QDialog
{  
  Q_OBJECT
public:
  WordPropertyDialog(Word* w, QWidget *parent = 0);
			
protected slots:
  void onColourChangeButtonClicked();
  void onFontChangeButtonClicked();
  
private:
  QLabel *wordPreview;
  QPushButton *fontChangeButton;
  QPushButton *colourChangeButton;    
  QDialogButtonBox *buttonBox;

  bool fontChanged;
  bool colourChanged;
  Word *w;
};

#endif
