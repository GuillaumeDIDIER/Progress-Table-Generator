/*
 *  ProgressTableW.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011.
 *
 */

#include "gui/ProgressTableW.h"

ProgressTableW::ProgressTableW(QWidget* parent) : QDialog(parent), m_document(new QTextDocument(this)), m_EquationOld(EquationOld()) {
        //	GUI to implement
    m_menubar = new MainMenuBar(this);
    m_menubar->setCopy(false);
    QGridLayout *layout = new QGridLayout(this);
    m_textEdit = new QTextEdit();
    setWindowTitle(tr("Empty Progress Table"));
    m_textEdit->setDocument(m_document);
    layout->addWidget(m_textEdit, 0, 0);
    QPushButton *OkButton = new QPushButton(tr("Ok"));
    OkButton->setDefault(true);
    layout->addWidget(OkButton, 1, 0);
    connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_textEdit, SIGNAL(copyAvailable(bool)), m_menubar, SLOT(setCopy(bool)));
    connect(m_menubar, SIGNAL(copyRequested()), m_textEdit, SLOT(copy()));
}

ProgressTableW::ProgressTableW(QTextDocument* table, const EquationOld & eq, QWidget* parent) : QDialog(parent), m_document(table->clone(this)), m_EquationOld(eq) {
    m_menubar = new MainMenuBar(this);
    m_menubar->setCopy(false);
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_textEdit = new QTextEdit(this);
    if (eq != EquationOld()) {
        setWindowTitle(tr("Progress Table : %1").arg(eq.toStr()));
    }
    else {
        setWindowTitle(tr("Progress Table"));
    }
    layout->addWidget(m_textEdit, 0, 0);
    QPushButton *OkButton = new QPushButton(tr("Ok"));
    OkButton->setDefault(true);
    layout->addWidget(OkButton, 1, 0);
    connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
    m_textEdit->setDocument(m_document);
    connect(m_textEdit, SIGNAL(copyAvailable(bool)), m_menubar, SLOT(setCopy(bool)));
    connect(m_menubar, SIGNAL(copyRequested()), m_textEdit, SLOT(copy()));

}

ProgressTableW::~ProgressTableW(){
        //	GUI to implement
    delete m_textEdit;
    delete m_document;
}

