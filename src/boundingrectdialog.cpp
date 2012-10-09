#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QVBoxLayout>

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
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(widthSpinner);
  layout->addWidget(heightSpinner);
  layout->addWidget(buttonBox);
  
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
