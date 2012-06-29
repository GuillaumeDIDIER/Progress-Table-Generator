/*
 *  main.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 02/02/11.
 *  Copyright 2011. All rights reserved.
 *	main function file
 */

#include "Physique.h"

#include "physics/Element.h"
#include "physics/ElementTable.h"
#include "physics/Molecule.h"
#include "physics/Equation.h"

#include "gui/MainMenuBar.h"
#include "gui/MainWindow.h"
#include "gui/ProgressTableW.h"

#ifdef DEBUG
#include "unittest/UnitTest.h"
#endif

    //	the main (no more comments needed)
int main (int argc, char ** argv) {
    Q_INIT_RESOURCE(Physique);
#ifdef Q_OS_DARWIN
    Q_INIT_RESOURCE(Physique_mac);
#else
    Q_INIT_RESOURCE(Physique_other);
#endif
        //	App
    QApplication app(argc, argv);

        //	translators, emebeded into the application qrc
            //	qt translator
    QTranslator translator;
    translator.load(QString(":/translations/qt_") + QLocale::system().name());
            //	app translator
    QTranslator translator2;
    translator2.load(QString(":/translations/Physique_") + QLocale::system().name());
    app.installTranslator(&translator);
    app.installTranslator(&translator2);

    app.setApplicationName("ProgressTableGenerator");

        //	Globals initializations
#ifdef XMLPARSER_INCLUDED
        //QStringList().append(MainWindow::tr("ElementTable.xml")).append(
    QStringList xmlFiles;
    QString xmlFileRoot = MainWindow::tr("ElementTable.xml");
    xmlFiles.append(xmlFileRoot);
#ifdef Q_OS_DARWIN
    xmlFiles.append("../../../" + xmlFileRoot);
#endif // Q_OS_DARWIN
    xmlFiles.append(":/" + xmlFileRoot);
    ElementTable::loadElementTable(xmlFiles);
#endif	//	XMLPARSER_INCLUDED
    Molecule::setPeriodicElementTable(&PeriodicTable);
#ifdef Q_OS_DARWIN
    app.setAttribute(Qt::AA_DontShowIconsInMenus);
#endif	//	Q_OS_DARWIN
#ifdef DEBUG
        //	run unit tests
    qDebug() << "Running Unit Tests";
    qDebug() << "Unit tests successful : " << RunUnitTests();
#endif

        //	Widgets
    MainWindow* mainWizard = new MainWindow;
//#ifdef Q_OS_DARWIN
//	MainMenuBar main_menubar;
//	main_menubar.setAttribute(Qt::WA_MacBrushedMetal);
//#endif
    mainWizard->show();
        //	main loop (Qt)
    return app.exec();
}
