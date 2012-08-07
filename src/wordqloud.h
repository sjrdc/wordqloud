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

class WordQloud : public QMainWindow
{
  Q_OBJECT

  public:
  WordQloud();

protected:
  void contextMenuEvent(QContextMenuEvent *event);
  void renderImage();
		    
private slots:
  void about();
  void createCloudBoundsFromImage();
  void load();
  void loadWordlist();
  void open();
  void onOrientationAction(QAction*);
  void onColourschemeActionGroupTriggered(QAction*);
  void onColourVariationAction(QAction*);
  void onLayoutBoundsAction(QAction*);
  void onUnpinAllButtonClicked();
  void reCreateLayout();
  void saveBitmap();
  void savePDF();  
  void setBackgroundColor();
  void setCustomScheme();
  void setFont();
  void spinColours();
  void spinOrientations();  
  
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

  QActionGroup *layoutBoundsActionGroup;
  QAction *sceneBoundOnlyAction;
  QAction *imageBoundOnlyAction;
  QAction *imageAndSceneBoundAction;    

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

  QGraphicsView *view;
  Canvas *canvas;
};

#endif
