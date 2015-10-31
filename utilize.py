#!/usr/bin/env python

import os
import sys
import subprocess

def main():
    from_dirname = os.path.join(os.path.dirname(os.path.abspath(__file__)), "lab")
    filenames = subprocess.check_output(["ls", from_dirname]).split()[:-1]
    utilfilenames = [filename for filename in filenames if filename[:4] == "util"]
    to_dirname = "/usr/local/include"
    for utilfilename in utilfilenames:
        from_filename = os.path.join(from_dirname, utilfilename)
        trimed_name = utilfilename.split(".")[0]
        to_filename = os.path.join(to_dirname, trimed_name)
        subprocess.call('sh -c "sudo cp {from_filename} {to_filename}"'.format(from_filename=from_filename, to_filename=to_filename), shell=True)

if __name__ == "__main__":
    main()
