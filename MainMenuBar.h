/*
 *  MainMenuBar.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#ifndef MainMenuBar_H
#define MainMenuBar_H

#include "Physique.h"

class MainMenuBar : public QMenuBar {
	Q_OBJECT
public:
	MainMenuBar(QWidget* parent);
	virtual ~MainMenuBar();
public slots:
	void newWizard();
	void about();
	void aboutQt();
	void setCopy(bool copy);
	void copy();
signals:
	void copyRequested();
protected:
	QMap<QString, QMenu*> m_menuList;
	QMap<QString, QAction*> m_actionList;
};

#endif