#!/usr/bin/env python
# -*- encoding: utf-8 -*-


import logging
import auxiliary

'''
debug - info - waring - error - critical
'''

# create logger with "logdemo1"
logger = logging.getLogger("log_in_multi_modules")
logger.setLevel(logging.DEBUG)

# create file handler which logs even debug messages
fh = logging.FileHandler("log_in_multi_modules.log")
fh.setLevel(logging.DEBUG)

# create console handler with a higher log level
ch = logging.StreamHandler()
ch.setLevel(logging.ERROR)

# create formatter and add it to the handlers
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
fh.setFormatter(formatter)
ch.setFormatter(formatter)

# add the handlers to the logger
logger.addHandler(fh)
logger.addHandler(ch)

logger.info("creating an instance of auxiliary.Auxiliary")
a = auxiliary.Auxiliary()
logger.info("created an instance of auxiliary.Auxiliary")

logger.info("calling auxiliary.Auxiliary.do_something")
a.do_something()
logger.info("finished auxiliary.Auxiliary.do_something")

logger.info("calling auxiliary.some_function")
auxiliary.some_function()
logger.info("done calling auxiliary.some_function")
