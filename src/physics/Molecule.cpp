/*
 *  Molecule.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 29/06/12.
 *  Copyright 2012.
 *
 */

#include "Molecule.h"

MoleculeFactory* MoleculeFactory::instance = (MoleculeFactory*)0;
ElementTable* Molecule::c_PeriodicElementTable = 0;

//	accessors to the private periodic element table class member
ElementTable* Molecule::getPeriodicElementTable(){
return c_PeriodicElementTable;
}

void Molecule::setPeriodicElementTable(ElementTable* pTable){
c_PeriodicElementTable = pTable;
}


Molecule::Molecule(QString formula, bool *ok) : ChemicalReactive(ok), m_charge(0), m_state(NonMentioned)
{
    if(ok && *ok)
        *ok = setFormula(formula);
    else if (!ok) {
        setFormula(formula);
    }
    else
       m_state = Invalid;
}

Molecule::~Molecule()
{
}

bool Molecule::isValid() const
{
    if(!m_state || (isEmpty()))
        return false;
    return true;
}

enum HtmlConverterState {
    Error = 0, ReadingElement, ReadingGroupOrState, ReadingCount, ReadingState, ReadingCharge
};

QString Molecule::toHtml() const
{
    enum HtmlConverterState {
        ReadingElement, ReadingGroupOrState, ReadingCount, ReadingState, ReadingCharge
    };
    HtmlConverterState htmlConverterState(ReadingElement);
    QString htmlFormula("");
    foreach (QChar character, m_formula) {
        switch (htmlConverterState) {
        case ReadingElement:
            if (character.isLetter()){
                htmlFormula.append(character);
                continue;
            }
            if (character.isDigit()){
                htmlFormula.append("<sub>").append(character);
                htmlConverterState = ReadingCount;
                continue;
            }
            if (character == QChar('(')) {
                htmlConverterState = ReadingGroupOrState;
                continue;
            }
            if (character == QChar(')')) {
                htmlFormula.append(character);
                continue;
            }
            if (character == QChar('[')){
                htmlConverterState = ReadingCharge;
                htmlFormula.append("<sup>");
                continue;
            }
            break;
        case ReadingGroupOrState:
            if (character.isUpper() || character == QChar('(')){
                htmlConverterState = ReadingElement;
                htmlFormula.append("(").append(character);
                continue;
            }
            if (character.isLower()) {
                htmlConverterState = ReadingState;
                htmlFormula.append("<sub>(").append(character);
                continue;
            }
        case ReadingCharge:
            if (character.isDigit() || character == QChar('+') || character == QChar('-')) {
                htmlFormula.append(character);
                continue;
            }
            else {
                htmlConverterState = ReadingElement;
                htmlFormula.append("</sup>");
                        continue;
            }
        case ReadingState:
            if (character == QChar(')')) {
                htmlConverterState = ReadingElement;
                htmlFormula.append(")</sub>");
                continue;
            }
            if (character.isLower()){
                htmlFormula.append(character);
                continue;
            }
        case ReadingCount:
            if (character.isDigit()){
                htmlFormula.append(character);
                continue;
            }
            else {
                htmlFormula.append("</sub>");
                if (character.isLetter()){
                    htmlConverterState = ReadingElement;
                    htmlFormula.append(character);
                    continue;
                }
                if (character == QChar('(')) {
                    htmlConverterState = ReadingGroupOrState;
                    continue;
                }
                if (character == QChar(')')) {
                    htmlConverterState = ReadingElement;
                    htmlFormula.append(character);
                    continue;
                }
                if (character == QChar('[')){
                    htmlConverterState = ReadingCharge;
                    htmlFormula.append("<sup>");
                    continue;
                }

            }
        }
    }
    switch (htmlConverterState) {
    case ReadingCount:
        htmlFormula.append("</sub>");
        break;
    case ReadingCharge:
        htmlFormula.append("</sup>");
        break;
    default:
        break;
    }
    return htmlFormula;
}

// todo move this logic to a molecule parser class.
bool Molecule::setFormula(QString newFormula)
{

    enum MoleculeParserState{
        Error = 0, BeginParsing, ParsingElement, ParsingElementCount, ParsingGroupOrState, ParsingGroup, ParsingGroupElement, ParsingGroupElementCount, ParsingGroupCount, ParsingCharge, ChargeParsed, ParsingState, StateParsed
    };


    if (c_PeriodicElementTable == 0){
        m_state = Invalid;
        return false;
    }
    MoleculeParserState parserState(BeginParsing);
    bool state_set(false);
    bool charge_set(false);
    QStack<QMap<const Element*, int> > group_stack;
    QString tmp("");
    const Element* currentElement = 0;
    foreach(QChar character, newFormula){
        switch(parserState){
        case BeginParsing:
            if (character.isLetter() && character.isUpper()){
                tmp.append(character);
                parserState = ParsingElement;
                continue;
            }
            if (character == QChar('(')){
                parserState = ParsingGroupOrState;
                continue;
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingElement:
            if (character.isLetter() && character.isLower()){
                tmp.append(character);
                continue;
            }
            if (character.isLetter() && character.isUpper()){
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                if (currentElement) {
                    (*this)[currentElement] += 1;
                    tmp.clear();
                    tmp.append(character);
                    continue;
                }
            }
            if (character.isDigit()){
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                if (currentElement){
                    tmp.clear();
                    parserState = ParsingElementCount;
                    tmp.append(character);
                    continue;
                }
            }
            if (character == QChar('(')) {
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                if (currentElement){
                    (*this)[currentElement] += 1;
                    tmp.clear();
                    parserState = ParsingGroupOrState;
                    continue;
                }
            }
            if (character == QChar('[')){
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                if (currentElement){
                    (*this)[currentElement] += 1;
                    tmp.clear();
                    parserState = ParsingCharge;
                    continue;
                }
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingElementCount:
            if (character.isDigit()) {
                tmp.append(character);
                continue;
            }
            if (character.isLetter() && character.isUpper()){
                if (currentElement){
                    (*this)[currentElement] += tmp.toInt();
                    tmp.clear();
                    parserState = ParsingElement;
                    tmp.append(character);
                    continue;
                }
            }
            if (character == QChar('(')) {
                if (currentElement){
                (*this)[currentElement] += tmp.toInt();
                    tmp.clear();
                    parserState = ParsingGroupOrState;
                    continue;
                }
            }
            if (character == QChar('[')) {
                if (currentElement){
                    (*this)[currentElement] += tmp.toInt();
                    tmp.clear();
                    parserState = ParsingCharge;
                    continue;
                }
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingGroupOrState:
            if (character.isLetter() && character.isLower()){
                tmp.append(character);
                parserState = ParsingState;
                continue;
            }
            if (character.isLetter() && character.isUpper()){
                tmp.append(character);
                group_stack.push(QMap<const Element*, int>());
                parserState = ParsingGroupElement;
                continue;
            }
            if (character == QChar('(')){
                group_stack.push(QMap<const Element*, int>());
                group_stack.push(QMap<const Element*, int>());
                parserState = ParsingGroup;
                continue;
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingGroup:
            if (character.isLetter() && character.isUpper()){
                tmp.append(character);
                parserState = ParsingGroupElement;
                continue;
            }
            if (character == QChar('(')){
                group_stack.push(QMap<const Element*, int>());
                parserState = ParsingGroup;
                continue;
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingGroupElement:
            if (character.isLetter() && character.isLower()){
                tmp.append(character);
                continue;
            }
            if (character.isLetter() && character.isUpper()){
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                if (currentElement){
                    group_stack.top()[currentElement] += 1;
                    tmp.clear();
                    tmp.append(character);
                    continue;
                }
            }
            if (character.isDigit()){
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                tmp.clear();
                parserState = ParsingGroupElementCount;
                tmp.append(character);
                continue;
            }
            if (character == QChar('(')) {
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                if (currentElement){
                    group_stack.top()[currentElement] += 1;
                    tmp.clear();
                    group_stack.push(QMap<const Element*, int>());
                    parserState = ParsingGroup;
                    continue;
                }
            }
            if (character == QChar(')')){
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
                if (currentElement){
                    group_stack.top()[currentElement] += 1;
                    tmp.clear();
                    parserState = ParsingGroupCount;
                    continue;
                }
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingGroupElementCount:
            if (character.isDigit()) {
                tmp.append(character);
                continue;
            }
            if (character.isLetter() && character.isUpper()){
                if (currentElement){
                    group_stack.top()[currentElement] += tmp.toInt();
                    tmp.clear();
                    parserState = ParsingGroupElement;
                    tmp.append(character);
                    continue;
                }
            }
            if (character == QChar('(')) {
                if (currentElement){
                    group_stack.top()[currentElement] += tmp.toInt();
                    tmp.clear();
                    group_stack.push(QMap<const Element*, int>());
                    parserState = ParsingGroup;
                    continue;
                }
            }
            if (character == QChar(')')){

                if (currentElement){
                    group_stack.top()[currentElement] += tmp.toInt();
                    tmp.clear();
                    parserState = ParsingGroupCount;
                    continue;
                }
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingGroupCount:
            if (character.isDigit()){
                tmp.append(character);
                continue;
            }else {
                QMap<const Element*, int> group_being_left(group_stack.pop());
                if (!group_stack.isEmpty()){
                    foreach (const Element* element, group_being_left.keys()) {
                        group_stack.top()[element] += group_being_left[element] *((tmp.isEmpty()? 1:tmp.toInt()));
                    }
                }
                else {
                    foreach (const Element* element, group_being_left.keys()) {
                        (*this)[element] += group_being_left[element] *((tmp.isEmpty()? 1:tmp.toInt()));
                    }

                }
                tmp.clear();
                if (character.isLetter() && character.isUpper()){
                    tmp.append(character);
                    parserState = ((group_stack.isEmpty())? ParsingElement:ParsingGroupElement);
                    continue;
                }
                if (character == QChar('(')) {
                    if (group_stack.isEmpty()){
                        parserState = ParsingGroupOrState;
                    } else {
                        group_stack.push(QMap<const Element*, int>());
                        parserState = ParsingGroup;
                    }
                    continue;
                }
                if (!group_stack.isEmpty() && character == QChar(')'))
                    continue;
                if (group_stack.isEmpty() && character == QChar('[')) {
                    parserState = ParsingCharge;
                    continue;
                }
                parserState = Error;
                m_state = Invalid;
                return false;
                break;
            }
        case ParsingCharge:
            if (character.isDigit() && !charge_set) {
                tmp.append(character);
                continue;
            }
            if (character == QChar('+') || character == QChar('-')){
                if (tmp.isEmpty())
                    tmp.append("1");
                tmp.prepend(character);
                m_charge = tmp.toInt();
                tmp.clear();
                charge_set = true;
                continue;
            }
            if (character == QChar(']') && charge_set) {
                parserState = ChargeParsed;
                continue;
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ChargeParsed:
            if (character == QChar('(') && !state_set){
                parserState = ParsingState;
                continue;
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case ParsingState:
            if (character.isLower() && !state_set) {
                tmp.append(character);
                continue;
            }
            if (character == QChar(')')) {

                if (tmp == QString("aq")){
                    m_state = Aqueous;
                }
                else if (tmp == QString("s")){
                    m_state  = Solid;
                }
                else if (tmp == QString("g")){
                    m_state = Gas;
                }
                else if (tmp == QString("l")){
                    m_state = Liquid;
                }
                else {
                    m_state = OtherSoluted;
                    m_state_str = tmp;
                }
                state_set = true;
                parserState = StateParsed;
                continue;
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        case StateParsed:
            if (character == QChar('[') && !charge_set) {
                parserState = ParsingCharge;
                continue;
            }
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        default:
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        }
    }
    switch (parserState) {
    case ParsingElement:
        currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp);
        if (currentElement)
            (*this)[currentElement] += 1;
        else {
            parserState = Error;
            m_state = Invalid;
            return false;
            break;
        }
        tmp.clear();
        break;
    case ParsingElementCount:
        if (currentElement){
            (*this)[currentElement] += tmp.toInt();
            tmp.clear();
            break;
        }
        parserState = Error;
        m_state = Invalid;
        return false;
        break;
    case ParsingGroupCount:
        if (group_stack.size() == 1) {
            foreach (const Element* element, group_stack.top().keys()) {
                (*this)[element] += group_stack.top()[element] *((tmp.isEmpty()? 1:tmp.toInt()));
            }
        break;
        }
    case ParsingCharge:
    case ParsingState:
    case ParsingGroup:
    case ParsingGroupElement:
    case ParsingGroupOrState:
    case ParsingGroupElementCount:
    case BeginParsing:
        parserState = Error;
        m_state = Invalid;
        return false;
    default:
        break;
    }
    m_formula = newFormula;
    return true;
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
#ifdef DEBUG

QDebug operator<<(QDebug out, const Molecule & mol){
    out << mol.toStr() << " : " << mol.elementMap() << " : " << mol.charge() << "\n";
    foreach (const Element* element, mol.elementMap().keys()) {
        out << element << " : " << element->symbol();
    }
    return out;
}
#endif	//	ifdef DEBUG

