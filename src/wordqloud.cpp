#include <QAction>
#include <QColorDialog>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QFontDialog>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPrinter>
#include <QPushButton>
#include <QStatusBar>
#include <QSvgGenerator>
#include <QVBoxLayout>

#include <ctime>

#include "boundsdialog.h"
#include "colourschemedialog.h"
#include "canvas.h"
#include "view.moc"
#include "wordqloud.moc"
#include "word.h"
#include "wordlist.h"

WordQloud::WordQloud()
{
  QWidget *widget = new QWidget;
  setCentralWidget(widget);

  QWidget *topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  canvas = new Canvas;
  view = new View(canvas);
  
  QWidget *bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  unpinAllButton = new QPushButton("unpin all");
  connect(unpinAllButton, SIGNAL(clicked()), this, SLOT(onUnpinAllButtonClicked()));
  reCreateLayoutButton = new QPushButton("re-create layout");
  connect(reCreateLayoutButton, SIGNAL(clicked()), this, SLOT(reCreateLayout()));  
  spinColoursButton = new QPushButton("spin colours");
  connect(spinColoursButton, SIGNAL(clicked()), this, SLOT(spinColours()));   
  spinOrientationsButton = new QPushButton("spin orientation");
  connect(spinOrientationsButton, SIGNAL(clicked()), 
	  this, SLOT(spinOrientations()));  
  
  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(spinColoursButton);
  buttonLayout->addWidget(spinOrientationsButton); 
  buttonLayout->addWidget(unpinAllButton);  
  buttonLayout->addWidget(reCreateLayoutButton);
			  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(5);
  layout->addWidget(topFiller);
  layout->addWidget(view);
  layout->addItem(buttonLayout);
  layout->addWidget(bottomFiller);
  widget->setLayout(layout);

  createActions();
  createMenus();

  setWindowTitle(tr("wordQloud"));
  setMinimumSize(400, 600);
}

void WordQloud::about()
{
  QMessageBox::about(this, tr("About wordQloud"),
		     tr("short description of how it came to be..."));
}

void WordQloud::addColourVariations(QList<QColor> &colourlist, ColourVariation v)
{
  int nrColourvariations;
  int hmax;
  switch (v)
    {
    case AsPalette:
      nrColourvariations = 0;
      break;
    case LittleVariation:
      nrColourvariations = 2;
      hmax = 25;
      break;
    case SomeVariation:
      nrColourvariations = 4;
      hmax = 30;
      break;
    case LotsOfVariation:
      nrColourvariations = 5;
      hmax = 35;
      break;
    case WildVariation:
      nrColourvariations = 7;
      hmax = 40;
      break;
    }

  // create random number generator
  boost::mt19937 rng;
  rng.seed(static_cast<unsigned int>(std::time(0)));
  boost::uniform_int<> unidist(-hmax, hmax);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > huePicker(rng, unidist);
  
  QList<QColor> newColours;
  foreach (QColor colour, colourlist)
    {
      // compute colourvariations
      QColor newColour = colour.toHsv();
      int h = newColour.hue();
      for (int i = 0; i < nrColourvariations; ++i)
	{
	  int hplus = huePicker();
	  while (hplus > 360) hplus -= 360;
	  while (hplus < 0) hplus += 360;
	  newColours.push_back(QColor::fromHsv((h + hplus) % 360,
					       newColour.saturation(), 
					       newColour.value()).toRgb());
	}
    }
  colourlist.append(newColours);
}

QList<QColor> WordQloud::checkedColourscheme()
{
  QList<QVariant> varlist = 
    colourschemeActionGroup->checkedAction()->data().toList();
  QList<QColor> colourlist;
  foreach(QVariant var, varlist)
    colourlist.push_back(QColor(var.toInt()));

  return colourlist;
}

ColourVariation WordQloud::checkedColourVariation()
{
  return (ColourVariation)colourVariationActionGroup->
    checkedAction()->data().toInt();
}

void WordQloud::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  menu.addAction(backgroundColorAction);
  menu.exec(event->globalPos());
}

void WordQloud::createActions()
{
  horizontalOrientationAction = new QAction(tr("horizontal"), this);
  horizontalOrientationAction->setCheckable(true);
  horizontalOrientationAction->setData(HorizontalWordOrientation);
  anyOrientationAction = new QAction(tr("any"), this);
  anyOrientationAction->setCheckable(true);
  anyOrientationAction->setData(AnyWordOrientation);
  mostlyHorizontalOrientationAction = new QAction(tr("mostly horizontal"), this);
  mostlyHorizontalOrientationAction->setData(MostlyHorizontalWordOrientation);
  mostlyHorizontalOrientationAction->setCheckable(true);  
  halfAndHalfOrientationAction = new QAction(tr("half and half"), this);
  halfAndHalfOrientationAction->setData(HalfAndHalfWordOrientation);
  halfAndHalfOrientationAction->setCheckable(true);  
  mostlyVerticalOrientationAction = new QAction(tr("mostly vertical"), this);
  mostlyVerticalOrientationAction->setData(MostlyVerticalWordOrientation);
  mostlyVerticalOrientationAction->setCheckable(true);  
  verticalOrientationAction = new QAction(tr("vertical"), this);
  verticalOrientationAction->setData(VerticalWordOrientation);
  verticalOrientationAction->setCheckable(true);  

  orientationActionGroup = new QActionGroup(this);
  orientationActionGroup->addAction(horizontalOrientationAction);
  orientationActionGroup->addAction(mostlyHorizontalOrientationAction);
  orientationActionGroup->addAction(halfAndHalfOrientationAction);
  orientationActionGroup->addAction(mostlyVerticalOrientationAction);
  orientationActionGroup->addAction(verticalOrientationAction);
  orientationActionGroup->addAction(anyOrientationAction);
  orientationActionGroup->setExclusive(true);
  horizontalOrientationAction->setChecked(true);
  connect(orientationActionGroup, SIGNAL(triggered(QAction*)),
	  this, SLOT(onOrientationAction(QAction*)));

  asPaletteAction = new QAction(tr("as palette"), this);
  asPaletteAction->setCheckable(true);
  asPaletteAction->setData(AsPalette);
  littleVariationAction = new QAction(tr("a little variation"), this);
  littleVariationAction->setData(LittleVariation);
  littleVariationAction->setCheckable(true);  
  someVariationAction = new QAction(tr("some variation"), this);
  someVariationAction->setData(SomeVariation);
  someVariationAction->setCheckable(true);
  lotsOfVariationAction = new QAction(tr("lots of variation"), this);
  lotsOfVariationAction->setData(LotsOfVariation);
  lotsOfVariationAction->setCheckable(true);
  wildVariationAction = new QAction(tr("wild variation"), this);
  wildVariationAction->setCheckable(true);
  wildVariationAction->setData(WildVariation);
  
  colourVariationActionGroup = new QActionGroup(this);
  colourVariationActionGroup->addAction(asPaletteAction);
  colourVariationActionGroup->addAction(littleVariationAction);
  colourVariationActionGroup->addAction(someVariationAction);
  colourVariationActionGroup->addAction(lotsOfVariationAction);
  colourVariationActionGroup->addAction(wildVariationAction);
  colourVariationActionGroup->setExclusive(true);
  littleVariationAction->setChecked(true);
  connect(colourVariationActionGroup, SIGNAL(triggered(QAction*)),
	  this, SLOT(onColourVariationAction(QAction*)));
  
  customColourschemeAction = new QAction(tr("Custom..."), this);
  customColourschemeAction->setCheckable(true);
  connect(customColourschemeAction, SIGNAL(triggered()), 
	  this, SLOT(setCustomScheme()));

  backgroundColorAction = new QAction(tr("Set background color"), this);
  connect(backgroundColorAction, SIGNAL(triggered()), 
	  this, SLOT(setBackgroundColor()));
  
  loadAction = new QAction(tr("Load &text..."), this);
  loadAction->setStatusTip(tr("load text file"));
  connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));

  loadWordlistAction = new QAction(tr("Load word list..."), this);
  connect(loadWordlistAction, SIGNAL(triggered()), this, SLOT(loadWordlist()));
  
  openAction = new QAction(tr("&Open..."), this);
  openAction->setShortcuts(QKeySequence::Open);
  openAction->setStatusTip(tr("Open an existing file"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

  saveSvgAction = new QAction(tr("&Save PDF"), this);
  saveSvgAction->setShortcuts(QKeySequence::Save);
  saveSvgAction->setStatusTip(tr("Save the document to disk"));
  connect(saveSvgAction, SIGNAL(triggered()), this, SLOT(savePDF()));

  savePngAction = new QAction(tr("&Save bitmap"), this);
  savePngAction->setStatusTip(tr("Save the document to disk"));
  connect(savePngAction, SIGNAL(triggered()), this, SLOT(saveBitmap()));

  exitAction = new QAction(tr("E&xit"), this);
  exitAction->setShortcuts(QKeySequence::Quit);
  exitAction->setStatusTip(tr("Exit the application"));
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  aboutAction = new QAction(tr("&About"), this);
  aboutAction->setStatusTip(tr("Show the application's About box"));
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  fontAction = new QAction(tr("&Font"), this);
  connect(fontAction, SIGNAL(triggered()), this, SLOT(setFont()));

  boundsFromImageAction = new QAction(tr("&Bounds from image"), this);
  connect(boundsFromImageAction, SIGNAL(triggered()), 
	  this, SLOT(createCloudBoundsFromImage()));
}

void WordQloud::createCloudBoundsFromImage()
{
  BoundsDialog *b = new BoundsDialog;
  b->exec();

  if (b->result() == QDialog::Accepted)
    canvas->setBoundingRegions(b->getRegions());

  // clean up
  delete b;

  // re-do the layout
  canvas->reCreateLayout();
}

QIcon WordQloud::createColourschemeIcon(QColor backgroundColour,
					QVector<QColor> foregroundColours)
{
  int bsize = 7;
  int squaresize = 20;
  int between = 5;
    
  QImage iconImage(bsize + (squaresize + between)*foregroundColours.size() - 
		   between + bsize, bsize*2 + squaresize, QImage::Format_RGB32);
  iconImage.fill(backgroundColour);

  QPainter painter(&iconImage);
  int c = 0;
  foreach (QColor colour, foregroundColours)
    {
      QRect rectangle(bsize + c*(squaresize + bsize), bsize, 
		      squaresize, squaresize);
      painter.setBrush(colour);
      painter.drawRect(rectangle);
      c++;
    }
  painter.end();
  
  return QIcon(QPixmap::fromImage(iconImage));
}

void WordQloud::createColourschemeMenu()
{
  QMenu *colourschemeMenu = layoutMenu->addMenu(tr("&Colours"));
  colourschemeMenu->addAction(customColourschemeAction);
  colourschemeMenu->addSeparator();

  colourschemeActionGroup = new QActionGroup(this);
  colourschemeActionGroup->addAction(customColourschemeAction);
  
  QFile colourfile("../src/colourschemes.txt");
  if (!colourfile.open(QIODevice::ReadOnly))		
    qDebug() << "Could not read colourscheme file";

  QTextStream textstream(&colourfile);
  int c = 0;
  while (!textstream.atEnd() && textstream.status() == QTextStream::Ok)
    {
      QString line = textstream.readLine();
      QStringList colourlist = line.split(' ');

      if (colourlist.size() > 2)
	{
	  // extract colourscheme name
	  QString schemeName = colourlist.first();
	  colourlist.pop_front();

	  // extract backgroundcolour
	  QColor backgroundColour(colourlist.first());
	  colourlist.pop_front();

	  // a varlist to store colours in the action object
	  QList<QVariant> varlist;
	  varlist.push_back(backgroundColour.rgb()); 

	  // extract all foregroundcolours;
	  QVector<QColor> foregroundColours;
	  foreach (QString colourstring, colourlist)
	    {
	      QColor colour(colourstring);
	      foregroundColours.push_back(colour);
	      varlist.push_back(QVariant(QColor(colour).rgb()));
	    }

	  // create an icon to use in the menu
	  QIcon schemeIcon = createColourschemeIcon(backgroundColour,
						    foregroundColours);

	  // create action for the current scheme
	  QAction *action = new QAction(schemeIcon, schemeName, this);
	  action->setData(varlist);
	  action->setCheckable(true);
	  
	  // add action to menu and group
	  colourschemeMenu->addAction(action);
	  colourschemeActionGroup->addAction(action);

	  if (c == 0) action->setChecked(true);
	  c++;
	}
    }

  colourschemeActionGroup->setExclusive(true);
  customColourschemeAction->
    setData(colourschemeActionGroup->checkedAction()->data());

  // done - close the file
  colourfile.close();

  // hook up the triggered signal to an event handler
  connect(colourschemeActionGroup, SIGNAL(triggered(QAction*)),
	  this, SLOT(onColourschemeActionGroupTriggered(QAction*)));
}

void WordQloud::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(loadAction);
  fileMenu->addAction(loadWordlistAction);
  fileMenu->addAction(openAction);  
  fileMenu->addAction(saveSvgAction);
  fileMenu->addAction(savePngAction);  
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  layoutMenu = menuBar()->addMenu(tr("&Layout"));
  QMenu *orientationMenu = layoutMenu->addMenu(tr("&Orientation"));
  orientationMenu->addAction(horizontalOrientationAction);
  orientationMenu->addAction(mostlyHorizontalOrientationAction);
  orientationMenu->addAction(halfAndHalfOrientationAction);
  orientationMenu->addAction(mostlyVerticalOrientationAction);
  orientationMenu->addAction(verticalOrientationAction); 
  orientationMenu->addSeparator();
  orientationMenu->addAction(anyOrientationAction);        
        
  
  layoutMenu->addAction(fontAction);
  layoutMenu->addAction(boundsFromImageAction);

  QMenu *colourVariationMenu = layoutMenu->addMenu(tr("Colour &variation"));
  colourVariationMenu->addAction(asPaletteAction);
  colourVariationMenu->addAction(littleVariationAction);
  colourVariationMenu->addAction(someVariationAction);
  colourVariationMenu->addAction(lotsOfVariationAction);
  colourVariationMenu->addAction(wildVariationAction);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction);

  createColourschemeMenu();
}

void WordQloud::load()
{
  QString filename = 
    QFileDialog::getOpenFileName(this, "Load text file");

  QAction *schemeAction = colourschemeActionGroup->checkedAction();
  QAction *colourVariationAction = colourVariationActionGroup->checkedAction();
  
  QList<QVariant> varlist = schemeAction->data().toList();
  QColor backgroundColour(varlist.first().toInt());
  varlist.pop_front();

  QList<QColor> colourlist;
  foreach(QVariant var, varlist)
    colourlist.push_back(QColor(var.toInt()));

  addColourVariations(colourlist,
		      (ColourVariation)colourVariationAction->data().toInt());
  
  WordList wordlist;
  try { wordlist.fromTextFile(filename, colourlist); }
  catch (...) 
    {
      statusBar()->showMessage("Could not create wordlist from text file " 
			       + filename);
      return;
    }
  
  canvas->setBackgroundBrush(backgroundColour);
  canvas->setWordList(wordlist);
  canvas->createLayout();
}

void WordQloud::loadWordlist()
{
  QString filename = 
    QFileDialog::getOpenFileName(this, "Load text file");

  WordList wordlist;
  try { wordlist.fromWordFile(filename); }
  catch (...) 
    {
      statusBar()->showMessage("Could not create wordlist from text file " 
			       + filename);
      return;
    }
  
  canvas->setWordList(wordlist);
  canvas->createLayout();
}

void WordQloud::open()
{
}

void WordQloud::onColourschemeActionGroupTriggered(QAction *a)
{
  QList<QColor> colourlist = checkedColourscheme();
  QColor backgroundColour(colourlist.first());
  colourlist.pop_front();

  customColourschemeAction->setData(a->data());

  // create colourvariations
  addColourVariations(colourlist, 
		      this->checkedColourVariation());
  
  canvas->setBackgroundBrush(backgroundColour);
  canvas->randomiseWordColours(colourlist.toVector());
}

void WordQloud::onColourVariationAction(QAction *a)
{
  QList<QColor> colourlist = this->checkedColourscheme();
  QColor backgroundColour = colourlist.first();
  colourlist.pop_front();

  addColourVariations(colourlist, (ColourVariation)a->data().toInt());

  canvas->setBackgroundBrush(backgroundColour);
  canvas->randomiseWordColours(colourlist.toVector());
}

void WordQloud::onOrientationAction(QAction* a)
{
  canvas->randomiseOrientations((WordOrientation)a->data().toInt());
}

void WordQloud::onUnpinAllButtonClicked()
{
  canvas->unpinAll();
}

void WordQloud::reCreateLayout()
{
  canvas->reCreateLayout();
}

void WordQloud::saveBitmap()
{
  QString filename = 
    QFileDialog::getSaveFileName(this, "Save bitmap");

  QImage img(canvas->width(), canvas->height(),
	     QImage::Format_ARGB32_Premultiplied);

  QPainter painter(&img);
  canvas->render(&painter);  
  painter.end();
 
  // save image
  img.save(filename);  
}

void WordQloud::savePDF()
{
  QString filename = 
    QFileDialog::getSaveFileName(this, "Save PDF");

  QPrinter pdfprinter;
  pdfprinter.setOutputFormat(QPrinter::PdfFormat);
  pdfprinter.setOutputFileName(filename);

  QPainter painter(&pdfprinter);
  canvas->render(&painter);
  painter.end();
}

void WordQloud::setBackgroundColor()
{
  QColor color = 
    QColorDialog::getColor(canvas->backgroundBrush().color(),
			   this, "Select background color");
  canvas->setBackgroundBrush(color);
}

void WordQloud::setCustomScheme()
{
  QList<QColor> colourlist = checkedColourscheme();
  QColor backgroundColour = colourlist.first();
  colourlist.pop_front();

  ColourschemeDialog *c = new ColourschemeDialog(colourlist);
  c->exec();
  if (c->result() == QDialog::Accepted)
    {
      QList<QColor> customscheme = c->getScheme();
      
      QList<QVariant> varlist;
      varlist.push_back(backgroundColour);
      foreach(QColor colour, customscheme)
	varlist.push_back(QVariant(colour.rgb()));

      customColourschemeAction->setData(varlist); 
      addColourVariations(customscheme, 
			  this->checkedColourVariation());

      canvas->setBackgroundBrush(backgroundColour);
      canvas->randomiseWordColours(customscheme.toVector());
    }
  delete c;
}

void WordQloud::setFont()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, this);
  if (ok) canvas->setWordFont(font);
}

void WordQloud::spinColours()
{
  QList<QColor> colourlist = checkedColourscheme();
  QColor backgroundColour = colourlist.first();
  colourlist.pop_front();

  addColourVariations(colourlist, 
		      this->checkedColourVariation());

  canvas->setBackgroundBrush(backgroundColour);
  canvas->randomiseWordColours(colourlist.toVector());
}

void WordQloud::spinOrientations()
{
  this->onOrientationAction(orientationActionGroup->checkedAction());
}
