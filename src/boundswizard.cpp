#include <QtGui>
#include "boundswizard.moc"

BoundsWizard::BoundsWizard(QWidget *parent)
{
  addPage(createIntroPage());
  addPage(new SelectImagePage);   
  addPage(new ThresholdImagePage);   
}

QWizardPage *BoundsWizard::createIntroPage()
{
  QWizardPage *page = new QWizardPage;
  page->setTitle("Introduction");
  
  QLabel *label = new QLabel("This wizard will help you create cloud bounds "
			     "from an image file.");
  label->setWordWrap(true);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(label);
  page->setLayout(layout);

  return page;
}

SelectImagePage::SelectImagePage(QWidget *parent)
{
  this->setTitle("Select picture");

  textbox = new QLineEdit;
  QPushButton *btn = new QPushButton("browse...");

  preview = new QLabel;
  preview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  preview->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
  
  connect(btn, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
  connect(textbox, SIGNAL(textChanged(QString)), 
	  this, SLOT(onFileChanged(QString)));

  QHBoxLayout *hlayout = new QHBoxLayout;
  hlayout->addWidget(textbox);
  hlayout->addWidget(btn);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addItem(hlayout);
  layout->addWidget(preview);
  
  this->setLayout(layout);

  registerField("imageFilename*", textbox);
}

void SelectImagePage::onButtonClicked()
{
  QString filename = QFileDialog::getOpenFileName(this, "Select picture");
  if (!filename.isEmpty()) textbox->setText(filename);
}

void SelectImagePage::onFileChanged(QString filename)
{
  QPixmap pixmap(textbox->text());
  if (!pixmap.isNull())
    preview->setPixmap(pixmap.scaled(preview->width(), preview->height(),
				     Qt::KeepAspectRatio));
}

ThresholdImagePage::ThresholdImagePage(QWidget *parent)
{
  this->setTitle("Set threshold");
  
  previewLabel = new QLabel;
  previewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  previewLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

  slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, 255);
  slider->setValue(128);

  connect(slider, SIGNAL(valueChanged(int)),
	  this, SLOT(onSliderValueChanged(int)));
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(previewLabel);
  layout->addWidget(slider);
  this->setLayout(layout);

  bwcolors.push_back(Qt::black);
  bwcolors.push_back(Qt::white);
}

void ThresholdImagePage::initializePage()
{
  imagefile = field("imageFilename").toString();
  orgImage = new QImage(imagefile);
  
  pixmap = new QPixmap(imagefile);
  if (!pixmap->isNull())
    previewLabel->setPixmap(pixmap->scaled(previewLabel->width(), 
  					   previewLabel->height(), 
  					   Qt::KeepAspectRatio));
}

void ThresholdImagePage::onSliderValueChanged(int v)
{
  QImage tmp = *orgImage;
  for (int i = 0; i < tmp.width(); ++i)
    for (int j = 0; j < tmp.height(); ++j)
      {
	QRgb c = tmp.pixel(i, j) < QColor(v, v, v).rgb() ? 
	  QColor(0, 0, 0).rgb() : QColor(255, 255, 255).rgb();
	
  	tmp.setPixel(i, j, c);
      }

  pixmap = new QPixmap(QPixmap::fromImage(tmp));
  previewLabel->setPixmap(pixmap->scaled(previewLabel->width(), 
  					   previewLabel->height(), 
  					   Qt::KeepAspectRatio));
}
