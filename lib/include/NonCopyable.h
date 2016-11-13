#pragma once

namespace fm_system {
class NonCopyable {
protected:
    NonCopyable() {
    }

public:
    NonCopyable(const NonCopyable& other) = delete;      // non construction-copyable
    NonCopyable& operator=(const NonCopyable&) = delete; // non copyable
};
}