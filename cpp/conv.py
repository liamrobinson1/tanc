import numpy as np

data = np.loadtxt("data/gpsrapid.txt", comments="p")

import subprocess 
subprocess.run("pbcopy", text=True, input=data[:,5].tolist().__str__().replace("{", "").replace("}", ""))