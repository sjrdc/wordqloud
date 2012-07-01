#ifndef WORDQLOUD_H_
#define WORDQLOUD_H_

#include <QMainWindow>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class WordQloud : public QMainWindow
{
  Q_OBJECT

  public:
  WordQloud();

protected:
  void contextMenuEvent(QContextMenuEvent *event);

private slots:
  void open();
  void save();
  void about();
  
private:
  void createActions();
  void createMenus();

  QMenu *fileMenu;
  QMenu *helpMenu;
  QActionGroup *alignmentGroup;

  QAction *openAction;
  QAction *saveAction;
  QAction *exitAction;
  QAction *aboutAction;

  QLabel *infoLabel;
};

#endif
