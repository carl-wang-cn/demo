#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import os
import glob
import logging
import logging.handlers
import time

LOG_FILENAME = 'logging_rotatingfile_example.out'

# Set up a specific logger with our desired output level
my_logger = logging.getLogger('MyLogger')
my_logger.setLevel(logging.DEBUG)

# Check if log exists and should therefore be rolled
needRoll = os.path.isfile(LOG_FILENAME)

# Add the log message handler to the logger
handler = logging.handlers.RotatingFileHandler(LOG_FILENAME, backupCount=50)

my_logger.addHandler(handler)

# This is a stale log, so roll it
if needRoll:    
    # Add timestamp
    my_logger.debug('\n---------\nLog closed on %s.\n---------\n' % time.asctime())

    # Roll over on application start
    my_logger.handlers[0].doRollover()

# Add timestamp
my_logger.debug('\n---------\nLog started on %s.\n---------\n' % time.asctime())

# Log some messages
for i in xrange(20):
    my_logger.debug('i = %d' % i)

# See what files are created
logfiles = glob.glob('%s*' % LOG_FILENAME)

