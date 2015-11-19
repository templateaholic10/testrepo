#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess

def main():
    from_dirname = os.path.join(os.path.dirname(os.path.abspath(__file__)), "lab")
    to_dirname = "/usr/local/include"

    # ファイルコピー
    lab_filenames = subprocess.check_output(["ls", from_dirname]).split()[:-1]
    util_filenames = [filename for filename in lab_filenames if filename[:4] == "util"] + ["timer.hpp", "os.hpp", "zero_one.hpp", "element_wise.hpp", "array_matrix.hpp", "seq_io.hpp", "array_io.hpp", "vector_io.hpp", "eigen_io.hpp", "exfstream.hpp", "interactive.hpp"]
    for util_filename in util_filenames:
        from_filename = os.path.join(from_dirname, util_filename)
        trimed_name = util_filename.split(".")[0]
        to_filename = os.path.join(to_dirname, trimed_name)
        subprocess.call('sh -c "sudo cp {from_filename} {to_filename}"'.format(from_filename=from_filename, to_filename=to_filename), shell=True)

    # ディレクトリコピー
    util_dirnames = ["_header"]
    for util_dirname in util_dirnames:
        dirname = os.path.join(from_dirname, util_dirname)
        subprocess.call('sh -c "sudo cp -R {from_dirname} {to_dirname}"'.format(from_dirname=dirname, to_dirname=to_dirname), shell=True)

if __name__ == "__main__":
    main()
