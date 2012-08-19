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
    if(ok && *ok)   //  if the ok pointer isn't null and *ok is true
        *ok = setFormula(formula);  // we set the formula and set *ok to the return value
    else if (!ok) {
        setFormula(formula);    //  if ok is null; we ignore the ok return value
    }
    else
       m_state = Invalid;   //  if *ok is false, thge Molecule is invalid
}

Molecule::~Molecule()   //  destructor
{
}

bool Molecule::isValid() const
{
    if(!m_state || (isEmpty())) //  if the molecule is in an invalid state or if it is empty / null
        return false;   //  the Molecule is not valid
    return true;    //  in the other case it must be valid
}
    //  converts the string representation to html
QString Molecule::toHtml() const
{
    //  state machine
    enum HtmlConverterState {
        ReadingElement, ReadingGroupOrState, ReadingCount, ReadingState, ReadingCharge
    };
    HtmlConverterState htmlConverterState(ReadingElement);
    QString htmlFormula("");
    foreach (QChar character, m_formula) {
        switch (htmlConverterState) {
        case ReadingElement:    //  we are reading an element
            if (character.isLetter()){  //  either it is lower and continue the element's symbol or it is a new element and the old one has no count
                htmlFormula.append(character);  //  nothing to modify
                continue;
            }
            if (character.isDigit()){   //  element count
                htmlFormula.append("<sub>").append(character);  //  we append the subscript tag
                htmlConverterState = ReadingCount;  //  we are now reading a count
                continue;
            }
            if (character == QChar('(')) {  //  we are now reading a state or group
                htmlConverterState = ReadingGroupOrState;   //  we will se what to do at the next iteration
                continue;
            }
            if (character == QChar(')')) {
                htmlFormula.append(character);  //  ending a group, nothing to do
                continue;
            }
            if (character == QChar('[')){   //  we are no reading a charge.
                htmlConverterState = ReadingCharge;
                htmlFormula.append("<sup>");    //  we append the superscript tag
                continue;
            }
            break;
        case ReadingGroupOrState:   //  we've just read a '('
            if (character.isUpper() || character == QChar('(')){    //  that means that we are reading a group
                htmlConverterState = ReadingElement;    //  new state is reading element.
                htmlFormula.append("(").append(character);  //  we append the bracket and the character
                continue;
            }
            if (character.isLower()) {  //  that's a state
                htmlConverterState = ReadingState;  //  state is now reading state
                htmlFormula.append("<sub>(").append(character); //  we append the subscript tag, the bracket and the character
                continue;
            }
        case ReadingCharge:
            if (character.isDigit() || character == QChar('+') || character == QChar('-')) {    //  valid charge character
                htmlFormula.append(character);  //  we append it
                continue;
            }
            else {
                htmlConverterState = ReadingElement;    //  we have finished reading the charge
                htmlFormula.append("</sup>");   //  end superscript
                        continue;
            }
        case ReadingState:
            if (character == QChar(')')) {  //  end of state
                htmlConverterState = ReadingElement;
                htmlFormula.append(")</sub>");  //  end subscript
                continue;
            }
            if (character.isLower()){   //  valid charge character
                htmlFormula.append(character);
                continue;
            }
        case ReadingCount:
            if (character.isDigit()){   //  valid count char
                htmlFormula.append(character);  //  we continue the count
                continue;
            }
            else {  //  if not finished
                htmlFormula.append("</sub>");   //  we end subscript
                if (character.isLetter()){  //  letter -> Element
                    htmlConverterState = ReadingElement;
                    htmlFormula.append(character);
                    continue;
                }
                if (character == QChar('(')) {  //  '(' -> either group or state
                    htmlConverterState = ReadingGroupOrState;
                    continue;
                }
                if (character == QChar(')')) {  //  ')' -> we were reading a group, continue with the normal ElementReading
                    htmlConverterState = ReadingElement;
                    htmlFormula.append(character);
                    continue;
                }
                if (character == QChar('[')){   //  charge
                    htmlConverterState = ReadingCharge;
                    htmlFormula.append("<sup>");    //  begin superscript
                    continue;
                }

            }
        }
    }
    switch (htmlConverterState) {   //  if we are in a state that leaves a super or sub script tag we close it
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

    //  molecule formula setter
    //  state machine based logic
    //  todo move this logic to a molecule parser class.
bool Molecule::setFormula(QString newFormula)
{

    enum MoleculeParserState{
        Error = 0, BeginParsing, ParsingElement, ParsingElementCount, ParsingGroupOrState, ParsingGroup, ParsingGroupElement, ParsingGroupElementCount, ParsingGroupCount, ParsingCharge, ChargeParsed, ParsingState, StateParsed
    };


    if (c_PeriodicElementTable == 0){   //  if there is no periodic element table it is impossible to parse a molecule.
        m_state = Invalid;              //  the molecule is therefore invalid
        return false;
    }
    MoleculeParserState parserState(BeginParsing);  //  Begin parsing
    bool state_set(false);  //  neither styate or charge have been set
    bool charge_set(false);
    QStack<QMap<const Element*, int> > group_stack; //  a QStack to stack the groups of Element.
        /*
         *  each group is stored as a QMap<const Element*, int>
         *  and once we know what its count is we reintegrate it into the preceding level,
         *  or the Molecule QMap if we have reach the level 0.
         */
    QString tmp("");    //  tmp for storing the current token, until it is complete and is treated.
    const Element* currentElement = 0;  //  the Elemnt we have parsed and whose associated count we might be Parsing
    foreach(QChar character, newFormula){   //  Foreach charater in the formula
        switch(parserState){    //  depending on the current state, the character is treated differently.
        case BeginParsing:  //  Begin parsing
            /*
             *  either we have an element or the begining of a group( or state)
             */
            if (character.isLetter() && character.isUpper()){   //  Upper letter -> Elemnt symbol
                tmp.append(character);  //  we append this to the token
                parserState = ParsingElement;   //  and switch to the Parsing elemnt State
                continue;   //  next char (leave the case block and continue with the next character
            }
            if (character == QChar('(')){   //  '(' -> Group
                parserState = ParsingGroupOrState;  //  Move to the GroupOrState state which takes care of the rest.
                continue;   //  next char
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingElement:
            /*
             *  We have parsed an Upper letter which is the begining of an element Symbol or an upper letter followed by lower letters
             *  Either we parse a lower letter, which continues the element Symbol or we parse the begining of the next token.
             *  in that case we set the current element from the element symbol stored in tmp and take the appropriate transition measures.
             */
            if (character.isLetter() && character.isLower()){   //  lower letter -> continues the symbol
                tmp.append(character);  //  we append it to the Symbol
                continue;   //  next char
            }
            if (character.isLetter() && character.isUpper()){   //  Upper letter -> new Element
                    //  the element parsing is finished
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if (currentElement) {   //  if the symbol represented a valid element
                    (*this)[currentElement] += 1;   //  we add 1 to the count of the element, adding it to the keys if it isn't already in it
                    tmp.clear();    //  we clear the token and append the begining of the new token
                    tmp.append(character);
                    //  parserState = ParsingElement;   //  not required since we are already in this state
                    continue;   //  next char if success
                }//else (failure) we get out of this if block and end up in the error handling block (the other if condition will be false)
            }
            if (character.isDigit()){   //  Digit -> Element count
                    //  the element parsing is finished
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if (currentElement){    //  if the symbol represented a valid element
                    tmp.clear();    //  we clear the token
                    parserState = ParsingElementCount;  //  move to the element cunt parsing
                    tmp.append(character);  //   and append the begining of the new token
                    continue;   //  next char if success
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character == QChar('(')) {  //  '(' -> Group or state
                    //  the element parsing is finished
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if (currentElement){    //  if the symbol represented a valid element
                    (*this)[currentElement] += 1;   //  we add 1 to the count of the element, adding it to the keys if it isn't already in it
                    tmp.clear();    //  we clear the token
                    parserState = ParsingGroupOrState;  //  and we move to the ParsingGroupOrState parser state
                    continue;   //  next char if success
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character == QChar('[')){   //  '[' -> Charge
                    //  the element parsing is finished
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if (currentElement){    //  if the symbol represented a valid element
                    (*this)[currentElement] += 1;   //  we add 1 to the count of the element, adding it to the keys if it isn't already in it
                    tmp.clear();    //  we clear the token
                    parserState = ParsingCharge;    //  and we move to the ParsingCharge parser state
                    continue;   //  next char if success
                }//else (failure) we get out of this if block and end up in the error handling block
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingElementCount:
            /*
             *  We have parsed a top level element and we are now parsing its count
             *  We have thus parsed at least one digit
             *  Either we parse another Digit, which continue the count, or we parse the begining of the next token.
             */
            if (character.isDigit()) {  //  Digit -> continue the Element count
                tmp.append(character);  //  we simply append it to the count
                continue;   //  next char
            }
            if (character.isLetter() && character.isUpper()){   //  Upper letter -> New Element
                if (currentElement){    //  if the symbol represented a valid element
                    (*this)[currentElement] += tmp.toInt(); //  the count is now contained in tmp (string). We add its value to the element count.
                    tmp.clear();    //  we clear the token
                    parserState = ParsingElement;   //  we return to the ParsingElement parser state
                    tmp.append(character);  //  we append to the tmp the begining of the new token
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character == QChar('(')) {  //  '(' -> Group or state
                if (currentElement){    //  if the symbol represented a valid element
                (*this)[currentElement] += tmp.toInt(); //  the count is now contained in tmp (string). We add its value to the elemnt count.
                    tmp.clear();    //  we clear the token
                    parserState = ParsingGroupOrState;  //  Move to the GroupOrState state which takes care of the rest.
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character == QChar('[')) {   //  '[' -> Charge
                if (currentElement){    //  if the symbol represented a valid element
                    (*this)[currentElement] += tmp.toInt(); //  the count is now contained in tmp (string). We add its value to the elemnt count.
                    tmp.clear();    //  we clear the token
                    parserState = ParsingCharge;    //  and we move to the ParsingCharge parser state
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingGroupOrState:
            /*
             *  We have parsed a '('
             *  Either we have parsed the beginning of a group or the begining of the elemnt State
             *  We therefore wait until the next character to see what it is.
             *  Either we have an upper letter or a '(' and we are parsing a group
             *  Or a lower letter and we are parsing the state.
             */
            if (character.isLetter() && character.isLower()){   //  lower letter -> State
                tmp.append(character);  //  we append to the tmp the beginning of the state
                parserState = ParsingState; //  we move to the ParsingState parser state
                continue;   //  next char
            }
            if (character.isLetter() && character.isUpper()){   //  Upper letter -> Element
                tmp.append(character);  //  We append the begining ot its symbol to the tmp
                group_stack.push(QMap<const Element*, int>());  //  we push on the stack a new level of group Element-Count Map
                parserState = ParsingGroupElement;  //  we move to the ParsingGroupElement parser state
                continue;   //  next char
            }
            if (character == QChar('(')){   //  '(' -> second level group
                group_stack.push(QMap<const Element*, int>());  //  we push a first group Element-Count Map, for the group we have already stepped into
                group_stack.push(QMap<const Element*, int>());  //  and a second for the group we are now stepping in
                parserState = ParsingGroup; //  we move to the ParsingGroup state
                continue;   //  next char
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingGroup:
            /*
             *  We were parsing a group when we stumbled upon a sub group
             *  The group Map has already been pushed on the stack
             *  We have Either an Element or the begining of (again) another sub-group
             */
            if (character.isLetter() && character.isUpper()){   //  Upper letter -> Element
                tmp.append(character);  //  We append the character to the tmp
                parserState = ParsingGroupElement;  //  We move to the ParsingGroupElement
                continue;   //  next char
            }
            if (character == QChar('(')){   //  '(' -> Sub group
                group_stack.push(QMap<const Element*, int>());  //  push on the stack its Element-Count Map
                parserState = ParsingGroup; //  redundant
                continue;   //  next char
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingGroupElement:
            /*
             *  We are parsing an element symbol inside a group
             *  We can have :
             *      the suite of the element Symbol
             *      A new element
             *      The current element count
             *      a new sub-group
             *      the end of the sub group
             */
            if (character.isLetter() && character.isLower()){   //  lower letter -> suite of the symbol
                tmp.append(character);  //  we append it to tmp
                continue;   //  next char
            }
            if (character.isLetter() && character.isUpper()){   //  Upper letter -> new element
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if (currentElement){    //  if the symbol represented a valid element
                    group_stack.top()[currentElement] += 1; //  we add 1 to the count of the element, adding it to the keys if it isn't already in it
                    tmp.clear();    //  we clear the token and append the begining of the new token
                    tmp.append(character);
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character.isDigit()){   //  Digit -> Element count
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if(currentElement){ //  if the symbol represented a valid element
                    tmp.clear();    //  we clear the token
                    parserState = ParsingGroupElementCount; //  we move to the ParsingGroupElementCount state
                    tmp.append(character);  //   and append the begining of the new token
                    continue;   //  next char if success
                }
            }
            if (character == QChar('(')) {  //  '(' -> New sub group
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if (currentElement){    //  if the symbol represented a valid element
                    group_stack.top()[currentElement] += 1; //  we add 1 to the count of the element, adding it to the keys if it isn't already in it
                    tmp.clear();    //  we clear the token
                    group_stack.push(QMap<const Element*, int>());  //  push on the stack its Element-Count Map
                    parserState = ParsingGroup; //  we move to the ParsingGroup state
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character == QChar(')')){   //  ')' -> end of group
                currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  Get the element from the periodic table
                if (currentElement){    //  if the symbol represented a valid element
                    group_stack.top()[currentElement] += 1; //  we add 1 to the count of the element, adding it to the keys if it isn't already in it
                    tmp.clear();    //  we clear the token
                    parserState = ParsingGroupCount;    //  we move to the ParsingGroupCount state which takes care of adding the group to the preceding level
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingGroupElementCount:
            /*
             *  We are parsing an element count inside a group
             *  We can have :
             *      the suite of the element count
             *      A new element
             *      a new sub-group
             *      the end of the sub group
             */
            if (character.isDigit()) {  //  digit -> suite of the element count
                tmp.append(character);  //  we append it to the tmp
                continue;   //  next char
            }
            if (character.isLetter() && character.isUpper()){   //  Upper letter -> new element
                if (currentElement){    //  if the symbol represented a valid element
                    group_stack.top()[currentElement] += tmp.toInt();   //  the count is now contained in tmp (string). We add its value to the element count.
                    tmp.clear();    //  we clear the token
                    parserState = ParsingGroupElement;  //  we move back to the ParsingGroupElement state
                    tmp.append(character);  //  we append to the tmp the begining of the new token
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character == QChar('(')) {  //  '(' -> New sub group
                if (currentElement){    //  if the symbol represented a valid element
                    group_stack.top()[currentElement] += tmp.toInt();   //  the count is now contained in tmp (string). We add its value to the element count.
                    tmp.clear();    //  we clear the token
                    group_stack.push(QMap<const Element*, int>());  //  push on the stack its Element-Count Map
                    parserState = ParsingGroup; //  we move to the ParsingGroup state
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
            if (character == QChar(')')){   //  '(' -> end of group
                if (currentElement){    //  if the symbol represented a valid element
                    group_stack.top()[currentElement] += tmp.toInt();   //  the count is now contained in tmp (string). We add its value to the element count.
                    tmp.clear();    //  we clear the token
                    parserState = ParsingGroupCount;    //  we move to the ParsingGroupCount state which takes care of adding the group to the preceding level
                    continue;   //  next char
                }//else (failure) we get out of this if block and end up in the error handling block
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingGroupCount:
            /*
             *  We are exiting a group and are now parsing its count.
             *  If the character is a digit it continues the count.
             *  If not it is the end of the group and we add the El-count map multiplied by the count or one if absent, to the preceding level
             */
            if (character.isDigit()){   //  Digit -> continues the count
                tmp.append(character);  //  we append it to the count string (tmp)
                continue;   //  next char
            }else { //  it isn't a digit -> end of count
                    //  add the top level group map to the level under it
                QMap<const Element*, int> group_being_left(group_stack.pop());  //  extract the group map from the stack
                if (group_stack.isEmpty()){    //  if the stack is empty (top level group) -> add it to the formula
                    foreach (const Element* element, group_being_left.keys()) {
                        (*this)[element] += group_being_left[element] * ((tmp.isEmpty()? 1:tmp.toInt()));
                        //  add its count multiplied by the group count or one if there's no count to the main Element-Count map
                    }
                }
                else {  //  else (the stack isn't empty -> add it to the new top level group (the group which was under the one being left
                    foreach (const Element* element, group_being_left.keys()) { //  foreach elemnt in the group
                        group_stack.top()[element] += group_being_left[element] * ((tmp.isEmpty()? 1:tmp.toInt()));
                        //  add its count multiplied by the group count or one if there's no count to the new top level group
                    }

                }
                tmp.clear();    //  we clear the token
                    //  now let's take care of the current char
                if (character.isLetter() && character.isUpper()){   //  Upper letter -> New Element
                    tmp.append(character);  //  append it to the tmp
                    parserState = ((group_stack.isEmpty())? ParsingElement:ParsingGroupElement);
                        //  move to either the ParsingElement state if the stack was empty or the ParsingGroupElement state if not.
                    continue;   //  next char
                }
                if (character == QChar('(')) {  //  '(' -> new group or perhaps the state if group stack empty
                    if (group_stack.isEmpty()){ //  Stack empty (top level group)
                        parserState = ParsingGroupOrState;  //  group or state
                    } else {    //  non top level group -> new sub group
                        group_stack.push(QMap<const Element*, int>());  //   push a new El-Count map on the group stack
                        parserState = ParsingGroup; //  move to the ParsingGroup state
                    }
                    continue;   //  next char
                }
                if (!group_stack.isEmpty() && character == QChar(')'))  //  ')' and if the group stack isn't empty -> end of another group
                    //  we are already in the parsing group count state
                    continue;   //  next char
                if (group_stack.isEmpty() && character == QChar('[')) { //  '[' and if we are leaving a top level group -> charge
                    parserState = ParsingCharge;    //  Parsing charge
                    continue;   //  next char
                }
                    //  errror and invalid formula handling
                parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
                m_state = Invalid;      //  the molecule is therefore in valid
                return false;           //  we interupt the parsing by returning
                break;  //  not needed

            }
        case ParsingCharge:
            /*
             *  We are parsing the charge of the molecule
             *  If the character is a digit, it continues the charge
             *  If the character is + or -, it must be the last character befor the charge ending ']' char
             *  If the character is ']', then then the charge should have been set and valid (either + or - at the end)
             */
            if (character.isDigit() && !charge_set) {   //  the charge hasn't been set and Digit -> suite of the charge count
                tmp.append(character);  //  we append it to the tmp
                continue;   //  next char
            }
            if (!charge_set && (character == QChar('+') || character == QChar('-'))){    //  either + or - -> end of charge and charge sign
                if (tmp.isEmpty())  //  charge of the form XXX[+/-], implies charge count of 1
                    tmp.append("1");    //  we put 1 in the tmp
                tmp.prepend(character); //  we prepend to the tmp containing the count its sign
                m_charge = tmp.toInt(); //  we get the (signed) it contained in the tmp
                tmp.clear();    //  we clear the token
                charge_set = true;  //  the charge is now set
                continue;   //  next char
            }
            if (character == QChar(']') && charge_set) {    //  the charge has been set and we caught the charge ending ']' char
                parserState = ChargeParsed; //  the charge is parsed
                continue;   //  next char
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ChargeParsed:
            /*
             *  We have just finished pasring the Charge
             *  If the state wasn't specified before, we can have the state
             *  Anything else is an error
             */
            if (character == QChar('(') && !state_set){ //  '(' and state not set -> parsing state
                parserState = ParsingState; //  move to the ParsingState state
                continue;   //  next char
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case ParsingState:
            /*
             *  We are parsing the state of the molecule
             *  If the character is a lower letter, it continues the state. (TODO: should state contain upper letters ?)
             *  If the character is ')' it ends the state, and we set the state.
             */
            if (character.isLower() && !state_set) {    //  the state hasn't been set yet and we encounter a Lower letter -> append it to tmp
                tmp.append(character);
                continue;   //  next char
            }
            if (character == QChar(')')) {  //  '[' -> end of state
                                            //  Compare the string to the known states
                if (tmp == QString("aq")){  //  "aq" -> Aqueous
                    m_state = Aqueous;
                }
                else if (tmp == QString("s")){  //  "s" -> Solid
                    m_state  = Solid;
                }
                else if (tmp == QString("g")){  //  "g" -> Gas
                    m_state = Gas;
                }
                else if (tmp == QString("l")){  //  "l" -> Liquid
                    m_state = Liquid;
                }
                else {                          //  not a known state
                    m_state = OtherSoluted;     //  assume it's a soluted state in something which isn't water
                }
                m_state_str = tmp;
                tmp.clear();    //  we clear the token
                state_set = true;
                parserState = StateParsed;
                continue;   //  next char
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        case StateParsed:
            /*
             *  We have just set the state of the molecule
             *  If the charge hasn't been alredy set and we encounter a '[' we move to the ParsingCharge state which will try to set it.
             */
            if (character == QChar('[') && !charge_set) {
                parserState = ParsingCharge;
                continue;   //  next char
            }
                //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        default:    //  other invalid state -> Error
                //  errror and invalid formula handling
            parserState = Error;    //  Error
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        }
    }
    switch (parserState) {  //  do cleanup at the end of the loop
    case ParsingElement:    //  we left the loop parsing an element with no count
        currentElement = c_PeriodicElementTable->getElementFromSymbol(tmp); //  we get the element from the periodic table
        if (currentElement) //  check if it is valid
            (*this)[currentElement] += 1;   //  add it
        else {
            //  errror and invalid formula handling
            parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
            m_state = Invalid;      //  the molecule is therefore in valid
            return false;           //  we interupt the parsing by returning
            break;  //  not needed
        }
        tmp.clear();    //  clean up the tmp
        break;  //  clean up finished
    case ParsingElementCount:
        if (currentElement){
            (*this)[currentElement] += tmp.toInt();
            tmp.clear();
            break;
        }//else (failure) we get out of this if block and end up in the error handling block
            //  errror and invalid formula handling
        parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
        m_state = Invalid;      //  the molecule is therefore in valid
        return false;           //  we interupt the parsing by returning
        break;  //  not needed
    case ParsingGroupCount:
        if (group_stack.size() == 1) {
            foreach (const Element* element, group_stack.top().keys()) {
                (*this)[element] += group_stack.top()[element] * ((tmp.isEmpty()? 1:tmp.toInt()));
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
            //  errror and invalid formula handling
        parserState = Error;    //  if we haven't continue'd yet we are in neither valid states
        m_state = Invalid;      //  the molecule is therefore in valid
        return false;           //  we interupt the parsing by returning
        break;  //  not needed
    default:
            break;
    }
    m_formula = newFormula;
    return true;
}
    //  factory constructor and destructors.
MoleculeFactory::MoleculeFactory()
{

}

MoleculeFactory::~MoleculeFactory()
{
    instance = (MoleculeFactory*)0; //  not to leave the pointer dangling.
}

Molecule* MoleculeFactory::buildReactive(QString formula) const
{
    if(QRegExp(reactiveRX()).exactMatch(formula))   //  check the match of the RX
    {
        bool ok(true);  //  boolean passed to Molecule constructor
        Molecule * molecule = new Molecule(formula, &ok);   //  new Molecule from formula
        if(ok)  //  if it was ok
            return molecule;    //  return it

        delete molecule;    //  if not delete it
    }
    return 0;   // if we haven't already returned,
                // then the formula does not represent
                // a Molecule and we shall therefore return 0.
}


MoleculeFactory* MoleculeFactory::getInstance() //  creates a new instance if it does not exist (instance == 0)
{
    if(instance)    //  if instance exist
        return instance;    //  return it
    return instance = new MoleculeFactory();    //  else create a new instance and return it
}

    //  returns a map associating to each const Element* its count
QMap<const Element*, int> Molecule::elementMap() const
{
    return QMap<const Element*, int>(*this);    //  return a freshly created Map from *this, using QMap copy constructor.
                                                //  Molecule is internally implemented by private inheritance of QMap
}
#ifdef DEBUG
    //  debug output operator.
QDebug operator<<(QDebug out, const Molecule & mol){
    out << mol.toStr() << " : " << mol.elementMap() << " : " << mol.charge() << "\n";
    foreach (const Element* element, mol.elementMap().keys()) {
        out << element << " : " << element->symbol();
    }
    return out;
}
#endif	//	ifdef DEBUG

