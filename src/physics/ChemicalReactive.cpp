/*
 *  ChemicalReactive.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *
 */

#include "Chemicalreactive.h"
    //  ChemicalReactive constructor : naught to do, except passing the bool* to superclass.
ChemicalReactive::ChemicalReactive(bool *ok) : Reactive(ok)
{
}
    //  ChemicalReactiveFactory constructor and destructor, naught to do.
ChemicalReactiveFactory::ChemicalReactiveFactory() : ReactiveFactory()
{
}

ChemicalReactiveFactory::~ChemicalReactiveFactory()
{
}
