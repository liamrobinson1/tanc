#include "time.hpp"
#include "profile.hpp"
#include <iostream>
using namespace std;

int main() {
    std::cout.precision(20);
    DateTime dt_vallado = DateTime(2004, 4, 6, 7, 51, 28, 386009000);
    std::cout << "Full DateTime: " << dt_vallado << std::endl;


    // to sidereal time
    std::cout << "Julian Date UTC: " << dt_vallado.jd_utc << std::endl;
    std::cout << "Julian Date TAI: " << dt_vallado.jd_tai << std::endl;
    std::cout << "Julian Date UT1: " << dt_vallado.jd_ut1 << std::endl;
    std::cout << "GMST: " << dt_vallado.gmst << std::endl;

    // testing timedelta
    TimeDelta tdelta(0, 0, 0, 0, 0, 1.2);
    std::cout << tdelta << std::endl;

    // testing datetime + timedelta
    DateTime dt3 = dt_vallado + tdelta;
    std::cout << dt3 << std::endl;

    // time how many seconds it takes to initialize 1e6 datetimes with tic/tic
    int n = 1e5;
    DateTime dt4 = DateTime(2023, 3, 15, 14, 30, 45.123456789);
    std::cout << "Timing how long it takes to initialize " << n << " datetimes with tic/toc..." << std::endl;
    tic();
    std::vector<DateTime> date_vec = datetime_linspace(dt_vallado, dt4, n);
    toc();

    // testing jd to datetime
    std::cout << "Starting DateTime: " << dt4 << std::endl;
    DateTime dt5 = jd_to_datetime(dt4.jd_utc);
    std::cout << "Full DateTime: " << dt5 << std::endl;

    // datetime arange
    std::vector<DateTime> date_vec2 = datetime_arange(dt_vallado, dt4, TimeDelta(1, 0, 0, 0, 0, 0));
    for (int i = 0; i < date_vec2.size(); i++) {
        std::cout << date_vec2[i] << std::endl;
    }

    // testing py px
    std::cout << "py: " << dt_vallado.py << std::endl;
    std::cout << "px: " << dt_vallado.px << std::endl;

    // testing ut1-utc
    std::cout << "UT1-UTC: " << dt_vallado.ut1_minus_utc << std::endl;
    // testing tai-utc
    std::cout << "TAI-UTC: " << dt_vallado.tai_minus_utc << std::endl;

    // testing itrf to j2000
    std::cout << "T: " << dt_vallado.T << std::endl;
    std::cout << "ITRF to GTOD" << std::endl << dt_vallado.gtod_to_itrf().transpose() << std::endl;   
    std::cout << "GTOD to TEME" << std::endl << dt_vallado.teme_to_gtod().transpose() << std::endl;   
    std::cout << "TEME to TOD" << std::endl << dt_vallado.tod_to_teme().transpose() << std::endl;   
    std::cout << "TOD to MOD: " << std::endl << dt_vallado.mod_to_tod().transpose() << std::endl;
    std::cout << "MOD to J2000: " << std::endl << dt_vallado.j2000_to_mod().transpose() << std::endl;
    std::cout << "ITRF to J2000: " << std::endl << dt_vallado.itrf_to_j2000() << std::endl;

    return 0;
}