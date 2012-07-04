/*
 *  Reactive.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *
 */

#include "Reactive.h"

Reactive::Reactive(bool *ok)
{
    if(ok)
        *ok = true;
}


ReactiveFactory::ReactiveFactory()
{
}

ReactiveFactory::~ReactiveFactory()
{
}
