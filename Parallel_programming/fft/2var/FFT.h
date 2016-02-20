#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <thread>
#include <bitset>
#include <climits>
#include "cyclic_barrier.h"

void trim(std::vector<int>& v) {
    int i = v.size() - 1;
    while (i > 0 && !v[i]) {
        --i;
    }
    v.resize(i + 1);
}

int calculate_normalized_size(std::vector<int> a, std::vector<int> b) {
    int size = 1;
    int max_size = std::max(a.size(), b.size());
    while (size < max_size) {
        size *= 2;
    }
    size *= 2;
    return size;
}

cyclic_barrier barrier;

void bit_reverse_copy(std::vector<std::complex<double>>& a) {
    int n = a.size();
    int p = log2(n);
    std::vector<std::complex<double>> b(n);
    for (int k = 0; k < n; k++) {
        std::bitset<sizeof(k) * CHAR_BIT> x = k;
        std::bitset<sizeof(k) * CHAR_BIT> y;
        for (int i = 0; i < p; i++) {
            y[i] = x[p - i - 1];
        }
        b[y.to_ulong()] = a[k];
    }
    for (int k = 0; k < n; k += 1) {
        a[k] = b[k];
    }
}

void fft(std::vector<std::complex<double>>& a, int thread_id, int thread_cnt, bool invert) {
    if (thread_id == 0) {
        bit_reverse_copy(a);
    }
    barrier.enter();
    int n = a.size();
    double m, theta;
    std::complex<double> omega, omega_m, omega_th;
    int block_thread_cnt = n / thread_cnt;
    if (block_thread_cnt > thread_cnt) {
        block_thread_cnt = thread_cnt;
    }

    if (thread_id < block_thread_cnt) {
        for (int s = 1; s <= log2(thread_cnt); s++) {
            m = pow(2, s);
            theta = 2 * M_PI / m * (invert ? -1 : 1);
            omega_m = std::complex<double>(cos(theta), sin(theta));
            for (int k = n / block_thread_cnt * thread_id; k < n / block_thread_cnt * (thread_id + 1); k+=m) {
                omega = std::complex<double>(1, 0);
                for (int j = 0; j < m/2; j++) {
                    std::complex<double> t = omega * a[k + j + (m / 2)];
                    std::complex<double> u = a[k + j];
                    a[k + j] = u + t;
                    a[k + j + (m / 2)] = u - t;
                    omega *= omega_m;

                }
            }
        }
    }
    barrier.enter();
    for (int s = log2(thread_cnt) + 1; s <= log2(n); s++) {
        m = pow(2, s);
        theta = 2 * M_PI / m * (invert ? -1 : 1);
        omega_m = std::complex<double>(cos(theta), sin(theta));
        for (int k = thread_id; k < n; k += m) {
            omega_th = std::complex<double> (pow(omega_m, thread_id));
            for (int j = 0; j < m/2; j+=thread_cnt) {
                std::complex<double> t = omega_th * a[k + j + (m / 2)];
                std::complex<double> u = a[k + j];
                a[k + j] = u + t;
                a[k + j + (m / 2)] = u - t;
                omega_th *= pow(omega_m, thread_cnt);
            }
        }
    }
    barrier.enter();
}

std::vector<int> multiply(std::vector<int> real_a, std::vector<int> real_b) {
    int size = calculate_normalized_size(real_a, real_b);
    std::vector<std::complex<double>> a(real_a.begin(), real_a.end());
    std::vector<std::complex<double>> b(real_b.begin(), real_b.end());
    a.resize(size);
    b.resize(size);
    std::vector<std::complex<double>> c(size);

    int thread_cnt = 4;
    barrier.set_size(thread_cnt);
    std::vector<std::thread> threads;
    for (int i = 0; i < thread_cnt; i++) {
        threads.emplace_back([&a, &b, &c, i, size, thread_cnt] () {
            fft(a, i, thread_cnt, false);
            fft(b, i, thread_cnt, false);
            for (int j = i; j < size; j+=thread_cnt) {
                c[j] = a[j] * b[j];
            }
            barrier.enter();
            fft(c, i, thread_cnt, true);
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    std::vector<int> answer(size);
    for (int i = 0; i < size; ++i) {
        answer[i] = floor(c[i].real() + 0.5) / size;
    }
    trim(answer);
    return answer;
}