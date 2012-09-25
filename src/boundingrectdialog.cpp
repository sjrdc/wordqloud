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
  widthSpinner->setValue(rect.width());
  connect(widthSpinner, SIGNAL(valueChanged(float)),
	  this, SLOT(widthChanged(float)));

  heightSpinner = new QDoubleSpinBox();
  heightSpinner->setValue(rect.height());
  connect(heightSpinner, SIGNAL(valueChanged(float)),
	  this, SLOT(heightChanged(float)));
  
  QHBoxLayout *hlayout = new QHBoxLayout;
  hlayout->addWidget(widthSpinner);
  hlayout->addWidget(heightSpinner);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addItem(hlayout);
  layout->addWidget(buttonBox);
  
  this->setLayout(layout);
  this->setMinimumSize(QSize(500, 100));
}

void BoundingRectDialog::heightChanged(float f)
{
  rect.setHeight(f);
}

void BoundingRectDialog::widthChanged(float f)
{
  rect.setWidth(f);
}
