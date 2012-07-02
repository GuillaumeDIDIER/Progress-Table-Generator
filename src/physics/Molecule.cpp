#include "Molecule.h"

MoleculeFactory* MoleculeFactory::instance = (MoleculeFactory*)0;

Molecule::Molecule(QString formula, bool *ok) : ChemicalReactive(ok), m_error(false)
{
    if(ok && *ok)
        *ok = setFormula(formula);
    else
       m_error = true;
}

Molecule::~Molecule()
{
}

bool Molecule::isValid() const
{
    if(m_error || (isEmpty()))
        return false;
}

QString Molecule::toHtml() const
{

}

bool Molecule::setFormula(QString newFormula)
{
    //  todo : updates element map.
    m_formula = newFormula;
}

MoleculeFactory::MoleculeFactory()
{
}

MoleculeFactory::~MoleculeFactory()
{
    instance = (MoleculeFactory*)0;
}

Molecule* MoleculeFactory::buildReactive(QString formula) const
{
    if(QRegExp(reactiveRX()).exactMatch(formula))
    {
        bool ok(true);
        Molecule * molecule = new Molecule(formula, &ok);
        if(ok)
            return molecule;
        else
            delete molecule;
    }
    return 0;   // if we haven't already returned,
                // then the formula does not represent
                // a Molecule and we shall therefore return 0.
}


MoleculeFactory* MoleculeFactory::getInstance()
{
    if(instance)
        return instance;
    return instance = new MoleculeFactory();
}

QMap<const Element*, int> Molecule::elementMap() const
{
    return QMap<const Element*, int>(*this);
}

