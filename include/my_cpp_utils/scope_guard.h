#pragma once
#include <exception>
#include <functional>

class ScopeGuard
{
public:
    using ScopeGuardOnExit = std::function<void()>;
    using ScopeGuardOnException = std::function<void(int countOfNewExceptions)>;

    explicit ScopeGuard(ScopeGuardOnExit onExitScope) : onExitScope_(onExitScope), onException_(nullptr)
    {
        originalExceptionCount_ = std::uncaught_exceptions();
    }

    explicit ScopeGuard(ScopeGuardOnException onException) : onExitScope_(nullptr), onException_(onException)
    {
        originalExceptionCount_ = std::uncaught_exceptions();
    }

    ~ScopeGuard()
    {
        int countOfNewExceptions = std::uncaught_exceptions() - originalExceptionCount_;
        if (countOfNewExceptions > 0 && onException_)
            onException_(countOfNewExceptions);

        if (onExitScope_)
            onExitScope_();
    }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
private:
    ScopeGuardOnExit onExitScope_;
    ScopeGuardOnException onException_;
    int originalExceptionCount_ = 0;
};
