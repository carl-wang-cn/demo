#!/usr/bin/env python
# -*- encoding: utf-8 -*-

import logging

logging.basicConfig(filename='example.log', level=logging.DEBUG)
logging.debug("This msg should go to the log file")
logging.info("So should this")
logging.warning("And this, too")
