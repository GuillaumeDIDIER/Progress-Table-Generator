/*
 *  ChemicalReactive.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *	ChemicalReactive class header
 *  Abstract interface inheriting the Reactive interface
 *  for all chemical reactives (reactives in chemical EquationOld).
 */


#ifndef CHEMICALREACTIVE_H
#define CHEMICALREACTIVE_H

#include "Physique.h"
#include "physics/Reactive.h"
#include "physics/Element.h"
#include "physics/ElementTable.h"



class ChemicalReactive : public Reactive
{
public:
        //  Enum for representing the state of chemicals.
        //  NonMentioned is for the case the state is not known but the reactive admits a state
        //  NoStae is for the case it is non-sensical to speak of state
        //  OtherSoluted is for the case th reaction takes place in solution but not in an Aqueous solution.
        // in that case, te user should specify a string for the state which is stored.
    enum State {
        Invalid=0, NonMentioned, Gas, Liquid, Solid, Aqueous, NoState, OtherSoluted
    };

    ChemicalReactive(bool * ok);    //  Constructor (no comment)
    virtual bool isValid() const =0;//  whether the reactive isValid.
    virtual QString toStr() const =0;   //  String representation.
    virtual QString toHtml() const =0;  //  pretty output format in HTML.
        //  the ChemicalReactive factory, does not add any method to the interface,
        //  but override the return types referring to the Reactive* classes.
    class ChemicalReactiveFactory : public ReactiveFactory {
    protected:
        ChemicalReactiveFactory();          //  constructor and Destructors protected, Singleton Design Pattern
        virtual ~ChemicalReactiveFactory();
    public:
        virtual ChemicalReactive * buildReactive(QString formula) const =0;
            //  the main function, tries to build a reactive from the given formula. Return nul pointer if not possible.
        virtual QString reactiveRX() const =0;
            // returns the regExp validating the string representation of the reactives built by the factory.
            // it may validate some incorect string reppresentations.
    };

    virtual ChemicalReactiveFactory* factory() const =0;
        //  Returns a pointer to the factory corrsponding to the reactive type.
    virtual QMap<const Element*, int> elementMap() const =0;
        //  Returns a QMap associating to all the element which the chemical reactive contains,
        //  the amount in which they are present.
    virtual int charge() const =0;  //  the chemical reactive charge
    virtual State state() const =0; //  its state (NoState if speaking of state is a non sense for the reactive.
    virtual QString stateStr() const =0; // its state, represented by a string.
};

typedef ChemicalReactive::ChemicalReactiveFactory ChemicalReactiveFactory;  //  exports the Fatory in the global namespace.

    //  returns a list containing all the built-in chemical reactive factories.
    //  implemented in BuiltinReactiveFactories.cpp
QList<ChemicalReactiveFactory*> builtinReactiveChemicalFactories();

#endif // CHEMICALREACTIVE_H
