from set1.c3 import xor_cipher


def c4():
    with open("./set1/input/c4.txt") as f:
        lines = f.read().splitlines()
        possible_char_strs = []
        for line in lines:
            xc = xor_cipher(line)
            possible_char_strs.append(xc)
        res = max(
            possible_char_strs,
            key=lambda x: sum(1 for ch in x if ch.isalpha() or ch.isspace()),
        )
        expected = "Now that the party is jumping"
        assert (
            res == expected
        ), f"Expected output does not match: {res}\nExpected: {expected}"
        print(
            "Set 1 Challenge 4 was successful.\nOutput:", res, "\nExpected:", expected
        )


if __name__ == "__main__":
    c4()
