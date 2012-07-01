#include <QAction>
#include <QColorDialog>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>

#include "canvas.h"
#include "wordqloud.moc"
#include "wordlist.h"

WordQloud::WordQloud()
{
  QWidget *widget = new QWidget;
  setCentralWidget(widget);

  QWidget *topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  canvas = new Canvas;
  view = new QGraphicsView(canvas);
  
  infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
			    "invoke a context menu</i>"));
  infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  infoLabel->setAlignment(Qt::AlignCenter);

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

  QString message = tr("A context menu is available by right-clicking");
  statusBar()->showMessage(message);

  setWindowTitle(tr("wordQloud"));
  setMinimumSize(160, 160);
}

void WordQloud::about()
{
  infoLabel->setText(tr("Invoked <b>Help|About</b>"));
  QMessageBox::about(this, tr("About Menu"),
		     tr("The <b>Menu</b> example shows how to create "
			"menu-bar menus and context menus."));
}

void WordQloud::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  menu.addAction(backgroundColorAction);
  menu.exec(event->globalPos());
}

void WordQloud::createActions()
{
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

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcuts(QKeySequence::Save);
  saveAction->setStatusTip(tr("Save the document to disk"));
  connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

  exitAction = new QAction(tr("E&xit"), this);
  exitAction->setShortcuts(QKeySequence::Quit);
  exitAction->setStatusTip(tr("Exit the application"));
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  aboutAction = new QAction(tr("&About"), this);
  aboutAction->setStatusTip(tr("Show the application's About box"));
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void WordQloud::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(loadAction);
  fileMenu->addAction(openAction);  
  fileMenu->addAction(saveAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

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

void WordQloud::reCreateLayout()
{
  canvas->reCreateLayout();
}

void WordQloud::save()
{
}

void WordQloud::setBackgroundColor()
{
  QColor color = 
    QColorDialog::getColor(canvas->backgroundBrush().color(),
			   this, "Select background color");
  canvas->setBackgroundBrush(color);
}
