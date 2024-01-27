#ifdef _MSC_VER
    #define _USE_MATH_DEFINES // For MS Visual Studio
    #include <math.h>
#else
    #include <cmath>
#endif
#include "Eigen/Eigen"

Eigen::MatrixXd eye(int n) {
    return Eigen::MatrixXd::Identity(n, n);
}

Eigen::MatrixXd zeros(int n) {
    return Eigen::MatrixXd::Zero(n, n);
}

Eigen::MatrixXd ones(int n) {
    return Eigen::MatrixXd::Ones(n, n);
}

Eigen::MatrixXd zeros_like(Eigen::MatrixXd A) {
    return Eigen::MatrixXd::Zero(A.rows(), A.cols());
}

Eigen::VectorXd linspace(double start, double end, int num) {
    return Eigen::VectorXd::LinSpaced(num, start, end);
}

Eigen::VectorXd arange(double start, double end, double step) {
    int num = (end - start) / step;
    return Eigen::VectorXd::LinSpaced(num, start, end);
}

// dot product for two nxm matrices, returning an nx1 vector
Eigen::VectorXd dot(Eigen::MatrixXd A, Eigen::MatrixXd B) {
    return (A.array() * B.array()).rowwise().sum();
}

double dms_to_rad(double deg, double min, double sec) {
    return (deg + min / 60.0 + sec / 3600.0) * M_PI / 180.0;
}

double deg_to_rad(double deg) {
    return deg * M_PI / 180.0;
}

Eigen::VectorXd deg_to_rad(Eigen::VectorXd deg) {
    return deg * M_PI / 180.0;
}

double rad_to_deg(double rad) {
    return rad * 180.0 / M_PI;
}

Eigen::VectorXd rad_to_deg(Eigen::VectorXd rad) {
    return rad * 180.0 / M_PI;
}

Eigen::MatrixXd r1(double theta) {
    Eigen::MatrixXd R(3, 3);
    R << 1, 0, 0,
         0, cos(theta), sin(theta),
         0, -sin(theta), cos(theta);
    return R;
}

Eigen::MatrixXd r2(double theta) {
    Eigen::MatrixXd R(3, 3);
    R << cos(theta), 0, -sin(theta),
         0, 1, 0,
         sin(theta), 0, cos(theta);
    return R;
}

Eigen::MatrixXd r3(double theta) {
    Eigen::MatrixXd R(3, 3);
    R << cos(theta), sin(theta), 0,
         -sin(theta), cos(theta), 0,
         0, 0, 1;
    return R;
}