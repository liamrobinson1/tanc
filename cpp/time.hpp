#include "Eigen/Eigen"
#include <iostream>
#include <cmath>
#include "math.hpp"
#include "iau1980.hpp"

double RAD_TO_ARCSECOND = 180.0 * 3600.0 / M_PI;

class TimeDelta {
    public:
        int years;
        int months;
        int days;
        int hours;
        int minutes;
        int seconds;
        int nanoseconds;
        // constructor if nanoseconds are given
        TimeDelta(int years, int months, int days, int hours, int minutes, int seconds, int nanoseconds)
            : years(years), months(months), days(days), hours(hours), minutes(minutes), seconds(seconds), nanoseconds(nanoseconds) {}
        
        // constructor if seconds are given as double
        TimeDelta(int years, int months, int days, int hours, int minutes, double seconds)
            : years(years), months(months), days(days), hours(hours), minutes(minutes), seconds(static_cast<int>(seconds)), nanoseconds(static_cast<int>((seconds - static_cast<int>(seconds)) * 1e9)) {}

        // print when called with std::cout
        friend std::ostream& operator<<(std::ostream& os, const TimeDelta tdelta) {
            os << "\u0394" << tdelta.years << "-" << tdelta.months << "-" << tdelta.days << " " << tdelta.hours << ":" << tdelta.minutes << ":" << tdelta.seconds << "." << tdelta.nanoseconds;
            return os;
        }   

        double total_seconds() {
            return years * 365.25 * 24 * 60 * 60 + months * 31 * 24 * 60 * 60 + days * 24 * 60 * 60 + hours * 60 * 60 + minutes * 60 + seconds + nanoseconds / 1e9; // Note: assumes 31 days in a month
        }
        
};

class DateTime {
    private:
        void setup() {
            while (nanosecond >= 1e9) {
                nanosecond -= 1e9;
                second += 1;
            }
            while (second >= 60) {
                second -= 60;
                minute += 1;
            }
            while (minute >= 60) {
                minute -= 60;
                hour += 1;
            }
            while (hour >= 24) {
                hour -= 24;
                day += 1;
            }
            jd_utc = julian_date();
            mjd_utc = modified_julian_date();

            tai_minus_utc = compute_tai_minus_utc();
            ut1_minus_utc = compute_utc_minus_ut1();

            jd_ut1 = jd_utc + ut1_minus_utc / 86400.0;
            mjd_ut1 = mjd_utc + ut1_minus_utc / 86400.0;
            jd_tai = jd_utc + tai_minus_utc / 86400.0;
            mjd_tai = mjd_utc + tai_minus_utc / 86400.0;
            jd_tt = jd_tai + TT_MINUS_TAI / 86400.0;
            mjd_tt = mjd_tai + TT_MINUS_TAI / 86400.0;

            T = julian_centuries();
            epsilon_bar = mean_obliquity_of_ecliptic();
            gmst = greenwich_mean_sidereal_time();
            std::vector<double> dpsi_deps = delta_psi_delta_epsilon();
            delta_psi = dpsi_deps[0];
            delta_eps = dpsi_deps[1];
            gast = date_to_gast();

            std::vector<double> eop = eop_py_px();
            px = eop[0];
            py = eop[1];
        }
    public:
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        int nanosecond;
        double jd_utc;
        double jd_ut1;
        double jd_tai;
        double jd_tt;
        double mjd_utc;
        double mjd_ut1;
        double mjd_tai;
        double mjd_tt;
        double T;
        double gmst;
        double gast;
        double delta_psi;
        double delta_eps;
        double epsilon_bar;
        double px;
        double py;
        double tai_minus_utc;
        double ut1_minus_utc;
        Eigen::MatrixXd P;
        Eigen::MatrixXd Theta;
        Eigen::MatrixXd N;
        Eigen::MatrixXd Pi;
        // constructor if nanoseconds are given
        DateTime(int year, int month, int day, int hour, int minute, int second, int nanosecond)
            : year(year), month(month), day(day), hour(hour), minute(minute), second(second), nanosecond(nanosecond) {
                // set its julian date
                setup();
            }
        
        // constructor if seconds are given as double
        DateTime(int year, int month, int day, int hour, int minute, double second)
            : year(year), month(month), day(day), hour(hour), minute(minute), second(static_cast<int>(second)), nanosecond(static_cast<int>((second - static_cast<int>(second)) * 1e9)) {
                setup();
            }
        
        // constructor if *PyObject args are given as tuple of int
        DateTime(PyObject* args) {
            // Parse the input objects as two datetime objects and an integer
            // number of arguments passed:
            int n = PyTuple_Size(args);
            if (n == 3) {
                if (!PyArg_ParseTuple(args, "iii", &year, &month, &day)) {
                    std::cout << "Error parsing datetime" << std::endl;
                }
                hour = 0;
                minute = 0;
                second = 0;
                nanosecond = 0;
            }
            else if (n == 4) {
                if (!PyArg_ParseTuple(args, "iiii", &year, &month, &day, &hour)) {
                    std::cout << "Error parsing datetime" << std::endl;
                }
                minute = 0;
                second = 0;
                nanosecond = 0;
            }
            else if (n == 5) {
                if (!PyArg_ParseTuple(args, "iiiii", &year, &month, &day, &hour, &minute)) {
                    std::cout << "Error parsing datetime" << std::endl;
                }
                second = 0;
                nanosecond = 0;
            }
            else if (n == 6) {
                if (!PyArg_ParseTuple(args, "iiiiii", &year, &month, &day, &hour, &minute, &second)) {
                    std::cout << "Error parsing datetime" << std::endl;
                }
                nanosecond = 0;
            }
            else if (n == 7) {
                if (!PyArg_ParseTuple(args, "iiiiiii", &year, &month, &day, &hour, &minute, &second, &nanosecond)) {
                    std::cout << "Error parsing datetime" << std::endl;
                }
            }
            else {
                std::cout << "Error parsing datetime" << std::endl;
            }
            setup();
        }

    // print when called with std::cout
    friend std::ostream& operator<<(std::ostream& os, const DateTime dtime) {
        os << dtime.year << "-" << dtime.month << "-" << dtime.day << " " << dtime.hour << ":" << dtime.minute << ":" << dtime.second << "." << dtime.nanosecond;
        return os;
    } 

    double modified_julian_date() {
        return jd_utc - 2400000.5;
    }

    double julian_date() {
        int y = year;
        int m = month;
        if (month <= 2) {
            y = year - 1;
            m = month + 12;
        }
        int B = floor(y / 400) - floor(y / 100);
        return floor(365.25 * y) + floor(30.6001 * (m + 1)) + B + 1720996.5 + day + (hour + (minute + (second + (nanosecond / 1e9)) / 60) / 60) / 24;
    }

    double julian_centuries() {
        return (jd_tt - 2451545.0) / 36525.0;
    }

    double greenwich_mean_sidereal_time() {
        double T1 = (jd_ut1 - 2451545.0) / 36525.0;  // Time since Jan 1 2000, 12h UT to now
        double sid_seconds = fmod(67310.54841 
        + (876600.0 * 3600.0 + 8640184.812866) * T1 
        + 0.093104 * pow(T1, 2.0) 
        - 0.0000062 * pow(T1, 3.0), 86400);
        return sid_seconds / 86400.0 * 2.0 * M_PI;
    }

    double asc_node_moon() {
        double Omega = 450160.398036 + T * (-6962890.5431 + 7.4722 * T + 0.007702 * pow(T, 2) - 0.00005939 * pow(T,3)) / RAD_TO_ARCSECOND;
        return Omega;
    }

    double mean_obliquity_of_ecliptic() {
        double epsilon = dms_to_rad(23.43929111, 0, 0) \
            - dms_to_rad(0, 0, 46.8150) * T \
            - dms_to_rad(0, 0, 0.00059) * pow(T,2) \
            + dms_to_rad(0, 0, 0.001813) * pow(T,3);
        return epsilon;
    }

    std::vector<double> delta_psi_delta_epsilon() {
        double days_since_j2k = 36525.0 * T;
        double l = 2 * M_PI * (0.374897 + 0.03629164709 * days_since_j2k);
        double lprime = 2 * M_PI * (0.993126 + 0.00273777850 * days_since_j2k);
        double F = (335779.526232 + T * (1739527262.8478 - 12.7512 * T - 0.001037 * pow(T, 2) + 0.00000417 * pow(T,3))) / RAD_TO_ARCSECOND;
        double D = (1072260.70369 + T * (1602961601.2090 - 6.3706 * T + 0.00693 * pow(T, 2) - 0.00003169 * pow(T, 3))) / RAD_TO_ARCSECOND;
        double Omega = (450160.398036 + T * (-6962890.5431 + 7.4722 * T + 0.007702 * pow(T, 2) - 0.00005939 * pow(T, 3))) / RAD_TO_ARCSECOND;

        Eigen::VectorXd deltaPsi_i = deg_to_rad((P6 + T * P7) / 3600e4);
        // Computes the change in the equinox
        Eigen::VectorXd deltaepsilon_i = deg_to_rad((P8 + T * P9) / 3600e4);
        // Computes the change in the ecliptic

        Eigen::VectorXd phi_i = PL * l + PLPRIME * lprime + PF * F + PD * D + POMEGA * Omega;
        // Series form of phi

        Eigen::VectorXd cos_phi = phi_i.array().cos();
        Eigen::VectorXd sin_phi = phi_i.array().sin();

        double delta_psi = (deltaPsi_i.array() * sin_phi.array()).sum();
        double delta_eps = (deltaepsilon_i.array() * cos_phi.array()).sum();

        return std::vector<double>({delta_psi, delta_eps});
    }

    double compute_tai_minus_utc() {
        // find the first index where jd is greater than leap_jds
        int ind = 0;
        while (jd_utc > LEAP_JDS[ind]) {
            ind++;
            if(ind == LEAP_JDS.size()) {
                break;
            }
        }
        ind--;
        return TAI_MINUS_UTC[ind];
    }

    double compute_utc_minus_ut1() {
        int ind0 = floor(mjd_utc) - vEOPMJD[0];
        double mjd_frac = mjd_utc - floor(mjd_utc);
        return (1-mjd_frac) * vUTC_MINUS_UT1[ind0] + mjd_frac * vUTC_MINUS_UT1[ind0+1];
    }

    Eigen::Matrix3d j2000_to_mod() {
        double zeta = dms_to_rad(0, 0, 2306.2181 * T + 0.30188 * pow(T, 2) + 0.017998 * pow(T, 3));
        double theta = dms_to_rad(0, 0, 2004.3109 * T - 0.42665 * pow(T, 2) - 0.041833 * pow(T, 3));
        double z = dms_to_rad(0, 0, 2306.2181 * T + 1.09468 * pow(T, 2) + 0.018203 * pow(T, 3));
        Eigen::Matrix3d P = r3(-z) * r2(theta) * r3(-zeta);
        return P;
    }

    Eigen::Matrix3d mod_to_tod() {
        Eigen::Matrix3d N = r1(-epsilon_bar - delta_eps) * r3(-delta_psi) * r1(epsilon_bar);
        return N;
    }

    Eigen::Matrix3d tod_to_teme() {
        double dpsi_cos_eps = delta_psi * cos(epsilon_bar);
        return r3(dpsi_cos_eps);
    }

    Eigen::Matrix3d teme_to_gtod() {
        return r3(gmst);
    }

    Eigen::Matrix3d gtod_to_itrf() {
        double x_p = dms_to_rad(0, 0, px);
        double y_p = dms_to_rad(0, 0, py);
        Eigen::Matrix3d Pi = r2(-x_p) * r1(-y_p);
        return Pi;
    }

    Eigen::Matrix3d itrf_to_j2000() {
        return (gtod_to_itrf() * teme_to_gtod() * tod_to_teme() * mod_to_tod() * j2000_to_mod()).transpose();
    }

    double date_to_gast() {
        double omega_moon = asc_node_moon();
        double gast = gmst + delta_psi * cos(epsilon_bar) + dms_to_rad(0, 0, 0.00264) * sin(omega_moon) + dms_to_rad(0, 0, 0.000063) * sin(2 * omega_moon);
        return gast;
    }

    std::vector<double> eop_py_px() {
        int ind0 = floor(mjd_utc) - vEOPMJD[0];
        double mjd_frac = mjd_utc - floor(mjd_utc);
        double pxi = (1-mjd_frac) * vEOPx[ind0] + mjd_frac * vEOPx[ind0+1];
        double pyi = (1-mjd_frac) * vEOPy[ind0] + mjd_frac * vEOPy[ind0+1];
        return std::vector<double>({pxi, pyi});
    }


    DateTime operator+(const TimeDelta& tdelta) {
        int y = year + tdelta.years;
        int m = month + tdelta.months;
        int d = day + tdelta.days;
        int h = hour + tdelta.hours;
        int min = minute + tdelta.minutes;
        int s = second + tdelta.seconds;
        int ns = nanosecond + tdelta.nanoseconds;
        return DateTime(y, m, d, h, min, s, ns);
    }
};

DateTime jd_to_datetime(double jd) {
    int a = floor(jd + 0.5);
    int b = floor((a - 1867216.25) / 36524.25);
    int c;
    if (a < 2299161) {
        c = a + 1524;
    } else {
        c = a + b - floor(b / 4) + 1525;
    }
    int d = floor((c - 122.1) / 365.25);
    int e = floor(365.25 * d);
    int f = floor((c - e) / 30.6001);
    int D = (c - e - floor(30.6001 * f) + (jd + 0.5 - a));
    int M = (f - 1 - 12 * floor(f / 14));
    int Y = (d - 4715 - floor((7 + M) / 10));
    double dy_frac = fmod(jd - floor(jd) + 0.5, 1);
    double add_sec = dy_frac * 86400.0;
    int hr = floor(add_sec / 3600);
    add_sec -= hr * 3600;
    int min = floor(add_sec / 60);
    add_sec -= min * 60;
    return DateTime(Y, M, D, hr, min, add_sec);
}

// datetime linspace returning as vec of datetimes
std::vector<DateTime> datetime_linspace(DateTime start, DateTime end, int num) {
    std::vector<DateTime> vec;
    double jd_start = start.jd_utc;
    double jd_end = end.jd_utc;
    double jd_step = (jd_end - jd_start) / (num - 1);
    for (int i = 0; i < num; i++) {
        vec.push_back(jd_to_datetime(jd_start + i * jd_step));
    }
    return vec;
}

std::vector<DateTime> datetime_arange(DateTime start, DateTime end, TimeDelta step) {
    std::vector<DateTime> vec;
    double jd_start = start.jd_utc;
    double jd_end = end.jd_utc;
    double jd_step = step.total_seconds() / 86400.0;
    int n_steps = (jd_end - jd_start) / jd_step;
    for (int i = 0; i < n_steps; i++) {
        vec.push_back(jd_to_datetime(jd_start + i * jd_step));
    }
    return vec;
}