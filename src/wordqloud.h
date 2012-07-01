#ifndef WORDQLOUD_H_
#define WORDQLOUD_H_

#include <QMainWindow>

class Canvas;
class QAction;
class QActionGroup;
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

private slots:
  void about();
  void load();
  void open();
  void reCreateLayout();
  void save();
  
private:
  void createActions();
  void createMenus();

  QMenu *fileMenu;
  QMenu *helpMenu;
  QActionGroup *alignmentGroup;

  QPushButton *reCreateLayoutButton;

  QAction *aboutAction;
  QAction *exitAction;
  QAction *loadAction;  
  QAction *openAction;
  QAction *saveAction;

  QLabel *infoLabel;

  QGraphicsView *view;
  Canvas *canvas;
};

#endif
