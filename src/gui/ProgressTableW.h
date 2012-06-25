/*
 *  ProgressTableW.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#ifndef ProgressTableW_H
#define ProgressTableW_H

#include "Physique.h"

#include "physics/Element.h"
#include "physics/ElementTable.h"
#include "physics/Molecule.h"
#include "physics/Equation.h"

#include "gui/MainMenuBar.h"

class ProgressTableW : public QDialog {
    Q_OBJECT
public:
    ProgressTableW(QWidget* parent = 0);
    ProgressTableW(QTextDocument* table, const Equation & eq = Equation(), QWidget* parent = 0);
    virtual ~ProgressTableW();



protected:
    QTextEdit* m_textEdit;
    QTextDocument* m_document;
    Equation m_equation;
    MainMenuBar * m_menubar;
};

#endif
