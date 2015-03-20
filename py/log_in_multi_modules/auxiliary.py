#!/usr/bin/env python
# -*- encoding: utf-8 -*-

import logging

# create logger
module_logger = logging.getLogger("log_in_multi_modules.auxiliary")

class Auxiliary:
    def __init__(self):
        self.logger = logging.getLogger("log_in_multi_modules.auxiliary.Auxiliary")
        self.logger.info("creating an instance of Auxiliary")

    def do_something(self):
        self.logger.info("do something")
        a = 1 + 1
        self.logger.info("done doing something")
    
def some_function():
    module_logger.info("received a call to 'some_functiong'")
