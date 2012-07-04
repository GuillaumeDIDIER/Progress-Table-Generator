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
    Element(QString name,QString symbol, int Z/*, int externalElectron, int maxBond*/, int A = 0);
    Element(const Element& other);
    ~Element();
        //	accessors
    QString name() const;
    QString symbol() const;
    int Z() const;
    int A() const;
/*	int maxBond() const;
    int externalElectronNb() const;*/

        //	Todo add setters

        //	comparison
    static bool symbolCompare(const Element* a, const Element* b);
    static bool ZCompare(const Element* a, const Element* b);
        //	RX
    static const QString element_rx();
protected:
        /*	members
            name : its name
            symbol : its symbol ex Fe
            Z : atomic number
            A : mass number
            maxBond : number of bonds the element can form inside a molecule or ion
            externalElectronNb : number of e- on its external orbit
         */
    QString m_name, m_symbol;
    int m_Z, m_A/*, m_maxBond, m_externalElectronNb*/;

};

#endif
