/*
 *  Element.cpp
 *  Physique
 *
 *  Created by Guillaume DIDIER on 18/02/11.
 *  Copyright 2011. All rights reserved.
 *	Element class implementation
 */

#include "Physique.h"

#include "Element.h"

	//	Default constructor, not very useful
Element::Element() : m_name(""), m_symbol(""), m_Z(0), m_A(0)/*, m_maxBond(0), m_externalElectronNb(0)*/{}

	//	Main constructor
Element::Element(QString name,QString symbol, int Z/*, int externalElectron, int maxBond*/, int A) :
m_name(name), m_symbol(symbol), m_Z(Z), m_A(A)/*, m_maxBond(maxBond), m_externalElectronNb(externalElectron)*/{
	if (m_A<m_Z) {
		m_A = m_Z;
	}
}

	//	copy constructor
Element::Element(const Element& other) : m_name(other.m_name), m_symbol(other.m_symbol), m_Z(other.m_Z), m_A(other.m_A)/*,
m_maxBond(other.m_maxBond), m_externalElectronNb(other.m_externalElectronNb)*/{}

	//	Destructor, naught to do here, only on the stacks variables, deallocated by compiler
Element::~Element(){}

	//	Accessors
		//	name getter
QString Element::name() const{
	return m_name;
}
		//	symbol getter
QString Element::symbol() const{
	return m_symbol;
}
		//	Z (atomic number) getter
int Element::Z() const{
	return m_Z;
}
		//	A (mass number) getter
int Element::A() const{
	return m_A;
}
/*		//	max_bond getter
int Element::maxBond() const{
	return m_maxBond;
}
		//	external electron number getter
int Element::externalElectronNb() const{
	return m_externalElectronNb;
}
*/
	// Todo add setters

	//	comparison
bool Element::symbolCompare(const Element* a, const Element* b){
	int res = QString::compare(a->symbol(), b->symbol());
	if (res >= 0) {
		return false;
	}
	else {
		return true;
	}

}

bool Element::ZCompare(const Element* a, const Element* b){
	if (a->Z() >= b->Z()) {
		return false;
	}
	else {
		return true;
	}

}
