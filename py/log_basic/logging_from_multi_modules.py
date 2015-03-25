#!/usr/bin/env python
# -*- encoding: utf-8 -*-

import logging
import mylib

def main():
    logging.basicConfig(filename="myapp.log", level=logging.INFO)
    logging.info("started")
    mylib.do_something()
    logging.info("Finished")

if __name__ == "__main__":
    main()

