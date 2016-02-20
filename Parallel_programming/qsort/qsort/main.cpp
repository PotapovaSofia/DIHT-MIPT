#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#define it std::vector<int>::iterator

using namespace std;


void q_sort(const it& begin, const it& end) {
    if (end - begin < 1) {
        //std::sort(begin, end);
        return;
    }

    int pivot = *(begin + (end - begin)/2);
    cout << "pivot: " << pivot << endl;
    /*int i = begin, j = end;
    do {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        if (i <= j) {
            if (i < j) std::swap(array[i], array[j]);
            i++;
            i--;
        }
    } while(i <= j);
     */
    auto pivot_it = std::partition(begin, end, [pivot](int val) {return val < pivot;});
    q_sort(begin, pivot_it - 1);
    q_sort(pivot_it + 1, end);
    for (auto i = begin; i != end; i++) {
        cout << *i << " ";
    }
    cout << endl;
}

void quicksort(it first, it last) {
    //typedef typename std::iterator_traits<it>::value_type value_type;
    if (last - first < 2)
        return;
    it middle = first + (last - first)/2;
    //std::iter_swap(middle, last-1);
    middle = std::partition(first, last-1,
                            [middle] (const int& x) { return x < *(middle); });
    //std::iter_swap(middle, last-1);
    quicksort(first, middle);
    quicksort(middle+1, last);
}

int main() {
    std::vector<int> data;
    for (int i = 0; i < 10; i++) {
        std::random_device generator;
        std::uniform_int_distribution<int> distribution(1, 100);
        data.push_back(distribution(generator));
    }
    for (auto i: data) {
        cout << i << " ";
    }
    cout << endl;
    quicksort(data.begin(), data.end());
    for (auto i: data) {
        cout << i << " ";
    }
    if(std::is_sorted(data.begin(), data.end())) {
        std::cout << "Correct" << std::endl;
    } else {
        std::cout << "Incorrect" << std::endl;
    }
    return 0;
}