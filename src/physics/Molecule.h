/*
 *  Molecule.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 2/07/12.
 *  Copyright 2012.
 *	Molecule class header
 *  Concrete implementation of the Chemical reactive interface,
 *  for representing chemical compounds.
 */


#ifndef MOLECULE_H
#define MOLECULE_H

#include "Physique.h"
#include "physics/ChemicalReactive.h"
#include "physics/Element.h"
#include "physics/ElementTable.h"

class Molecule  : public ChemicalReactive, private QMap<const Element*, int>
{
public:
    Molecule(QString formula, bool* ok=0);  // constructor (no comment)
    virtual ~Molecule();    //  destructor (no comment)
    virtual bool isValid() const;   //  whether the molecule is valid
    virtual QString toStr() const{return m_formula;}    //  String representation
    virtual QString toHtml() const; //  pretty output format in HTML
    virtual bool setFormula(QString newFormula);    //  Sets the molecule formula (modifies the molecule)
    static QString reactiveRX(){return QString(
"(?:(?:(?:[A-Z][a-z]{0,2})|(?:\\([A-Z][a-z]{0,2})|\\))\\d*)*"                       //  Mainformula
"(?:\\[\\d*(?:\\-|\\+)\\])?(?:\\(([a-z]{1,2})\\))?(?:\\[\\d*(?:\\-|\\+)\\])?");}    //  charge and state
    // returns the regExp validating the string representation of the reactives the class represents.
    // it may validate some incorect string reppresentations.

    static ElementTable * getPeriodicElementTable();            //  accessors to the class wide ElementTable
    static void setPeriodicElementTable(ElementTable* pTable);
        //  Factory
    class MoleculeFactory : public ChemicalReactiveFactory {
    protected:
        MoleculeFactory();          //  constructor and Destructors protected, Singleton Design Pattern
        virtual ~MoleculeFactory();
    public:
        virtual Molecule * buildReactive(QString formula) const;
            //  the main function, tries to build a reactive from the given formula. Return nul pointer if not possible.
        virtual QString reactiveRX() const{return Molecule::reactiveRX();}
            // returns the regExp validating the string representation of the reactives built by the factory.
            // it may validate some incorect string reppresentations.
        static MoleculeFactory* getInstance();  //  accessor to the global instance
    private:
        static MoleculeFactory* instance;   //  global instance
    };

    virtual MoleculeFactory* factory() const{return MoleculeFactory::getInstance();}
        //  Returns a pointer to the factory corrsponding to the reactive type.
    virtual QMap<const Element*, int> elementMap() const;
        //  Returns a QMap associating to all the element which the chemical reactive contains,
        //  the amount in which they are present.
    virtual int charge() const{return m_charge;}    //  the chemical reactive charge
    virtual State state() const{return m_state;}    //  its state (NoState if speaking of state is a non sense for the reactive.
    virtual QString stateStr() const {return m_state_str;}  // its state, represented by a string.
private:
        /* instance members :
         *  m_formula : the string representation
         *  m_charge : the charge
         *  m_state : the state
         *  m_state_str : the state in string form
         *
         * class members :
         *  c_PeriodicTable : the global PeriodicTable used for building molecules.
         */
    QString m_formula;
    int m_charge;
    State m_state;
    static ElementTable * c_PeriodicElementTable;
    QString m_state_str;
};

typedef Molecule::MoleculeFactory MoleculeFactory;  //  exports the Fatory in the global namespace.
#ifdef DEBUG
QDebug operator<<(QDebug out, const Molecule & mol);
#endif


#endif // MOLECULE_H
