#ifndef CHEMICALREACTIVE_H
#define CHEMICALREACTIVE_H

#include "Physique.h"
#include "physics/Reactive.h"
#include "physics/Element.h"
#include "physics/ElementTable.h"



class ChemicalReactive : public Reactive
{
public:
    ChemicalReactive();
    virtual bool isValid() const =0;
    virtual QString toStr() const =0;
    virtual QString toHtml() const =0;
    class ChemicalReactiveFactory : public ReactiveFactory {
    protected:
        ChemicalReactiveFactory();
    public:
        virtual ChemicalReactive * buildReactive(QString formula) =0; // Return nul pointer if not possible.
    };
    virtual ChemicalReactiveFactory* factory() const =0;
    virtual QMap<const Element*, int> elementMap() const =0;
    virtual int charge() const =0;
};

typedef ChemicalReactive::ChemicalReactiveFactory ChemicalReactiveFactory;

#endif // CHEMICALREACTIVE_H
