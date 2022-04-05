#pragma once
#ifndef _METAPROGRAMMING_HPP_
#define _METAPROGRAMMING_HPP_

#include <memory> // unique_ptr

/**
 * @brief A simple CRTP that provides other comparison functions
 * given operator< implementation on the derived class.
 * 
 * @tparam cls The most specific class to acquire these operators.
 */
template<typename cls>
class CompareFromLessCRTP {
private:
    const cls& self() const {
        return *static_cast<const cls*>(this);
    }
    cls& self() {
        return const_cast<cls&>(self());
    }
public:
    bool operator>(const cls& other) const {
        return other < self();
    }
    bool operator==(const cls& other) const {
        return !(other < self()) && !(self() < other);
    }
    bool operator!=(const cls& other) const {
        return !(self() == other);
    }
    bool operator<=(const cls& other) const {
        return self() < other || self() == other;
    }
    bool operator>=(const cls& other) const {
        return self() == other || other < self();
    }
};

/**
 * @brief A simple wrapper class that supports Nullable/emptiable/optional value.
 * This is helpful for specificity in our interface/class design
 * 
 * @tparam T 
 */
template<typename T>
class Optional: std::unique_ptr<T> {
public:
    using std::unique_ptr<T>::unique_ptr;
    using std::unique_ptr<T>::operator*;
    using std::unique_ptr<T>::operator->;
    using std::unique_ptr<T>::operator bool;

    bool has_value() {
        return static_cast<bool>(*this);
    }
    const T& value() const& {
        return *this;
    }
    T& value() & {
        return const_cast<T&>(static_cast<const Optional<T>*>(this)->value());
    }
    const T&& value() const&& {
        return *this;
    }
    T&& value() && {
        return const_cast<T&&>(static_cast<const Optional<T>*>(this)->value());
    }
};
template<typename T, typename... TConstructs>
[[nodiscard]] inline Optional<T> make_optional(TConstructs&&... args) {
    return {std::move(std::make_unique(std::forward<TConstructs>(args)...))};
}


#endif
