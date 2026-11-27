#ifndef INSUFFICIENT_FUNDS_EXCEPTION_H
#define INSUFFICIENT_FUNDS_EXCEPTION_H

#include <exception>

class InsufficientFundsException : public std::exception {
  public:
    InsufficientFundsException() noexcept = default;
    ~InsufficientFundsException() = default;
    virtual const char *what() const noexcept override { return "Insufficient funds exception"; }
};

#endif // INSUFFICIENT_FUNDS_EXCEPTION_H
