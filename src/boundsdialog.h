#ifndef BOUNDSWIZARD_H_
#define BOUNDSWIZARD_H_

#include "cv.h"
#include <QDialog>

class QCheckBox;
class QDialogButtonBox;
class QLineEdit;
class QSlider;

class BoundsDialog : public QDialog
{  
  Q_OBJECT
public:
  BoundsDialog(QWidget *parent = 0);
  QVector<QRegion> getRegions();

protected slots:
  void onButtonClicked();
  void onFileChanged(QString filename);
  void onInvertStateChanged(int s);
  void onSliderValueChanged(int v);  

private:
  cv::Mat *img;
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> boundaryHierarchy;
  QCheckBox *invert;
  QImage *orgImage;
  QLineEdit *textbox;
  QLabel *previewLabel;
  QPixmap *pixmap;
  QSlider *slider;
  QDialogButtonBox *buttonBox;
};

#endif
