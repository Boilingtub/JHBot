from ctypes import *
from os.path import exists
from sys import platform
import socket

if platform == "linux" or platform == "linux2":
    if(exists("./libJHBot.so")):
        c_lib = CDLL('./libJHBot.so')
    else:
        print("Cannot find libJHBot.so");
        exit()


if platform == "win32":
    if(exists("./libJHBot.dll")):
        
        if(exists("./libcurl-4.dll")):
            WinDLL("./libcurl-4.dll",winmode=0)
        else:
            print("Cannot find libcurl-4.dll\nsearching C:/msys64/mingw64/bin/libcurl-4.dll")
            
            if(exists("C:/msys64/mingw64/bin/libcurl-4.dll")):
                WinDLL("C:/msys64/mingw64/bin/libcurl-4.dll",winmode=0)
            else:
                print("C:/msys64/mingw64/bin/libcurl-4.dll does not exists");
                print("please download libcurl-4.dll");
                exit();
        c_lib = CDLL("./libJHBot.dll",winmode=0)
    else:
        print("Cannot find libJHBot.dll")
        exit()

def char_p_array(*args):
    Arr = (c_char_p*len(args))()
    i = 0
    for a in args:
        Arr[i] = a
        i = i + 1
    return Arr 

def int_p_array(*args):
    Arr = (c_int*len(args))()
    i = 0
    for a in args:
        Arr[i] = a
        i = i + 1
    return Arr

read_text_file = c_lib.python_read_text_file
read_text_file.restype = c_char_p
read_text_file.argtypes = [c_char_p]

post_data = c_lib.python_post_data
post_data.restype = c_int
post_data.argtypes = [c_char_p,POINTER(c_char_p),c_ulong,c_char_p]

create_new_listner_server = c_lib.python_create_new_listner_server
create_new_listner_server.restype = c_char_p
create_new_listner_server.argtypes = [c_int,c_int,c_int,c_ulong,c_int,c_int]

create_new_ssl_listner_server = c_lib.python_create_new_ssl_listner_server
create_new_ssl_listner_server.restype = c_char_p
create_new_ssl_listner_server.argtypes = [c_int,c_int,c_int,c_ulong,c_int,c_int,c_char_p,c_char_p]

launch_listner_server = c_lib.python_launch_listner_server
launch_listner_server.restype = c_int
launch_listner_server.argtypes = [c_int]

launch_ssl_listner_server = c_lib.python_launch_ssl_listner_server
launch_ssl_listner_server.restype = c_int
launch_ssl_listner_server.argtypes = [c_int]

parse_httprequest  = c_lib.python_parse_httprequest
parse_httprequest.restype = c_int;
parse_httprequest.argtypes = [c_char_p]

httprequest_search = c_lib.python_httprequest_search
httprequest_search.restype = c_char_p
httprequest_search.argtypes = [c_int,c_char_p,c_char_p]

whatsapp_message_to_data = c_lib.python_whatsapp_message_to_string
whatsapp_message_to_data.restype = c_char_p
whatsapp_message_to_data.args = c_int

create_whatsapp_message = c_lib.python_create_whatsapp_message
create_whatsapp_message.restype = c_int
create_whatsapp_message.argtypes = [c_char_p,c_char_p]

make_template_message = c_lib.python_Make_Template;
make_template_message.restype = None
make_template_message.argtypes = [c_int,c_char_p,c_char_p]

make_text_message = c_lib.python_Make_Text;
make_text_message.restype = None
make_text_message.argtypes = [c_int,c_char_p]

make_reply_message = c_lib.python_Make_reply
make_reply_message.restype = None
make_reply_message.argtypes = [c_int,c_char_p]

make_interactive_message = c_lib.python_Make_interactive
make_interactive_message.restype = None
make_interactive_message.argtypes = [c_int,c_char_p,c_char_p,c_char_p,c_int]

create_action_list = c_lib.python_create_action_list
create_action_list.restype = c_int
create_action_list.argtypes = [c_char_p,POINTER(c_int),c_int]

create_section = c_lib.python_create_section
create_section.restype = c_int
create_section.argtypes = [c_char_p,POINTER(c_char_p),c_int]

clear_httprequests = c_lib.python_clear_httprequests
clear_httprequests.restype = None
clear_httprequests.argtypes = None

clear_servers = c_lib.python_clear_servers
clear_servers.restype = None
clear_servers.argtypes = None

clear_messages = c_lib.python_clear_messages
clear_messages.restype = None
clear_messages.argtypes = None

'''
/*Shell script to test python_implementation for memory leaks*/
Linux -> overflow_test.sh
    #! /bin/bash

    serverpid=$(pidof python)
    echo "\n" > overflow_test_results.log
    while [ 1 -le 2 ]
    do
        #echo "\n\n\n" >> overflow_test_results
        python test_bot.py gen
        #echo "\n\n\n" >> overflow_test_results.log
        echo "jan" | sudo -S pmap ${serverpid} | grep "total" >> overflow_test_results.log
    done
'''
