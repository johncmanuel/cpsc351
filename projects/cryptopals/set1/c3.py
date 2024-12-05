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
    data = hex_to_bytes(text)

    # all possible ASCII printable characters
    printable_chars = set(string.printable)

    likely_plaintxt = []

    # there are 256 (2^8 = 256) different possible keys in a single byte
    possible_keys = 2**8

    for key in range(possible_keys):
        plaintext = "".join(chr(byte ^ key) for byte in data)
        # Check if the plaintext is readable
        if all(char in printable_chars for char in plaintext):
            score = sum(1 for char in plaintext if char.isalpha() or char.isspace())
            likely_plaintxt.append((score, key, plaintext))

    if likely_plaintxt:
        return max(likely_plaintxt, key=lambda x: x[0])[2]
    return "None found"


def c3():
    string = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
    res = xor_cipher(string)
    expected = "Cooking MC's like a pound of bacon"
    assert (
        res == expected
    ), f"Expected output does not match: {res}\nExpected: {expected}"
    print("Set 1 Challenge 3 was successful.\nOutput:", res, "\nExpected:", expected)


c3()
