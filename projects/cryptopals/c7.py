from Crypto.Cipher import AES
from c1 import BASE64_TABLE


def decrypt_aes_ecb(ciphertext: bytes, key: str) -> str:
    cipher = AES.new(key.encode("utf-8"), AES.MODE_ECB)
    plaintext = cipher.decrypt(ciphertext)
    return plaintext.decode("utf-8")


def base64_to_bytes(s: str) -> bytes:
    s = s.rstrip("=")

    bin_str = ""
    for ch in s:
        bin_str += format(BASE64_TABLE.index(ch), "06b")

    bytes_data = bytearray()
    for i in range(0, len(bin_str), 8):
        bytes_data.append(int(bin_str[i : i + 8], 2))

    return bytes(bytes_data)


def c7():
    key = "YELLOW SUBMARINE"
    with open("c7.txt") as f:
        data = f.read().splitlines()
        ciphertext = base64_to_bytes("".join(data))
        if ciphertext:
            plaintext = decrypt_aes_ecb(ciphertext, key)
            print(plaintext)


if __name__ == "__main__":
    c7()
