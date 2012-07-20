#ifndef COLOURSCHEMEDIALOG_H_
#define COLOURSCHEMEDIALOG_H_

#include <QDialog>

class QAbstractButton;
class QDialogButtonBox;
class QLineEdit;
class QSlider;

class ColourschemeDialog : public QDialog
{  
  Q_OBJECT
public:
  ColourschemeDialog(QList<QColor> initialScheme, QWidget *parent = 0);
  QList<QColor> getScheme();
			     
protected slots:
  void onButtonClicked(QAbstractButton *button);
  
private:
  QList<QColor> colourscheme;
  QDialogButtonBox *buttonBox;
  QButtonGroup *buttonGroup;
};

inline QList<QColor> ColourschemeDialog::getScheme() { return colourscheme; }

#endif
