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
    while (i.hasNext()) {   //  looping over the list of element
        QSet<int> Zs;   //  set of atomic numbers
        QSet<QString> names, symbols;   //  set of names and symbols

        el = i.peekNext();  //  getting the next element

        if (Zs.contains(el.Z())) {  //  checking for Z duplicates
            i.remove(); //  remove duplicate
        }
        else {
            Zs += el.Z();   //  or add the new Z to the existing Z
        }

        if (names.contains(el.name())) {    //  checking for name duplicates
            i.remove();
        }
        else {
            names += el.name();
        }

        if (symbols.contains(el.symbol())) {    //  checking for Symbol duplicates
            i.remove();
        }
        else {
            symbols += el.symbol();
        }
    } // check up for duplicates, remove duplicated element second occurence

    append(elementList);    //  set the internal QList to the freshly checked list
}
    //	the class method which initialise the ElementTable
void ElementTable::loadElementTable(QStringList xmlFiles){
    bool ok(false); //  bool set to true as soon as loading is done
    foreach(QString xmlFile ,xmlFiles){ //  for each name
        PeriodicTable = parseTableFromXml(xmlFile, &ok);    //  try to read table from that name
        if (ok) {   //  if the parsing was done
#ifdef DEBUG
            qDebug() << "parsed table from : " << xmlFile;
#endif
            break;  //  we are finished, breaking out of the loop
            //  (and as ok is true we reach end of function)
        }
    }
    if (!ok) {  //  if we exited from the loop by the end ok is false, and no file name contained a valid element table
        PeriodicTable = getDefaultElementTable();   //  thus we fall back to default
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
ElementTable ElementTable::parseTableFromXml(QString xmlFileName, bool* ok){
    ElementTable XMLParsedElementTable; //  return value
    QFile schemaFile(":/ElementTable.xsd"); //  creating a file from the built-in schema
    if (schemaFile.open(QIODevice::ReadOnly)) { //  opening it
        QXmlSchema schema;          //  creating a schema and loading the file
        schema.load(&schemaFile);
        if (schema.isValid()) { //  if it is valid
            QXmlSchemaValidator XMLElmtTblValidator(schema);    //  we create a XMLSchema Validator
                                                    //  to validate the Schema of the ElementTable
            QDomDocument XMLElementTable;   //  we create a DOM document object DOM object representing the whole document            if(QFile::exists(xmlFileName)){
            QFile XMLElementTableFile(xmlFileName); //  we create a file from the file name
            if (XMLElementTableFile.open(QIODevice::ReadOnly)) {    //  we open it
                if (XMLElmtTblValidator.validate(&XMLElementTableFile, QUrl::fromLocalFile(xmlFileName)) && XMLElementTableFile.reset() && XMLElementTable.setContent(&XMLElementTableFile)) {
                    //  if the schema validates the file and if after reseting the position to origin the DOMDocument can parse it
                    XMLElementTableFile.close();    //  we can close the file
                    QDomElement domRoot = XMLElementTable.documentElement();	//	the document Root
                    QDomNode node = domRoot.firstChild();			//	the first child
                    while (!node.isNull()) {						//	looping for all the childs
                        QDomElement element = node.toElement();		//	Node to DOM Element
                        if (!element.isNull() && element.tagName() == "Element") {	//	if the nod was an <Element> DOM Element
                            QString name(element.firstChildElement("Name").text()), symbol(element.firstChildElement("Symbol").text());
                            //	extracting the informations
                            int Z(element.firstChildElement("Z").text().toInt()), A(element.firstChildElement("A").text().toInt());
                            //	adding to the ElementTable a brand new Element initialized with those datas
                            XMLParsedElementTable << Element(name, symbol, Z, A);
                        }
                        node = node.nextSibling();	// next node
                    }
                    if (ok)
                        *ok = true; //  we have parsed the table successfully
                    return XMLParsedElementTable;			//	returning the parsed table
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
    while (i.hasNext()) {   //  we loop over the element list
        if ((i.peekNext()).symbol() == Gsymbol) {   //  if we have found the symbol we are looking for
            return &(i.next()); //  we return it
        }
        else {
            i.next();   //  else we continue with the next iteration
        }

    }
    return 0;

}
        //	from atomic number
const Element* ElementTable::getElementFromZ(int Z) const{  //  same thing as before
    QListIterator<Element> i(*this);
    while (i.hasNext()) {
        if (i.peekNext().Z() == Z) {
            return &(i.next());
        }
    }
    return 0;
}

    //  Todo implement the others "finders"

