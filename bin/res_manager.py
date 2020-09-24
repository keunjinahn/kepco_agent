#!/usr/bin/env python
# -*- coding:utf-8 -*-
__author__ = 'sisung'

import json
import os
import sys
import time
import logging
from logging import handlers
import threading
import signal
from datetime import datetime,timedelta
import platform
from os import walk
import getpass
from ConfigParser import ConfigParser
version = "1.0.0"
import getpass
import psutil

current_dir = os.path.dirname(os.path.realpath(__file__))
up_dir = os.path.dirname(current_dir)
sys.path.append(up_dir + '/lib')

REQ_TIMEOUT = 3
import requests
import ctypes

import warnings
warnings.filterwarnings("ignore", category=UnicodeWarning);

def read_from_file(filename, section, required_props=None):
    config_parser = ConfigParser()
    print("config_parser : ", config_parser)
    config_parser.optionxform = str
    data = dict()

    try:
        data_set = config_parser.read(filename)
        print("data_set : ",data_set)
        if len(data_set) == 0:
            return None

        if section not in config_parser.sections():
            return dict()

        for k, v in config_parser.items(section):
            data[k] = v

        return data

    except:
        print("read_from_file Open  file failed ")
        return None

config = None

config = read_from_file('config.ini', 'INFO')
print("config", config)
SERVER_INFO = config['server_ip'].replace("\"", "")
print("SERVER INFO : ", SERVER_INFO)
SERVER_PORT = config['port'].replace("\"", "")
print("SERVER PORT : ", SERVER_PORT)

class ResourceManager(object):
    def __init__(self, logger=None):
        print("ResourceManager init")
        self.is_running = True
        if logger is None:
            logging.basicConfig(level=logging.INFO)
            self.logger = logging.getLogger('ResourceManager')
        else:
            self.logger = logger
        self.log_print_enable = True
        self.kepco_server_info = None
        self.api_headers = {'Content-type': 'application/json'}
        self.sleep_seconds = 30

    def Log(self,msg) :
        self.logger.info(msg)
        if self.log_print_enable is True :
            print(msg)

    def get_server_ip(self):
        if SERVER_PORT == 443 :
            return "https://" + SERVER_INFO + ":443"
        return "http://" + SERVER_INFO + ":" + SERVER_PORT

    def get_server_info(self):
        try:
            self.Log("get_server_info start ...")
            servers_url = self.get_server_ip() + "/api/v1/agent/kepco_server_info"
            result = requests.get(servers_url, verify=False).text
            # self.kepco_server_info = json.loads(result)
            self.kepco_server_info = {}
            self.kepco_server_info = json.loads(result)
            self.Log(str(self.kepco_server_info))
        except:
            self.Log("get_server_info Exception")

    def _check_usage_of_cpu_and_memory(self):
        print("cpu : ",psutil.cpu_percent())
        print("mem : ",psutil.virtual_memory())  # physical memory usage
        print('memory % used:', psutil.virtual_memory()[2])
        obj = {
            "cpu_value":psutil.cpu_percent(),
            "ram_avail_value":psutil.virtual_memory()[1],
            "ram_phys_value": psutil.virtual_memory()[0]
        }
        return obj

    def run(self):
        # try:
        self.Log('ResourceManager  START....')
        self.get_server_info()
        obj = self._check_usage_of_cpu_and_memory()
        dataobj = json.dumps(obj)
        servers_url = self.get_server_ip() + "/api/v1/agent/kepco_resource_send"
        r = requests.post(servers_url, verify=False, data=dataobj, headers=self.api_headers)
        data = json.loads(r.text)
        self.Log(str(data))
        self.Log('ResourceManager  END....')

    def stop_process(self):
        self.is_running = False


if __name__ == '__main__':
    file_logger = logging.getLogger("ResourceManager")
    file_logger.setLevel(logging.INFO)
    file_handler = handlers.RotatingFileHandler(
        "./ResourceManager.log",
        maxBytes=(1024 * 1024 * 1),
        backupCount=5
    )
    formatter = logging.Formatter('%(asctime)s %(name)s %(levelname)s %(message)s')
    file_handler.setFormatter(formatter)
    file_logger.addHandler(file_handler)

    app = ResourceManager(logger=file_logger)
    app.run()
    app.stop_process()

