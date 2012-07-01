#ifndef REACTIVE_H
#define REACTIVE_H

#include "Physique.h"


class Reactive
{
public:
    Reactive();
    virtual bool isValid() const =0;
    virtual QString toStr() const =0;
    virtual QString toHtml() const =0;

    class ReactiveFactory{
    protected:
        ReactiveFactory();
    public:
        virtual Reactive * buildReactive(QString formula) =0; // Return nul pointer if not possible.
        virtual QString reactiveRX() =0; // returns the regExp validating the reactives built by the factory.
    };

    virtual ReactiveFactory* factory() const =0;
};

typedef Reactive::ReactiveFactory ReactiveFactory;

#endif // REACTIVE_H
