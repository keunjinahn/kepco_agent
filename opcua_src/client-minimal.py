import sys
sys.path.insert(0, "..")


from opcua import Client

from configparser import ConfigParser

import time
from NamedMutex import NamedMutex
mutex_opcua_c_ini = NamedMutex('opcua_c.ini')
def write_to_file(category, filename, inputs, order=None):
    try:
        f = open(filename, 'w')

        f.write('[%s]\n' % category.upper())

        if order is not None:
            for item in order:
                line = "%s=%s\n" % (item, inputs[item])
                f.write(line)
        else:
            for key in inputs:
                line = "%s=%s\n" % (key, inputs[key])
                f.write(line)

        f.close()
        return True

    except :
        return False

if __name__ == "__main__":

    client = Client("opc.tcp://localhost:4840/freeopcua/server/")
    # client = Client("opc.tcp://admin@localhost:4841/freeopcua/server/") #connect using a user
    try:
        client.connect()

        # Client has a few methods to get proxy to UA nodes that should always be in address space such as Root or Objects
        root = client.get_root_node()
        print("Objects node is: ", root)

        # Node objects have methods to read and write node attributes as well as browse or populate address space
        print("Children of root are: ", root.get_children())

        # get a specific node knowing its node id
        #var = client.get_node(ua.NodeId(1002, 2))
        #var = client.get_node("ns=3;i=2002")
        #print(var)
        #var.get_data_value() # get value of node as a DataValue object
        #var.get_value() # get value of node as a python builtin
        #var.set_value(ua.Variant([23], ua.VariantType.Int64)) #set node value using explicit data type
        #var.set_value(3.9) # set node value using implicit data type

        while True :
            # Now getting a variable node using its browse path
            var_sensor_1 = root.get_child(["0:Objects", "2:SensorObj", "2:var_sensor_1"])
            obj = root.get_child(["0:Objects", "2:SensorObj"])
            print("var_sensor_1 is: ", var_sensor_1)
            print("SensorObj is: ", obj)

            # Stacked myvar access

            inputs =  dict()
            inputs['count'] = 6
            inputs['sensor_1'] = root.get_children()[0].get_children()[1].get_variables()[0].get_value()
            inputs['sensor_2'] = root.get_children()[0].get_children()[1].get_variables()[1].get_value()
            inputs['sensor_3'] = root.get_children()[0].get_children()[1].get_variables()[2].get_value()
            inputs['sensor_4'] = root.get_children()[0].get_children()[1].get_variables()[3].get_value()
            inputs['sensor_5'] = root.get_children()[0].get_children()[1].get_variables()[4].get_value()
            inputs['sensor_6'] = root.get_children()[0].get_children()[1].get_variables()[5].get_value()
            print("receive data is: ", inputs)
            with mutex_opcua_c_ini:
                write_to_file('INFO', 'D:\\project\\kepco\\kepco_agent\\bin\\opcua_c.ini', inputs)
            time.sleep(1)
    finally:
        client.disconnect()
