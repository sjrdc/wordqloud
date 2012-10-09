#ifndef WORDQLOUD_H_
#define WORDQLOUD_H_

#include <QMainWindow>
#include "enumerations.h"

class Canvas;
class QAction;
class QActionGroup;
class QContextMenuEvent;
class QGraphicsView;
class QLabel;
class QMenu;
class QPushButton;
class QProgressBar;
class View;

class WordQloud : public QMainWindow
{
  Q_OBJECT

  public:
  WordQloud();

  public slots:
  void onProgressChanged(int, int);

protected:
  void contextMenuEvent(QContextMenuEvent *event);
  void renderImage();
		    
private slots:
  void about();
  void createCloudBoundsFromImage();
  void load();
  void onLoadWordlist();
  void open();
  void onOrientationActionTriggered(QAction*);
  void onColourschemeActionGroupTriggered(QAction*);
  void onColourVariationActionTriggered(QAction*);
  void onLayoutBoundsActionTriggered(QAction*);
  void onLayoutEnded();
  void onLayoutStarted();
  void onPathGroupActionTriggered(QAction*);
  void onSceneRectActionTriggered();
  void onUnpinAllButtonClicked();
  void reCreateLayout();
  void saveBitmap();
  void savePDF();  
  void setBackgroundColor();
  void setCustomScheme();
  void setFont();
  void spinColours();
  void spinOrientations();  
  void onStatusChanged(QString);
  void onStopButtonClicked();

private:
  void addColourVariations(QList<QColor> &colourlis, ColourVariation v);
  ColourVariation checkedColourVariation();
  void createActions();
  void createMenus();
  void createColourschemeMenu();
  QIcon createColourschemeIcon(QColor backgroundColour, 
			       QVector<QColor> foregroundColours);
  QList<QColor> checkedColourscheme();

  QMenu *fileMenu;
  QMenu *layoutMenu;
  QMenu *helpMenu;
  
  QAction *customColourschemeAction;
  QActionGroup *alignmentGroup;
  QPushButton *unpinAllButton;
  QPushButton *spinColoursButton;
  QPushButton *spinOrientationsButton;  
  QPushButton *reCreateLayoutButton;
  QActionGroup *colourschemeActionGroup;
  QAction *aboutAction;
  QAction *backgroundColorAction;
  QAction *exitAction;
  QAction *fontAction;
  QAction *loadAction;
  QAction *loadWordlistAction;
  QAction *openAction;
  QAction *savePngAction;
  QAction *saveSvgAction;  
  QAction *boundsFromImageAction;

  QActionGroup *pathGroup;
  QAction *spiralPathAction;
  QAction *rectPathAction;

  QActionGroup *layoutBoundsActionGroup;
  QAction *sceneBoundOnlyAction;
  QAction *imageBoundOnlyAction;
  QAction *imageAndSceneBoundAction;    

  QAction *sceneRectAction;

  QActionGroup *orientationActionGroup;
  QAction *anyOrientationAction;
  QAction *horizontalOrientationAction;
  QAction *mostlyHorizontalOrientationAction;
  QAction *halfAndHalfOrientationAction;
  QAction *mostlyVerticalOrientationAction;
  QAction *verticalOrientationAction;
    
  QActionGroup *colourVariationActionGroup;
  QAction *asPaletteAction;
  QAction *littleVariationAction;
  QAction *someVariationAction;
  QAction *lotsOfVariationAction;
  QAction *wildVariationAction;

  View *view;
  Canvas *canvas;
  QProgressBar *progressBar;
  QPushButton *stopLayoutButton;
};

#endif
