#include <exception>
#include <iostream>
#include <QApplication>

#include "LoanCalcQtMainWindow.h"
#include "CmdLineParser.h"
#include "LoanCalculator.h"

using namespace std;

enum CALC_TYPE
{
    CALC_UNKNOWN=0,
    CALC_BALANCE=100,
    CALC_PAYMENT,
    CALC_NUMPAYMENTS,
    CALC_AMOUNT,
    CALC_INTEREST
};

const string ARG_CALC_BALANCE      = "-cb";
const string ARG_CALC_PAYMENT      = "-cp";
const string ARG_CALC_NUMPAYMENTS  = "-cn";
const string ARG_CALC_AMOUNT       = "-ca";
const string ARG_CALC_INTEREST     = "-ci";

const string ARG_PAYMENT           = "-p";
const string ARG_PERIOD_TOTAL      = "-N";
const string ARG_PERIOD_ELAPSED    = "-n";
const string ARG_AMOUNT            = "-a";
const string ARG_INITIAL_PAYMENT   = "-ai";
const string ARG_INTEREST          = "-i";
const string ARG_OPENFEE           = "-of";
const string ARG_OPENPERCENT       = "-op";

void loadCmdLine(CmdLineParser &clp)
{
    clp.setMainHelpText("A simple loan calculator");
    clp.setMainHelpTextEnd("With no options set, GUI will start");

    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_BALANCE,"Calculate balance",false,CALC_BALANCE));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_PAYMENT,"Calculate payment",false,CALC_PAYMENT));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_NUMPAYMENTS,"Calculate number of payments",false,CALC_NUMPAYMENTS));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_AMOUNT,"Calculate initial amount",false,CALC_AMOUNT));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_INTEREST,"Calculate interest",false,CALC_INTEREST));

    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_PAYMENT,"Set payment"));
    clp.addCmdLineOption(new CmdLineOptionInt(ARG_PERIOD_TOTAL,"Set total periods"));
    clp.addCmdLineOption(new CmdLineOptionInt(ARG_PERIOD_ELAPSED,"Set elapsed periods"));
    clp.addCmdLineOption(new CmdLineOptionInt(ARG_AMOUNT,"Set amount"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_INITIAL_PAYMENT,"Initial payment"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_INTEREST,"Set interest"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_OPENFEE,"Opening fee"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_OPENPERCENT,"Opening percent"));

    clp.setMinNumberArgs(3);
}

CALC_TYPE parseCommandLine(int argc, char **argv, CmdLineParser &clp, LoanCalculator &calculator)
{
    CALC_TYPE ct(CALC_UNKNOWN);

    if(!clp.parseCmdLine(argc, argv))
    {
        clp.printUsage();
        return ct;
    }

    calculator.setAmount(((CmdLineOptionInt*)clp.getCmdLineOption(ARG_AMOUNT))->getValue());
    calculator.setInitialPayment(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_INITIAL_PAYMENT))->getValue());
    calculator.setInterest(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_INTEREST))->getValue());
    calculator.setPayment(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_PAYMENT))->getValue());
    calculator.setPeriodTotal(((CmdLineOptionInt*)clp.getCmdLineOption(ARG_PERIOD_TOTAL))->getValue());
    calculator.setPeriodElapsed(((CmdLineOptionInt*)clp.getCmdLineOption(ARG_PERIOD_ELAPSED))->getValue());
    calculator.setOpeningFee(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_OPENFEE))->getValue());
    calculator.setOpeningPercent(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_OPENPERCENT))->getValue());

    CmdLineOption *opt = clp.getMutExclOption();
    if (opt != nullptr)
        ct = (CALC_TYPE)((CmdLineOptionFlag*)opt)->getValueKey();

    return ct;
}

int main(int argc, char **argv)
{
    LoanCalculator calculator;

    if(argc == 1)
    {
        QApplication app(argc, argv);
        LoanCalcQtMainWindow win(&calculator);
        win.show();
        return app.exec();
    }

    CmdLineParser clp;
    loadCmdLine(clp);
    CALC_TYPE ct = parseCommandLine(argc, argv, clp, calculator);

    try
    {
        cout << endl;

        switch (ct)
        {
            case CALC_BALANCE:
                cout << "Loan Balance = " << calculator.calculateLoanBalance() << endl;
                break;

            case CALC_PAYMENT:
            {
                float p = calculator.calculatePayment();
                cout << "Monthly Payment = " << p << endl;
                cout << "Total Paid = " << p * calculator.getPeriodTotal() << endl;
                if (calculator.getOpeningFee() > 0 || calculator.getOpeningPercent() > 0)
                    cout << "Effective Interest = " << calculator.calculateEffectiveInterestRate() << "%\n";
            }
            break;

            case CALC_NUMPAYMENTS:
                cout << "Number of payments = " << calculator.calculateNumberPayments() << endl;
                break;

            case CALC_AMOUNT:
                cout << "Initial Loan amount = " << calculator.calculateLoanAmount() << endl;
                break;

            case CALC_INTEREST:
                cout << "Interest rate = " << calculator.calculateInterestRate() << "%\n";
                break;

            case CALC_UNKNOWN:
                return 1;
        }

        cout << calculator.toString() << endl;
    }
    catch(const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    cout << endl;
}
