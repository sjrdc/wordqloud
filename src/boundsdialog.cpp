#include <QtGui>
#include "boundsdialog.moc"
#include "cv.h"
#include "highgui.h"

BoundsDialog::BoundsDialog(QWidget *parent)
  : QDialog(parent, Qt::Dialog)
{
  textbox = new QLineEdit;
  QPushButton *btn = new QPushButton("browse...");

  this->setMinimumSize(QSize(500, 500));
  
  previewLabel = new QLabel;
  previewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  previewLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

  slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, 255);
  slider->setValue(128);

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
				   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(btn, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
  connect(textbox, SIGNAL(textChanged(QString)), 
	  this, SLOT(onFileChanged(QString)));
  connect(slider, SIGNAL(valueChanged(int)),
	  this, SLOT(onSliderValueChanged(int)));

  QHBoxLayout *hlayout = new QHBoxLayout;
  hlayout->addWidget(textbox);
  hlayout->addWidget(btn);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addItem(hlayout);
  layout->addWidget(previewLabel);
  layout->addWidget(slider);
  layout->addWidget(buttonBox);
  
  this->setLayout(layout);
}

void BoundsDialog::onButtonClicked()
{
  QString filename = QFileDialog::getOpenFileName(this, "Select picture");
  if (!filename.isEmpty()) textbox->setText(filename);
}

void BoundsDialog::onFileChanged(QString filename)
{
  img = cvLoadImageM(filename.toStdString().c_str());
  orgImage = new QImage(filename);

  pixmap = new QPixmap(QPixmap::fromImage(*orgImage));
  if (!pixmap->isNull())
    previewLabel->setPixmap(pixmap->scaled(previewLabel->width(), 
  					   previewLabel->height(),
  					   Qt::KeepAspectRatio));

  onSliderValueChanged(slider->value());
}

void BoundsDialog::onSliderValueChanged(int v)
{
  CvMat *tmp = cvCreateMat(img->rows, img->cols, img->type);
  cvThreshold(img, tmp, v, 255, CV_THRESH_BINARY);
  
  QImage tmpimg = *orgImage;
  for (int i = 0; i < tmpimg.width(); ++i)
    for (int j = 0; j < tmpimg.height(); ++j)
      {
  	QRgb c = tmpimg.pixel(i, j);
  	tmpimg.setPixel(i, j, c < QColor(v, v, v).rgb() ? 
  		     QColor(0, 0, 0).rgb() : QColor(255, 255, 255).rgb());
      }

  pixmap = new QPixmap(QPixmap::fromImage(tmpimg));
  previewLabel->setPixmap(pixmap->scaled(previewLabel->width(), 
  					 previewLabel->height(), 
  					 Qt::KeepAspectRatio));
}
