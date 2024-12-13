from c7 import base64_to_bytes


def c10():
    with open("c10.txt") as f:
        data = f.read().splitlines()
        ciphertext = base64_to_bytes("".join(data))
        if ciphertext:
            print(ciphertext)
