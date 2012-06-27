######################################################################
# Automatically generated by qmake (2.01a) mer. fvr. 2 17:47:29 2011
######################################################################
# copyright Guillaume DIDIER 2011
######################################################################

TEMPLATE = app
CONFIG += debug_and_release build_all warn_on
QT += xml xmlpatterns
mac:TARGET = "Progress Table Generator"
else:TARGET = ProgressTableGenerator
VERSION = 0.1.0 # Changes must be reflected in Physique.h and InfoPlist-Physique.plist
DESTDIR = ../bin
DEPENDPATH += .
INCLUDEPATH += . "../include"

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc

# Input
GuiSources += gui/MainMenuBar.cpp gui/MainWindow.cpp gui/ProgressTableW.cpp
PhysicsSources += physics/Element.cpp physics/ElementTable.cpp physics/Molecule.cpp physics/Equation.cpp
SOURCES += main.cpp $$GuiSources $$PhysicsSources

PrivateGuiHeader = gui/MainWindow_p.h
GuiHeaders += gui/MainMenuBar.h gui/MainWindow.h gui/ProgressTableW.h  $$PrivateGuiHeader
PhysicsHeaders += physics/Element.h physics/ElementTable.h physics/Molecule.h physics/Equation.h
HEADERS += Physique.h $$GuiHeaders $$PhysicsHeaders
TRANSLATIONS += ../translation/Physique_fr.ts

RESOURCES += Physique.qrc

macx {
    include(../Physique_mac.pri)
    QMAKE_INFO_PLIST = platform/mac/InfoPlist-Physique.plist
    Bundle_Ressources.files = platform/mac/en.lproj platform/mac/fr.lproj
    Bundle_Ressources.path = Contents/Resources
    QMAKE_BUNDLE_DATA += Bundle_Ressources
    RESOURCES  += platform/mac/Physique_mac.qrc
    ICON = platform/mac/Erlenmeyer.icns
}
else{
    win32:RESOURCES += platform/windows/Physique_win.qrc
    else:RESOURCES += platform/other/Physique_other.qrc
}
CONFIG(debug) {
     DEFINES += DEBUG
     SOURCES += unittest/UnitTest.cpp
     HEADERS += unittest/UnitTest.h
}
win32:RC_FILE = platform/windows/Physique_win_icon.rc
DEFINES += XMLPARSER_INCLUDED
macx-xcode:DEFINES += VERSION=\"$${VERSION}\"
!macx-xcode:DEFINES += VERSION=\\\"$${VERSION}\\\"
elementTableFiles += physics/XmlElementTable/ElementTable.xml

OTHER_FILES += elementTableFiles
