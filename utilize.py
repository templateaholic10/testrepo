#!/usr/bin/env python

import os
import sys
import subprocess

def main():
    from_dirname = os.path.join(os.path.dirname(os.path.abspath(__file__)), "lab")
    lab_filenames = subprocess.check_output(["ls", from_dirname]).split()[:-1]
    to_dirname = "/usr/local/include"

    util_filenames = [filename for filename in lab_filenames if filename[:4] == "util"] + ["timer.hpp"]
    for util_filename in util_filenames:
        from_filename = os.path.join(from_dirname, util_filename)
        trimed_name = util_filename.split(".")[0]
        to_filename = os.path.join(to_dirname, trimed_name)
        subprocess.call('sh -c "sudo cp {from_filename} {to_filename}"'.format(from_filename=from_filename, to_filename=to_filename), shell=True)

if __name__ == "__main__":
    main()
