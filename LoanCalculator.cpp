#include "LoanCalculator.h"
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

static const double MAX_EXP = 1e6;   // Max limit to prevent overflow
static const double EPS     = 1e-9;  // Small epsilon

LoanCalculator::LoanCalculator()
{
    reset();

    // -------- LOAD DEFAULTS FROM CONFIG.TXT --------
    ifstream file("config.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            size_t pos = line.find('=');
            if (pos == string::npos) continue;

            string key = line.substr(0, pos);
            string val = line.substr(pos + 1);

            try {
                if (key == "amount") amount_ = stof(val), amountSet_ = true;
                if (key == "interest") setInterest(stof(val));
                if (key == "payment") payment_ = stof(val), paymentSet_ = true;
                if (key == "N") periodTotal_ = stoi(val), periodTotalSet_ = true;
                if (key == "n") periodElapsed_ = stoi(val), periodElapsedSet_ = true;
                if (key == "openingFee") openingFee_ = stof(val);
                if (key == "openingPercent") openingPercent_ = stof(val);
            } catch (...) {
                // ignore corrupted lines
            }
        }
        file.close();
    }
}

//--------------------------------------------------------------------
// HELPER
//--------------------------------------------------------------------
static double safePow(double base, double exp)
{
    if (base <= 0) return 0;
    double v = pow(base, exp);
    if (v > MAX_EXP) return MAX_EXP;
    return v;
}

//--------------------------------------------------------------------
// BALANCE CALCULATION
//--------------------------------------------------------------------
float LoanCalculator::calculateLoanBalance()
{
    if (!amountSet_ || !interestSet_ || !paymentSet_ || !periodElapsedSet_)
        throw runtime_error("Missing required values");

    if (interestPeriodic_ <= 0)
        throw runtime_error("Interest must be > 0");

    double A = amount_;
    double P = payment_;
    double i = interestPeriodic_;
    double n = periodElapsed_;

    double powVal = safePow(1 + i, n);
    double bn = A * powVal - (P / i) * (powVal - 1);

    return (float)bn;
}

//--------------------------------------------------------------------
// PAYMENT CALCULATION
//--------------------------------------------------------------------
float LoanCalculator::calculatePayment()
{
    if (!amountSet_ || !interestSet_ || !periodTotalSet_)
        throw runtime_error("Missing required values");

    if (interestPeriodic_ <= 0)
        throw runtime_error("Interest must be > 0");

    if (periodTotal_ <= 0)
        throw runtime_error("Total periods must be > 0");

    double A = amount_;
    double i = interestPeriodic_;
    double N = periodTotal_;

    double denom = 1 - safePow(1 + i, -N);

    if (fabs(denom) < EPS)
        throw runtime_error("Invalid loan parameters");

    return (float)((i * A) / denom);
}

//--------------------------------------------------------------------
// NUMBER OF PAYMENTS
//--------------------------------------------------------------------
float LoanCalculator::calculateNumberPayments()
{
    if (!amountSet_ || !interestSet_ || !paymentSet_)
        throw runtime_error("Missing required values");

    if (payment_ <= 0)
        throw runtime_error("Payment must be > 0");

    double A = amount_;
    double i = interestPeriodic_;
    double P = payment_;

    double inner = 1 - i * A / P;

    if (inner <= 0 || inner >= 1)
        throw runtime_error("Invalid parameters; cannot compute N");

    return (float)(-log(inner) / log(1 + i));
}

//--------------------------------------------------------------------
// LOAN AMOUNT
//--------------------------------------------------------------------
float LoanCalculator::calculateLoanAmount()
{
    if (!paymentSet_ || !interestSet_ || !periodTotalSet_)
        throw runtime_error("Missing required values");

    double P = payment_;
    double i = interestPeriodic_;
    double N = periodTotal_;

    double powVal = safePow(1 + i, -N);

    return (float)((P / i) * (1 - powVal));
}

//--------------------------------------------------------------------
// INTEREST RATE APPROXIMATION
//--------------------------------------------------------------------
float LoanCalculator::calculateInterestRate()
{
    if (!amountSet_ || !paymentSet_ || !periodTotalSet_)
        throw runtime_error("Missing required values");

    double A = amount_;
    double P = payment_;
    double N = periodTotal_;

    if (A <= 0 || P <= 0 || N <= 0)
        throw runtime_error("Invalid parameters");

    double q = log(1 + 1.0 / N) / log(2);
    double approx = pow(pow(1 + P / A, 1.0 / q) - 1, q) - 1;

    return (float)(approx * 100);  // Convert to %
}

//--------------------------------------------------------------------
// EFFECTIVE INTEREST WITH FEES
//--------------------------------------------------------------------
float LoanCalculator::calculateEffectiveInterestRate()
{
    double A = amount_;
    double fee = openingFee_ + (openingPercent_ / 100.0 * A);

    if (A <= 0) return 0;

    return (float)(((fee / A) + interest_ / 100.0) * 100);
}

//--------------------------------------------------------------------
// STRING OUTPUT
//--------------------------------------------------------------------
string LoanCalculator::toString()
{
    stringstream ss;
    ss << "Loan amount: " << amount_ << "\n"
       << "Initial payment: " << initialPayment_ << "\n"
       << "Interest yearly: " << interest_ << "%\n"
       << "Interest periodic: " << interestPeriodic_ << "\n"
       << "Payment: " << payment_ << "\n"
       << "Total periods: " << periodTotal_ << "\n"
       << "Elapsed periods: " << periodElapsed_ << "\n"
       << "Opening fee: " << openingFee_ << "\n"
       << "Opening percent: " << openingPercent_ << "%";

    return ss.str();
}
