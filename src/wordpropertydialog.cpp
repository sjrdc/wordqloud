#include <QCheckBox>
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "wordpropertydialog.moc"
#include "word.h"

WordPropertyDialog::WordPropertyDialog(Word *w, QWidget *parent)
{
  fontChanged = false;
  colourChanged = false;
  word = w;
  
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
				   | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  colourChangeButton = new QPushButton(tr("Change colour"));
  connect(colourChangeButton, SIGNAL(clicked()), 
	  this, SLOT(onColourChangeButtonClicked()));
  fontChangeButton = new QPushButton(tr("Change font"));  
  connect(fontChangeButton, SIGNAL(clicked()), 
	  this, SLOT(onFontChangeButtonClicked()));

  wordPreview = new QLabel(w->text());
  wordPreview->setFont(w->getFont());

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(wordPreview);
  layout->addWidget(fontChangeButton);
  layout->addWidget(colourChangeButton);

  this->setLayout(layout);
  this->setMinimumSize(QSize(200, 100));
}

void WordPropertyDialog::accept()
{
  if (colourChanged) 
    {
      word->setColour(wordPreview->palette().brush(QPalette::WindowText).color());
      if (lockColour->isChecked()) word->lockColour(true);
    }

  if (fontChanged)
    {
      word->setFont(wordPreview->font());
      if (lockFont->isChecked()) word->lockFont(true);
    }

  QDialog::accept();
}

void WordPropertyDialog::onColourChangeButtonClicked()
{  
  QPalette p = wordPreview->palette();
  QColor colour = 
    QColorDialog::getColor(p.brush(QPalette::WindowText).color(),
			   this, "Select word colour");
  p.setBrush(QPalette::WindowText, colour);
  wordPreview->setPalette(p);
  colourChanged = true;
}

void WordPropertyDialog::onFontChangeButtonClicked()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, this);
  if (ok) 
    {
      wordPreview->setFont(font);
      fontChanged = true;
    }
}
