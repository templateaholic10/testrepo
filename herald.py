#!/usr/bin/env python
# -*- coding: utf-8 -*-

import mailer

_addr = "akari401test@gmail.com"
_password = "04armeria_prism01"

def herald(to_addr):
    def _herald(func):
        import functools
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            result = func(*args, **kwargs)

            subject = u"PROCESS COMPLETE"
            func_info = u"function: {function}".format(function=func.__name__)
            file_info = u"filename: {filename}".format(filename=__file__)
            body = "\n".join([subject, func_info, file_info])

            gmailer = mailer.Gmail_sender(_addr, _password)
            gmailer.send(subject, body, to_addr)

            return result
        return wrapper
    return _herald

def main():
    to_addr = "zonmin10@gmail.com"

    @herald(to_addr)
    def test():
        import time
        print("SLEEP 5 sec")
        time.sleep(5)

    test()

if __name__ == "__main__":
    main()
