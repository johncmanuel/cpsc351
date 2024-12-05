import sys
import os
from set1.c3 import hex_to_bytes, xor_cipher


def c4():
    with open("./set1/input/c4.txt") as f:
        lines = f.read().splitlines()
        for line in lines:
            xc = xor_cipher(line)
            if not xc:
                continue
            print(xc)


c4()
