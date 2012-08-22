#ifndef BUILTINREACTIVEFACTORIES_H
#define BUILTINREACTIVEFACTORIES_H

#include "Physique.h"

#include "Reactive.h"
#include "ChemicalReactive.h"

    //  functions
    //  return the list of the built-in factories for the specified interface.
QList<ReactiveFactory*> builtinReactiveFactories();
QList<ChemicalReactiveFactory*> builtinReactiveChemicalFactories();


#endif // BUILTINREACTIVEFACTORIES_H
