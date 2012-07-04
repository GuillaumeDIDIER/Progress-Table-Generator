/*
 *  Electron.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 2/07/12.
 *  Copyright 2012.
 *
 */

#include "Electron.h"

ElectronFactory* ElectronFactory::instance = (ElectronFactory*)0;

Electron::Electron(QString symbol, bool *ok) : ChemicalReactive(ok)
{
    if(!(symbol.remove(' ')!="e-"))
        *ok = false;
}

ElectronFactory::ElectronFactory()
{
}

ElectronFactory::~ElectronFactory()
{
    instance = (ElectronFactory*)0;
}

Electron * ElectronFactory::buildReactive(QString formula) const
{
    if(QRegExp(reactiveRX()).exactMatch(formula))
    {
        bool ok(true);
        Electron * electron = new Electron(formula, &ok);
        if(ok)
            return electron;

        delete electron;
    }
    return 0;
}
ElectronFactory* ElectronFactory::getInstance()
{
    if(instance)
        return instance;
    return instance = new ElectronFactory();
}
//ChemicalReactive * ElectronFactory::buildReactive(QString formula) const
//{
//    return buildReactive(formula);
//}
