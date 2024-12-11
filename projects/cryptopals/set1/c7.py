from Crypto.Cipher import AES


def decrypt_aes_ecb(ciphertext: bytes, key: str) -> str:
    cipher = AES.new(key, AES.MODE_ECB)
    plaintext = cipher.decrypt(ciphertext)
    return plaintext


def base64_to_bytes(base64_string):
    pass


def c7():
    key = "YELLOW SUBMARINE"
    with open("./set1/input/c7.txt") as f:
        data = f.read().splitlines()
        ciphertext = base64_to_bytes("".join(data))
        plaintext = decrypt_aes_ecb(ciphertext, key)
        print(plaintext)


if __name__ == "__main__":
    c7()
