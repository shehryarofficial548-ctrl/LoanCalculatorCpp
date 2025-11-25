#ifndef LOANCALCULATOR_H_INCLUDED
#define LOANCALCULATOR_H_INCLUDED

#include <string>

/**
 * @class LoanCalculator
 * @brief A class that performs various loan-related financial calculations.
 *
 * This class provides functions to compute loan payment, loan amount,
 * number of payments, interest rate, effective interest, and loan balance.
 * It also supports loading default configuration values from a config file.
 */
class LoanCalculator
{
public:

    /**
     * @brief Constructor for LoanCalculator.
     * @param loadConfig If true, loads default values from config.txt.
     */
    LoanCalculator(bool loadConfig = true);

    /**
     * @brief Destructor (default).
     */
    ~LoanCalculator() {}

    // ───────────────────────────────
    // Setters & Getters
    // ───────────────────────────────

    /**
     * @brief Set the loan amount.
     * @param A Principal loan amount.
     */
    inline void setAmount(float A) { amount_ = A; amountSet_ = true; }

    /**
     * @brief Get the loan amount.
     * @return Loan amount.
     */
    inline float getAmount() const { return amount_; }

    /**
     * @brief Set the initial payment.
     * @param val Initial down payment.
     */
    inline void setInitialPayment(float val) { initialPayment_ = val; }

    /**
     * @brief Get the initial payment.
     * @return Initial payment.
     */
    inline float getInitialPayment() const { return initialPayment_; }

    /**
     * @brief Set annual interest rate.
     * @param i Annual interest percentage (e.g., 7.5 for 7.5%).
     */
    void setInterest(float i) { interest_ = i; interestPeriodic_ = i/100.0/12.0; interestSet_ = true; }

    /**
     * @brief Get annual interest rate.
     * @return Annual interest rate in percent.
     */
    inline float getInterest() const { return interest_; }

    /**
     * @brief Get monthly periodic interest rate.
     * @return Monthly interest fraction.
     */
    inline float getPeriodicInterest() const { return interestPeriodic_; }

    /**
     * @brief Set monthly payment amount.
     * @param P Monthly payment.
     */
    inline void setPayment(float P) { payment_ = P; paymentSet_ = true; }

    /**
     * @brief Get monthly payment.
     * @return Payment amount.
     */
    inline float getPayment() const { return payment_; }

    /**
     * @brief Set total number of periods (months).
     * @param N Total months.
     */
    inline void setPeriodTotal(int N) { periodTotal_ = N; periodTotalSet_ = true; }

    /**
     * @brief Get total number of periods.
     * @return Total periods.
     */
    inline int getPeriodTotal() const { return periodTotal_; }

    /**
     * @brief Set elapsed periods (months passed).
     * @param n Elapsed months.
     */
    inline void setPeriodElapsed(int n) { periodElapsed_ = n; periodElapsedSet_ = true; }

    /**
     * @brief Get elapsed periods.
     * @return Elapsed months.
     */
    inline int getPeriodElapsed() const { return periodElapsed_; }

    /**
     * @brief Set flat opening fee.
     * @param v Opening fee.
     */
    inline void setOpeningFee(float v) { openingFee_ = v; }

    /**
     * @brief Get opening fee.
     * @return Opening fee.
     */
    inline float getOpeningFee() const { return openingFee_; }

    /**
     * @brief Set percentage-based opening fee.
     * @param p Fee percentage.
     */
    inline void setOpeningPercent(float p) { openingPercent_ = p; }

    /**
     * @brief Get opening percentage fee.
     * @return Percentage.
     */
    inline float getOpeningPercent() const { return openingPercent_; }

    /**
     * @brief Reset all internal values to default state.
     */
    inline void reset()
    {
        amount_ = initialPayment_ = interest_ = interestPeriodic_ =
        payment_ = openingFee_ = openingPercent_ = 0.0;

        periodTotal_ = periodElapsed_ = 0;

        amountSet_ = interestSet_ = paymentSet_ =
        periodTotalSet_ = periodElapsedSet_ = false;
    }

    // ───────────────────────────────
    //     Calculation Methods
    // ───────────────────────────────

    /**
     * @brief Calculate remaining balance after given months.
     * @return Remaining loan balance.
     * @throws std::runtime_error if required values are missing.
     */
    float calculateLoanBalance();

    /**
     * @brief Calculate monthly EMI payment.
     * @return Monthly payment amount.
     * @throws std::runtime_error if parameters are invalid.
     */
    float calculatePayment();

    /**
     * @brief Calculate number of required payments.
     * @return Number of months.
     */
    float calculateNumberPayments();

    /**
     * @brief Calculate loan amount using payment, rate & tenure.
     * @return Principal loan amount.
     */
    float calculateLoanAmount();

    /**
     * @brief Estimate interest rate (approximation).
     * @return Annual interest percentage.
     */
    float calculateInterestRate();

    /**
     * @brief Calculate effective interest rate including fees.
     * @return Effective interest (percent).
     */
    float calculateEffectiveInterestRate();

    /**
     * @brief Convert object data to a formatted string.
     * @return String representation.
     */
    std::string toString();

private:

    float amount_;          ///< Loan amount
    bool amountSet_;        ///< True if amount has been assigned

    float initialPayment_;  ///< Initial down payment

    float interest_;        ///< Annual interest percent
    float interestPeriodic_;///< Monthly interest fraction
    bool interestSet_;      ///< True if interest has been set

    float payment_;         ///< Monthly payment
    bool paymentSet_;       ///< True if payment has been assigned

    int periodTotal_;       ///< Total months
    bool periodTotalSet_;   ///< True if period total is set

    int periodElapsed_;     ///< Elapsed months
    bool periodElapsedSet_; ///< True if elapsed period is set

    float openingFee_;      ///< Fixed opening fee
    float openingPercent_;  ///< Percentage-based opening fee
};

#endif
