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
import telnetlib
from ConfigParser import ConfigParser
version = "1.0.0"
import getpass
import telnetlib
import pyodbc

current_dir = os.path.dirname(os.path.realpath(__file__))
up_dir = os.path.dirname(current_dir)
sys.path.append(up_dir + '/lib')

REQ_TIMEOUT = 3
import requests
import ctypes

import warnings
warnings.filterwarnings("ignore", category=UnicodeWarning);

SERVER_MODE = True
if SERVER_MODE is True :
    ZUSI_COMP = b"ZUSI:COMP;\r\n"
    ZUSI_TBS = b"ZUSI:TBS;\r\n"
    ZUSI_ET = b"ZUSI:ET;\r\n"
    ZPAL_TSL = b"ZPAL::TSL;\r\n"
    ZAHO = b"ZAHO;\r\n"
    ZDTF_TTRX = b"ZDTF:TTRX;\r\n"
    EXIT = b"ZZZ;\r\n"
else :
    ZUSI_COMP = b"./ZUSI:COMP.sh\n"
    ZUSI_TBS = b"./ZUSI:TBS.sh\n"
    ZUSI_ET = b"./ZUSI:ET.sh\n"
    ZPAL_TSL = b"./ZPAL::TSL.sh\n"
    ZAHO = b"./ZAHO.sh\n"
    ZDTF_TTRX = b"./ZDTF:TTRX.sh\n"
    EXIT = b"exit\n"

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

class EventManager(object):
    def __init__(self, logger=None):
        print("EventManager init")
        self.is_running = True
        if logger is None:
            logging.basicConfig(level=logging.INFO)
            self.logger = logging.getLogger('EventManager')
        else:
            self.logger = logger
        self.log_print_enable = True
        self.kepco_server_info = None
        self.api_headers = {'Content-type': 'application/json'}
        self.db = None
        self.cnxn = None
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
            # self.kepco_server_info['trs_server_ip'] = '70.106.1.110'
            # self.kepco_server_info['trs_server_user_id']='SYSTEM'
            # self.kepco_server_info['trs_server_user_pass'] = 'EKADRS'
            # self.kepco_server_info['tdis_server_ip'] = '70.109.1.140'
            # self.kepco_server_info['tdis_server_user_id']='EVENTMANAGER'
            # self.kepco_server_info['tdis_server_user_pass'] = 'kwkepcose[9470'
            # self.kepco_server_info['tbms_server_ip'] = '70.109.1.159'
            # self.kepco_server_info['tbms_server_user_id']='EVENTMANAGER'
            # self.kepco_server_info['tbms_server_user_pass'] = 'kwkepcose[9470'
            # self.sleep_seconds = int(self.kepco_server_info['sleep_seconds']) / 1000
            #self.sleep_seconds = int(self.kepco_server_info['sleep_seconds']) / 1000
            self.Log(str(self.kepco_server_info))
        except:
            self.Log("get_server_info Exception")

    def login_trs_server(self):
        try :
            HOST = self.kepco_server_info['trs_server_ip']
            user = self.kepco_server_info['trs_server_user_id']
            password = self.kepco_server_info['trs_server_user_pass']
            if SERVER_MODE is True:
                tn = telnetlib.Telnet(HOST)
                print(tn.read_until('<'))
                # tn.read_until(b"ENTER USERNAME < ")
                tn.write("SYSTEM\r\n")
                # tn.read_until(b"ENTER PASSWORD < ")
                print(tn.read_until('<'))
                tn.write(password.encode('ascii') + "\r\n")
                print(tn.read_until('<'))
            else :
                tn = telnetlib.Telnet(HOST)
                tn.read_until(b"login: ")
                tn.write(user.encode('ascii') + b"\n")
                tn.read_until(b"Password: ")
                tn.write(password.encode('ascii') + b"\n")
        except :
            self.Log("login_trs_server Exception")
        return tn

    def get_trs_event(self):
        self.Log("start get_trs_event :")
        trs_event_info = {}
        trs_event_info["zusi_comp"] = {}
        trs_event_info["zusi_tbs"] = {}
        trs_event_info["zusi_et"] = {}
        trs_event_info["zpal_tsl"] = {}
        trs_event_info["zaho"] = {}
        trs_event_info["zdtf_ttrx"] = {}

        try:
            tn = self.login_trs_server()
            tn.write(ZUSI_COMP)
            tn.write(EXIT)
            zusi_comp_list = []
            b_start = False
            for line in tn.read_all().decode('ascii').split('\r\n'):
                # print(line)
                if len(line) > 1 :
                    if line.find('WORKING') != -1 :
                        b_start = True
                        # print(line)
                        continue
                    if b_start is True and  line.find('LOCATION') != -1:
                        continue
                    if line.find('COMMAND EXECUTED') != -1 :
                        break
                    if b_start is True:
                        u_obj = {
                            "UNIT":line[:12].strip(),
                            "PHYS": line[12:16].strip(),
                            "STATE": line[17:22].strip(),
                            "LOCATION": line[23:45].strip(),
                            "INFO": line[45:].strip(),
                        }
                        # print(u_obj)
                        zusi_comp_list.append(u_obj)
            trs_event_info["zusi_comp"] = zusi_comp_list
            #
            tn = self.login_trs_server()
            tn.write(ZUSI_TBS)
            tn.write(EXIT)
            zusi_tbs_list = []
            b_start = False
            for line in tn.read_all().decode('ascii').split('\n'):
                if len(line) > 1:
                    if line.find('WORKING') != -1:
                        b_start = True
                        # print(line)
                        continue
                    if b_start is True and line.find('LOCATION') != -1:
                        continue
                    if line.find('COMMAND EXECUTED') != -1:
                        break
                    if b_start is True:
                        u_obj = {
                            "UNIT": line[:12].strip(),
                            "PHYS": line[12:16].strip(),
                            "STATE": line[17:22].strip(),
                            "LOCATION": line[23:45].strip(),
                            "INFO": line[45:].strip(),
                        }
                        zusi_tbs_list.append(u_obj)
            trs_event_info["zusi_tbs"] = zusi_tbs_list

            tn = self.login_trs_server()
            tn.write(ZUSI_ET)
            tn.write(EXIT)
            zusi_et_list = []
            b_start = False
            for line in tn.read_all().decode('ascii').split('\n'):
                if len(line) > 1:
                    if line.find('WORKING') != -1:
                        b_start = True
                        # print(line)
                        continue
                    if b_start is True and line.find('LOCATION') != -1:
                        continue
                    if line.find('COMMAND EXECUTED') != -1:
                        break
                    if b_start is True:
                        u_obj = {
                            "UNIT": line[:12].strip(),
                            "PHYS": line[12:16].strip(),
                            "STATE": line[17:22].strip(),
                            "LOCATION": line[23:45].strip(),
                            "INFO": line[45:].strip(),
                        }
                        zusi_et_list.append(u_obj)
            trs_event_info["zusi_et"] = zusi_et_list

            tn = self.login_trs_server()
            tn.write(ZPAL_TSL)
            tn.write(EXIT)
            zpal_tsl_list = []
            b_start = False
            for line in tn.read_all().decode('ascii').split('\n'):
                if len(line) > 1 :
                    if line.find('RESERVATION') != -1 :
                        b_start = True
                        # print(line)
                        continue
                    if b_start is True and  \
                            (line.find('UNIT') != -1 or line.find('------------') != -1) :
                        continue
                    if line.find('COMMAND EXECUTED') != -1 :
                        break
                    if b_start is True:
                        u_obj = {
                            "UNIT":line[:12].strip(),
                            "TSL0": line[12:25].strip(),
                            "TSL1": line[25:38].strip(),
                            "TSL2": line[38:51].strip(),
                            "TSL3": line[51:].strip(),
                        }
                        zpal_tsl_list.append(u_obj)

            trs_event_info["zpal_tsl"] = zpal_tsl_list

            tn = self.login_trs_server()
            tn.write(ZAHO)
            tn.write(EXIT)
            zaho_list = []
            b_start = False
            temp_line_list = []
            # print(tn.read_all().decode('ascii'))
            b_start = False
            for line in tn.read_all().decode('ascii').split('\n'):
                if len(line) > 1:
                    if line.find('ALARMS') != -1:
                        b_start = True
                        continue
                    if line.find('COMMAND EXECUTED') != -1:
                        break
                    if b_start is True:
                        temp_line_list.append(line)

            line_cur = 0
            # print("temp_line_list len : ", len(temp_line_list))
            event_list = []
            for line in temp_line_list:
                if line.find('(') != -1 and line.find(')') != -1:
                    start = line.find('(')
                    end = line.find(')')
                    event_id = line[start + 1:end]
                    event_msg = temp_line_list[line_cur - 2].replace('\r', ' ') + temp_line_list[line_cur - 1].replace('\r',
                                                                                                                       ' ') + line.replace(
                        '\r', ' ')
                    if line_cur < (len(temp_line_list) - 1):
                        event_msg += temp_line_list[line_cur + 1].replace('\r', ' ')
                    first_line = temp_line_list[line_cur - 2]
                    event_date = first_line[-24:-4]
                    e_obj = {
                        "event_id": event_id,
                        "event_date":event_date,
                        "event_msg": event_msg
                    }
                    zaho_list.append(e_obj)
                line_cur += 1
            trs_event_info["zaho"] = zaho_list

            tn = self.login_trs_server()
            tn.write(ZDTF_TTRX)
            tn.write(EXIT)
            zdtf_ttrx_list = []
            b_start = False
            for line in tn.read_all().decode('ascii').split('\n'):
                if len(line) > 1:
                    if line.find('SIGNALLING') != -1:
                        b_start = True
                        # print(line)
                        continue
                    if b_start is True and \
                            (line.find('UNIT') != -1 or line.find('----------') != -1):
                        continue
                    if line.find('COMMAND EXECUTED') != -1:
                        break
                    if b_start is True:
                        u_obj = {
                            "FUNCTIONAL": line[:12].strip(),
                            "SIGNALLING": line[12:27].strip(),
                            "D-CHA": line[27:31].strip(),
                            "MASTER": line[31:44].strip(),
                            "WORKING": line[44:].strip(),
                        }
                        zdtf_ttrx_list.append(u_obj)
            trs_event_info["zdtf_ttrx"] = zdtf_ttrx_list
        except:
            self.Log("get_trs_info Exception")
        return trs_event_info

    def get_tdis_event(self):
        self.Log("get_tdis_event start")
        tdis_event_list = []
        if not self.connect_to_db(self.kepco_server_info['tdis_server_ip'],'TDIS',self.kepco_server_info['tdis_server_user_id'],self.kepco_server_info['tdis_server_user_pass']):
            self.logger.error('Database initialize failed. Exit.')
            return False
        # if not self.connect_to_db('70.109.1.140', 'TDIS',
        #                               'EVENTMANAGER',
        #                               'kwkepcose[9470'):
        #     self.Log('Database initialize failed. Exit.')
        #     return False
        cursor = self.cursor()
        try:
            sql = "select top 20 A.*,B.event_name,B.event_kind from event_his AS A JOIN event_code_info AS B ON A.event_code=B.event_code ORDER BY A.event_dtime DESC"
            cursor.execute(sql)
            recv_data = [dict(zip(zip(*cursor.description)[0], row)) for row in cursor.fetchall()]
            for recv in recv_data:
                tdis_event_list.append(recv)
        except :
            self.Log("get_tdis_event except")
        finally:
            if not cursor:
                cursor.close()
            self.disconnect()
        return tdis_event_list

    def get_tdis_event_change(self):
        self.Log("get_tdis_event_change start")
        tdis_event_list = []
        if not self.connect_to_db(self.kepco_server_info['tbms_server_ip'],'TDIS',self.kepco_server_info['tbms_server_user_id'],self.kepco_server_info['tbms_server_user_pass']):
            self.logger.error('Database initialize failed. Exit.')
            return False
        cursor = self.cursor()
        try:
            sql = "select top 20 A.*,B.event_name,B.event_kind from event_his AS A JOIN event_code_info AS B ON A.event_code=B.event_code ORDER BY A.event_dtime DESC"
            cursor.execute(sql)
            recv_data = [dict(zip(zip(*cursor.description)[0], row)) for row in cursor.fetchall()]
            for recv in recv_data:
                tdis_event_list.append(recv)
        except :
            self.Log("get_tdis_event except")
        finally:
            if not cursor:
                cursor.close()
            self.disconnect()
        return tdis_event_list

    def connect_to_db(self,db_ip,db_name,user_id,user_passwd,db_info=None):
        try :
            # DB_INFO_MODULE = "Driver={SQL Server};Server=tcp:70.109.1.159,1433;Database=TBMS;Uid=EVENTMANAGER;Pwd=kwkepcose[9470;Encrypt=no;TrustServerCertificate=no;Connection Timeout=30;"
            DB_INFO_MODULE = "Driver={SQL Server};Server=tcp:" + db_ip + ",1433;Database=" + db_name + ";Uid=" + user_id + ";Pwd=" + user_passwd + ";Encrypt=no;TrustServerCertificate=no;Connection Timeout=30;"
            # print("DB_INFO_MODULE :",DB_INFO_MODULE)
            info = db_info
            print("start")
            self.cnxn = pyodbc.connect(DB_INFO_MODULE,unicode_results=True)
            print("connected...")
            cursor = self.cnxn.cursor()
            self.Log('Database connected.')
        except pyodbc.Error, e:
            self.Log('Database connection failed: %s' % str(e))
        return True
        #
        # except pyodbc.Error, e:
        #     self.logger.error('Database connection failed: %s' % str(e))
        #
        #     return False

    def disconnect(self):
        if self.db:
            self.db.close()

    def cursor(self):
        if self.cnxn is None:
            return None
        return self.cnxn.cursor()

    def get_tbms_event(self):
        self.Log('get_tbms_event start')
        tbms_event_list = []
        if not self.connect_to_db(self.kepco_server_info['tbms_server_ip'],'TBMS',self.kepco_server_info['tbms_server_user_id'],self.kepco_server_info['tbms_server_user_pass']):
        # if not self.connect_to_db('70.109.1.159', 'TBMS',
        #                               'EVENTMANAGER',
        #                               'kwkepcose[9470'):
            self.Log('Database initialize failed. Exit.')
            return False
        cursor = self.cursor()
        try :
            # sql = "select top 20 A.*,B.ALARM_NM,B.ALARM_CLASS, C.TBS_NM, C.IP_ADDR from RTU_ALARM AS A JOIN ALARM_CODE AS B ON A.ALARM_CD=B.ALARM_CD LEFT JOIN RTU_INFO AS C ON A.RTU_ID=C.RTU_ID ORDER BY A.OCCR_DTM DESC"
            sql = "select '2020-09-23 12:00:00' as OCCR_DTM,TBS_NM from RTU_INFO"
            cursor.execute(sql)
            recv_data = [dict(zip(zip(*cursor.description)[0], row)) for row in cursor.fetchall()]
            for recv in recv_data:
                # print("date : ",recv['OCCR_DTM'])
                tbms_event_list.append(recv)
        except :
            self.Log('get_tbms_event exception!')
            pass
        finally:
            if not cursor:
                cursor.close()
            # self.disconnect()
        return tbms_event_list

    def run(self):
        # try:
        self.Log('EVENT COLLECTION START....')
        self.get_server_info()
        trs_event_info_list = []
        tdis_event_info_list = []
        tbms_event_info_list = []
        # if int(self.kepco_server_info['tdis_tbms_change']) == 1:
        #     if int(self.kepco_server_info['trs_check']) == 1 :
        #         trs_event_info_list = self.get_trs_event()
        #     if int(self.kepco_server_info['tdis_check']) == 1 :
        #         tdis_event_info_list = self.get_tdis_event()
        #     if int(self.kepco_server_info['tbms_check']) == 1 :
        #         tdis_event_info_change = self.get_tdis_event_change()
        #         for event in tdis_event_info_change:
        #             tdis_event_info_list.append(event)
        # else :
        #     if int(self.kepco_server_info['trs_check']) == 1 :
        #         trs_event_info_list = self.get_trs_event()
        #     if int(self.kepco_server_info['tdis_check']) == 1 :
        #         tdis_event_info_list = self.get_tdis_event()
        #     if int(self.kepco_server_info['tbms_check']) == 1 :
        #         tbms_event_info_list = self.get_tbms_event()
        # keop_event_info = {
        #     "trs_event": json.dumps(trs_event_info_list),
        #     "tdis_event":json.dumps(tdis_event_info_list),
        #     "tbms_event":json.dumps(tbms_event_info_list)
        # }
        if int(self.kepco_server_info['tbms_check']) == 1 :
            tbms_event_info_list = self.get_tbms_event()
        print("tbms_event_info_list : ",tbms_event_info_list)
        keop_event_info = {
            "trs_event": json.dumps([]),
            "tdis_event":json.dumps([]),
            "tbms_event":json.dumps(tbms_event_info_list,ensure_ascii = False,encoding='latin1')
        }
        dataobj = json.dumps(keop_event_info)
        servers_url = self.get_server_ip() + "/api/v1/agent/kepco_event_send"
        r = requests.post(servers_url, verify=False, data=dataobj, headers=self.api_headers)
        data = json.loads(r.text)
        self.Log(str(data))
        self.Log('EVENT COLLECTION END....')
        # except :
        #     self.Log('Exception run')
        # while self.is_running:
        #     try:
        #         self.Log('EVENT COLLECTION START....')
        #         self.get_server_info()
        #         trs_event_info = json.dumps(self.get_trs_event())
        #         keop_event_info = {
        #             "trs_event": trs_event_info,
        #             "tdis_event": json.dumps(''),
        #             "tbms_event": json.dumps('')
        #         }
        #         # print("len : ", len(str(keop_event_info)), ", kepco_event_send : ",keop_event_info)
        #         dataobj = json.dumps(keop_event_info)
        #         servers_url = self.get_server_ip() + "/api/v1/agent/kepco_event_send"
        #         r = requests.post(servers_url, verify=False, data=dataobj, headers=self.api_headers)
        #         data = json.loads(r.text)
        #         self.Log(str(data))
        #         self.Log('EVENT COLLECTION END....')
        #         for i in range(self.sleep_seconds):
        #             if not self.is_running:
        #                 break
        #             time.sleep(1)
        #
        #     except KeyboardInterrupt:
        #         self.Log('Exception KeyboardInterrupt')
        #         self.is_running = False

    def stop_process(self):
        self.is_running = False


if __name__ == '__main__':
    file_logger = logging.getLogger("EventManager")
    file_logger.setLevel(logging.INFO)

    file_handler = handlers.RotatingFileHandler(
        "./EventManager.log",
        maxBytes=(1024 * 1024 * 1),
        backupCount=5
    )
    formatter = logging.Formatter('%(asctime)s %(name)s %(levelname)s %(message)s')
    file_handler.setFormatter(formatter)
    file_logger.addHandler(file_handler)

    app = EventManager(logger=file_logger)
    app.run()
    app.stop_process()

