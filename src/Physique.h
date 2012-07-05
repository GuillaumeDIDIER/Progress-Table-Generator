/*
 *  Physique.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 19/03/11.
 *  Copyright 2011.
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
 *	Code Documentation of the src folder.
 *	This file, Physique.h : global includes.
 *	a main.cpp : no comment
 *	the gui folder, containing three Qt Gui classes
 *		MainWindow.[h/cpp] : no more comment needed
 *		MainMenuBar.[h/cpp] : the menu bar
 *		ProgressTableW.[h/cpp] : the dialog which shows the Progress Table
 *	the physics folder containing the model Layer. several Classes
 *		Element.[h/cpp] : represents a chemical element ex O C He
 *		ElementTable : provide a periodic element Table
 *		MoleculeOld.[h/cpp] : old representation for molecules
 *
 *      Reactive.[h/cpp] : new interface for all reactives.
 *      ChemicalReactive.[h/cpp] : new interface for chemical reactives.
 *      Electron.[h/cpp] : implementation of the ChemicalReactive interface, for the electrons (e-)
 *      Molecule.[h/cpp] : implementation of the ChemicalReactive interface, for chemical compounds.
 *          See documentation in Reactive.h and in the classes' headers.
 *
 *		EquationOld.[h/cpp] : represents a chemical equation, old class,
 *                         will be replaced by an inheritance class hierarchy,
 *                         based on the Reactive classes
 *	an additional file for unit tests, UnitTest.[h/cpp]
 *	src.pro : qmake project for the subdirectory.
 *	Physique_mac.pri : project include use for mac config (SDKs et arch), meant for end user customisaton
 *	Physique.qrc : the project built-in resources
 *	platform/[mac/other/win]/Physique_[mac/other/win].qrc : built-in resources for mac or other platforms, and plafform spécific stuffs
 *	../translation/Physique.[ts/qm] : Qt traduction files [sources/compiled]
 *
 *	platform/windows/Physique_win_icon.rc : windows resources for setting the icon on the W system
 *
 *	Other Files :
 *		physics/XmlElementTable/Element.xml : XML Periodic Element Table
 *		physics/XmlElementTable/Element.xsd : XML Schema of the XML Peridic Element Table
 *
 *		some images.
 *
 *		../Todo/ : things to do
 *		../Todo/done/ : things that were to do and have been done.
 */
#ifndef VERSION
#define VERSION "0.2"
    //	Changes must reflect those in Physique.pro and InfoPlist-Physique.plist
#endif // ifndef VERSION


#endif // Physique_H HeaderGuard
