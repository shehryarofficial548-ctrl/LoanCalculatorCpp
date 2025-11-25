#include <gtest/gtest.h>
#include "LoanCalculator.h"

// ----------------------------------------------
// 1. NORMAL EMI CALCULATION
// ----------------------------------------------
TEST(LoanCalculatorTest, NormalPaymentCalculation) {

    LoanCalculator calc;

    calc.setAmount(100000);         // Loan 100,000
    calc.setInterest(6);            // 6% annual interest
    calc.setPeriodTotal(120);       // 10 years = 120 months

    float payment = calc.calculatePayment();

    // Expected EMI ≈ 1110.21 for 6%/year and 10 years
    EXPECT_NEAR(payment, 1110.21, 1.0);
}

// ----------------------------------------------
// 2. INVALID INPUT HANDLING
// ----------------------------------------------
TEST(LoanCalculatorTest, InvalidInputThrows) {
    LoanCalculator calc;
    calc.reset();           // remove config.txt values
    calc.setAmount(100000);
    calc.setInterest(0.0f); // EXACT zero
    calc.setPeriodTotal(120);

    EXPECT_THROW({
        calc.calculatePayment();
    }, std::runtime_error);
}

// ----------------------------------------------
// 3. LARGE TENURE OVERFLOW SAFE
// ----------------------------------------------
TEST(LoanCalculatorTest, LargeTenureDoesNotOverflow) {

    LoanCalculator calc;

    calc.setAmount(250000);
    calc.setInterest(7.5);
    calc.setPeriodTotal(1200);    // 100 YEARS (large)

    float payment = calc.calculatePayment();

    // Should return a positive EMI and NOT overflow
    EXPECT_GT(payment, 0);         // strictly > 0
    EXPECT_LT(payment, 100000);    // reasonable upper bound
}
