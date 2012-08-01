/*
 *  Reactive.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *  Reactive class implementation.
 *  See header for documentation.
 */

#include "Reactive.h"

    //  Reactive constructor : naught to do, except setting the boolean ok to true, if the pointer isn't nul.
Reactive::Reactive(bool *ok)
{
    if(ok)
        *ok = true;
}

    //  ReactiveFactory constructor and destructor, naught to do.
ReactiveFactory::ReactiveFactory()
{
}

ReactiveFactory::~ReactiveFactory()
{
}
