#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification {
public:
    virtual void update(const std::string& message) = 0;
};

#endif // NOTIFICATION_H
