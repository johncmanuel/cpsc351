import string


def hex_to_bytes(hex_string: str):
    if len(hex_string) % 2 != 0:
        raise ValueError("Hex string must have an even length")

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

    for key in range(256):
        plaintext = "".join(chr(byte ^ key) for byte in ciphertext)

        # Check if the plaintext is readable
        if all(char in printable_chars for char in plaintext):
            res += f"Key: {chr(key)} (0x{key:02x}), Decrypted: {plaintext}\n"

    return res


def c3():
    string = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
    res = xor_cipher(string)
    print("Output:", res)


c3()
