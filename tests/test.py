import tanc 
import time
from profiling import tic, toc

def test_datetime_init():
    dtime1 = tanc.DateTime(2018, 1, 1)

def test_datetime_subtract():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dtime2 = tanc.DateTime(2018, 1, 2)
    dt = dtime2 - dtime1
    assert dt.days == 1

def test_datetime_plus_timedelta():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dt = tanc.TimeDelta(days=1)
    dtime2 = dtime1 + dt
    assert dtime2.day == 2

def test_datetime_linspace():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dtime2 = tanc.DateTime(2018, 1, 2)
    linspace = tanc.linspace(dtime1, dtime2, 100_000)
    assert len(linspace) == 100_000

def test_datetime_arange():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dtime2 = tanc.DateTime(2018, 1, 2)
    dt = tanc.TimeDelta(seconds=1)
    arange = tanc.arange(dtime1, dtime2, dt)
    assert len(arange) == 86_400

def test_datetime_itrf_to_j2000():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dtime1.itrf_to_j2000()

def test_gast():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dtime1.gast

def test_gmst():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dtime1.gmst

def test_py():
    dtime1 = tanc.DateTime(2018, 1, 1)
    dtime1.py