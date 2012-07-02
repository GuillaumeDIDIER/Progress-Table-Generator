/*
 *  Molecule.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 23/03/11.
 *  Copyright 2011. All rights reserved.
 * Molecule class implementation
 */

#include "MoleculeOld.h"

    //	class const variables
const QString MoleculeOld::nb_rx(){return QString("\\d*");}
const QString MoleculeOld::symbolNb_rx(){return Element::element_rx() + MoleculeOld::nb_rx();}
const QString MoleculeOld::symbolNbCapture_rx(){return QString("(") + Element::element_rx() + QString(")(") + MoleculeOld::nb_rx() + QString(")");}
const QString MoleculeOld::bracketed_rx(){return QString("\\((?:") + MoleculeOld::symbolNb_rx() + QString(")+\\)") + MoleculeOld::nb_rx();}
const QString MoleculeOld::bracketedCapture_rx(){return QString("\\(((?:") + MoleculeOld::symbolNb_rx() + QString(")+)\\)(") + MoleculeOld::nb_rx() + QString(")");}
const QString MoleculeOld::symbolNb_or_Bracketed_rx(){return QString("(?:") + MoleculeOld::symbolNb_rx() + QString(")|(?:") + MoleculeOld::bracketed_rx() + QString(")");}
const QString MoleculeOld::chargeCaptured_rx(){return QString("\\[(") + MoleculeOld::nb_rx() + QString(")(\\-|\\+)\\]");}
const QString MoleculeOld::charge_rx(){return QString("\\[") + MoleculeOld::nb_rx() + QString("(?:\\-|\\+)\\]");}
const QString MoleculeOld::state_rx(){return QString("(?:\\(([a-z]{1,2})\\))");}
const QString MoleculeOld::molecule_rx(){return QString("((?:") + MoleculeOld::symbolNb_or_Bracketed_rx() + QString(")+)(") + MoleculeOld::charge_rx() + QString(")?") + MoleculeOld::state_rx() + QString("?");}

    //	initialization of the periodic element table class member
ElementTable* MoleculeOld::c_PeriodicElementTable = 0;

    //	constructors destructors
MoleculeOld::MoleculeOld() : QMap<const Element*, int>(), m_formula(""), m_htmlFormula(""),
m_validComputed(true), m_valid(false), m_htmlC(false), m_charge(0), m_state(NonMentioned) {
        //	naught to do
}

MoleculeOld::MoleculeOld(QString formula) : QMap<const Element*, int>(), m_formula(formula), m_htmlFormula(""),
m_validComputed(false), m_valid(false), m_htmlC(false), m_charge(0), m_state(NonMentioned)
{
    setFormula(formula); //	cf setFormula
}

MoleculeOld::MoleculeOld(const MoleculeOld & other) : QMap<const Element*, int>(other),
m_formula(other.m_formula), m_htmlFormula(other.m_htmlFormula), m_validComputed(other.m_validComputed),
m_valid(other.m_valid), m_htmlC(other.m_htmlC), m_charge(other.m_charge), m_order(other.m_order), m_state(other.m_state)
{
        //	naught to do
}

MoleculeOld::~MoleculeOld(){
        //	deallocation by compiler
}

    //	accessors
bool MoleculeOld::isValid() const{
    if (!m_validComputed){
        m_valid = true;	//	for the moment nothing invalid has been found
        if (this->isEmpty() && m_charge == 0) {	//	verifying that the molecule is not null
            m_valid = false;
            m_validComputed = true;
            return m_valid;
        }
        QListIterator<const Element*> i(this->keys());	//	looping over the elements
        while (i.hasNext()) {
            if (!i.next()) {		//	if there's an invalid element (address = 0)
                m_valid = false;	//	then the molecule is not valid !
                break;				//	so we break out of the loop
            }
        }
        m_validComputed = true;		//	the validity has been successfully computed
    }
    return m_valid;	//	we therefore return it
}

void MoleculeOld::setFormula(QString formula){
        //	the regexp
    QRegExp moleculeRX("^" + MoleculeOld::molecule_rx() + "$");
    if(moleculeRX.exactMatch(formula)){	//	Matching
        m_formula = formula;	//	setting the formula
        m_validComputed = false;//	it might not be valid
        QStringList matches = moleculeRX.capturedTexts();	//	the matched groups
        if (matches[2] != "")					//	if the charge group isn't empty
            setChargeFromRX(matches.value(2));	//	Setting the charge
        if (matches[3] != "") {						//	if the state group isn' empty
            setStateFromRX(matches.value(3));	//	Setting the state
        }
        QRegExp symbolNb_or_bracketedRX("(" + MoleculeOld::symbolNb_or_Bracketed_rx() + ")");	//	RegExp for matching a group ex O2 or (OH)2
        QStringList symbolNbs;	//	all the groups we've already matched
        int pos = 0;			//	our position inside the string containing the groups
        while ((pos = symbolNb_or_bracketedRX.indexIn(matches[1], pos)) != -1) {	//	while we can find a match
            QString capturedSN = symbolNb_or_bracketedRX.cap(1);	//	the group which was captured
            pos += symbolNb_or_bracketedRX.matchedLength();		//	increasing the position by the matched lenght
            if (QRegExp(MoleculeOld::bracketed_rx()).exactMatch(capturedSN)) {	//if it's a bracketed group
                symbolNbs.append(replaceBracketInFormula(capturedSN));	//	we append to symbolNbs the conversion of it into
                                                                        //	an elements with numbers list (OH2)2 => ("O", "O", "H2", "H2")
            }
            else {
                symbolNbs.append(capturedSN);	//	if it is simply a element with a number then we append it raw
            }
        }
        QStringListIterator i(symbolNbs);	//	looping on the element with number list
        while (i.hasNext()) {					//	foreach element with number
            addElementFromSymbolNb(i.next());	//	we append it
        }

    }
}

QString MoleculeOld::rawFormula() const{
    QList<const Element*> key = this->keys();	//	the list of element
    qStableSort(key.begin(), key.end(), Element::symbolCompare);	//	sorted by Alphabetical order
    QString rawFormula("");											//	an for the moment empty raw Formula
    foreach(const Element* el, key){								//	foreach element
        rawFormula += el->symbol() + this->value(el);				//	we append its symbol and count
    }
    if (m_charge) {
        rawFormula += "[" + qAbs(m_charge);
        if (m_charge < 0) {
            rawFormula += "-]";
        }else {
            rawFormula += "+]";
        }
    }
    switch (m_state) {
        case Gas:
            rawFormula += "(g)";
            break;
        case Liquid:
            rawFormula += "(l)";
            break;
        case Solid:
            rawFormula += "(s)";
            break;
        case Aqueous:
            rawFormula += "(aq)";
            break;
        default:
            break;
    }
    return rawFormula;	//	we return the freshly processed raw formula
}

QString MoleculeOld::htmlRawFormula() const{
    QList<const Element*> key = this->keys();	//	the list of element
    qStableSort(key.begin(), key.end(), Element::symbolCompare);	//	sortened by Alphabetical order
    QString htmlRawFormula("");											//	an for the moment empty raw Formula
    foreach(const Element* el, key){								//	foreach element
        htmlRawFormula += el->symbol() + "<sub>" + this->value(el) + "</sub>";				//	we append its symbol and count
    }
    if (m_charge) {
        htmlRawFormula += "<sup>" + qAbs(m_charge);
        if (m_charge < 0) {
            htmlRawFormula += "-";
        }
        else {
            htmlRawFormula += "+";
        }
        htmlRawFormula += "</sup>";
    }
    switch (m_state) {
        case Gas:
            htmlRawFormula += "<sub>(g)</sub>";
            break;
        case Liquid:
            htmlRawFormula += "<sub>(l)</sub>";
            break;
        case Solid:
            htmlRawFormula += "<sub>(s)</sub>";
            break;
        case Aqueous:
            htmlRawFormula += "<sub>(aq)</sub>";
            break;

        default:
            break;
    }
    return htmlRawFormula;	//	we return the freshly processed raw formula
}


QString MoleculeOld::formula() const{
    return m_formula;
}

QString MoleculeOld::htmlFormula() const{
    if (m_htmlC) {
        return m_htmlFormula;
    }

    int state = 0;
    for (int i = 0; i < m_formula.length(); i++) {
        QChar character = m_formula[i];
        if (character.isSpace()) {
            m_htmlFormula += character;
            continue;
        }
        switch (state) {
            case 0:
                if (character.isLetter()) {
                    m_htmlFormula += character;
                    break;
                }
                if (character.isNumber()) {
                    m_htmlFormula += QString("<sub>") + character;
                    state = 4;
                    break;
                }
                if (character == QChar('(')){
                    state = 1;
                    continue;
                }
                if (character == QChar(')')) {
                    m_htmlFormula += character;
                    break;
                }
                if (character == QChar('[')) {
                    state = 2;
                    m_htmlFormula += QString("<sup>");
                    continue;
                }
                break;
            case 1:
                if (character.isUpper() || character == QChar('(')) {
                    state = 0;
                    m_htmlFormula += QString("(") + character;
                    continue;
                }
                if (character.isLower()) {
                    state = 3;
                    m_htmlFormula += QString("<sub>(") + character;
                    continue;
                }
                break;
            case 2:
                if (character.isNumber() || character == QChar('+') || character == QChar('-')) {
                    m_htmlFormula += character;
                }else {
                    state = 0;
                    m_htmlFormula += QString("</sup>");
                }

                break;
            case 3:
                if (character == QChar(')') ) {
                    state = 0;
                    m_htmlFormula += QString(")</sub>");
                    break;
                }
                if (character.isLetter() && character.isLower()){
                    m_htmlFormula += character;
                }
                break;
            case 4:
                if (character.isNumber()) {
                    m_htmlFormula += character;
                    break;
                }
                else {
                    state = 0;
                    m_htmlFormula += "</sub>";
                    i--;
                    continue;
                }

                break;

            default:
                break;
        }
    }
    switch (state) {
        case 2:
            m_htmlFormula += "</sup>";
            break;
        case 3:
            m_htmlFormula += ")</sub>";
            break;
        case 4:
            m_htmlFormula += "</sub>";
            break;

        default:
            break;
    }
    m_htmlC = true;
    return m_htmlFormula;
}

QSet<const Element*> MoleculeOld::elementSet() const{
    return (this->keys()).toSet();
}

int MoleculeOld::charge() const{
    return m_charge;
}

int MoleculeOld::elementCount(const Element* el) const{
    return this->value(el, 0);
}

int MoleculeOld::order() const{
    return m_order;
}

void MoleculeOld::setOrder(int order){
    m_order = order;
}

MoleculeOld::State MoleculeOld::state() const{
    return m_state;
}
    //	accessors to the private periodic element table class member
ElementTable* MoleculeOld::getPeriodicElementTable(){
    return c_PeriodicElementTable;
}

void MoleculeOld::setPeriodicElementTable(ElementTable* pTable){
    c_PeriodicElementTable = pTable;
}
    //	private methods
QStringList MoleculeOld::replaceBracketInFormula(QString bracketedGroup){	//	to replace bracketed groups
    QRegExp RX(MoleculeOld::bracketedCapture_rx());
    RX.indexIn(bracketedGroup);
    int grpCount = RX.cap(2).toInt();	//	how many time the group is repeated
    QStringList unBracketed;			//	the still empty return value
    QRegExp symbolNbRX(MoleculeOld::symbolNb_rx());	//	a reg exp for matching an element and its count
    int pos = 0;							//	position in the string
    QString symbols = RX.capturedTexts()[1];	//	the group formula
    while ((pos = symbolNbRX.indexIn(symbols, pos)) != -1) {	//	on which we loop
        QString capturedSymbol = symbolNbRX.cap(0);	//	the element and count matched
        pos += symbolNbRX.matchedLength();			//	increasing the position by the freshly matched lenght,
        for (int i = 0; i < grpCount; i++) {			//	so as we dont match repeatedly an element
            unBracketed += capturedSymbol;			//	we append the element and its count repeated the group count
        }
    }
return unBracketed;	//	we return the now filled return value
}

void MoleculeOld::addElementFromSymbolNb(QString matchedGroup){
    QRegExp symbolNbRX(MoleculeOld::symbolNbCapture_rx());
    symbolNbRX.indexIn(matchedGroup);
    if ((symbolNbRX.cap(2)).isNull()) {
        (*this)[c_PeriodicElementTable->getElementFromSymbol(symbolNbRX.cap(1))] += 1;
    }
    else {
        (*this)[c_PeriodicElementTable->getElementFromSymbol(symbolNbRX.cap(1))] += symbolNbRX.cap(2).toInt();
    }


}

void MoleculeOld::setChargeFromRX(QString matchedCharge){
    QRegExp chargeRX("^" + MoleculeOld::chargeCaptured_rx() + "$");
    chargeRX.indexIn(matchedCharge);
    QString chargeStr(chargeRX.cap(2) + chargeRX.cap(1));
    if (chargeStr == "-" || chargeStr == "+") {
        chargeStr += "1";
    }
    m_charge = chargeStr.toInt();
}

void MoleculeOld::setStateFromRX(QString matchedState){
    if (matchedState == "") {
        m_state = NonMentioned;
    }
    else if (matchedState == "g") {
        m_state = Gas;
    }
    else if (matchedState == "l") {
        m_state = Liquid;
    }
    else if (matchedState == "s") {
        m_state = Solid;
    }
    else if (matchedState == "aq") {
        m_state = Aqueous;
    }
    else {
        m_state = NonMentioned;
    }

}

bool operator==(const MoleculeOld& a, const MoleculeOld& b){
    if (a.formula() == b.formula())
        return true;
    else
        return false;
}

#ifdef DEBUG
QDebug& operator<<(QDebug& out, const MoleculeOld& mol){
    out << mol.formula();
    return out;
}
#endif	//	ifdef DEBUG

