/*
 *  Electron.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 2/07/12.
 *  Copyright 2012.
 *	Electron class header
 *  Concrete implementation of the Chemical reactive interface,
 *  for representing electrons.
 */


#ifndef ELECTRON_H
#define ELECTRON_H

#include "Physique.h"
#include "physics/ChemicalReactive.h"
#include "physics/Element.h"
#include "physics/ElementTable.h"


class Electron : public ChemicalReactive
{
public:
    Electron(QString symbol="e-", bool* ok = 0);    //  constructor :
        //  the symbol defaults to "e-" because it is the only valid formula.
        //  if it differs, the electron is not valid.
    static QString reactiveRX(){return QString(" *e- *");}
        //  the reg exp returned by the reactiveRX() method of ReactiveFactories
    virtual bool isValid() const{return m_valid;}   //  whether the reactive isValid.
        //  see doc there.
    virtual QString toStr() const{return QString("e-");}    //  String Repr
    virtual QString toHtml() const{return QString("e<sup>-</sup>");}    //  pretty output format in HTML.
        //  The electron Factory
    class ElectronFactory : public ChemicalReactiveFactory {
    protected:
        ElectronFactory();          //  constructor and Destructors protected, Singleton Design Pattern
        virtual ~ElectronFactory();
    public:
        virtual Electron * buildReactive(QString formula="e-") const;
            //  the main function, tries to build a reactive from the given formula. Return nul pointer if not possible.
        virtual QString reactiveRX() const{return Electron::reactiveRX();}
        // returns the regExp validating the string representation of the reactives built by the factory.
        // it may validate some incorect string reppresentations.
        static ElectronFactory* getInstance();  //  for singleton access
    private:
        static ElectronFactory* instance;
    };

    virtual ElectronFactory* factory() const{return ElectronFactory::getInstance();}
        //  Returns a pointer to the factory corrsponding to the reactive type.
    virtual QMap<const Element*, int> elementMap() const{return QMap<const Element*, int>();}
        //  Returns a QMap associating to all the element which the chemical reactive contains, in the present case an empty one.
    virtual int charge() const {return -1;} //  the chemical reactive charge
    virtual State state() const {return NoState;}   //  the stae is NoState, it is non-sensical to speak of an electron's state
    virtual QString stateStr() const {return QString("");}  //  the State String is empty, an electron hasn't got a state.

private:
    bool m_valid;   //  whether the electron is valid.

};

typedef Electron::ElectronFactory ElectronFactory;

#endif // ELECTRON_H
