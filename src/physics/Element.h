/*
 *  Element.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 18/02/11.
 *  Copyright 2011.
 *	Element class header
 */

#ifndef Element_H
#define Element_H

#include "Physique.h"
    //	this class represents an Element, ex Oxygene, Hydrogene etc
class Element {
public:
        //	constructors destructors
    Element();
    Element(QString name, QString symbol, int Z, int A = 0); //  designed constructor
    Element(const Element& other);  //  copy constructor
    ~Element();
        //	accessors : see member doc in the protected field
    QString name() const;
    QString symbol() const;
    int Z() const;
    int A() const;

        //	comparison
    static bool symbolCompare(const Element* a, const Element* b);  //  Compare by symbol
    static bool ZCompare(const Element* a, const Element* b);   //  Compare by Z
        //	RX
    static const QString element_rx();  //  returns a regexp validating an element Symbol
protected:
        /*	members
            name : its name
            symbol : its symbol ex Fe
            Z : atomic number (the number of protons in its nucleus)
            A : mass number (the number of nucleons in its nucleus)
         */
    QString m_name, m_symbol;
    int m_Z, m_A;

};

#endif
