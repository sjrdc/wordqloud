#ifndef BOUNDINGRECTDIALOG_H_
#define BOUNDINGRECTDIALOG_H_

#include <QDialog>

class QAbstractButton;
class QDialogButtonBox;
class QDoubleSpinBox;

class BoundingRectDialog : public QDialog
{  
  Q_OBJECT
public:
  BoundingRectDialog(QRectF initRect, QWidget *parent = 0);
  QRectF getRect() const;
			
protected slots:
  void heightChanged(float);
  void widthChanged(float);  

private:
  QRectF rect;

  QDoubleSpinBox *widthSpinner;
  QDoubleSpinBox *heightSpinner;  
  QDialogButtonBox *buttonBox;
};

inline QRectF BoundingRectDialog::getRect() const { return rect; }

#endif
