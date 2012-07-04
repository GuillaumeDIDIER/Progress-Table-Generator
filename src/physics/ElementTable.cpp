/*
 *  ElementTable.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 19/03/11.
 *  Copyright 2011.
 * ElementTable class implementation
 */

#include "Physique.h"

#include "physics/ElementTable.h"
    //	the actual global peridic table variable
#ifndef XMLPARSER_INCLUDED
ElementTable PeriodicTable = ElementTable::getDefaultElementTable();

#else
ElementTable PeriodicTable;

#endif

    //	default constructor, not very useful
ElementTable::ElementTable(){}

    //	A useful constructor, with some check up
ElementTable::ElementTable(QList<Element> elementList){
    QMutableListIterator<Element> i(elementList);
    Element el;
    while (i.hasNext()) {
        QSet<int> Zs;
        QSet<QString> names, symbols;

        el = i.peekNext();

        if (Zs.contains(el.Z())) {
            i.remove();
        }
        else {
            Zs += el.Z();
        }

        if (names.contains(el.name())) {
            i.remove();
        }
        else {
            names += el.name();
        }

        if (symbols.contains(el.symbol())) {
            i.remove();
        }
        else {
            symbols += el.symbol();
        }
    } // check up for duplicates, remove duplicated lement second occurence

    append(elementList);
}
    //	the class method which initialise the ElementTable
void ElementTable::loadElementTable(QStringList xmlFiles){
    bool ok(false);
    foreach(QString xmlFile ,xmlFiles){
        PeriodicTable = parseTableFromXml(xmlFile, &ok);
        if (ok) {
#ifdef DEBUG
            qDebug() << "parsed table from : " << xmlFile;
#endif
            break;
        }
    }
    if (!ok) {
        PeriodicTable = getDefaultElementTable();
#ifdef DEBUG
            qDebug() << "error : falling back to Default";
#endif
    }
}

    //	A class method which return a built in element table, only meant for unit tests
ElementTable ElementTable::getDefaultElementTable(){
    ElementTable elementTable;

        // to complete with correct values
    elementTable << Element("Hydrogene", "H", 1,1);
    elementTable << Element("Carbon", "C", 6, 12);
    elementTable << Element("Oxygene", "O", 8, 16);
    elementTable << Element("Chlorine", "Cl", 17, 35);

    return elementTable;
}

    //	a class methods which parses an ElementTable xml file to create a Periodic element table
    //	to implement for final version
ElementTable ElementTable::parseTableFromXml(QString xmlFileName, bool* ok){
    ElementTable XMLParsedElementTable;
    QFile schemaFile(":/ElementTable.xsd");
    if (schemaFile.open(QIODevice::ReadOnly)) {
        QXmlSchema schema;
        schema.load(&schemaFile);
        if (schema.isValid()) {
            QXmlSchemaValidator XMLElmtTblValidator(schema);
            QDomDocument XMLElementTable;
            if(QFile::exists(xmlFileName)){
                QFile XMLElementTableFile(xmlFileName);
                if (XMLElementTableFile.open(QIODevice::ReadOnly)) {
                    if (XMLElmtTblValidator.validate(&XMLElementTableFile, QUrl::fromLocalFile(xmlFileName)) && XMLElementTableFile.reset() && XMLElementTable.setContent(&XMLElementTableFile)) {
                        XMLElementTableFile.close();
                        QDomElement domRoot = XMLElementTable.documentElement();	//	the document Root
                        QDomNode node = domRoot.firstChild();			//	the first child
                        while (!node.isNull()) {						//	looping for all the childs
                            QDomElement element = node.toElement();		//	Node to Element
                            if (!element.isNull() && element.tagName() == "Element") {	//	if the nod was an <Element> Element
                                QString name(element.firstChildElement("Name").text()), symbol(element.firstChildElement("Symbol").text());
                                //	extracting the informations
                                int Z(element.firstChildElement("Z").text().toInt()), A(element.firstChildElement("A").text().toInt());
                                //	adding to the ElementTable a brand new Element initialized with those datas
                                XMLParsedElementTable << Element(name, symbol, Z, A);
                            }
                            node = node.nextSibling();	// next node
                        }
                        *ok = true;
                        return XMLParsedElementTable;			//	returning the parsed table
                    }
                }
            }
        }
    }
    if (ok)
        *ok = false;
    return ElementTable();
}

    //	finders
        //	from symbol
const Element* ElementTable::getElementFromSymbol(QString Gsymbol) const{
    QListIterator<Element> i(*this);
    while (i.hasNext()) {
        if ((i.peekNext()).symbol() == Gsymbol) {
            return &(i.next());
        }
        else {
            i.next();
        }

    }
    return 0;

}
        //	from atomic number
const Element* ElementTable::getElementFromZ(int Z) const{
    QListIterator<Element> i(*this);
    while (i.hasNext()) {
        if (i.peekNext().Z() == Z) {
            return &(i.next());
        }
    }
    return 0;
}

    //  Todo implement the others "finders"

