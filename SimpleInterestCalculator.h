#ifndef SIMPLEINTERESTCALCULATOR_H
#define SIMPLEINTERESTCALCULATOR_H

#include "InterestCalculator.h"

class SimpleInterestCalculator : public InterestCalculator {
public:
    double calculateInterest(double balance) override;
};

#endif // SIMPLEINTERESTCALCULATOR_H
