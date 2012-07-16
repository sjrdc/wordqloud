#include <QtGui>
#include "colourschemedialog.moc"

ColourschemeDialog::ColourschemeDialog(QList<QColor> initialScheme,
				       QWidget *parent)
  : QDialog(parent, Qt::Dialog)
{
  colourscheme = initialScheme;
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
				   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QHBoxLayout *hlayout = new QHBoxLayout;
  buttonGroup = new QButtonGroup;
  connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
	  this, SLOT(onButtonClicked(QAbstractButton*)));
  int c = 0;
  foreach(QColor colour, initialScheme)
    {
      QPushButton *label = new QPushButton;
      label->setMinimumSize(QSize(100, 100));
      label->setAutoFillBackground(true);
      int alpha  = 255;
      label->setStyleSheet("QPushButton { background-color: " +
  			    colour.name() + "; }");
      buttonGroup->addButton(label, c++);
      hlayout->addWidget(label);
    }

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addItem(hlayout);
  layout->addWidget(buttonBox);
  
  this->setLayout(layout);
  this->setMinimumSize(QSize(500, 100));
}

void ColourschemeDialog::onButtonClicked(QAbstractButton *button)
{
  int id = buttonGroup->id(button);
  QColor colour = 
    QColorDialog::getColor(colourscheme[id],
			   this, "Select background color");
  colourscheme[id] = colour;
  button->setStyleSheet("QPushButton { background-color: " +
			colour.name() + "; }");
}
