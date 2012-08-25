#include "ChemicalEquation.h"

ChemicalEquationFactory* ChemicalEquationFactory::instance = (ChemicalEquationFactory*)0;

ChemicalEquation::ChemicalEquation(bool * ok) : Equation(ok)
{
}

ChemicalEquation::~ChemicalEquation()
{
}

QString ChemicalEquation::toStr() const
{
}

QString ChemicalEquation::toHtml() const
{
}

Equation::State ChemicalEquation::state() const
{
}

ChemicalEquationFactory::ChemicalEquationFactory() : EquationFactory()
{
}

ChemicalEquationFactory::~ChemicalEquationFactory()
{
}

ChemicalEquation* ChemicalEquationFactory::buildEquation(QString stringRepr) const
{

}

ChemicalEquationFactory* ChemicalEquationFactory::getInstance()
{
    if(instance)
        return instance;
    return instance = new ChemicalEquationFactory();
}

ChemicalEquationFactory* ChemicalEquation::factory() const
{
    return ChemicalEquationFactory::getInstance();
}
