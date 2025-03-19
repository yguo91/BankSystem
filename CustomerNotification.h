#ifndef CUSTOMERNOTIFICATION_H
#define CUSTOMERNOTIFICATION_H

#include "Notification.h"
#include <iostream>
#include <string>

class CustomerNotification : public Notification {
public:
    void update(const std::string& message) override;
};

#endif // CUSTOMERNOTIFICATION_H
