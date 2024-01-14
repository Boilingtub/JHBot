from ctypes import *
import socket
c_lib = CDLL('./libJHbot.so')

class Node(Structure):
    _fields_=[("data",c_void_p),
              ("next",c_void_p),
              ("previous",c_void_p)]

class LinkedList(Structure):
    _fields_=[("head",POINTER(Node)),
              ("length",c_int)]

class BinarySearchTree(Structure):
    _fields_=[("head",POINTER(Node)),
              ("compare",CFUNCTYPE(c_int,POINTER(c_void_p),POINTER(c_void_p)))]

class Dictionary(Structure):
    _fields_=[("binary_search_tree",BinarySearchTree),
              ("keys",LinkedList)]

class HTTPRequest(Structure):
    _fields_=[("request_line",Dictionary),
              ("header_fields",Dictionary),
              ("body",Dictionary)]
    
class Server(Structure):
    _fields_=[("domain",c_int),
              ("service",c_int),
              ("protocol",c_int),
              ("interface",c_ulong),
              ("port",c_int),
              ("backlog",c_int)]


c_str = c_char_p()

read_text_file = c_lib.read_text_file
read_text_file.restype = None
read_text_file.argtypes = [c_char_p,POINTER(c_char_p)]

write_text_file = c_lib.write_text_file
write_text_file.restype = None
write_text_file.argtypes = [c_char_p,c_char_p]

Server_constructor = c_lib.Server_constructor
Server_constructor.restype = Server
Server_constructor.argtypes = [c_int,c_int,c_int,c_ulong,c_int,c_int]

launch_listner_server = c_lib.launch_listner_server
launch_listner_server.restype = c_char_p
launch_listner_server.argtypes = [POINTER(Server)]

parse_to_httpresponse = c_lib.parse_to_httpresponse
parse_to_httpresponse.restype = HTTPRequest
parse_to_httpresponse.argtypes = [c_char_p]

Dictionary_search = c_lib.Dictionary_search
Dictionary_search.restype = c_void_p
Dictionary_search.argtypes = [POINTER(Dictionary),c_void_p,c_ulong]

Dictionary_print = c_lib.Dictionary_print
Dictionary_print.restype = c_char_p
Dictionary_print.argtypes = [POINTER(Dictionary)]

strcpy = c_lib.strcpy
strcpy.restype = None;
strcpy.argtypes = [c_char_p,c_char_p]

post_data = c_lib.post_data
post_data.restype = c_int 
post_data.argtypes = [c_char_p, POINTER(c_char_p), c_int, c_char_p]
