#ifndef COMPOUNDINTERESTCALCULATOR_H
#define COMPOUNDINTERESTCALCULATOR_H

#include "InterestCalculator.h"

class CompoundInterestCalculator : public InterestCalculator {
public:
    double calculateInterest(double balance) override;
};

#endif // COMPOUNDINTERESTCALCULATOR_H
