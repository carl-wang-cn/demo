#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import logging
import logging.config

'''
logging.config.dictConfig(config)

Takes the logging configuration from a dictionary.

If an error is encountered during configuration, this function will
raise a ValueError, TypeError, AttributeError or ImportError with a
suitably descriptive message. The following is a (possibly incomplete)
list of conditions which will raise an error:
    * A level which is not a string or which is a string not
      corresponding to an actual logging level.
    * A propagate value which is not a boolean.
    * An id which does not have a corresponding destination.
    * A non-existent handler id found during an incremental call.
    * An invalid logger name.
    * Inability to resolve to an internal or external object.

'''

LOG_SETTINGS = {
    'version': 1,
    'formatters': {
        'brief':{ # configuration for formatter with id "brief" goes here
            'format': '%(asctime)s - %(name)s - %(levelname)s - %(message)s'
        },
        'precise':{ # configuration for formatter with id "precise" goes here
            'format': '%(asctime)s - %(name)s - %(message)s'
        }
    },
    'handlers': {
        'console':{ # This is an id
            # configuration of handler with id "console" goes here
            'class': 'logging.StreamHandler',
            'level': 'DEBUG',
            'formatter': 'brief',
            'stream': 'ext://sys.stdout'
        },
        'file':{ # This is another id
            # configuration of handler with id "file" goes here
            'class': 'logging.handlers.RotatingFileHandler',
            'formatter': 'precise',
            'filename': 'log_dict.log',
            'maxBytes': 1024,
            'backupCount': 3
        }
    },
    'loggers':{
        'logging_dict_conf':{
            # other configuration for logger "logging_dict_conf"
            'level': 'DEBUG',
            'handlers': ['console', 'file'],
            'propagate': 'no'
        }
    },
    'root':{
        'level': 'DEBUG',
        # 'handlers': ['console'], # uncomment this will result log printed on screen one more time
    }
}

logging.config.dictConfig(LOG_SETTINGS)

logger = logging.getLogger("logging_dict_conf")
logger.info("log from dict_conf")

logger2 = logging.getLogger("logging_dict_conf")
new_handler = logging.FileHandler("new_handler.log")
logger2.handler = []
logger2.addHandler(new_handler)
logger2.info("log for logger2")
