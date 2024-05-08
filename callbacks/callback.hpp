#pragma once

#include <any>
#include <cstdint>
#include <functional>
#include <map>
#include <unordered_map>

namespace stmbed::callback {
inline std::unordered_map<intptr_t, std::multimap<uint8_t, std::any>> callback_fns;

inline void attach(intptr_t handle, std::any &&fn, uint8_t priority) {
    callback_fns[handle].insert({priority, std::move(fn)});
}

template <class T, class... Args> inline void callback(intptr_t handle, Args... args) {
    for (const auto &f : callback_fns[handle]) {
        std::any_cast<std::function<T>>(f.second)(args...);
    }
}
} // namespace stmbed::callback
