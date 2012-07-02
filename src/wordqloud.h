#ifndef WORDQLOUD_H_
#define WORDQLOUD_H_

#include <QMainWindow>

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
  void load();
  void open();
  void randomiseOrientations();
  void reCreateLayout();
  void saveBitmap();
  void savePDF();  
  void setBackgroundColor();
  
private:
  void createActions();
  void createMenus();

  QMenu *fileMenu;
  QMenu *layoutMenu;
  QMenu *helpMenu;
  
  QActionGroup *alignmentGroup;
  QPushButton *reCreateLayoutButton;

  QAction *aboutAction;
  QAction *backgroundColorAction;
  QAction *exitAction;
  QAction *loadAction;  
  QAction *openAction;
  QAction *randomOrientationAction;
  QAction *savePngAction;
  QAction *saveSvgAction;  
  QLabel *infoLabel;

  QGraphicsView *view;
  Canvas *canvas;
};

#endif
