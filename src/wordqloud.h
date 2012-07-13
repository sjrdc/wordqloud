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
  void open();
  void onOrientationAction(QAction*);
  void reCreateLayout();
  void saveBitmap();
  void savePDF();  
  void setBackgroundColor();
  void setFont();
  
private:
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

  QAction *aboutAction;
  QAction *backgroundColorAction;
  QAction *exitAction;
  QAction *fontAction;
  QAction *loadAction;  
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
    
  QGraphicsView *view;
  Canvas *canvas;
};

#endif
