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

#include "boundsdialog.h"
#include "canvas.h"
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
  view = new QGraphicsView(canvas);
  
  QWidget *bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  reCreateLayoutButton = new QPushButton("re-create layout");
  connect(reCreateLayoutButton, SIGNAL(clicked()), this, SLOT(reCreateLayout()));  
  
  QHBoxLayout *buttonLayout = new QHBoxLayout;
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

void WordQloud::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  menu.addAction(backgroundColorAction);
  menu.exec(event->globalPos());
}

void WordQloud::createActions()
{
  horizontalOrientationAction = new QAction(tr("horizontal"), this);
  horizontalOrientationAction->setData(HorizontalWordOrientation);
  mostlyHorizontalOrientationAction = new QAction(tr("mostly horizontal"), this);
  mostlyHorizontalOrientationAction->setData(MostlyHorizontalWordOrientation);
  halfAndHalfOrientationAction = new QAction(tr("half and half"), this);
  halfAndHalfOrientationAction->setData(HalfAndHalfWordOrientation);
  mostlyVerticalOrientationAction = new QAction(tr("mostly vertical"), this);
  mostlyVerticalOrientationAction->setData(MostlyVerticalWordOrientation);
  verticalOrientationAction = new QAction(tr("vertical"), this);
  verticalOrientationAction->setData(VerticalWordOrientation);

  orientationActionGroup = new QActionGroup(this);
  orientationActionGroup->addAction(horizontalOrientationAction);
  orientationActionGroup->addAction(mostlyHorizontalOrientationAction);
  orientationActionGroup->addAction(halfAndHalfOrientationAction);
  orientationActionGroup->addAction(mostlyVerticalOrientationAction);
  orientationActionGroup->addAction(verticalOrientationAction);
  connect(orientationActionGroup, SIGNAL(triggered(QAction*)),
	  this, SLOT(onOrientationAction(QAction*)));
  
  backgroundColorAction = new QAction(tr("Set background color"), this);
  connect(backgroundColorAction, SIGNAL(triggered()), 
	  this, SLOT(setBackgroundColor()));
  
  loadAction = new QAction(tr("&Load text..."), this);
  loadAction->setStatusTip(tr("load text file"));
  connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));

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

  canvas->setBoundingRegions(b->getRegions());
  delete b;
}

void WordQloud::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(loadAction);
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
  
  layoutMenu->addAction(fontAction);
  layoutMenu->addAction(boundsFromImageAction);
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction);
}

void WordQloud::load()
{
  QString filename = 
    QFileDialog::getOpenFileName(this, "Load text file");

  WordList wordlist;
  try { wordlist.fromTextFile(filename); }
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

void WordQloud::onOrientationAction(QAction* a)
{
  canvas->randomiseOrientations((WordOrientation)a->data().toInt());
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

void WordQloud::setFont()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, this);
  if (ok) canvas->setWordFont(font);
}
