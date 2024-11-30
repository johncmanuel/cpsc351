def xor(buffer1: str, buffer2: str) -> str:
    res = ""
    for ch1, ch2 in zip(buffer1, buffer2):
        res += format(int(ch1, 16) ^ int(ch2, 16), "x")
    return res


def c2():
    buffer1 = "1c0111001f010100061a024b53535009181c"
    buffer2 = "686974207468652062756c6c277320657965"
    expected = "746865206b696420646f6e277420706c6179"
    res = xor(buffer1, buffer2)
    assert (
        res == expected
    ), f"Expected output does not match: {res}\nExpected: {expected}"
    print("Set 1 Challenge 2 is successful.\nOutput:", res, "\nExpected:", expected)


c2()
