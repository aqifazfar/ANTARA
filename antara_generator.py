import xml.etree.ElementTree as ET


data_types = {'uint8_t': 1 , 'uint16_t' : 2 , 'uint32_t' : 4, 'uint64_t' : 8,
              'int8_t': 1 , 'int16_t' : 2 , 'int32_t' : 4, 'int64_t' : 8,
              'float' : 4, 'double' : 8}


def generate_enum(messageName:str, enum:list):
    code = f"enum {messageName}" + "{\n"
    for i in enum:
        code += f"{str(i[0]).upper()} = {i[1]},\n"
    
    code += "}\n\n"

    return code

def generate_message_function(messageName:str,controlFlags:str,nodeID:int,componentID:int,messageID:int,length:int,field:list):

    message_length = 15 + length + 4 + 8 if 'SIGNATURE_EXTENSION' in controlFlags else 0

    def generate_pack_function():

        code = f"inline antara_msg_t<{message_length}> {messageName}_pack(std::uint8_t const (&data)[{length}]" + ")\n{\n"
        code += f"\tantara_msg_t<{message_length}> output;\n"
        code += f"\tantara_payload_t payload = {{{nodeID},{componentID},{messageID},{length}"
        code += ",{"

        for i in range(length):
            code += f"data[{i}],"
        
        code =  code[:-1] + "}};\n\n"
        code += f"\toutput = Antara::Core::Get_Instance().Pack<{message_length}>({controlFlags}, payload);\n"
        code += "\n\treturn output;\n}\n"

        return code

    def generate_unpack_function():

        code = f"inline antara_payload_t {messageName}_unpack(antara_msg_t<{message_length}> const &msgIn" +")\n{\n"
        code += f"\tantara_payload_t output = Antara::Core::Get_Instance().Unpack<{message_length}>(msgIn);\n"
        code += "\n\treturn output;"
        code += "\n}\n"

        return code
        
        pass

    code = generate_pack_function()
    code += "\n"
    code += generate_unpack_function()
    code += "\n"

    return code


def parse(dialect_file):

    tree = ET.parse(dialect_file)
    root = tree.getroot()

    n_message = len(root.findall('message'))

    messages = []
    enums = []
    description =[]

    header = [head.attrib for head in root.findall('message')]

    payload = [pay.attrib for pay in root.findall('./message/payload')]

    field = [fld.attrib for fld in root.findall('./message/payload/field')]

    for i in range(n_message):
        
        messages.append([])

        messages[i].append(header[i].get('name'))
        messages[i].append(header[i].get('mode').split(','))
        messages[i].append(header[i].get('extension'))
        messages[i].append(header[i].get('nodeID'))
        messages[i].append(header[i].get('componentID'))

        messages[i].append(payload[i].get('msgID'))
        messages[i].append(payload[i].get('length'))

        for j in range(int(messages[i][6])):
            messages[i].append([])
            messages[i][7+j].append(field[j].get('name'))
            messages[i][7+j].append(field[j].get('type'))
            messages[i][7+j].append(field[j].get('value'))

        enum_entry = [entry.attrib for entry in root.findall(f"./message[@name = '{messages[i][0]}']/enum/entry")]

        enums.append([])
        k=0
        for j in enum_entry:
            enums[i].append([])
            enums[i][k].append(j['name'])
            enums[i][k].append(j['value'])
            k+=1
        
        


    return (messages, enums)

def main(dialect_file:str, output_file:str = ''):

    file_name = dialect_file.split('/')

    for i in file_name:
        if '.xml' in i:
            file_name = i

    code = r"\\" + f"{file_name} generated C++ code\n\n"
    code += f"#include \"Antara/Core\"\n\n"


    msgs,enums = parse(dialect_file)

    k=0
    for msg in msgs:
        code += generate_enum(msg[0],enums[k])
        k+=1


    
    for msg in msgs:
       
        msg_name = msg[0]

        msg_cflags = msg[2]
        for i in msg[1]:
            msg_cflags += " | " + i 

        node_id = int(msg[3])
        component_id = int(msg[4])
        msg_id = int(msg[5])
        data_length = 0
        n_field = 0

        for i in msg[7:]:
            data_length += data_types[i[1]]
            n_field += 1

        data = []
        for i in range(n_field):
            data.append(msg[7+i])
        
    
        code += generate_message_function(msg_name,msg_cflags,node_id,component_id,msg_id,data_length,data)
        

    
    print(code)


        
        
    
        

           


main('./tests/dialect_test.xml')