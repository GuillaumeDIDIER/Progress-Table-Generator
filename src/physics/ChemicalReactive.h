/*
 *  ChemicalReactive.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *	ChemicalReactive class header
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
    enum State {
        Invalid=0, NonMentioned, Gas, Liquid, Solid, Aqueous, NoState, OtherSoluted
    };

    ChemicalReactive(bool * ok);
    virtual bool isValid() const =0;
    virtual QString toStr() const =0;
    virtual QString toHtml() const =0;
    class ChemicalReactiveFactory : public ReactiveFactory {
    protected:
        ChemicalReactiveFactory();
        virtual ~ChemicalReactiveFactory();
    public:
        virtual ChemicalReactive * buildReactive(QString formula) const =0; // Return nul pointer if not possible.
        virtual QString reactiveRX() const =0;
        // returns the regExp validating the string representation of the reactives built by the factory.
        // it may validate some incorect string reppresentations.
    };

    virtual ChemicalReactiveFactory* factory() const =0;
    virtual QMap<const Element*, int> elementMap() const =0;
    virtual int charge() const =0;
    virtual State state() const =0;
};

typedef ChemicalReactive::ChemicalReactiveFactory ChemicalReactiveFactory;

#endif // CHEMICALREACTIVE_H
