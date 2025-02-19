#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

class RequestHandler {
public:
    std::string processRequest(const std::string& request);
};

#endif // REQUEST_HANDLER_H
