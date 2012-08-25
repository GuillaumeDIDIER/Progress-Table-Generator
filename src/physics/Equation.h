#ifndef EQUATION_H
#define EQUATION_H

#include "Physique.h"
#include "Element.h"
#include "ElementTable.h"
#include "Reactive.h"

class Equation
{
public:
    Equation( bool* ok = 0);
    virtual ~Equation();
    virtual QString toStr() const =0;
    virtual QString toHtml() const =0;
    enum State {
        Invalid = 0, UnEquilibrated, Equilibrated, EquilibratingError
    };
    virtual State state() const =0;

    class EquationFactory{
    protected:
        EquationFactory();
        virtual ~EquationFactory();
    public:
        virtual Equation* buildEquation(QString stringRepr) const =0;   //  possibly overloaded
    };
    virtual EquationFactory* factory() const =0;
};
typedef Equation::EquationFactory EquationFactory;
#endif // EQUATION_H
