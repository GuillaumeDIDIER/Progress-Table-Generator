/*
 *  EquationOld.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011.
 *
 */

#ifndef EquationOld_H
#define EquationOld_H

#include "Physique.h"

#include "physics/Element.h"
#include "physics/MoleculeOld.h"
#include "physics/ElementTable.h"



class EquationOld {
    friend bool operator==(const EquationOld& a, const EquationOld& b);
    friend bool operator!=(const EquationOld& a, const EquationOld& b){return !(a == b);}
public:
    EquationOld();							//	should not be used
    EquationOld(const EquationOld& other);	//	neither
    EquationOld(QString reactivesStr, QString productsStr);	//	"designed constructor" (Objc allusion)
    ~EquationOld();

    bool equilibrate();
    bool isValid() const;
    bool isEquilibrated() const;
    bool isEmpty() const { return (m_reactives.isEmpty() || m_products.isEmpty());}

    QString reactivesStr(bool html = false) const;
    QString productsStr(bool html = false) const;
    QString toStr() const;
    QString toHtml() const;

    int stoechNumOf(MoleculeOld mol) const;
    QSet<const Element*> elementSet() const;

    QMap<MoleculeOld, int> reactives();
    QMap<MoleculeOld, int> products();


    static const QString EquationOld_rx();


protected:
    mutable bool m_validComputed, m_valid, m_equilibratedC, m_equilibrated;	//	for caching purposes, might be suppressed
    QMap<MoleculeOld, int> m_reactives, m_products;
        //	respectively for reactives and	products
    static QMap<MoleculeOld, int> memberStrToDict(QString str, int start = 0);
    static QString memberDictToStr(QMap<MoleculeOld, int> map, bool html = false);
};



#ifdef DEBUG
QDebug& operator<<(QDebug& out, const MoleculeOld& mol);
QDebug& operator<<(QDebug& out, const EquationOld& eq);
#endif	//	ifdef DEBUG
bool moleculeLessThanUser(const MoleculeOld& a, const MoleculeOld& b);
#endif	//	ifndef EquationOld_H
