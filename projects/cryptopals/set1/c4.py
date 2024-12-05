import sys
import os

# https://stackoverflow.com/questions/16981921/relative-imports-in-python-3
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(SCRIPT_DIR))

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
