#ifndef BOUNDINGRECTDIALOG_H_
#define BOUNDINGRECTDIALOG_H_

#include <QDialog>

class QAbstractButton;
class QDialogButtonBox;
class QPushButton;
class QSlider;

class ColourschemeDialog : public QDialog
{  
  Q_OBJECT
public:
  BoundingRectDialog(QList<QColor> initialScheme, QWidget *parent = 0);
			     
protected slots:
  
private:
  QRectF rect;
};

inline QList<QColor> BoundingRectDialog::getRect() const { return rect; }

#endif
