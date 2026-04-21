// Implementation for ACMOJ 2208 - Resistive Network
// The OJ will include this file directly.

#pragma once

#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>

#if defined(__has_include)
#  if __has_include(fraction.hpp)
#    define HAS_OFFICIAL_FRACTION 1
#    include fraction.hpp
#  endif
#endif

#ifndef HAS_OFFICIAL_FRACTION
// Fallback lightweight fraction implementation for local checks only.
class fraction {
public:
    long long num, den;
    fraction(long long n = 0, long long d = 1) : num(n), den(d) { normalize(); }
    void normalize() {
        if (den < 0) { den = -den; num = -num; }
        long long a = num < 0 ? -num : num;
        long long b = den < 0 ? -den : den;
        while (b) { long long t = a % b; a = b; b = t; }
        long long g = a ? a : 1;
        num /= g; den /= g;
    }
    fraction operator+(const fraction& o) const { return fraction(num*o.den + o.num*den, den*o.den); }
    fraction operator-(const fraction& o) const { return fraction(num*o.den - o.num*den, den*o.den); }
    fraction operator*(const fraction& o) const { return fraction(num*o.num, den*o.den); }
    fraction operator/(const fraction& o) const { return fraction(num*o.den, den*o.num); }
    fraction& operator+=(const fraction& o) { *this = *this + o; return *this; }
    fraction& operator-=(const fraction& o) { *this = *this - o; return *this; }
    fraction& operator*=(const fraction& o) { *this = *this * o; return *this; }
    fraction& operator/=(const fraction& o) { *this = *this / o; return *this; }
    bool operator==(const fraction& o) const { return num == o.num && den == o.den; }
    bool operator!=(const fraction& o) const { return !(*this == o); }
};
#endif

// Minimal matrix stub (not used by tests but provided to match template expectations)
class matrix {
public:
    matrix() : r(0), c(0) {}
    matrix(size_t rows, size_t cols) : r(rows), c(cols), a(rows, std::vector<fraction>(cols)) {}
    size_t rows() const { return r; }
    size_t cols() const { return c; }
    std::vector<fraction>& operator[](size_t i) { return a[i]; }
    const std::vector<fraction>& operator[](size_t i) const { return a[i]; }
private:
    size_t r, c;
    std::vector<std::vector<fraction>> a;
};

// Resistive network using nodal analysis with exact fractions
class resistive_network {
public:
    resistive_network(int n_nodes,
                      const std::vector<int>& from,
                      const std::vector<int>& to,
                      const std::vector<fraction>& resistances)
        : n(n_nodes)
    {
        add_edges(from, to, resistances);
    }

    resistive_network(int n_nodes,
                      int m_edges,
                      const int* from,
                      const int* to,
                      const fraction* resistances)
        : n(n_nodes)
    {
        std::vector<int> vf(from, from + m_edges);
        std::vector<int> vt(to, to + m_edges);
        std::vector<fraction> vr(resistances, resistances + m_edges);
        add_edges(vf, vt, vr);
    }

    resistive_network(int n_nodes,
                      const std::vector<std::pair<int,int>>& es,
                      const std::vector<fraction>& resistances)
        : n(n_nodes)
    {
        std::vector<int> f(es.size()), t(es.size());
        for (size_t i = 0; i < es.size(); ++i) { f[i] = es[i].first; t[i] = es[i].second; }
        add_edges(f, t, resistances);
    }

    // Equivalent resistance between nodes a and b (1-based indices)
    fraction get_equivalent_resistance(int a, int b) const {
        if (a == b) return fraction(0);
        auto L = build_laplacian();
        int ground = b;
        std::vector<std::vector<fraction>> A;
        std::vector<fraction> rhs;
        reduce_with_ground(L, ground, A, rhs);
        if (a != ground) {
            int ai = map_index(a, ground);
            rhs[ai] += fraction(1);
        }
        auto x = solve(A, rhs);
        if (a == ground) return fraction(0);
        int ai = map_index(a, ground);
        return x[ai];
    }

    // Node voltage at k given injections I and u_n = 0
    fraction get_voltage(const std::vector<fraction>& I, int k) const {
        if ((int)I.size() != n) throw std::invalid_argument(I
