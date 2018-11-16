#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QTextStream>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
	void closeEvent(QCloseEvent* event);

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

	bool checkSuccessfullyModified();
	void initializeComponents();
	void initializeShortcuts();

private slots:
	void chooseFile();
	void setFont();
	void save();
	void saveAs();
	void setUnsaved();
	void showAbout();
	void addDate();
	void setReadOnlyText(bool);
	void displayCursorPos();
	void displayWrittingMode();

private:
	bool saved;

	QFile* file;
	QTextStream* fileStream;

	//Menus
	QMenu* menuFile;
	QMenu* menuEdit;
	QMenu* menuDisplay;
	QMenu* menuHelp;

	QAction* actionOpenFile;
	QAction* actionSave;
	QAction* actionSaveAs;
	QAction* actionLeave;

	QAction* actionUndo;
	QAction* actionRedo;
	QAction* actionCopy;
	QAction* actionPaste;
	QAction* actionCut;
	QAction* actionSelectAll;
	QAction* actionAddDate;
	QAction* actionReadOnly;

	QAction* actionSetFont;

	QAction* actionAboutThis;
	QAction* actionAboutQt;

	QLabel* labelLineColumn;
	QLabel* labelWritingMode;

	//Text Zone
	QPlainTextEdit* textEdit_textArea;
};

#endif // MAINWINDOW_H
