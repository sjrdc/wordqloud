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
  void reCreateLayout();
  void saveBitmap();
  void savePDF();  
  void setBackgroundColor();
  void setFont();
  
private:
  void addColourVariations(QList<QColor> &colourlis, ColourVariation v);
  void createActions();
  void createMenus();
  void createColourschemeMenu();
  QIcon createColourschemeIcon(QColor backgroundColour, 
			       QVector<QColor> foregroundColours);
  
  QMenu *fileMenu;
  QMenu *layoutMenu;
  QMenu *helpMenu;
  
  QActionGroup *alignmentGroup;
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

  QActionGroup *orientationActionGroup;
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
