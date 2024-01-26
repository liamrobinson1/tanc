#include <iostream>
#include "Eigen/Eigen"
#include "math.hpp"

int main(void) {
    // Create a 2x2 matrix
    Eigen :: Matrix2d mat;
    mat << 1, 2,
           3, 4;
    // Compute its inverse
    std :: cout << "mat.inverse() = \n" << mat.inverse() << std :: endl;
    // Multiply the matrix by its inverse
    std :: cout << "mat * mat.inverse() = \n" << mat * mat.inverse() << std :: endl;

    // making a linspace with eigen
    Eigen::VectorXd v = linspace(0, 1, 10);
    std::cout << v << std::endl << std::endl;

    // testing identity
    Eigen::MatrixXd I = eye(3);
    std::cout << I << std::endl << std::endl;

    // testing zeros
    Eigen::MatrixXd Z = zeros(3);
    std::cout << Z << std::endl << std::endl;

    // testing ones
    Eigen::MatrixXd O = ones(3);
    std::cout << O << std::endl << std::endl;

    // testing zeros_like
    Eigen::MatrixXd ZL = zeros_like(mat);
    std::cout << ZL << std::endl << std::endl;

    // testing arange
    Eigen::VectorXd A = arange(0, 1, 0.1);
    std::cout << A << std::endl << std::endl;

    // testing dot
    Eigen::VectorXd D = dot(mat, mat);
    std::cout << D << std::endl << std::endl;



    return 0;
}