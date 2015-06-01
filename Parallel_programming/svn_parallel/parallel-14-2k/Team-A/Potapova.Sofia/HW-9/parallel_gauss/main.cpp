#include <iostream>
#include <vector>

#include "parallel_gauss.h"

int main() {
    unsigned long n;
    std::cin >> n;
    std::vector<std::vector<double>> matrix(n);
    std::vector<double> result(n);
    for (int i = 0; i < n; i++) {
        matrix[i] = std::vector<double> (n+1);
        for (int j = 0; j < n; j++) {
            std::cin >> matrix[i][j];
        }
    }
    for (int j = 0; j < n; j++) {
        std::cin >> matrix[j][n];
    }

    if(solve_system_of_linear_equations(matrix, result)) {
        for (int i = 0; i < n; i++) {
            printf("%.9lf ", result[i]);
        }
        std::cout << std::endl;
    } else {
        std::cout << "no solution" << std::endl;
    }
    return 0;
}