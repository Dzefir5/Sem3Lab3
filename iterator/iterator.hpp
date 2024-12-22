#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP
#pragma once


#include <cstddef>
#include <iterator>
#include <iostream>

template <typename T> class DynamicArray;


template <typename T> class Array_iterator
{
    private:
        T *p;

    public:
        friend class DynamicArray<T>;
        using difference_type = std::ptrdiff_t;
        using value_type = std::remove_cv_t<T>;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using iterator_category = std::random_access_iterator_tag;

        Array_iterator(T *p);
        Array_iterator(const Array_iterator &it);

        Array_iterator<T> operator+(const size_t &num) const noexcept;
        Array_iterator<T> operator-(const size_t &num) const noexcept;

        size_t operator-(const Array_iterator &other) const;
        bool operator!=(const Array_iterator &other) const noexcept;
        bool operator==(const Array_iterator &other) const noexcept;
        bool operator>(const Array_iterator &other) const noexcept;
        bool operator<(const Array_iterator &other) const noexcept;

        T &operator*() const noexcept;

        Array_iterator &operator++() noexcept;
        Array_iterator &operator--() noexcept;
};

template <typename T> Array_iterator<T>::Array_iterator(T *p) : p(p)
{
}

template <typename T> Array_iterator<T>::Array_iterator(const Array_iterator &it)
{
    p = it.p;
}

template <typename T> Array_iterator<T> Array_iterator<T>::operator+(const size_t &num) const noexcept
{
    return Array_iterator<T>(p + num);
}

template <typename T> Array_iterator<T> Array_iterator<T>::operator-(const size_t &num) const noexcept
{
    return Array_iterator<T>(p - num);
}

template <typename T> size_t Array_iterator<T>::operator-(const Array_iterator &other) const
{
    if (*this < other)
        throw std::invalid_argument("Cannot subtract iterators from different vectors");
    return p - other.p;
}

template <typename T> bool Array_iterator<T>::operator!=(const Array_iterator &other) const noexcept
{
    return p != other.p;
}

template <typename T> bool Array_iterator<T>::operator==(const Array_iterator &other) const noexcept
{
    return p == other.p;
}

template <typename T> bool Array_iterator<T>::operator>(const Array_iterator &other) const noexcept
{
    return p > other.p;
}


template <typename T> bool Array_iterator<T>::operator<(const Array_iterator &other) const noexcept
{
    return p < other.p;
}


template <typename T> T &Array_iterator<T>::operator*() const noexcept
{
    return *p;
}

template <typename T> Array_iterator<T> &Array_iterator<T>::operator++() noexcept
{
    ++p;
    return *this;
}

template <typename T> Array_iterator<T> &Array_iterator<T>::operator--() noexcept
{
    --p;
    return *this;
}

#endif // VECTOR_ITERATOR_HPP
