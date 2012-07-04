/*
 *  MainMenuBar.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011.
 *
 */

#include "gui/MainMenuBar.h"
#include "gui/MainWindow.h"
    //	work to do
MainMenuBar::MainMenuBar(QWidget* parent) : QMenuBar(parent), m_menuList() {
    m_menuList.insert("File", new QMenu(tr("File"), this));
    addMenu(m_menuList["File"]);

    m_actionList.insert("New", new QAction(QIcon(/*:/images/...*/), tr("New"), this));	//	new icon (.png)
    m_actionList["New"]->setShortcuts(QKeySequence::New);
    connect(m_actionList["New"], SIGNAL(triggered()), this, SLOT(newWizard()));
    m_menuList["File"]->addAction(m_actionList["New"]);

    m_actionList.insert("About", new QAction(QIcon(/*:/images/...*/), tr("About..."), this));	//	about icon (.png)
    m_actionList["About"]->setMenuRole(QAction::AboutRole);
    connect(m_actionList["About"], SIGNAL(triggered()), this, SLOT(about()));
    m_menuList["File"]->addAction(m_actionList["About"]);

    m_actionList.insert("About Qt", new QAction(QIcon(/*:/images/...*/), tr("About Qt..."), this));	//	about icon (.png)
    m_actionList["About Qt"]->setMenuRole(QAction::AboutQtRole);
    connect(m_actionList["About Qt"], SIGNAL(triggered()), this, SLOT(aboutQt()));
    m_menuList["File"]->addAction(m_actionList["About Qt"]);

    m_actionList.insert("Quit", new QAction(QIcon(/*:images/...*/), tr("Quit"), this)); //	Quit icon (.png)
    m_actionList["Quit"]->setMenuRole(QAction::QuitRole);
    m_actionList["Quit"]->setShortcuts(QKeySequence::Quit);
    connect(m_actionList["Quit"], SIGNAL(triggered()), qApp, SLOT(quit()));
    m_menuList["File"]->addAction(m_actionList["Quit"]);

    m_menuList.insert("Edit", new QMenu(tr("Edit"), this));
    addMenu(m_menuList["Edit"]);

    m_actionList.insert("Copy", new QAction(QIcon(/*:/images/...*/), tr("Copy"), this));	//	Copy icon (.png)
    m_actionList["Copy"]->setMenuRole(QAction::NoRole);
    m_actionList["Copy"]->setShortcuts(QKeySequence::Copy);
    connect(m_actionList["Copy"], SIGNAL(triggered()), this, SLOT(copy()));
    m_menuList["Edit"]->addAction(m_actionList["Copy"]);
    setCopy(false);
}

MainMenuBar::~MainMenuBar(){
        //	action and menu deleted by the QObject parentship tree
        //	so naught to do
}

void MainMenuBar::newWizard(){
    qobject_cast<QWidget*>(parent())->close();
    MainWindow* other = new MainWindow();
//	other->move(QApplication::activeWindow()->pos() + QPoint(23, 23));
    other->show();
}

void MainMenuBar::copy(){
    emit copyRequested();
}

void MainMenuBar::setCopy(bool copy){
    m_actionList["Copy"]->setEnabled(copy);
}
    //	work to do
void MainMenuBar::about(){

#ifdef Q_WS_MAC
    static QPointer<QMessageBox> oldAboutBox = 0;
    if (!oldAboutBox.isNull()) {
        oldAboutBox->show();
        oldAboutBox->raise();
        oldAboutBox->activateWindow();
        return;

    }
#endif
    QMessageBox *aboutBox = new QMessageBox(qApp->activeWindow());
    aboutBox->setAttribute(Qt::WA_DeleteOnClose);
    QIcon icon(":/icon.png");
    aboutBox->setIconPixmap(icon.pixmap(icon.actualSize(QSize(64, 64))));
    aboutBox->setWindowTitle(tr("About %1").arg(qAppName()));
    aboutBox->setText(tr("%1").arg(qAppName()));
    aboutBox->setInformativeText(tr(
                                    "<p>Version %1</p>"
                                    "<p>Copyright Guillaume DIDIER 2011-2012</p>"
                                    "<p>Graphics Copyright Alexis FALAISE 2011-2012<p/>"
                                    "<p>Periodic Element Table Copyright Gustave TANURI-DENIS<p/>"
                                    ).arg(VERSION));
    aboutBox->show();


}

void MainMenuBar::aboutQt(){
    QMessageBox::aboutQt(qApp->activeWindow());
}
