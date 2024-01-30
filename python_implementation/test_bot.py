from libJHBotBindings import *
import sys
FaceBook_URL = b"https://graph.facebook.com/v17.0/149485694924975/messages"

Header_Autorization = b"Authorization: Bearer EAAzl9nSKvWYBO6LowDSFq9PxrdznQVytZAAKGZBG0ZAJ4ZCKcdgnKubQH4ibxDrsAuVgLYnaO2gsrjpRUWYmxv65k40WZCOZBughPPoY7CYSzTWlNoZCdeiNWwsny0o5zxHDqG1B4e9uyBTKo7meZC7mvqzANOpRZAyLDJpMuWrhoHghZAyWWO2FXhM1Xqig2hPNQec20U6rk26DeZA7OFAOOfxMLZA9Vy4ZD"

Header_ContentType = b"Content-Type: application/json"

Headers = char_p_array(Header_Autorization,Header_ContentType)

if(sys.argv[1] == "send"):
    print("SEND TEST")
    data = read_text_file(b"./../samples/json.txt")
    post_data(b"127.0.0.1",Headers,2,data)
    
if(sys.argv[1] == "server"):
    print("SERVER TEST")
    listner_server = create_new_listner_server(socket.AF_INET,socket.SOCK_STREAM,0,socket.INADDR_ANY,50,10)
    while(True):
        http_response =  launch_listner_server(listner_server)
        form = print(httprequest_search(http_response,b"body",b"messages.from"))
        print(httprequest_search(http_response,b"body",b"messages.body.text"))
        print(httprequest_search(http_response,b"blah",b"blah"))
        print(httprequest_search(http_response,b"body",b"unknwn"))
        clear_httprequests()
    clear_servers();
if(sys.argv[1] == "parse"):
    print("PARSE TEST")
    data = read_text_file(b"../../samples/sample.txt")
    http_response = parse_httprequest(data);
    print(httprequest_search(http_response,b"body",b"messages.from"))
    print(httprequest_search(http_response,b"body",b"messages.body.text"))
    print(httprequest_search(http_response,b"blah",b"blah"))
    print(httprequest_search(http_response,b"body",b"unknwn"))
    clear_httprequests()
if(sys.argv[1] == "gen"):
    message = create_whatsapp_message(b"individual",b"27769827148")
    Section1_Options = char_p_array(b"Sec1_Opt1",b"Sec1_Opt2");
    Section2_Options = char_p_array(b"Sec2_Opt1",b"Sec2,Opt2");
    Section1 = create_section(b"Section 1",Section1_Options,len(Section1_Options))
    Section2 = create_section(b"Section 2",Section2_Options,len(Section2_Options))
    Sections = int_p_array(Section1,Section2)
    actions = create_action_list(b"quick reply",Sections,len(Sections))
    make_interactive_message(message,b"head ^o^",b"Select an OPTION",b"foot =)",actions)
    post_data(b"127.0.0.1",Headers,len(Headers),whatsapp_message_to_data(message))
    clear_messages()

         
    

'''
from libJHBotBindings import *

Headers = char_p_array(b"header1", b"header2");
listner_server = create_new_listner_server(socket.AF_INET,socket.SOCK_STREAM,0,socket.INADDR_ANY,80,10)
while(True):
    response = launch_listner_server(listner_server)
    
    #decision making based on response behaviour
    str = httprequest_search(response,"part","field")
     
    data = create_whatsapp_text_message(b"individual",b"27832732630",b"HELLO")

    post_data(b"127.0.0.1",Headers,2,data)

    clear_httprequests()
    clear_messages()
clear_servers()

'''


