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

class FFT {
    
    cyclic_barrier barrier;
    int size;
    int thread_cnt;
    std::vector<std::complex<double>> a;
    std::vector<std::complex<double>> b;
    std::vector<std::complex<double>> c;
    std::vector<std::thread> threads;

public:
    FFT(std::vector<int> real_a, std::vector<int> real_b, std::vector<int>& answer) {
        size = calculate_normalized_size(real_a, real_b);
        a = std::vector<std::complex<double>>(real_a.begin(), real_a.end());
        b = std::vector<std::complex<double>>(real_b.begin(), real_b.end());
        a.resize(size);
        b.resize(size);
        c.resize(size);

        thread_cnt = std::thread::hardware_concurrency();
        if (thread_cnt == 0) {
            thread_cnt = 4;
        }

        thread_cnt = 1;
        barrier.set_size(thread_cnt);
        for (int i = 0; i < thread_cnt; i++) {
            threads.emplace_back(&FFT::worker_fn, this, i);
        }
        for (int i = 0; i < size; ++i) {
            answer[i] = floor(c[i].real() + 0.5) / size;
        }
        trim(answer);
    }

    ~FFT() {
        for (auto& thread : threads) {
            thread.join();
        }
    }

private:
    
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

    void bit_reverse_copy(std::vector<std::complex<double>>& array, int thread_id) {
        //int n = a.size();
        int p = log2(size);
        std::vector<std::complex<double>> b(size);
        for (int k = thread_id; k < size; k+=thread_cnt) {
            std::bitset<sizeof(k) * CHAR_BIT> x = k;
            std::bitset<sizeof(k) * CHAR_BIT> y;
            for (int i = 0; i < p; i++) {
                y[i] = x[p - i - 1];
            }
            b[y.to_ulong()] = array[k];
        }
        for (int k = thread_id; k < size; k+=thread_cnt) {
            array[k] = b[k];
        }
    }

    void fft(std::vector<std::complex<double>>& array, int thread_id, bool invert) {
        bit_reverse_copy(array, thread_id);
        //int n = data.size();
        double m, theta;
        std::complex<double> omega, omega_m;
        int block_thread_cnt = size / thread_cnt;
        if (block_thread_cnt > thread_cnt) {
            block_thread_cnt = thread_cnt;
        }
        if (thread_id < block_thread_cnt) {
            for (int s = 1; s <= log2(thread_cnt); s++) {
                m = pow(2, s);
                theta = 2 * M_PI / m * (invert ? -1 : 1);
                omega_m = std::complex<double>(cos(theta), sin(theta));
                for (int k = size / block_thread_cnt * thread_id; k < size / block_thread_cnt * (thread_id + 1); k+=m) {
                    omega = std::complex<double>(1, 0);
                    for (int j = 0; j < m/2; j++) {
                        std::complex<double> t = omega * array[k + j + (m / 2)];
                        std::complex<double> u = array[k + j];
                        array[k + j] = u + t;
                        array[k + j + (m / 2)] = u - t;
                        omega *= omega_m;
                    }
                }
            }
        }
        barrier.enter();
        for (int s = log2(thread_cnt) + 1; s <= log2(size); s++) {
            m = pow(2, s);
            theta = 2 * M_PI / m * (invert ? -1 : 1);
            omega_m = std::complex<double>(cos(theta), sin(theta));
            for (int k = thread_id; k < size; k += m) {
                omega = std::complex<double>(1, 0);
                for (int j = 0; j < m/2; j+=thread_cnt) {
                    std::complex<double> t = omega * array[k + j + (m / 2)];
                    std::complex<double> u = array[k + j];
                    array[k + j] = u + t;
                    array[k + j + (m / 2)] = u - t;
                    omega *= omega_m;
                }
            }
        }
        barrier.enter();
    }

    void worker_fn(int thread_id) {
        fft(a, thread_id, false);
        fft(b, thread_id, false);
        for (int j = thread_id; j < size; j+=thread_cnt) {
            c[j] = a[j] * b[j];
        }
        barrier.enter();
        fft(c, thread_id, true);
    }
};

