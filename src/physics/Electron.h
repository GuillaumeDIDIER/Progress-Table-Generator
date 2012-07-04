/*
 *  Electron.h
 *  Physique
 *
 *  Created by Guillaume DIDIER on 2/07/12.
 *  Copyright 2012.
 *	Electron class header
 */


#ifndef ELECTRON_H
#define ELECTRON_H

#include "Physique.h"
#include "physics/ChemicalReactive.h"
#include "physics/Element.h"
#include "physics/ElementTable.h"


class Electron : public ChemicalReactive
{
public:
    Electron(QString symbol="e-", bool* ok = 0);
    static QString reactiveRX(){return QString(" *e- *");}
    virtual bool isValid() const{return m_valid;}
    virtual QString toStr() const{return QString("e-");}
    virtual QString toHtml() const{return QString("e<sup>-</sup>");}
    class ElectronFactory : public ChemicalReactiveFactory {
    protected:
        ElectronFactory();
        virtual ~ElectronFactory();
    public:
        virtual Electron * buildReactive(QString formula="e-") const; // Return nul pointer if not possible.
        virtual QString reactiveRX() const{return Electron::reactiveRX();}
        // returns the regExp validating the string representation of the reactives built by the factory.
        // it may validate some incorect string reppresentations.
        static ElectronFactory* getInstance();
    private:
        static ElectronFactory* instance;
    };

    virtual ElectronFactory* factory() const{return ElectronFactory::getInstance();}
    virtual QMap<const Element*, int> elementMap() const{return QMap<const Element*, int>();}
    virtual int charge() const {return -1;}
    virtual State state() const {return NoState;}

private:
    bool m_valid;

};

typedef Electron::ElectronFactory ElectronFactory;

#endif // ELECTRON_H
