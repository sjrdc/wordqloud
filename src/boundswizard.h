#ifndef BOUNDSWIZARD_H_
#define BOUNDSWIZARD_H_

#include <QWizard>
#include <QWizardPage>

class QLineEdit;
class QSlider;

class BoundsWizard : public QWizard
{  
  Q_OBJECT
public:
  
  BoundsWizard(QWidget *parent = 0);

private:
  QWizardPage* createIntroPage();  
};

class SelectImagePage : public QWizardPage
{
  Q_OBJECT
public:
  SelectImagePage(QWidget *parent = 0);
protected slots:
  void onButtonClicked();
  void onFileChanged(QString filename);

private:
  QLineEdit *textbox;
  QLabel *preview;
};

class ThresholdImagePage : public QWizardPage
{
  Q_OBJECT
  public:
  ThresholdImagePage(QWidget *parent = 0);

protected slots:
  void onSliderValueChanged(int v);
protected:
  virtual void initializePage();
  
  QString imagefile;

  QImage *orgImage;
  QImage workImage;
  QPixmap *pixmap;
  QLabel *previewLabel;
  QSlider *slider;

  QVector<QRgb> bwcolors;
};

#endif
