#!/usr/bin/env python3
import re
import sys


def width(text):
    col = 0
    for ch in text:
        col = (col // 4 + 1) * 4 if ch == "\t" else col + 1
    return col


def align_run(lines):
    parts = [ln.split("§", 1) for ln in lines]
    target = max(width(left) // 4 * 4 + 4 for left, _ in parts)
    out = []
    for left, right in parts:
        tabs = (target - width(left) // 4 * 4) // 4
        out.append(left + "\t" * tabs + right)
    return out


PROTO = re.compile(r"^([A-Za-z_][^\t(]*?)\t+(\**\w+\(.*\);)$")


def normalize(line):
    found = PROTO.match(line)
    return found.group(1) + "§" + found.group(2) if found else line


def main(path):
    with open(path, encoding="utf-8") as fh:
        lines = [normalize(ln) for ln in fh.read().split("\n")]
    res, run = [], []
    for ln in lines + [None]:
        if ln is not None and "§" in ln:
            run.append(ln)
            continue
        res.extend(align_run(run) if run else [])
        run = []
        if ln is not None:
            res.append(ln)
    with open(path, "w", encoding="utf-8") as fh:
        fh.write("\n".join(res))


if __name__ == "__main__":
    for target_path in sys.argv[1:]:
        main(target_path)
