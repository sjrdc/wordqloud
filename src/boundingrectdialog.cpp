#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>

#include "boundingrectdialog.moc"

BoundingRectDialog::BoundingRectDialog(QRectF initRect, QWidget *parent)
  : QDialog(parent, Qt::Dialog)
{
  rect = initRect;

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
				   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  widthSpinner = new QDoubleSpinBox();
  widthSpinner->setRange(0, 1000000);
  widthSpinner->setValue(rect.width());
  connect(widthSpinner, SIGNAL(valueChanged(double)),
	  this, SLOT(widthChanged(double)));

  heightSpinner = new QDoubleSpinBox();
  heightSpinner->setRange(0, 1000000);
  heightSpinner->setValue(rect.height());
  connect(heightSpinner, SIGNAL(valueChanged(double)),
	  this, SLOT(heightChanged(double)));
  
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(new QLabel("Width"), 0, 0);
  layout->addWidget(widthSpinner, 0, 1);
  layout->addWidget(new QLabel("Height"), 1, 0);  
  layout->addWidget(heightSpinner, 1, 1);
  layout->addWidget(buttonBox, 2, 0, 1, 2);
  
  this->setLayout(layout);
  this->setMinimumSize(QSize(500, 100));
}

void BoundingRectDialog::heightChanged(double f)
{
  rect.setHeight(f);
}

void BoundingRectDialog::widthChanged(double f)
{
  rect.setWidth(f);
}
