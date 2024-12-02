import string


def hex_to_bytes(hex_string: str):
    assert len(hex_string) % 2 == 0, "Hex string must have an even length"

    res = []

    for i in range(0, len(hex_string), 2):
        byte = int(hex_string[i : i + 2], 16)
        res.append(byte)

    return bytes(res)


# Use brute force approach to decrypt the ciphertext by
# trying all possible keys
def xor_cipher(text: str) -> str:
    ciphertext = hex_to_bytes(text)

    # all possible ASCII printable characters
    printable_chars = set(string.printable)

    res = ""

    # there are 256 different possible keys in a single byte
    possible_keys = 2**8

    for key in range(possible_keys):
        plaintext = "".join(chr(byte ^ key) for byte in ciphertext)

        # Check if the plaintext is readable
        if all(char in printable_chars for char in plaintext):
            res += f"Key: {chr(key)} (0x{key:02x}), Decrypted: {plaintext}\n"

    return res


def c4():
    with open("./set1/input/c4.txt") as f:
        lines = f.read().splitlines()
        for line in lines:
            xc = xor_cipher(line)
            if not xc:
                continue
            print(xc)


c4()
