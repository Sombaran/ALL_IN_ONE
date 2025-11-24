import time
ts = time.time()
ts = str(int(time.time()))
f = open("datefile", "w")
f.write(ts)
f.close()
