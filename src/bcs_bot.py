from ctypes import *
c_lib = CDLL('./libwhatsappbot.so')

print("Starting Whatsapp_bot")

class WhatsappMessage(Structure):
    _fields_=[("name",c_char_p),
              ("wa_id",c_char_p),
              ("from",c_char_p),
              ("id",c_char_p),
              ("timestamp",c_char_p),
              ("type",c_char_p),
              ("text_body",c_char_p)]

class WhatsappCurl(Structure):
    _fields_=[("URL",c_char_p),
              ("Header_Autorization",c_char_p),
              ("Header_ContentType",c_char_p),
              ("Data",c_char_p)]

class Server(Structure):
    _fields_=[("domain"c_int),
              ("service",c_int),
              ("protocol",c_int),
              ("interface"c_ulong),
              ("port",c_int),
              ("backlog",c_int)]

class WhatsappBot(Structure):
    _fields_=[("name",c_char_p),
              ("whatsappcurl",WhatsappCurl),
              ("webhook",Server),
              ("received_message",WhatsappMessage),
              ("read_text_file",c_),
              (),
              ()
              ]
        

whatsappbot_constructor = c_lib.whatsappbot_constructor
whatsappbot_constructor.restype = POINTER(WhatsappBot)
whatsappbot_constructor.argtypes = [c_char_p,c_int]

bcs_bot = whatsappbot_constructor(b"bosshof 1",1)

print(whatsapp_bot.fields.name);
