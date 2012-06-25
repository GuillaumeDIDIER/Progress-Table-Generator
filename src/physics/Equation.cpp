/*
 *  Equation.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 15/04/11.
 *  Copyright 2011. All rights reserved.
 *	Equation class implementation
 */

#include "Physique.h"

#include "physics/Element.h"
#include "physics/ElementTable.h"
#include "physics/Molecule.h"
#include "physics/Equation.h"

#include <../../../Libraries/rmath/Rational.hpp>
#include <../../../Libraries/rmath/MathsFuncs.hpp>

    //	class const variables
const QString Equation::equation_rx(){return QString("(") + Molecule::nb_rx()  + QString(" *") + Molecule::molecule_rx() + QString(" \\+ )*(") + Molecule::nb_rx() + QString(" *") + Molecule::molecule_rx() + QString(")");}
    //	prototypes
#ifdef DEBUG
QDebug& operator<<(QDebug& out, const Rational& rat);
#endif	//	ifdef DEBUG

    //	private funcs
bool operator<(const Molecule& a, const Molecule& b){
    return a.formula() < b.formula();
}

bool moleculeLessThanUser(const Molecule& a, const Molecule& b){
    return a.order() < b.order();
}

    //	class Equation
        //	constructors
            //	default constructor
Equation::Equation() : m_validComputed(false), m_equilibratedC(false)
{
        //	naught to do
}
            //	copy consructor
Equation::Equation(const Equation& other) : m_validComputed(other.m_validComputed), m_valid(other.m_valid),
                                            m_equilibratedC(other.m_equilibratedC), m_equilibrated(other.m_equilibrated),
                                            m_reactives(other.m_reactives), m_products(other.m_products)
{
        //	all has been done above
}
            //	designated constructor
Equation::Equation(QString reactivesStr, QString productsStr) : m_validComputed(false), m_equilibratedC(false)
{
    m_reactives = memberStrToDict(reactivesStr);
    QList<Molecule> tmp = m_reactives.keys();
    qSort(tmp.begin(), tmp.end(), moleculeLessThanUser);
    m_products = memberStrToDict(productsStr, tmp.last().order());

}
        // destructor
Equation::~Equation(){
        //	naught to do
}
        // public
            //	instance method equilibrate
            //	todo : add comments
bool Equation::equilibrate(){
        //	init solver "row header" with the element set
    QList<const Element*> elements = elementSet().toList();
    int rowCount = elements.length();
        //	init solver "column header" with molecules
    QList<Molecule> molecules = QMap<Molecule, int>(m_reactives).unite(m_products).uniqueKeys();
    qSort(molecules.begin(), molecules.end(), moleculeLessThanUser);
    int columnCount = molecules.length();
        //	now we can create the solver matrix, with the right size
    QList<QList<Rational> > equationGaussSolver(QVector<QList<Rational> >(rowCount + 2, QVector<Rational>(columnCount + 1, 0).toList()).toList());
                                        //	matrix used for solving, not very elegant and optimised but it works.
        //	which we can now fill in
    for (int i = 0; i < columnCount; ++i) {			//	for each colmun
        Molecule molTmp = molecules[i];				//	the molecule which it represents
        foreach(const Element* element, elements){	//	for for each element
            equationGaussSolver[elements.indexOf(element)][i] = molTmp.elementCount(element);
        }											//	its count
        equationGaussSolver[rowCount][i] = molTmp.charge();
    }												//	the molecule charge
        //	shouldn't be needed
    for (int j = 0; j < rowCount + 1; ++j) {		//	the last column
        equationGaussSolver[j][columnCount] = 0;	//	initialized to zero
    }												//	the last line
    equationGaussSolver[rowCount + 1][0] = 1;		// first molecule count = 1
    equationGaussSolver[rowCount + 1][columnCount] = 1;

        //	The matrix is now ready for actual solving

        //	Some python code to port to our example
                                        // def gauss_jordan(m, eps = 1.0/(10**10)):
                                            //"""Puts given matrix (2D array) into the Reduced Row Echelon Form.
                                            //Returns True if successful, False if 'm' is singular.
                                            //NOTE: make sure all the matrix items support fractions! Int matrix will NOT work!
                                            //Written by Jarno Elonen in April 2005, released into Public Domain"""

    int h = rowCount + 2, w = columnCount + 1;//(h, w) = (len(m), len(m[0]))
    for (int i = 0; i < std::min(h, w-1); i++) {			//for y in range(0,h):
        int maxRow = i;							//	maxrow = y
        for (int i2 = i+1; i2 < h; ++i2) {	//	for y2 in range(y+1, h):    # Find max pivot
                                            //		if abs(m[y2][y]) > abs(m[maxrow][y]):
            if (abs(equationGaussSolver[i2][i]) > abs(equationGaussSolver[maxRow][i]))
                maxRow = i2;				//			maxrow = y2
        }
        equationGaussSolver.swap(maxRow, i);//	(m[y], m[maxrow]) = (m[maxrow], m[y])
                                            //	if abs(m[y][y]) <= eps:     # Singular?
        if (abs(equationGaussSolver[i][i]) == 0) {
            return false;					//		return False
        }
        for (int i2 = i+1; i2 < h; ++i2) {	//	for y2 in range(y+1, h):    # Eliminate column y
            Rational c = equationGaussSolver[i2][i] / equationGaussSolver[i][i];
                //		c = m[y2][y] / m[y][y]
            for (int j = i; j < w; ++j)		//		for x in range(y, w):
                equationGaussSolver[i2][j] -= equationGaussSolver[i][j] * c;
                //			m[y2][x] -= m[y][x] * c
        }
    }
    for (int i = std::min(h, w -1) - 1; i > -1; --i) {		//for y in range(h-1, 0-1, -1): # Backsubstitute
        Rational c = equationGaussSolver[i][i];
            //	c  = m[y][y]
        for (int i2 = 0; i2 < i; ++i2) {		//	for y2 in range(0,y):
            for (int j = w - 1; j > i - 1; --j) {
                    //		for x in range(w-1, y-1, -1):
                equationGaussSolver[i2][j] -= equationGaussSolver[i][j] * equationGaussSolver[i2][i] / c;
                    //			m[y2][x] -=  m[y][x] * m[y2][y] / c
            }
        }
        equationGaussSolver[i][i] /= c;		//	m[y][y] /= c
        for (int j = 0; j < w; ++j)			//	for x in range(y, w):       # Normalize row y
            equationGaussSolver[i][j] /= c;	//		m[y][x] /= c
    }


        //	solving is now done ; we just need to extract results
    QMap<Molecule, Rational> moleculesStoechs;	//	a dict of molecule stoech, negative fo products
    for (int i = 0; i < columnCount; ++i)		//	filling it
        moleculesStoechs[molecules[i]] = equationGaussSolver[i][columnCount];
    QMutableMapIterator<Molecule, Rational> i(moleculesStoechs);	//	for looping on it
    m_reactives.clear();	//	reseting thoses
    m_products.clear();
    int ppcm = 1;			//	to turn Rationals into ints (by homogeneity)
    while (i.hasNext()) {	//	getting the denominators ppcm
        i.next();
        ppcm *= i.value().denominator() / PGCD(ppcm, i.value().denominator());
    }
    while (i.hasPrevious()) {//	multiplying all values by it, looping in reverse order
        i.previous();
        i.value() *= ppcm;
    }
    m_equilibratedC = false;
    m_validComputed = false;
    while (i.hasNext()) {	//	extracting results to the reactives and products dicts
        i.next();
        if (i.value() < 0) {
            m_products.insert(i.key(), -(i.value().intPart()));
        }
        else if (i.value() > 0) {
            m_reactives.insert(i.key(), i.value().intPart());
        }
    }
    return isEquilibrated();	//	for if the algorithm fails
}
            //	instance method is valid
            //	todo : add comments
bool Equation::isValid() const{
    if (!m_validComputed) {
        m_validComputed = true;
        if (isEmpty()){
            m_valid = false;
            return m_valid;
        }
        QSet<const Element*> rElementSet, pElementSet;
        foreach(const Molecule mol,m_reactives.keys()){
            if (!mol.isValid()) {
                m_valid = false;
                return m_valid;
            }
            rElementSet.unite(mol.elementSet());
        }
        foreach(const Molecule mol,m_products.keys()){
            if (!mol.isValid()) {
                m_valid = false;
                return m_valid;
            }
            pElementSet.unite(mol.elementSet());
        }
        if (rElementSet != pElementSet) {
            m_valid = false;
            return m_valid;
        }
        m_valid = true;
    }

    return m_valid;

}

bool Equation::isEquilibrated() const{
    m_equilibratedC = true;
    m_equilibrated = true;
    QSet<const Element*> elementSet;
    foreach(Molecule mol, (m_reactives.keys() + m_products.keys())){
        elementSet += mol.elementSet();
    }
    foreach(const Element* el, elementSet){
                int elCount = 0;
        foreach(Molecule mol, m_reactives.keys()){
            elCount += mol.elementCount(el) * m_reactives.value(mol);
        }
        foreach(Molecule mol, m_products.keys()){
            elCount -= mol.elementCount(el) * m_products.value(mol);
        }
        if (elCount) {
            m_equilibrated = false;
            break;
        }
    }
    return m_equilibrated;
}

QString Equation::reactivesStr(bool html) const{
    return memberDictToStr(m_reactives, html);
}

QString Equation::productsStr(bool html) const{
    return memberDictToStr(m_products, html);
}

QString Equation::toStr() const{
    return reactivesStr(false) + ' ' + QChar(0x2192) + ' ' + productsStr(false);
}

QString Equation::toHtml() const{
    return reactivesStr(true) + ' ' + QChar(0x2192) + ' ' + productsStr(true);

}

int Equation::stoechNumOf(Molecule mol) const{
    int result = 0;
    result += m_reactives.value(mol, 0);
    result += m_products.value(mol, 0);
    return result;
}

QSet<const Element*> Equation::elementSet() const{
    QSet<const Element*> elementSet;
    foreach(Molecule mol, m_reactives.keys()){
        elementSet += mol.elementSet();
    }
    foreach(Molecule mol, m_products.keys()){
        elementSet += mol.elementSet();
    }
    return elementSet;
}

QMap<Molecule, int> Equation::reactives(){
    return m_reactives;
}

QMap<Molecule, int> Equation::products(){
    return m_products;
}

    //	private methods
QMap<Molecule, int> Equation::memberStrToDict(QString str, int start){
    QStringList moleculesStrs = str.split(QRegExp(" +\\+ +"));
    QRegExp stoechNumRX("^\\d*");
    QMap<Molecule, int> moleculeDict;
    int i = 0;
    foreach(QString molStr, moleculesStrs){
        stoechNumRX.indexIn(molStr);
        Molecule mol(molStr.remove(0, stoechNumRX.cap(0).length()).trimmed());
        if (!moleculeDict.contains(mol))
            mol.setOrder(i + start);
        if (stoechNumRX.cap(0).toInt() == 0)
            moleculeDict[mol] += 1;

        else
            moleculeDict[mol] += stoechNumRX.cap(0).toInt();
        ++i;
    }
    return moleculeDict;
}

QString Equation::memberDictToStr(QMap<Molecule, int> map, bool html){
    QList<Molecule> molList = map.keys();
    qStableSort(molList.begin(), molList.end(), moleculeLessThanUser);
    QListIterator<Molecule> i(molList);
    QString result("");
    while (i.hasNext()) {
        int stoechNum = map.value(i.peekNext());
        if (stoechNum > 1)
            result += QString::number(stoechNum) + QChar(' ');
        html ? result += i.next().htmlFormula() : result += i.next().formula();
        if (i.hasNext())
            result += " + ";
    }
    return result;
}

        //	friends
bool operator==(const Equation& a, const Equation& b){
    return (a.m_reactives == b.m_reactives) && (a.m_products == b.m_products);
}
#ifdef DEBUG
QDebug& operator<<(QDebug& out, const Rational& rat){
    out << rat.numerator() << "/" << rat.denominator();
    return out;
}

QDebug& operator<<(QDebug& out, const Equation& eq){
    out << eq.toStr();
    return out;
}
#endif
