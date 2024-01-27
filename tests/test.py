import sidereal 
from profiling import tic, toc
import numpy as np

dtime1 = sidereal.DateTime(2018, 1, 1)
dtime2 = sidereal.DateTime(2018, 1, 2)

def test_datetime_subtract():
    dt = dtime2 - dtime1
    assert dt.days == 1

def test_datetime_plus_timedelta():
    dt = sidereal.TimeDelta(days=1)
    dtime2 = dtime1 + dt
    assert dtime2.day == 2

def test_datetime_linspace():
    linspace = sidereal.linspace(dtime1, dtime2, 100_000)
    assert len(linspace) == 100_000

def test_datetime_arange():
    dt = sidereal.TimeDelta(seconds=1)
    arange = sidereal.arange(dtime1, dtime2, dt)
    assert len(arange) == 86_400

def test_datetime_itrf_to_j2000():
    dtime1.itrf_to_j2000()

def test_gast():
    dtime1.gast

def test_gmst():
    dtime1.gmst

def test_px_py():
    dtime1.px
    dtime1.py

def test_jds():
    dtime1.jd_utc
    dtime1.jd_tt
    dtime1.jd_ut1
    dtime1.jd_tai
    dtime1.mjd_utc
    dtime1.mjd_tt
    dtime1.mjd_ut1
    dtime1.mjd_tai

def test_tt_minus_tai():
    tt_minus_tai = (dtime1.mjd_tt - dtime1.mjd_tai) * 86400
    assert round(tt_minus_tai,6) == 32.184000 # accurate to the microsecond

def test_datetimearray_rotms():
    tic("init linspace")
    dtspace = sidereal.linspace(sidereal.DateTime(2018, 1, 1), sidereal.DateTime(2018, 1, 2), 100_000)
    assert toc(return_elapsed_seconds=True) < 0.5
    tic("itrf_to_j2000")
    mats = np.array(dtspace.itrf_to_j2000())
    assert toc(return_elapsed_seconds=True) < 0.3

    assert mats.shape == (len(dtspace), 3, 3)

    mats2 = np.zeros((len(dtspace), 3, 3))
    for i in range(mats.shape[0]):
        mats2[i,:,:] = dtspace[i].itrf_to_j2000()

    assert np.allclose(mats[0,:,:], mats2[0,:,:]), "Matrices are not equal"


if __name__ == "__main__":
    tic("init linspace")
    dtspace = sidereal.linspace(sidereal.DateTime(2018, 1, 1), sidereal.DateTime(2018, 1, 2), 100_000)
    toc()
    tic("itrf_to_j2000")
    mats = np.array(dtspace.j2000_to_mod())
    toc()

    print(mats)
    
