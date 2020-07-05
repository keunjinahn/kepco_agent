import sys
sys.path.insert(0, "..")
import time

from configparser import ConfigParser
from opcua import ua, Server
from NamedMutex import NamedMutex
mutex_opcua_s_ini = NamedMutex('opcua_s.ini')
def read_from_file(filename, section, required_props=None):
    config_parser = ConfigParser()
    config_parser.optionxform = str
    data = dict()

    try:
        data_set = config_parser.read(filename)
        if len(data_set) == 0:
            return None

        if section not in config_parser.sections():
            return dict()

        for k, v in config_parser.items(section):
            data[k] = v

        if required_props is not None:
            for prop in required_props:
                if prop not in data:
                    return None
        return data

    except:
        return None
if __name__ == "__main__":

    # setup our server
    server = Server()
    server.set_endpoint("opc.tcp://0.0.0.0:4840/freeopcua/server/")

    # setup our own namespace, not really necessary but should as spec
    uri = "http://examples.freeopcua.github.io"
    idx = server.register_namespace(uri)

    # get Objects node, this is where we should put our nodes
    objects = server.get_objects_node()

    # populating our address space
    objSensor = objects.add_object(idx, "SensorObj")
    var_sensor_1 = objSensor.add_variable(idx, "var_sensor_1", 6.7)
    var_sensor_1.set_writable()    # Set MyVariable to be writable by clients
    idx += 1
    var_sensor_2 = objSensor.add_variable(idx, "var_sensor_2", 6.7)
    var_sensor_2.set_writable()    # Set MyVariable to be writable by clients
    idx += 1
    var_sensor_3 = objSensor.add_variable(idx, "var_sensor_3", 6.7)
    var_sensor_3.set_writable()    # Set MyVariable to be writable by clients
    idx += 1
    var_sensor_4 = objSensor.add_variable(idx, "var_sensor_4", 6.7)
    var_sensor_4.set_writable()    # Set MyVariable to be writable by clients
    idx += 1
    var_sensor_5 = objSensor.add_variable(idx, "var_sensor_5", 6.7)
    var_sensor_5.set_writable()    # Set MyVariable to be writable by clients
    idx += 1
    var_sensor_6 = objSensor.add_variable(idx, "var_sensor_6", 6.7)
    var_sensor_6.set_writable()    # Set MyVariable to be writable by clients
    # starting!
    server.start()

    try:
        count = 0
        while True:
            time.sleep(1)
            with mutex_opcua_s_ini:
                config = read_from_file('D:\\project\\kepco\\kepco_agent\\bin\\opcua_s.ini', 'INFO')
                if config is not None and len(config['sensor_1']) > 20:
                    var_sensor_1.set_value(config['sensor_1'])
                    var_sensor_2.set_value(config['sensor_2'])
                    var_sensor_3.set_value(config['sensor_3'])
                    var_sensor_4.set_value(config['sensor_4'])
                    var_sensor_5.set_value(config['sensor_5'])
                    var_sensor_6.set_value(config['sensor_6'])
            # print("->",config['sensor_1'])
    finally:
        #close connection, remove subcsriptions, etc
        server.stop()
