/*
 *  Electron.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 2/07/12.
 *  Copyright 2012.
 *
 */

#include "Electron.h"
    //  singleton design patern.
ElectronFactory* ElectronFactory::instance = (ElectronFactory*)0;
    //  constructor
Electron::Electron(QString symbol, bool *ok) : ChemicalReactive(ok)
{
    if(!(symbol.remove(' ')!="e-")){    //  if the symbol is not "e-" (with perhaps some spaces)
        m_valid = false;    //  the reactive is not valid
        if(ok)  //  and if the pointer isn't null
            ok = false; //  set ok to false
    }

}
    //  factory constructor and destructors.
ElectronFactory::ElectronFactory()
{
}

ElectronFactory::~ElectronFactory()
{
    instance = (ElectronFactory*)0; //  not to leave the pointer dangling.
}

Electron * ElectronFactory::buildReactive(QString formula) const
{
    if(QRegExp(reactiveRX()).exactMatch(formula))   //  check the match of the RX
    {
        bool ok(true);  //  boolean passed to Electron constructor
        Electron * electron = new Electron(formula, &ok);   //  new electron from formula
        if(ok)  //  if it was ok
            return electron;    //  return it

        delete electron;    //  if not delete it
    }
    return 0;   //  return 0 if the passed formula is not an electron
}
ElectronFactory* ElectronFactory::getInstance() //  creates a new instance if it does not exist (instance == 0)
{
    if(instance)    //  if instance exist
        return instance;    //  return it
    return instance = new ElectronFactory();    //  else create a new instance and return it
}
