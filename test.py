import tanc 
import datetime
import time
dt1 = datetime.datetime(2018, 1, 1)
dt2 = datetime.datetime(2018, 1, 2)

t1 = time.time()
x = tanc.linspace(dt1, dt2, 100000)
t2 = time.time()

print(x.size)
print("Time taken: ", t2 - t1)

td = datetime.timedelta(seconds=1)

t1 = time.time()
x = tanc.arange(dt1, dt2, td)
t2 = time.time()

print(x.size)
print("Time taken: ", t2 - t1)

# testing out the datetime object

my_dt = tanc.DateTime(2018, 1, 1, 3, 4, 5, 6)
print(my_dt.year)