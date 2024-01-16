from ctypes import *
import socket

c_lib = CDLL('./libJHbot.so')

read_text_file = c_lib.read_text_file
read_text_file.restype = c_char_p
read_text_file.argtypes = [c_char_p]

post_data = c_lib.post_data
post_data.restype = c_int
post_data.argtypes = [c_char_p,POINTER(c_char_p),c_ulong,c_char_p]

initialize_bot = c_lib.python_initialize_bot;
initialize_bot.restype = None
initialize_bot.argtypes = None

create_new_listner_server = c_lib.python_create_new_listner_server
create_new_listner_server.restype = c_int
create_new_listner_server.argtypes = [c_int,c_int,c_int,c_ulong,c_int,c_int]

launch_listner_server = c_lib.python_launch_listner_server
launch_listner_server.restype = c_int
launch_listner_server.argtypes = [c_int]

parse_httprequest  = c_lib.python_parse_httprequest
parse_httprequest.restype = c_int;
parse_httprequest.argtypes = [c_char_p]

httprequest_search = c_lib.python_httprequest_search
httprequest_search.restype = c_char_p
httprequest_search.argtypes = [c_int,c_char_p,c_char_p]

create_whatsapp_template_message = c_lib.generate_whatsapp_template_message
create_whatsapp_template_message.restype = c_char_p
create_whatsapp_template_message.argtypes = [c_char_p,c_char_p,c_char_p,c_char_p]

create_whatsapp_text_message = c_lib.generate_whatsapp_text_message;
create_whatsapp_text_message.restype = c_char_p
create_whatsapp_text_message.argtypes = [c_char_p,c_char_p,c_char_p]

clear_httprequests = c_lib.python_clear_httprequests
clear_httprequests.restype = None
clear_httprequests.argtypes = None

clear_servers = c_lib.python_clear_servers
clear_servers.restype = None
clear_servers.argtypes = None
