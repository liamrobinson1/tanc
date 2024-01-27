import sidereal

dtime1 = sidereal.DateTime(2018, 1, 1)
dtime2 = sidereal.DateTime(2018, 1, 2)

now = sidereal.now()
jd = now.jd_utc

print(now, jd)