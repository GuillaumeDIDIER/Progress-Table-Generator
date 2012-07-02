#ifndef MOLECULE_H
#define MOLECULE_H

#include "Physique.h"
#include "physics/ChemicalReactive.h"
#include "physics/Element.h"
#include "physics/ElementTable.h"

class Molecule  : public ChemicalReactive
{
public:
    Molecule(QString formula, bool* ok=0);
    virtual ~Molecule();



};

#endif // MOLECULE_H
