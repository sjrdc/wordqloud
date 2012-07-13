#ifndef BOUNDSWIZARD_H_
#define BOUNDSWIZARD_H_

#include "cv.h"
#include <QWizard>
#include <QWizardPage>

class QDialogButtonBox;
class QLineEdit;
class QSlider;

class BoundsDialog : public QDialog
{  
  Q_OBJECT
public:
  BoundsDialog(QWidget *parent = 0);

protected slots:
  void onButtonClicked();
  void onFileChanged(QString filename);
  void onSliderValueChanged(int v);  

private:
  CvMat *img;
  QImage *orgImage;
  QLineEdit *textbox;
  QLabel *previewLabel;
  QPixmap *pixmap;
  QSlider *slider;
  QDialogButtonBox *buttonBox;
  QVector<QRegion> regions;
};

#endif
