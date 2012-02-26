/*
 *  Physique.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 19/03/11.
 *  Copyright 2011. All rights reserved.
 *	main include file for libraries includes
 *	can be turned into a precompiled header
 */

#ifndef Physique_H
#define Physique_H

#include <QtGui>
#include <QtCore>

#include <QtXml>
#include <QtXmlPatterns>
 //#define XMLPARSER_INCLUDED

#include <QTranslator>
#include <QLocale>

/*
 *	Code Documentation
 *	This file, Physique.h : global includes.
 *	a main.cpp : no comment
 *	three Qt Gui classes Files :
 *		MainWindow.[h/cpp] : no more comment needed
 *		MainMenuBar.[h/cpp] : the menu bar
 *		ProgressTableW.[h/cpp] : the dialog which shows the Progress Table
 *	four classes Files of the model layer :
 *		Element.[h/cpp] : represents a chemical element ex O C He
 *		ElementTable : provide a periodic element Table
 *		Molecule.[h/cpp] : represents molecules
 *		Equation.[h/cpp] : represents a chemical equation
 *	an additional file for unit tests, UnitTest
 *	4 Qt temporory sources (moc_... qrc_...) : Qt-generated. Not present on a clean repository.
 *	Physique.pro : qmake project for the project
 *	Physique_mac.pri : project include use for mac config (SDKs et arch)
 *	Physique.qrc : the project built-in resources
 *	Physique_[mac/other].qrc : built-in resources for mac or pther platforms
 *	Physique.[ts/qm] : Qt traduction files [sources/compiled]
 *	
 *	Physique_win_icon.rc : windows resources for setting the icon on the W system
 *
 *	Other Files :
 *		Element.xml : XML Periodic Element Table
 *		Element.xsd : XML Schema of the XML Peridic Element Table 
 *		
 *		Images :
 *			Erlenmeyer - 3232px, Erlenmeyer - Version 1, Erlenmeyer.icns, Erlenmeyer.ico :
 *				Application's Icon(mac=.icns et win=.ico) and its sources(png)
 *			Mac/, croix.png, question.png, tick.png : images used by the application
 *			Projet de Physique/ : Unused images
 *		
 *		Info.plist, InfoPlist-Physique.plist : the mac Application Bundle Info.plist
 *
 *		Todo/ : things to do
 *		Todo/done/ : things that were to do and have been done.
 */

#endif
