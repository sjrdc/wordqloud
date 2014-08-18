#ifndef COLOURSCHEMEDIALOG_H_
#define COLOURSCHEMEDIALOG_H_

#include <QDialog>

class QAbstractButton;
class QDialogButtonBox;
class QLineEdit;
class QPushButton;
class QSlider;

class ColourschemeDialog : public QDialog
{  
  Q_OBJECT
public:
  ColourschemeDialog(QList<QColor> initialScheme, QWidget *parent = 0);
  QList<QColor> getScheme();
			     
protected slots:
  void onColourButtonClicked(QAbstractButton *button);
  void onPlusButtonClicked();
  
private:
  QList<QColor> colourscheme;
  QPushButton *plusButton;
  QButtonGroup *buttonGroup;
  QDialogButtonBox *buttonBox;

};

inline QList<QColor> ColourschemeDialog::getScheme() { return colourscheme; }

#endif
