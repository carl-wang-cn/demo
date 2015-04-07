#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import logging
import logging.config

'''
logging.config.fileConfig(fname, defaults=None, disable_existing_loggers=True)
reads the logging configuration from a configparser-format file named fname. The
format of the file should be as described in Configuration file format. This
function can be called several times from an application, allowing an end user
to select from various pre-canned configurations

parameters:
    1. defaults – Defaults to be passed to the ConfigParser can be specified in this argument.
    2. disable_existing_loggers – If specified as False, loggers which exist when
       this call is made are left enabled. The default is True because this enables
       old behaviour in a backward- compatible way. This behaviour is to disable 
       any existing loggers unless they or their ancestors are explicitly named in
       the logging configuration.
'''

logging.config.fileConfig("logging.conf")

# create logger
logger = logging.getLogger("config_with_file")


# 'application' code
logger.debug("debug message")
logger.info("info message")
logger.warn("warn message")
logger.error("error message")
logger.critical("critical message")
