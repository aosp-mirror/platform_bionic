#!/usr/bin/env python2

import os
import subprocess
import sys

red = '\033[91m'
green = '\033[92m'
bold = '\033[1m'
reset = '\033[0m'
prefix_pass = bold + "[" + green + "PASS" + reset + bold + "]" + reset
prefix_fail = bold + "[" + red + "FAIL" + reset + bold + "]" + reset


def indent(text, spaces=4):
    text = text.decode("utf-8")
    prefix = "    "
    return "\n".join([prefix + line for line in text.split("\n")])


def run_test(test_name, path):
    os.chdir(path)
    process = subprocess.Popen(
        ["/bin/sh", "run.sh"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    (output, _) = process.communicate()

    if os.path.exists("expected_fail"):
        with open("expected_fail", "rb") as f:
            expected_output = f.read()
            if process.returncode == 0:
                print("{} {}: unexpected success:".format(prefix_fail, test_name))
                print("")
                print("  Expected:")
                print(indent(expected_output))
                print("  Actual:")
                print(indent(output))
                return False
            elif not output.endswith(expected_output):
                print("{} {}: expected output mismatch".format(
                    prefix_fail, test_name))
                print("")
                print("  Expected:")
                print(indent(expected_output))
                print("  Actual:")
                print(indent(output))
                return False
    elif process.returncode != 0:
        print("{} {}: unexpected failure:".format(prefix_fail, test_name))
        print("")
        print(indent(output))
        return False

    print("{} {}".format(prefix_pass, test_name))
    return True


def usage():
    print("Usage: run_tests.py [-f]")
    print("    -f\t\tdon't run slow tests")
    sys.exit(0)


root_dir = os.path.dirname(os.path.realpath(__file__))
test_dir = os.path.join(root_dir, "tests")
tests = os.listdir(test_dir)
run_slow = True

if len(sys.argv) > 2:
    usage()
elif len(sys.argv) == 2:
    if sys.argv[1] != "-f":
        usage()
    run_slow = False

success = True
for test in sorted(tests):
    if test.startswith("slow") and not run_slow:
        continue
    path = os.path.join(test_dir, test)
    if not os.path.isdir(path):
        continue
    if not run_test(test, path):
        success = False

sys.exit(0 if success else 1)
