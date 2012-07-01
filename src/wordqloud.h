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
  void about();
  void load();
  void open();
  void save();
  
private:
  void createActions();
  void createMenus();

  QMenu *fileMenu;
  QMenu *helpMenu;
  QActionGroup *alignmentGroup;

  QAction *aboutAction;
  QAction *exitAction;
  QAction *loadAction;  
  QAction *openAction;
  QAction *saveAction;

  QLabel *infoLabel;
};

#endif
