#pragma once
#ifndef _METAPROGRAMMING_HPP_
#define _METAPROGRAMMING_HPP_

#include <memory> // unique_ptr
#include <type_traits>
#include <sstream> // ostringstream

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
protected:
    ~CompareFromLessCRTP() = default;
};

/**
 * @brief A simple CRTP that provides toString() from
 * std:ostream& operator<<(std::ostream&, const T&)
 * 
 * @tparam cls 
 */
template<typename cls>
class ToStringCRTP {
private:
    const cls& self() const {
        return *static_cast<const cls*>(this);
    }
    cls& self() {
        return const_cast<cls&>(self());
    }
public:
    std::string toString() const {
        std::ostringstream str;
        str << self();
        return str.str();
    }
};

/**
 * @brief A simple wrapper class that supports Nullable/emptiable/optional value.
 * This is helpful for specificity in our interface/class design
 * 
 * @tparam T 
 */
template<typename T>
class Optional: 
private std::unique_ptr<T>, public ToStringCRTP<Optional<T>> {
public:
    using std::unique_ptr<T>::unique_ptr;
    using std::unique_ptr<T>::operator*;
    using std::unique_ptr<T>::operator->;
    using std::unique_ptr<T>::operator bool;
    using std::unique_ptr<T>::reset;

    template<typename... TForward>
    Optional(TForward&&... args): Optional<T>::unique_ptr(std::forward<TForward>(args)...){}

    bool has_value() const{
        return static_cast<bool>(*this);
    }
    const T& value() const& {
        return **this;
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

    void clear() {
        reset();
    }
    friend std::ostream& operator<<(std::ostream& os, const Optional<T>& val) {
        os << "Optional(";
        if(val.has_value()) {
            os << val.value();
        } else {
            os << "{}";
        }
        return os << ")";
    }
};



template<typename T>
[[nodiscard]] inline auto make_optional(T&& forward_arg) {
    using decayedT = std::decay_t<T>;
    return Optional<decayedT>{std::move(std::make_unique<decayedT>(std::forward<T>(forward_arg)))};
}
/**
 * @brief Constructs an Optional object.
 * 
 * @tparam T 
 * @tparam TConstructs 
 * @param args 
 * @return Optional<T> 
 */
template<typename T, typename... TConstructs>
[[nodiscard]] inline Optional<T> make_optional(TConstructs&&... args) {
    return Optional<T>{std::move(std::make_unique<T>(std::forward<TConstructs>(args)...))};
}


#endif
