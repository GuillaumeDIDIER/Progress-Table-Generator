#ifndef MOLECULE_H
#define MOLECULE_H

#include "Physique.h"
#include "physics/ChemicalReactive.h"
#include "physics/Element.h"
#include "physics/ElementTable.h"

class Molecule  : public ChemicalReactive, private QMap<const Element*, int>
{
public:
    Molecule(QString formula, bool* ok=0);
    virtual ~Molecule();
    virtual bool isValid() const;
    virtual QString toStr() const{return m_formula;}
    virtual QString toHtml() const;
    virtual bool setFormula(QString newFormula);
    static QString reactiveRX(){return QString("");}

    class MoleculeFactory : public ChemicalReactiveFactory {
    protected:
        MoleculeFactory();
        virtual ~MoleculeFactory();
    public:
        virtual Molecule * buildReactive(QString formula) const; // Return nul pointer if not possible.
        virtual QString reactiveRX() const{return Molecule::reactiveRX();} // returns the regExp validating the string representation of the reactives built by the factory.
        static MoleculeFactory* getInstance();
    private:
        static MoleculeFactory* instance;
    };

    virtual MoleculeFactory* factory() const{return MoleculeFactory::getInstance();}
    virtual QMap<const Element*, int> elementMap() const;
    virtual int charge() const{return m_charge;}
    virtual State state() const{return m_state;}
private:
    QString m_formula;
    int m_charge;
    State m_state;
    bool m_error; // in case an operation failed and left the molecule in an incorrect state

};

typedef Molecule::MoleculeFactory MoleculeFactory;

#endif // MOLECULE_H
