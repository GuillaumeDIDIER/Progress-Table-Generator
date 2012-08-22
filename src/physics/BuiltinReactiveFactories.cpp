#include "Physique.h"

#include "BuiltinReactiveFactories.h"

#include "Reactive.h"
#include "ChemicalReactive.h"
#include "Electron.h"
#include "Molecule.h"

QList<ReactiveFactory*> builtinReactiveFactories(){
    static QList<ReactiveFactory*> builtinReactiveFactoriesList;
    if (builtinReactiveFactoriesList.isEmpty()){
        builtinReactiveFactoriesList << ElectronFactory::getInstance() << MoleculeFactory::getInstance();
    }
    return builtinReactiveFactoriesList;
}

QList<ChemicalReactiveFactory*> builtinReactiveChemicalFactories(){
    static QList<ChemicalReactiveFactory *> builtinChemicalFactoriesList;
    if (builtinChemicalFactoriesList.isEmpty()) {
        builtinChemicalFactoriesList << ElectronFactory::getInstance() << MoleculeFactory::getInstance();
    }
    return builtinChemicalFactoriesList;
}
