#! /usr/bin/env python
# -*- encoding: utf-8 -*-

import logging

#logging.basicConfig(format="%(levelname)s:%(message)s", level=logging.DEBUG)
#logging.basicConfig(format="%(asctime)s %(levelname)s:%(message)s", level=logging.DEBUG)
logging.basicConfig(format="%(asctime)s %(levelname)s:%(message)s", level=logging.DEBUG, datefmt="%m/%d/%Y %I:%M:%S %p")

logging.debug("This message should appear on the console")
logging.info("So should this")
logging.warning("Add this, too")
