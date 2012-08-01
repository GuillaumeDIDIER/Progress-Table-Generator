/*
 *  ElementTable.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 19/03/11.
 *  Copyright 2011.
 *	ElementTable class header
 */
#ifndef ElementTable_H
#define ElementTable_H

#include "Physique.h"

#include "physics/Element.h"
    /*	this class represents the periodic table of element,
        which contains all the valid Elements and can be queried about them,
        by molecules for instance
     */
class ElementTable : protected QList<Element> {
public:
        //	constructors
    ElementTable();
    ElementTable(QList<Element> elementList);   //  Designed consructor
        //	class methods which create element table
    static ElementTable getDefaultElementTable();   //  create an ElementTable filed with only a few Elements, for test purposes
    static ElementTable getEmptyElementTable(); //  an empty ElementTable
    static ElementTable parseTableFromXml(QString xmlFile, bool* ok = 0);   //  try to parse an element table from the specified file
    static void loadElementTable(QStringList xmlFiles); //  try to load the global Periodic Element Table from the specified files

        //	finders, which are meant to find elements from one caractéristic
    const Element* getElementFromSymbol(QString Symbol) const;
    const Element* getElementFromZ(int Z) const;
        //	Todo implement the others "finders"
        //	Add an append element method ?
};

    //	A periodique element global variable
extern ElementTable PeriodicTable;

#endif
