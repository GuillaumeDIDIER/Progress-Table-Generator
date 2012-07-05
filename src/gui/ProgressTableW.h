/*
 *  ProgressTableW.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011.
 *
 */

#ifndef ProgressTableW_H
#define ProgressTableW_H

#include "Physique.h"

#include "physics/Element.h"
#include "physics/ElementTable.h"
#include "physics/MoleculeOld.h"
#include "physics/EquationOld.h"

#include "gui/MainMenuBar.h"

class ProgressTableW : public QDialog {
    Q_OBJECT
public:
    ProgressTableW(QWidget* parent = 0);
    ProgressTableW(QTextDocument* table, const EquationOld & eq = EquationOld(), QWidget* parent = 0);
    virtual ~ProgressTableW();



protected:
    QTextEdit* m_textEdit;
    QTextDocument* m_document;
    EquationOld m_EquationOld;
    MainMenuBar * m_menubar;
};

#endif
