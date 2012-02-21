/*
 *  MainWindow.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#ifndef MainWindow_H
#define MainWindow_H

#include "Physique.h"

#include "Element.h"
#include "ElementTable.h"
#include "Molecule.h"
#include "Equation.h"

#include "MainMenuBar.h"
#include "ProgressTableW.h"

class MainWindow : public QWizard {
	Q_OBJECT
public:
	MainWindow(QWidget* parent = 0);
	virtual ~MainWindow();

	const Equation* equation() const;
	void setEquation(Equation* eq);
public slots:
	void accept();
protected slots:
	void superAccept();
protected:
		//	members
	MainMenuBar * m_menuBar;
	enum WizardPage { P_Intro, P_Equation, P_Equilibrate, P_MatterQuantity, P_Conclusion};
	Equation* m_equation;


};


#endif
