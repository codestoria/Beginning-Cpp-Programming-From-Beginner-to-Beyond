#ifndef ILLEGAL_BALANCE_EXCEPTION_H
#define ILLEGAL_BALANCE_EXCEPTION_H

#include <exception>

class IllegalBalanceException : public std::exception {
  public:
    IllegalBalanceException() noexcept = default;
    ~IllegalBalanceException() = default;
    virtual const char *what() const noexcept override { return "Illegal balance exception"; }
};

#endif // ILLEGAL_BALANCE_EXCEPTION_H
