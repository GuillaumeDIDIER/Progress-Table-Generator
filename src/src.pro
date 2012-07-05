######################################################################
# Automatically generated by qmake (2.01a) mer. fvr. 2 17:47:29 2011
######################################################################
# copyright Guillaume DIDIER 2011
######################################################################

TEMPLATE = app
CONFIG += debug_and_release build_all warn_on fix_output_dir
QT += xml xmlpatterns
mac:TARGET = "Progress Table Generator"
else:TARGET = ProgressTableGenerator
VERSION = 0.2 # Changes must be reflected in Physique.h and InfoPlist-Physique.plist
DESTDIR = ../bin
DEPENDPATH += .
INCLUDEPATH += . "../include"

# Input
GuiSources += gui/MainMenuBar.cpp gui/MainWindow.cpp gui/ProgressTableW.cpp
PhysicsSources += physics/Element.cpp physics/ElementTable.cpp physics/MoleculeOld.cpp physics/EquationOld.cpp \
physics/Reactive.cpp physics/ChemicalReactive.cpp physics/Electron.cpp physics/Molecule.cpp
SOURCES += main.cpp $$GuiSources $$PhysicsSources

PrivateGuiHeader = gui/MainWindow_p.h
GuiHeaders += gui/MainMenuBar.h gui/MainWindow.h gui/ProgressTableW.h  $$PrivateGuiHeader
PhysicsHeaders += physics/Element.h physics/ElementTable.h physics/MoleculeOld.h physics/EquationOld.h \
physics/Reactive.h physics/ChemicalReactive.h physics/Electron.h physics/Molecule.h
HEADERS += Physique.h $$GuiHeaders $$PhysicsHeaders

TRANSLATIONS += ../translation/Physique_fr.ts

RESOURCES += Physique.qrc

macx {
    include(../Physique_mac.pri)
    QMAKE_INFO_PLIST = platform/mac/InfoPlist-Physique.plist
    RESOURCES  += platform/mac/Physique_mac.qrc
    ICON = platform/mac/Erlenmeyer.icns
}
else{
    win32:RESOURCES += platform/windows/Physique_win.qrc
    else:RESOURCES += platform/other/Physique_other.qrc
}
build_pass:CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    SOURCES += unittest/UnitTest.cpp
    HEADERS += unittest/UnitTest.h
    macx{
        Bundle_Ressources.files = platform/mac/debug/en.lproj platform/mac/debug/fr.lproj
        Bundle_Ressources.path = Contents/Resources
        QMAKE_BUNDLE_DATA += Bundle_Ressources
        TARGET = "$${TARGET} debug"
    }else:TARGET = $${TARGET}d
    OBJECTS_DIR = .obj/debug
    MOC_DIR = .moc/debug
    RCC_DIR = .rcc/debug
}
build_pass:CONFIG(release, debug|release){
    macx{
        Bundle_Ressources.files = platform/mac/release/en.lproj platform/mac/release/fr.lproj
        Bundle_Ressources.path = Contents/Resources
        QMAKE_BUNDLE_DATA += Bundle_Ressources
    }

    OBJECTS_DIR = .obj/release
    MOC_DIR = .moc/release
    RCC_DIR = .rcc/release
}
win32:RC_FILE = platform/windows/Physique_win_icon.rc
DEFINES += XMLPARSER_INCLUDED
macx-xcode:DEFINES += VERSION=\"$${VERSION}\"
!macx-xcode:DEFINES += VERSION=\\\"$${VERSION}\\\"
elementTableFiles += physics/XmlElementTable/ElementTable.xml

OTHER_FILES += elementTableFiles
