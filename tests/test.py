import sidereal
import numpy as np

import time
from typing import Union

tstart = None
timer_name = None


def tic(msg: Union[str, None] = None):
    """Starts a timer, ended by ``toc()``

    :param msg: Message to come before the elapsed time statement, defaults to None
    :type msg: str, optional
    :raises Exception: If there is already an active ``tic``
    """
    global tstart, timer_name
    timer_name = msg
    if tstart is not None:
        raise Exception("tic() called with another tic() active!")
    tstart = time.perf_counter()


def toc(return_elapsed_seconds: bool = False) -> Union[None, float]:
    """Ends the timer began by ``tic()``, returns the elapsed time

    :param return_elapsed_seconds: Whether to return the time as well as printing it, defaults to False
    :type return_elapsed_seconds: bool, optional
    :raises Exception: If there's no active ``tic()`` timer running
    :return: The elapsed time in seconds, if requested
    :rtype: Union[None, float]
    """
    global tstart, timer_name
    if tstart is None:
        raise Exception("toc() called without an active tic()!")
    tend = time.perf_counter()
    telapsed = tend - tstart
    if not return_elapsed_seconds:
        print(
            f"{timer_name if timer_name is not None else 'Elapsed time'}: {telapsed:.2e} seconds"
        )
    tstart = None
    timer_name = None
    if return_elapsed_seconds:
        return telapsed


dtime1 = sidereal.DateTime(2018, 1, 1)
dtime2 = sidereal.DateTime(2018, 1, 2)


def test_datetime_subtract():
    dt = dtime2 - dtime1
    assert dt.days == 1


def test_datetime_plus_timedelta():
    dtime2 = dtime1 + sidereal.days(1)
    assert dtime2.day == 2


def test_datetime_linspace():
    linspace = sidereal.linspace(dtime1, dtime2, 100_000)
    assert len(linspace) == 100_000


def test_datetime_arange():
    dt = sidereal.seconds(1)
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
    assert round(tt_minus_tai, 6) == 32.184000  # accurate to the microsecond


def test_datetimearray_rotms():
    # tic("init linspace")
    dtspace = sidereal.linspace(
        sidereal.DateTime(2018, 1, 1), sidereal.DateTime(2018, 1, 2), 100_000
    )
    # assert toc(return_elapsed_seconds=True) < 0.5
    # tic("itrf_to_j2000")
    mats = np.array(dtspace.itrf_to_j2000())
    # assert toc(return_elapsed_seconds=True) < 0.3

    assert mats.shape == (len(dtspace), 3, 3)

    mats2 = np.zeros((len(dtspace), 3, 3))
    for i in range(mats.shape[0]):
        mats2[i, :, :] = dtspace[i].itrf_to_j2000()

    assert np.allclose(mats[0, :, :], mats2[0, :, :]), "Matrices are not equal"


def test_jd_to_datetime():
    now = sidereal.now()
    jd = now.jd_utc
    dt = sidereal.jd_to_datetime(jd)

    assert abs((now - dt).nanoseconds) < 1e5


def test_timedelta_total_seconds():
    dt = sidereal.days(1)
    assert dt.total_seconds() == 86400


if __name__ == "__main__":
    tic("init linspace")
    dtspace = sidereal.linspace(
        sidereal.DateTime(2018, 1, 1), sidereal.DateTime(2018, 1, 2), 100_000
    )
    toc()
    tic("itrf_to_j2000")
    mats = np.array(dtspace.j2000_to_mod())
    toc()

    print(mats)
