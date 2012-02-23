######################################################################
# Automatically generated by qmake (2.01a) mer. fvr. 2 17:47:29 2011
######################################################################
# copyright Guillaume DIDIER 2011
######################################################################

TEMPLATE = app
CONFIG += debug_and_release build_all warn_on
# CONFIG += release
QT += xml xmlpatterns
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

mac:TARGET = "Progress Table Generator"
else:TARGET = ProgressTableGenerator
VERSION = 0.1.0

# Input
GuiSources += MainMenuBar.cpp MainWindow.cpp ProgressTableW.cpp
PhysicsSources += Element.cpp ElementTable.cpp Molecule.cpp Equation.cpp
SOURCES += main.cpp $$GuiSources $$PhysicsSources

PrivateGuiHeader = MainWindow_p.h
GuiHeaders += MainMenuBar.h MainWindow.h ProgressTableW.h  $$PrivateGuiHeader
PhysicsHeaders += Element.h ElementTable.h Molecule.h Equation.h
HEADERS += Physique.h $$GuiHeaders $$PhysicsHeaders
TRANSLATIONS += Physique_fr.ts

RESOURCES += Physique.qrc

macx {
	include(Physique_mac.pri)
	QMAKE_INFO_PLIST = InfoPlist-Physique.plist
	Bundle_Ressources.files = en.lproj fr.lproj
	Bundle_Ressources.path = Contents/Resources
	QMAKE_BUNDLE_DATA += Bundle_Ressources
	RESOURCES  += Physique_mac.qrc
	ICON = Erlenmeyer.icns
}
else:RESOURCES += Physique_other.qrc
CONFIG(debug) {
	 DEFINES += DEBUG
	 SOURCES += UnitTest.cpp
	 HEADERS += UnitTest.h
}
win32:RC_FILE = Physique_win_icon.rc
DEFINES += XMLPARSER_INCLUDED

elementTableFiles += ElementTable.xml

OTHER_FILES += elementTableFiles