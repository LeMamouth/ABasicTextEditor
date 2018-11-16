#include "mainwindow.h"
#include "aboutdialog.h"

#include <QColorDialog>
#include <QDateTime>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QMessageBox>
#include <QSysInfo>

#include <fstream>
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	showMaximized();

	saved = true;

	file = new QFile();
	fileStream = new QTextStream(file);

	//Setting the UI
	initializeComponents();

	//Setting shortcuts
	initializeShortcuts();

	//Status Bar
	labelLineColumn = new QLabel(this);
	statusBar()->addWidget(labelLineColumn);

	labelWritingMode = new QLabel(this);
	statusBar()->addWidget(labelWritingMode);

	//Connecting SIGNALS and SLOTS

	connect(textEdit_textArea, SIGNAL(textChanged()), this, SLOT(setUnsaved()));

	connect(actionOpenFile, SIGNAL(triggered(bool)), this, SLOT(chooseFile()));
	connect(actionSave, SIGNAL(triggered(bool)), this, SLOT(save()));
	connect(actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
	connect(actionSetFont, SIGNAL(triggered(bool)), this, SLOT(setFont()));
	connect(actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
	connect(actionLeave, SIGNAL(triggered(bool)), this, SLOT(close()));
	connect(actionAboutThis, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

	connect(actionUndo, SIGNAL(triggered(bool)), textEdit_textArea, SLOT(undo()));
	connect(actionRedo, SIGNAL(triggered(bool)), textEdit_textArea, SLOT(redo()));

	connect(actionCopy, SIGNAL(triggered(bool)), textEdit_textArea, SLOT(copy()));
	connect(actionPaste, SIGNAL(triggered(bool)), textEdit_textArea, SLOT(paste()));
	connect(actionCut, SIGNAL(triggered(bool)), textEdit_textArea, SLOT(cut()));

	connect(actionSelectAll, SIGNAL(triggered(bool)), textEdit_textArea, SLOT(selectAll()));
	connect(actionAddDate, SIGNAL(triggered(bool)), this, SLOT(addDate()));
	connect(actionReadOnly, SIGNAL(triggered(bool)), this, SLOT(setReadOnlyText(bool)));

	connect(textEdit_textArea, SIGNAL(textChanged()), this, SLOT(displayCursorPos()));
}
MainWindow::~MainWindow()
{
	file->close();
	delete file;
	delete fileStream;
}

void MainWindow::initializeComponents()
{
	menuFile = menuBar()->addMenu(tr("&Files"));
	menuEdit = menuBar()->addMenu(tr("&Edit"));
	menuDisplay = menuBar()->addMenu(tr("&Display"));
	menuHelp = menuBar()->addMenu("?");

	actionOpenFile = menuFile->addAction(tr("Open"));
	actionSave = menuFile->addAction(tr("Save"));
	actionSaveAs = menuFile->addAction(tr("Save As"));
	menuFile->addSeparator();
	actionLeave = menuFile->addAction(tr("Leave"));

	actionUndo = menuEdit->addAction(tr("Undo"));
	actionRedo = menuEdit->addAction(tr("Redo"));
	menuEdit->addSeparator();
	actionCopy = menuEdit->addAction(tr("Copy"));
	actionPaste = menuEdit->addAction(tr("Paste"));
	actionCut = menuEdit->addAction(tr("Cut"));
	menuEdit->addSeparator();
	actionSelectAll = menuEdit->addAction(tr("Select All"));
	menuEdit->addSeparator();
	actionAddDate = menuEdit->addAction(tr("Add date"));
	menuEdit->addSeparator();
	actionReadOnly = menuEdit->addAction(tr("Read Only"));
	actionReadOnly->setCheckable(true);
	actionReadOnly->setChecked(false);

	actionSetFont = menuDisplay->addAction(tr("Font"));

	actionAboutThis = menuHelp->addAction(tr("About"));
	actionAboutQt = menuHelp->addAction(tr("About Qt"));

	textEdit_textArea = new QPlainTextEdit(this);
	setCentralWidget(textEdit_textArea);
	textEdit_textArea->setFont(QFont("Monospace"));
}

void MainWindow::initializeShortcuts()
{
	actionOpenFile->setShortcut(QKeySequence("Ctrl+O"));
	actionSave->setShortcut(QKeySequence("Ctrl+S"));

	actionUndo->setShortcut(QKeySequence("Ctrl+Z"));
	actionRedo->setShortcut(QKeySequence("Ctrl+Shift+Z"));
	actionCopy->setShortcut(QKeySequence("Ctrl+C"));
	actionPaste->setShortcut(QKeySequence("Ctrl+V"));
	actionCut->setShortcut(QKeySequence("Ctrl+X"));
	actionSelectAll->setShortcut(QKeySequence("Ctrl+A"));
	actionReadOnly->setShortcut(QKeySequence("F5"));
}
void MainWindow::chooseFile()
{
	//Opening file
	file->setFileName(QFileDialog::getOpenFileName(this, tr("Choose a file")));
	file->open(QFile::ReadWrite);
	//Reading file
	textEdit_textArea->setPlainText(fileStream->readAll());
}

void MainWindow::setFont()
{
	QFont font = QFontDialog::getFont(nullptr, textEdit_textArea->font(), this, tr("Choose a font"));
	//Setting font
	textEdit_textArea->setFont(font);
}

void MainWindow::save()
{
	if (file->fileName().isEmpty()) //If the user has not selected a file before
	{
		saveAs();
	} else {
		file->resize(0); //Erases all file's content to rewrite it.
		std::ofstream flux(file->fileName().toStdString());
		flux << textEdit_textArea->toPlainText().toStdString();
		/* FIX ME : Please, make this function work without std::ofstream. PLEASE <3 */
	}
	saved = checkSuccessfullyModified();
}

void MainWindow::saveAs()
{
	file->setFileName(QFileDialog::getSaveFileName(this, tr("Choose a file")));
	std::ofstream flux(file->fileName().toStdString());
	flux << textEdit_textArea->toPlainText().toStdString();
	/* FIX ME : Same as save() */
	if (!file->fileName().isEmpty()) //Checks if the user has choosen a file.
		saved = checkSuccessfullyModified();
}

void MainWindow::setUnsaved()
{
	saved = false;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (!saved) {
		int answer;
		answer = QMessageBox::information(this,
			tr("Close a document"),
			tr("The file ") + file->fileName() + tr(" has been modified.\nDo you want to save the changes or discard them ?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Abort);
		if (answer == QMessageBox::Save)
			save();
		else if (answer == QMessageBox::Abort)
			event->ignore();
	}
}

void MainWindow::showAbout()
{
	AboutDialog* about = new AboutDialog(this);
	about->show();
}

bool MainWindow::checkSuccessfullyModified()
{
	QString fileContent = fileStream->readAll();
	if (textEdit_textArea->toPlainText() == fileContent) {
		return true;
	} else {
		QMessageBox::critical(this, tr("Error"), tr("The modifications failed to save\nPerhaps the file doesn't exist or you don't have enough privilege"));
		return false;
	}
}

void MainWindow::addDate()
{
	textEdit_textArea->setPlainText(textEdit_textArea->toPlainText() + QDateTime::currentDateTime().toString());
}

void MainWindow::setReadOnlyText(bool e)
{
	textEdit_textArea->setReadOnly(e);
}

void MainWindow::displayCursorPos()
{
	labelLineColumn->setText("LN : " + QString::number(textEdit_textArea->cursor().pos().x()));
}

void MainWindow::displayWrittingMode()
{
}
