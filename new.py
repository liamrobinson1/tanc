import sidereal

dtime1 = sidereal.DateTime(2018, 1, 1)
dtime2 = sidereal.DateTime(2018, 1, 2)

dtimes = sidereal.linspace(dtime1, dtime2, int(1e6))

print(sidereal.seconds(10))

print(dtime2 - dtime1)

print(dir(dtime1))

gast = dtimes.gast()
