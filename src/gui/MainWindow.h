/*
 *  MainWindow.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011.
 *
 */

#ifndef MainWindow_H
#define MainWindow_H

#include "Physique.h"

#include "physics/Element.h"
#include "physics/ElementTable.h"
#include "physics/MoleculeOld.h"
#include "physics/EquationOld.h"

#include "gui/MainMenuBar.h"
#include "gui/ProgressTableW.h"

class MainWindow : public QWizard {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();

    const EquationOld* equation() const;
    void setEquation(EquationOld* eq);
public slots:
    void accept();
protected slots:
    void superAccept();
protected:
        //	members
    MainMenuBar * m_menuBar;
    enum WizardPage { P_Intro, P_Equation, P_Equilibrate, P_MatterQuantity, P_Conclusion};
    EquationOld* m_equation;


};


#endif
