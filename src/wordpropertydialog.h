#ifndef WORDPROPERTYDIALOG_H_
#define WORDPROPERTYDIALOG_H_

#include <QDialog>

class Word;
class QCheckBox;
class QDialogButtonBox;
class QLabel;
class QPushButton;

class WordPropertyDialog : public QDialog
{  
  Q_OBJECT
public:
  WordPropertyDialog(Word* w, QWidget *parent = 0);
			
protected slots:
  void accept();
  void onColourChangeButtonClicked();
  void onFontChangeButtonClicked();
  
private:
  QLabel *wordPreview;
  QCheckBox *lockColour;
  QPushButton *fontChangeButton;
  QCheckBox *lockFont;
  QPushButton *colourChangeButton;    
  QDialogButtonBox *buttonBox;

  bool fontChanged;
  bool colourChanged;
  
  Word *word;
};

#endif
