#include "src/time.hpp"
#include "src/profile.hpp"
#include <iostream>

using namespace std;

int main() {
    std::cout.precision(20);
    DateTime dt_vallado = DateTime(2004, 4, 6, 7, 51, 28, 386009000);
    std::cout << "Full DateTime: " << dt_vallado << std::endl;

    tic();
    DateTimeArray date_vec = datetime_linspace(dt_vallado, dt_vallado + TimeDelta(1, 0, 0, 0, 0, 0), 1e4);
    toc();
    
    Eigen::MatrixXd mat = date_vec[0].itrf_to_j2000();
    // std::cout << mat << std::endl;


    tic();
    std::vector<Eigen::Matrix3d> mat_vec = date_vec.itrf_to_j2000();
    toc();
    // std::cout << mat_vec[0] << std::endl;
}