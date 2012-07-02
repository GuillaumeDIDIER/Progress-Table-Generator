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
        NonMentioned, Gas, Liquid, Solid, Aqueous, NoState, OtherSoluted, OtherNonSoluted
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
        virtual QString reactiveRX() const =0; // returns the regExp validating the reactives built by the factory.
    };

    virtual ChemicalReactiveFactory* factory() const =0;
    virtual QMap<const Element*, int> elementMap() const =0;
    virtual int charge() const =0;
    virtual State state() const =0;
};

typedef ChemicalReactive::ChemicalReactiveFactory ChemicalReactiveFactory;

#endif // CHEMICALREACTIVE_H
