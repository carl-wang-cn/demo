#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import glob
import time
import logging
import logging.handlers

LOG_FILENAME = 'logging_rotatingfile_by_time.out'

# Set up a specific logger with our desired output level
my_logger = logging.getLogger('MyLogger')
my_logger.setLevel(logging.DEBUG)

# Add the log message handler to the logger
# when - S--second
#        M--Minute
#        H--Hour default
#        D--Day
#        W{0-6}--roll over on a certain day: 0-Monday
#        midnight--roll over at midnight
# interval: 1 default

handler = logging.handlers.TimedRotatingFileHandler(
              LOG_FILENAME, when='S', interval=1, backupCount=20)

my_logger.addHandler(handler)

# Log some messages
for i in range(20):
    my_logger.debug('i = %d' % i)
    time.sleep(1)

# See what files are created
logfiles = glob.glob('%s*' % LOG_FILENAME)

for filename in logfiles:
    print(filename)
