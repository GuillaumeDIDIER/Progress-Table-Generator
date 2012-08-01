/*
 *  Reactive.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *	Reactive class header
 *  This class export an abstract interface for all the reactive,
 *  of any type of reaction (Chemical, Nuclear).
 *  Each reactive type should also have an associated factory singleton class,
 *  inheriting the ReactiveFactory abstract class.
 */


#ifndef REACTIVE_H
#define REACTIVE_H

#include "Physique.h"


class Reactive
{
public:
    Reactive(bool * ok = 0);        //  Constructor (no comment)
    virtual bool isValid() const =0;//  whether the reactive isValid.
    virtual QString toStr() const =0;   //  String representation.
    virtual QString toHtml() const =0;  //  pretty output format in HTML.
        //  the factory.
    class ReactiveFactory{
    protected:
        ReactiveFactory();          //  constructor and Destructors protected, Singleton Design Pattern
        virtual ~ReactiveFactory();
    public:
        virtual Reactive * buildReactive(QString formula) const =0;
            //  the main function, tries to build a reactive from the given formula. Return nul pointer if not possible.
        virtual QString reactiveRX() const =0;
            // returns the regExp validating the string representation of the reactives built by the factory.
            // it may validate some incorect string reppresentations.
    };

    virtual ReactiveFactory* factory() const =0;    //  Returns a pointer to the factory corrsponding to the reactive type.
};

typedef Reactive::ReactiveFactory ReactiveFactory;  //  exports the Fatory in the global namespace.

#endif // REACTIVE_H
