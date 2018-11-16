#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
	ui->aboutTextEdit->setPlainText("Name : ASimpleTextEditor\n"
									"Version : 1.0\n"
									"Description : A Cool and simple text Editor made with Qt and Love");
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
