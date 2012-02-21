/*
 *  ElementTable.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 19/03/11.
 *  Copyright 2011. All rights reserved.
 *	ElementTable class header
 */
#ifndef ElementTable_H
#define ElementTable_H

#include "Physique.h"

#include "Element.h"
	/*	this class represents the periodic table of element,
		which contains all the valid Elements and can be queried about them,
		by molecules for instance
	 */
class ElementTable : protected QList<Element> {
public:
		//	constructors
	ElementTable();
	ElementTable(QList<Element> elementList);
		//	class methods which create element table
	static ElementTable getDefaultElementTable();
	static ElementTable getEmptyElementTable();
	static ElementTable parseTableFromXml(QString xmlFile, bool* ok = 0);
	static void loadElementTable(QStringList xmlFiles);
	
		//	finders, which are meant to find elements from one caractéristic
	const Element* getElementFromSymbol(QString Symbol) const;
	const Element* getElementFromZ(int Z) const;
		//	Todo implement the others "finders"
		//	Add an append element method ?
};

	//	A periodique element global variable
extern ElementTable PeriodicTable;

#endif