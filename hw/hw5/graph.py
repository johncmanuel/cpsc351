import random

import matplotlib.pyplot as plt
import numpy as np

asizeBytes = 1024
psizeBytes = 16 * 1024
lst = [0] * psizeBytes

for i in range(300):
    random.seed(i)
    for j in range(psizeBytes):
        virtualAddr = int(asizeBytes * random.random())
        if virtualAddr < j:
            lst[j] += 1

limits = [u / 300 for u in lst]

fig = plt.figure()
x = np.linspace(1, psizeBytes, psizeBytes)
plt.plot(x, limits, color="green")
plt.ylim(0, 1)
plt.margins(0)
plt.title("Fraction of Valid Addresses v. Limit Values")
plt.xlabel("Limit Values")
plt.ylabel("Fraction of Valid Addresses")
plt.savefig("valid_fraction.png", dpi=500)
plt.show()
