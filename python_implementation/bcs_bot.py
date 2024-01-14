from ctypes import byref
import socket
from JHBotPythonBindings import *

Send_URL = b"127.0.0.1"

Header_Autorization = b"Authorization: Bearer EAAzl9nSKvWYBO0LY3NfQYtZC6s4UcofwzpZAnhPt0MHKoDIdF6TQGaLbe6gZBvkuFkHd42yRTtHWZAaJ870AJTOoZB9MfJHhSQJit13eLDUj19vReoaSAv4HBRPVs2nlqAbWW8QcetaodPDsYMcHMFafFNE10Nuxi1HtgtQrTE5JPMk3ksmk08y2hu76C6GkyHp7JSymZC6hObalsxdaCjBN5y6Rmg"

Header_ContentType = b"Content-Type: application/json"

to_do = "server"

if (to_do == "test"):
    print("send test")
    Headers = (c_char_p*2)()
    Headers[0] = c_char_p(Header_Autorization)
    Headers[1] = c_char_p(Header_ContentType)
    data = c_char_p()
    read_text_file(b"../samples/json.txt",byref(data))
    post_data(b"127.0.0.1",Headers,2,data);

if(to_do == "server"):
    print("server test")
    server = Server_constructor(socket.AF_INET,socket.SOCK_STREAM,0,socket.INADDR_ANY,80,10);
    while(True):
        response = launch_listner_server(byref(server))
        #body_str = Dictionary_print(byref(response.body))
        #print(body_str.value)

if(to_do == "parse"):
    print("Parse test")
    data = c_char_p()
    read_text_file(b"../samples/newest_sample.txt",byref(data))
    response = parse_to_httpresponse(data);
    body_str = Dictionary_print(byref(response.body))
    #print(body_str)
    



    

