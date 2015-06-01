/* 
  g++ -std=c++11 -pthread `Magick++-config --cxxflags --cppflags` -O2 -o demo main.cpp   `Magick++-config --ldflags --libs`
*/


#include <iostream>
#include "image_wrapper.h"
#include "parallel_kd_tree.h"

using namespace std; 
using namespace Magick;

int main(int argc,char **argv) {
    InitializeMagick(*argv);
    int n;
    vector<ImageWrapper> images;
    string file = "0.gif";
    for (int i = 0; i < 10; i++) {
        file[0] = '0' + i;
        images.emplace_back(file);
    }
    std::vector<Point> points;
    for (int i = 0; i < 10; i++) {
        points.emplace_back(images[i].get_coord(), &images[i]);
    }


    KD_Tree kd_tree(points, 1200);

    cout << "Enter number of picture: ";
    cin >> n;

    Point nearest = kd_tree.find_nearest_neighbor(points[n]);
    nearest.get_image().write_image("result.gif");
    cout << "Nearest for picture №" << n << " is \"result.gif\"" << endl;

    return 0;
}
