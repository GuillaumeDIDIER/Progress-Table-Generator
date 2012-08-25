#ifndef CHEMICALEQUATION_H
#define CHEMICALEQUATION_H

#include "Physique.h"
#include "Element.h"
#include "ElementTable.h"
#include "Reactive.h"
#include "ChemicalReactive.h"
#include "Equation.h"

class ChemicalEquation : public Equation
{
public:
    ChemicalEquation(bool *ok = 0);
    virtual ~ChemicalEquation();
    virtual QString toStr() const;
    virtual QString toHtml() const;
    virtual State state() const;

    class ChemicalEquationFactory : public EquationFactory
    {
    protected:
        ChemicalEquationFactory();
        virtual ~ChemicalEquationFactory();
    public:
        virtual ChemicalEquation* buildEquation(QString stringRepr) const;
        static ChemicalEquationFactory* getInstance();
    private:
        static ChemicalEquationFactory* instance;
    };
    virtual ChemicalEquationFactory* factory() const;
};

typedef ChemicalEquation::ChemicalEquationFactory ChemicalEquationFactory;
#endif // CHEMICALEQUATION_H
