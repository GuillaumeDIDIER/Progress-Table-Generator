#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include "gui/MainWindow.h"
//	QWizardPages' subclasses
class IntroPage : public QWizardPage {
Q_OBJECT
public:
    IntroPage(QWidget* parent = 0);
    virtual ~IntroPage();
    void initializePage();
    protected:
QLabel* m_introLabel;
};

class EquationPage : public QWizardPage {
Q_OBJECT
public:
EquationPage(QWidget* parent = 0);
    virtual ~EquationPage();
    void initializePage();
    bool validatePage();
protected:
    QVBoxLayout* m_mainLayout;
    QLabel* m_arrow;
    QLineEdit *m_reactives, *m_products;
};

class EquilibratePage : public QWizardPage {
Q_OBJECT
public:
    EquilibratePage(QWidget* parent = 0);
    virtual ~EquilibratePage();
    void initializePage();
    bool validatePage();
public slots:
    bool equilibrateEq();
protected:
    enum State {
        Invalid, Unequilibrated, Equilibrated, Error
    };
    void setState(State state);
    State updateState();
    QVBoxLayout* m_mainLayout;
    Equation* m_equation;
    QLabel* m_stateLabelImg,* m_eqStr,* m_stateLabel;
    QPushButton* m_equilibrate;
    State m_state;
    bool m_hasTryiedToEquilibrate;
};

class MatterQuantityPage : public QWizardPage {
Q_OBJECT
public:
    MatterQuantityPage(QWidget * parent = 0);
    virtual ~MatterQuantityPage();
    void initializePage();
protected:
    Equation* m_equation;
    QMap<Molecule, QDoubleSpinBox*> m_reactivesMatterQuantities;
    QFormLayout* m_layout;
};

class ConclusionPage : public QWizardPage {
Q_OBJECT
public:
    ConclusionPage(QWidget* parent = 0);
    virtual ~ConclusionPage();
protected:

};


#endif // MAINWINDOW_P_H
