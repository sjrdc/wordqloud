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
  cv::Mat tmp;
  tmp.create(img->rows, img->cols, CV_8UC1);

  cv::cvtColor(img, tmp, CV_BGR2GRAY);
  cv::threshold(tmp, tmp, v, 255, CV_THRESH_BINARY);

  std::vector<std::vector<cv::Point> > contours;
    cv::findContours(tmp, contours,
		     CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

  QVector<QPolygon> polygons;
  for (int c = 0; c < contours.size(); c++)
    {
      // retrieve outer connected component polygon
      QPolygon polygon;
      for (int i = 0; i < contours[c].size(); ++i)
	{
	  polygon.push_back(QPoint(contours[c][i].x, contours[c][i].y));	  
	}
      polygons.push_back(polygon);
    }

  QImage tmpimg = *orgImage;
  for (int i = 0; i < tmpimg.width(); ++i)
    for (int j = 0; j < tmpimg.height(); ++j)
      {
  	QColor c = tmpimg.pixel(i, j);
	float val = (11.*c.red() + 16.*c.green() + 5.*c.green())/32.;
  	tmpimg.setPixel(i, j, val < v ? 
			QColor(0, 0, 0).rgb() : QColor(255, 255, 255).rgb());
      }

  QPainter p(&tmpimg);
  p.setPen(Qt::red);
  foreach (QPolygon polygon, polygons)
    p.drawPolygon(polygon);

  // previewLabel->setPixmap(pixmap->scaled(previewLabel->width(), 
  // 					 previewLabel->height(), 
  // 					 Qt::KeepAspectRatio));

  pixmap = new QPixmap(QPixmap::fromImage(tmpimg));
  previewLabel->setPixmap(*pixmap);
}
