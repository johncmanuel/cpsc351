BASE64_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"


def hex_to_base64(hex_str: str) -> str:
    base64_str = ""

    bin_str = ""
    for ch in hex_str:
        bin_str += format(int(ch, 16), "04b")

    padding = (6 - (len(bin_str) % 6)) % 6
    bin_str += "0" * padding

    for i in range(0, len(bin_str), 6):
        base64_str += BASE64_TABLE[int(bin_str[i : i + 6], 2)]

    padding = (4 - (len(base64_str) % 4)) % 4
    base64_str += "=" * padding
    return base64_str


def c1():
    hex_str = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"
    expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"
    res = hex_to_base64(hex_str)
    assert (
        res == expected
    ), f"Expected output does not match: {res}\nExpected: {expected}"
    print("Set 1 Challenge 1 was successful.\nOutput:", res, "\nExpected:", expected)


if __name__ == "__main__":
    c1()
