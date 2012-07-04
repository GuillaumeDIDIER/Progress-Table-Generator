/*
 *  ChemicalReactive.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *
 */

#include "Chemicalreactive.h"

ChemicalReactive::ChemicalReactive(bool *ok) : Reactive(ok)
{
}

ChemicalReactiveFactory::ChemicalReactiveFactory() : ReactiveFactory()
{
}

ChemicalReactiveFactory::~ChemicalReactiveFactory()
{
}
