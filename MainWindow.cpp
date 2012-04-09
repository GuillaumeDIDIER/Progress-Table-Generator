/*
 *  MainWindow.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#include "MainWindow.h"
#include "MainWindow_p.h"

#define EQUATION_MEMBER_RX "(\\d*((?:(?:[A-Z][a-z]{0,2}\\d*)|(?:\\((?:[A-Z][a-z]{0,2}\\d*)+\\)\\d*))+)(\\[\\d*(?:\\-|\\+)\\])?(?:\\(([a-z]{1,2})\\))? \\+ )*(\\d*((?:(?:[A-Z][a-z]{0,2}\\d*)|(?:\\((?:[A-Z][a-z]{0,2}\\d*)+\\)\\d*))+)(\\[\\d*(?:\\-|\\+)\\])?(?:\\(([a-z]{1,2})\\))?)"

bool operator<(const Molecule& a, const Molecule& b); //	implemented in Equation.cpp
bool moleculeLessThanUser(const Molecule& a, const Molecule& b);	//	idem
	//	class MainWindow
MainWindow::MainWindow(QWidget* parent) : QWizard(parent), m_equation(NULL) {
	setAttribute(Qt::WA_DeleteOnClose);
	m_menuBar = new MainMenuBar(this);
	m_menuBar->setCopy(false);
		//	default graphics
	QIcon icon(":/icon.png");
	setPixmap(QWizard::LogoPixmap, icon.pixmap(icon.actualSize(QSize(64, 64))));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(QString(":/Images/watermark.png")));
	setPixmap(QWizard::BannerPixmap, QPixmap(QString(":/Images/banner.png")));
		//	creating the pages
	setPage(P_Intro, new IntroPage(this));
	setPage(P_Equation, new EquationPage(this));
	setPage(P_Equilibrate, new EquilibratePage(this));
	setPage(P_MatterQuantity, new MatterQuantityPage(this));
	setPage(P_Conclusion, new ConclusionPage(this));
		//	work to do : Pixmaps
	
}
MainWindow::~MainWindow(){
	if(m_equation)	//	deleting the equation on the heap if it exists
		delete m_equation;
}

const Equation* MainWindow::equation() const{
	return m_equation;	//	we can return it safely as we return a const *
}

void MainWindow::setEquation(Equation* eq){
	if (m_equation)	//	if the pointer isn't NULL
		delete m_equation;	//	we delete the old equation
	m_equation = new Equation(*eq);	//	and replace it by a copy of the new Equation
}
	//	work to do
void MainWindow::accept(){
		//	Global objects
	QTextDocument table;
	QTextCursor cursor(&table);
		//	Heading
	QTextBlockFormat headingBlockFormat;
	headingBlockFormat.setAlignment(Qt::AlignCenter);
	headingBlockFormat.setNonBreakableLines(true);
	QTextCharFormat headingCharFormat;
	headingCharFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
	headingCharFormat.setFontUnderline(true);
	cursor.setBlockFormat(headingBlockFormat);
	cursor.insertText(tr("Progress Table : \n"), headingCharFormat);
	cursor.insertHtml(m_equation->toHtml());
		//	saving the position
	QTextFrame * root = cursor.currentFrame();
		//	Table
			//	Global Formatting
	QTextTableFormat tableFormat;
	tableFormat.setHeaderRowCount(2);
	int columnCount = m_equation->reactives().size() + m_equation->products().size() + 2;
	QVector<QTextLength> constraints = QVector<QTextLength>(columnCount , QTextLength(QTextLength::PercentageLength, 100 / columnCount));
	tableFormat.setColumnWidthConstraints(constraints);
	QTextTable * mainTable = cursor.insertTable(5, columnCount, tableFormat);
	mainTable->mergeCells(0, 0, 1, 2);
	mainTable->mergeCells(0, 2, 1, columnCount - 2);
	mainTable->mergeCells(1, 2, 1, columnCount - 2);
	QTextBlockFormat cellBlockFormat;
	cellBlockFormat.setAlignment(Qt::AlignCenter);
	cellBlockFormat.setNonBreakableLines(true);
	QTextCharFormat cellCharFormat;
	cellCharFormat.setFontUnderline(false);
			//	equation writting (into a sub table, for formatting purposes
	cursor = mainTable->cellAt(0, 2).firstCursorPosition();
	QTextTableFormat eqTableFormat;
	int eqTableColCount = (columnCount - 2)*3 + (columnCount - 3)*2;
	eqTableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
	QVector<QTextLength> eqConstraint = QVector<QTextLength>(eqTableColCount, QTextLength(QTextLength::PercentageLength, 100/eqTableColCount));
	eqTableFormat.setColumnWidthConstraints(eqConstraint);
	QTextTable * eqTable = cursor.insertTable(1, eqTableColCount, eqTableFormat);
	for (int i = 0; i < eqTableColCount; i += 5) {
		eqTable->mergeCells(0, i, 1, 3);
		eqTable->mergeCells(0, i+3, 1, 2);
	}
	int j = 0;
	QList<Molecule> reactives = m_equation->reactives().keys();
	qSort(reactives.begin(), reactives.end(), moleculeLessThanUser);
	QListIterator<Molecule> i1(reactives);
	QTextCharFormat previous = cursor.charFormat();
	cursor.setCharFormat(cellCharFormat);
	while (i1.hasNext()) {
		cursor = eqTable->cellAt(0, j).firstCursorPosition();
		QString molecule;
		int stoechNum = m_equation->stoechNumOf(i1.peekNext());
		if (stoechNum > 1) {
			molecule += QString::number(stoechNum) + " ";
		}
		molecule += i1.next().htmlFormula();
		cursor.setBlockFormat(cellBlockFormat);
		cursor.insertHtml(molecule);
		if (i1.hasNext()) {
			cursor = eqTable->cellAt(0, j + 3).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText("+");
			j += 5;
		}
		
	}
	cursor = eqTable->cellAt(0, j + 3).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(QChar(0x2192), cellCharFormat);
	
	QList<Molecule> products = m_equation->products().keys();
	qSort(products.begin(), products.end(), moleculeLessThanUser);
	QListIterator<Molecule> i2(products);
	while (i2.hasNext()) {
		j += 5;
		cursor = eqTable->cellAt(0, j).firstCursorPosition();
		QString molecule;
		int stoechNum = m_equation->stoechNumOf(i2.peekNext());
		if (stoechNum > 1) {
			molecule += QString::number(stoechNum) + " ";
		}
		molecule += i2.next().htmlFormula();
		cursor.setBlockFormat(cellBlockFormat);
		cursor.insertHtml(molecule);
		if (i2.hasNext()) {
			cursor = eqTable->cellAt(0, j + 3).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText("+");
		}
	}
			//	filling the header's rest
	cursor = mainTable->cellAt(0, 1).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(tr("Equation"), cellCharFormat);
	
	cursor = mainTable->cellAt(1, 0).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(tr("State"), cellCharFormat);
	
	cursor = mainTable->cellAt(1, 1).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(tr("Progress (mol)"), cellCharFormat);
	
	cursor = mainTable->cellAt(1, 2).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(tr("Matter Quantities (mol)"), cellCharFormat);
	
	cursor = mainTable->cellAt(2, 0).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(tr("Initial"), cellCharFormat);
	
	cursor = mainTable->cellAt(3, 0).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(tr("Intermediate"), cellCharFormat);
	
	cursor = mainTable->cellAt(4, 0).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText(tr("Final"), cellCharFormat);

	cursor = mainTable->cellAt(2, 1).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText("0", cellCharFormat);

	cursor = mainTable->cellAt(3, 1).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertText("x", cellCharFormat);
	
	cursor = mainTable->cellAt(4, 1).firstCursorPosition();
	cursor.setBlockFormat(cellBlockFormat);
	cursor.insertHtml("X<sub>max</sub>");
	
	int reactivesCount = reactives.count();
	for (int i = 0; i < reactivesCount; ++i) {
		int stoech = m_equation->stoechNumOf(reactives[i]);
		bool ok;
		double n = field(reactives[i].formula()).toDouble(&ok);
		if (stoech > 1){
			cursor = mainTable->cellAt(2, i + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText(QString::number(n , 'g', 2), cellCharFormat); //	find a way to respect th significant digits.
			cursor = mainTable->cellAt(3, i + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText(QString::number(n , 'g', 2) + " - " + QString::number(stoech) +"x", cellCharFormat);
			cursor = mainTable->cellAt(4, i + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertHtml(QString::number(n , 'g', 2) + " - " + QString::number(stoech) +"X<sub>max</sub>");
		}
		else {
			cursor = mainTable->cellAt(2, i + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText(QString::number(n , 'g', 2), cellCharFormat); //	find a way to respect th significant digits.
			cursor = mainTable->cellAt(3, i + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText(QString::number(n , 'g', 2) + " - " + "x", cellCharFormat);
			cursor = mainTable->cellAt(4, i + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertHtml(QString::number(n , 'g', 2) + " - " + "X<sub>max</sub>");
		}

	}
	int productsCount = products.count();
	for (int i = 0; i < productsCount; ++i) {
		int stoech = m_equation->stoechNumOf(products[i]);
		if (stoech > 1) {
			cursor = mainTable->cellAt(2, i + reactivesCount + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText("0", cellCharFormat);
			cursor = mainTable->cellAt(3, i + reactivesCount + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText(QString::number(stoech) +"x", cellCharFormat);
			cursor = mainTable->cellAt(4, i + reactivesCount + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertHtml(QString::number(stoech) +"X<sub>max</sub>");
		}
		else {
			cursor = mainTable->cellAt(2, i + reactivesCount + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText("0", cellCharFormat);
			cursor = mainTable->cellAt(3, i + reactivesCount + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertText("x", cellCharFormat);
			cursor = mainTable->cellAt(4, i + reactivesCount + 2).firstCursorPosition();
			cursor.setBlockFormat(cellBlockFormat);
			cursor.insertHtml("X<sub>max</sub>");
		}

	}
	cursor.setCharFormat(previous);
	
	
	
	
			//	work to do here
			//	by Guillaume
	
	ProgressTableW * AdvTablWidget =  new ProgressTableW(&table, *m_equation, this);
	this->hide();
	AdvTablWidget->show();
	connect(AdvTablWidget, SIGNAL(accepted()), this, SLOT(superAccept()));
	connect(AdvTablWidget, SIGNAL(rejected()), this, SLOT(show()));
}
void MainWindow::superAccept(){
	QWizard::accept();
}

	//	private pages
	//	intro page
IntroPage::IntroPage(QWidget* parent) : QWizardPage(parent){
	setTitle(tr("Introduction"));
	m_introLabel = new QLabel(tr("This wizard will help you generating a Progress Table"));
	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->addWidget(m_introLabel);
}

IntroPage::~IntroPage(){
	delete m_introLabel;
}

void IntroPage::initializePage(){
		//	naught to do
}
	//	Equation Page
EquationPage::EquationPage(QWidget* parent) : QWizardPage(parent){
	setTitle(tr("Equation of the Chemical Reaction"));
	setSubTitle(tr("Please enter the equation of your chemical reaction.\nPlease note that electron (e-) is not yet supported"));
	m_mainLayout = new QVBoxLayout(this);

	m_reactives = new QLineEdit();
	m_reactives->setPlaceholderText(tr("Reactives"));
	m_reactives->setValidator(new QRegExpValidator(QRegExp(Equation::equation_rx()), this));
	m_mainLayout->addWidget(m_reactives);

	m_arrow = new QLabel(QChar(0x2192));
	m_mainLayout->addWidget(m_arrow);

	m_products = new QLineEdit();
	m_products->setPlaceholderText(tr("Products"));
	m_products->setValidator(new QRegExpValidator(QRegExp(Equation::equation_rx()), this));
	m_mainLayout->addWidget(m_products);
}

EquationPage::~EquationPage(){
	delete m_reactives;
	delete m_products;
	delete m_arrow;
	delete m_mainLayout;
}

void EquationPage::initializePage(){
		//	naught to do
}

bool EquationPage::validatePage(){
	MainWindow * main_wizard = qobject_cast<MainWindow*>(wizard());
	if (!main_wizard) {
		QMessageBox::critical(this, "Error", "MainWizard Cast Error");
		return false;
	}
	main_wizard->setEquation(new Equation(m_reactives->text(), m_products->text()));
	return true;
}

EquilibratePage::EquilibratePage(QWidget* parent) : QWizardPage(parent), m_equation(0){
	setTitle(tr("Equilibrating the equation"));
	m_mainLayout = new QVBoxLayout();
	m_eqStr = new QLabel();
	m_eqStr->setAlignment(Qt::AlignCenter);
	m_mainLayout->addWidget(m_eqStr);
	m_stateLabelImg = new QLabel();
	m_stateLabelImg->setAlignment(Qt::AlignCenter);
	m_mainLayout->addWidget(m_stateLabelImg);
	m_stateLabel = new QLabel();
	m_stateLabel->setAlignment(Qt::AlignCenter);
	m_mainLayout->addWidget(m_stateLabel);
	m_equilibrate = new QPushButton(tr("Equilibrate"));
	m_mainLayout->addWidget(m_equilibrate);
	setLayout(m_mainLayout);
	connect(m_equilibrate, SIGNAL(pressed()), this, SLOT(equilibrateEq()));
	setCommitPage(true);
}

EquilibratePage::~EquilibratePage(){
	if (m_equation) {
		delete m_equation;
	}
}

void EquilibratePage::initializePage(){
	MainWindow * main_wizard = qobject_cast<MainWindow*>(wizard());
	if (!main_wizard) {
		QMessageBox::critical(this, "Error", "MainWizard Cast Error");
		return;
	}
	m_equation = new Equation(*(main_wizard->equation()));
	updateState();
	m_eqStr->setText(m_equation->toHtml());
}
	//	work to do
bool EquilibratePage::validatePage(){
	MainWindow * main_wizard = qobject_cast<MainWindow*>(wizard());
	if (!main_wizard) {
		return false;
	}
	if(m_state == Equilibrated)
		main_wizard->setEquation(m_equation);
	else if (m_state == Invalid) {
		QMessageBox errorMessage1(QMessageBox::NoIcon, tr("Invalid equation"),
								  tr("The chemical equation isn't valid.\nPlease enter a valid equation"),
								  QMessageBox::Ok, wizard());
		errorMessage1.setIconPixmap(QPixmap(":/images/Failure.png"));	//	low size red PNG
		errorMessage1.exec();
		return false;
	}
			
	else if (m_state == Unequilibrated) {
		QMessageBox errorMessage2(QMessageBox::NoIcon , tr("Unequilibrated equation"),
									tr("The chemical equation is not equilibrated\nContinue all the same or cancel to equilibrate ?"),
									QMessageBox::Cancel, wizard());
		errorMessage2.setIconPixmap(QPixmap(":/images/Warning.png"));	//	low size orange '?' PNG
		errorMessage2.addButton(tr("Continue"), QMessageBox::AcceptRole);
		errorMessage2.setDefaultButton(QMessageBox::Cancel);
		errorMessage2.exec();
		if (errorMessage2.result() == QMessageBox::AcceptRole) {
			main_wizard->setEquation(m_equation);
			return true;
		}
		else {
			return false;
		}
	} 
	else if (m_state == Error) {
		QMessageBox errorMessage3(QMessageBox::NoIcon , tr("Error while equilibrating"),
									tr("The chemical equation couldn't be equilibrated.\nContinue all the same with this equation : %1 ?").arg(main_wizard->equation()->toStr()),
									QMessageBox::Cancel, wizard());
		errorMessage3.setIconPixmap(QPixmap(":/images/Warning.png"));	//	low size orange '?' PNG
		errorMessage3.addButton(tr("Continue"), QMessageBox::AcceptRole);
		errorMessage3.setDefaultButton(QMessageBox::Cancel);
		errorMessage3.exec();
		if (errorMessage3.result() == QMessageBox::AcceptRole) {
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}

bool EquilibratePage::equilibrateEq(){
	m_equation->equilibrate();
	m_hasTryiedToEquilibrate = true;
	updateState();
	m_eqStr->setText(m_equation->toHtml());
	return m_equation->isValid() && m_equation->isEquilibrated();
}

EquilibratePage::State EquilibratePage::updateState(){
	if (!m_equation->isValid()) {
		setState(Invalid);
		return Invalid;
	}
	else if (m_equation->isEquilibrated()){
		setState(Equilibrated);
		return Equilibrated;
	}
	else if (!m_hasTryiedToEquilibrate) {
		setState(Unequilibrated);
		return Unequilibrated;
	}
	else {
		setState(Error);
		return Error;
	}

	
}

void EquilibratePage::setState(State state){
	m_state = state;
	switch (state) {
		case Invalid:
			m_stateLabel->setText(tr("The equation is not valid"));
			m_stateLabelImg->setPixmap(QPixmap(":/images/Failure.png"));	//	red PNG
			m_equilibrate->setEnabled(false);
			break;
		case Equilibrated:
			m_stateLabel->setText(tr("The equation is equilibrated"));
			m_stateLabelImg->setPixmap(QPixmap(":/images/Success.png"));	//	green PNG
			m_equilibrate->setEnabled(false);
			break;
		case Unequilibrated:
			m_stateLabel->setText(tr("The equation is not equilibrated"));	
			m_stateLabelImg->setPixmap(QPixmap(":/images/Warning.png"));	//	orange '?' PNG
			m_equilibrate->setEnabled(true);
			break;
		case Error:
			m_stateLabel->setText(tr("Could not equilibrate equation"));
			m_stateLabelImg->setPixmap(QPixmap(":/images/Warning.png"));	//	orange '?' PNG
			m_equilibrate->setEnabled(true);
			break;

	}
}

MatterQuantityPage::MatterQuantityPage(QWidget* parent) : QWizardPage(parent) {

	m_layout = new QFormLayout(this);
	setTitle(tr("Reactives Matter Quantities"));
	setSubTitle(tr("Please enter the matter quantities of the following reactives"));
}

MatterQuantityPage::~MatterQuantityPage(){
	delete m_layout;
	
}

void MatterQuantityPage::initializePage(){
	MainWindow * main_wizard = qobject_cast<MainWindow*>(wizard());
	if (!main_wizard) {
		QMessageBox::critical(this, "Error", "MainWizard Cast Error");
		return;
	}
	m_equation = new Equation(*(main_wizard->equation()));
	QMap<Molecule, int> moleculeStoechs(m_equation->reactives());
	QList<Molecule> reactives(moleculeStoechs.keys());
	qSort(reactives.begin(), reactives.end(), moleculeLessThanUser);
	foreach(Molecule mol, reactives){
		m_reactivesMatterQuantities[mol] = new QDoubleSpinBox();	//	GD tmp
		m_reactivesMatterQuantities[mol]->setRange(0, 1000000);
		registerField(mol.formula(), m_reactivesMatterQuantities[mol], "value", SIGNAL(valueChanged(double)));
		m_layout->addRow(QString::number(moleculeStoechs[mol]) + " " + mol.htmlFormula(), m_reactivesMatterQuantities[mol]);
	}
}

ConclusionPage::ConclusionPage(QWidget* parent) : QWizardPage(parent){
	setTitle(tr("Generating the advancement table"));
	setSubTitle(tr("Click on %1 to generate the table.").arg(buttonText(QWizard::FinishButton)));
	setLayout(new QVBoxLayout());
}

ConclusionPage::~ConclusionPage(){
	
}
