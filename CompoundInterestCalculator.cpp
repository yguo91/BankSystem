#include "CompoundInterestCalculator.h"
#include <cmath>

double CompoundInterestCalculator::calculateInterest(double balance) {
    return balance * (pow(1 + 0.0225, 1) - 1);
}
