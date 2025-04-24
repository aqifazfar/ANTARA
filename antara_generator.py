import xml.etree.ElementTree as ET


data_types = {'uint8_t': 1 , 'uint16_t' : 2 , 'uint32_t' : 4, 'uint64_t' : 8,
              'int8_t': 1 , 'int16_t' : 2 , 'int32_t' : 4, 'int64_t' : 8,
              'float' : 4, 'double' : 8}


def generate_message_class(messageName:str,controlFlags:str,nodeID:int,componentID:int,messageID:int,length:int,field:list):

    message_length = 15 + length + 4 + 32 if 'SIGNATURE_EXTENSION' in controlFlags else 0

    def generate_pack_function():

        code = f"\tinline static antara_msg_t<{message_length}> Pack(std::uint32_t const &groupID, std::uint32_t const &systemID, std::uint8_t const (&data)[{length}]" + ")\n\t{\n"
        code += f"\t\tantara_msg_t<{message_length}> output =" + " {};\n"
        code += f"\t\tantara_payload_t payload = {{{nodeID},{componentID},{messageID},{length}"
        code += ",{"

        for i in range(length):
            code += f"data[{i}],"
        
        code =  code[:-1] + "}};\n\n"
        code += f"\t\toutput = Serialization::Serialize<{message_length}>({controlFlags}, groupID, systemID, payload);\n\n"
        code += "\t\t// To track the sequence for reliable mode\n"
        code += "\t\tcurrentSequence = Serialization::Get_Instance().Get_Current_Sequence();\n"
        code += "\n\t\treturn output;\n\t}\n"

        return code

    def generate_unpack_function():

        code = f"\tinline static int Unpack(std::uint32_t const &groupID, std::uint32_t const &systemID, antara_payload_t &output, antara_msg_t<{message_length}> const &msgIn" +")\n\t{\n"
        code += f"\t\tint status = Deserialization::Deserialize<{message_length}>(groupID,systemID,output,msgIn);\n"
        code += "\n\t\treturn status;"
        code += "\n\t}\n"

        return code
        
        pass

    code = f"class {messageName}\n" + "{\n\n"
    code += f"private:\n"
    code += f"\tinline static std::uint8_t currentSequence = 0;\n\n"
    code +="public:\n"

    code += generate_pack_function()
    code += "\n"
    code += generate_unpack_function()

    code += "};\n\n"

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

    enum_entry = [entry.attrib for entry in root.findall('./message/enum/entry')]

    entry_description = [entry.text for entry in root.findall('./message/enum/entry')]

    for i in range(len(root.findall('./message/enum'))):

        enums.append([])
        for j in range(i+1):
            enums[i].append(enum_entry[j]['name'])
            enums[i].append(enum_entry[j]['value'])
            enums[i].append(entry_description[j])

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

    return (messages, enums, description)

        

def main(dialect_file:str, output_file:str = ''):

    file_name = dialect_file.split('/')

    for i in file_name:
        if '.xml' in i:
            file_name = i

    code = r"\\" + f"{file_name} generated C++ code\n\n"
    code += f"#include \"Antara/Core\"\n\n"


    msgs,enums,descs = parse(dialect_file)

    for msg in msgs:
       
        msg_name = msg[0]
        msg_cflags = msg[1][0] + ' | ' + msg[1][1] + ' | ' + msg[2]
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
        
        code += generate_message_class(msg_name,msg_cflags,node_id,component_id,msg_id,data_length,data)
    
    print(code)


        
        
    
        

           


main('./tests/dialect_test.xml')