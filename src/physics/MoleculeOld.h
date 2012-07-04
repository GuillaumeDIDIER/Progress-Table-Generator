/*
 *  MoleculeOld.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 23/03/11.
 *  Copyright 2011.
 *	MoleculeOld class header
 */

#ifndef Molecule_H
#define Molecule_H

#include "Physique.h"

#include "physics/Element.h"
#include "physics/ElementTable.h"
    /*	this class represents a molecule
        created using its formula
        it also queries the its periodic element table
        to get informations about the elements which it is constituted by
    */
class MoleculeOld : public QMap<const Element*, int> {
    friend bool operator==(const MoleculeOld& a, const MoleculeOld& b);	//	should not be required
public:
        //	public types
    enum State {
        NonMentioned, Gas, Liquid, Solid, Aqueous
    };
        //	constructors destructors
    MoleculeOld();
    MoleculeOld(QString formula);
    MoleculeOld(const MoleculeOld & other);
    ~MoleculeOld();
        //	accessors
    bool isValid() const;
    void setFormula(QString formula);
    QString rawFormula() const;
    QString htmlRawFormula() const;
    QString formula() const;
    QString htmlFormula() const;
    QSet<const Element*> elementSet() const;
    int charge() const;
    int elementCount(const Element* el) const;

    int order() const;
    void setOrder(int order);

    State state() const;

        //	accessors to the private periodic element table class member
    static ElementTable * getPeriodicElementTable();
    static void setPeriodicElementTable(ElementTable* pTable);

    static const QString nb_rx();
    static const QString symbolNb_rx();
    static const QString symbolNbCapture_rx();
    static const QString bracketed_rx();
    static const QString bracketedCapture_rx();
    static const QString symbolNb_or_Bracketed_rx();
    static const QString chargeCaptured_rx();
    static const QString charge_rx();
    static const QString state_rx();
    static const QString molecule_rx();


protected:
    QStringList replaceBracketInFormula(QString bracketedGroup);
    void addElementFromSymbolNb(QString matchedGroup);
    void setChargeFromRX(QString matchedCharge);
    void setStateFromRX(QString matchedState);
        /*	members :
         c_PeriodicElementTable : the Element Table used by all molecules

         m_formula : the formula given, may not be a raw formula
         m_validComputed : if the m_valid attribute has been computed
         m_valid : if the molecule is valid might exist
         m_charge : its charge
        */
    static ElementTable * c_PeriodicElementTable;
    QString m_formula;
    mutable QString m_htmlFormula;
    mutable bool m_validComputed, m_valid, m_htmlC;
    int m_charge;
    int m_order;	//	for ordering Molecules purpose
    State m_state;
};


#endif // Molecule_H
