from libJHBot import *
import sys
FaceBook_URL = b"https://graph.facebook.com/v17.0/149485694924975/messages"

Header_Autorization = b"Authorization: Bearer EAAzl9nSKvWYBO0LY3NfQYtZC6s4UcofwzpZAnhPt0MHKoDIdF6TQGaLbe6gZBvkuFkHd42yRTtHWZAaJ870AJTOoZB9MfJHhSQJit13eLDUj19vReoaSAv4HBRPVs2nlqAbWW8QcetaodPDsYMcHMFafFNE10Nuxi1HtgtQrTE5JPMk3ksmk08y2hu76C6GkyHp7JSymZC6hObalsxdaCjBN5y6Rmg"

Header_ContentType = b"Content-Type: application/json"

Headers = (c_char_p*2)()
Headers[0] = c_char_p(Header_Autorization)
Headers[1] = c_char_p(Header_ContentType)

if(sys.argv[1] == "send"):
    print("SEND TEST")
    data = read_text_file(b"../../samples/json.txt")
    post_data(b"127.0.0.1",Headers,2,data)

if(sys.argv[1] == "server"):
    print("SERVER TEST")
    initialize_bot();
    listner_server = create_new_listner_server(socket.AF_INET,socket.SOCK_STREAM,0,socket.INADDR_ANY,80,10)
    while(True):
        http_response =  launch_listner_server(listner_server)
        print(httprequest_search(http_response,b"body",b"messages.from"))
        print(httprequest_search(http_response,b"body",b"messages.body.text"))
        print(httprequest_search(http_response,b"blah",b"blah"))
        print(httprequest_search(http_response,b"body",b"unknwn"))
    clear_servers()
    clear_httprequests()
if(sys.argv[1] == "parse"):
    print("PARSE TEST")
    data = read_text_file(b"../../samples/sample.txt")
    http_response = parse_httprequest(data);
    print(httprequest_search(http_response,b"body",b"messages.from"))
    print(httprequest_search(http_response,b"body",b"messages.body.text"))
    print(httprequest_search(http_response,b"blah",b"blah"))
    print(httprequest_search(http_response,b"body",b"unknwn"))
    clear_httprequests();
if(sys.argv[1] == "gen"):
    print(create_whatsapp_template_message(b"group",b"wa-grp-156",b"bcs_first",b"en_US"))
    print(create_whatsapp_text_message(b"individual",b"27769827148",b"Hello Hope you have a good day"))

    
    

'''
initialize_bot()
listner_server = create_new_listner_server(socket.AF_INET,socket.SOCK_STREAM,0,socket.INADDR_ANY,80,10)
while(True):
    response = launch_listner_server(listner_server)
    
    #decision making based on response behaviour
    str = httprequest_search(response,"part","field")
    
    data = read_text_file(b"../../samples/json.txt")
    data = create_whatsapp_text_message(recipient_type, to, body)
    post_data(b"127.0.0.1",Headers,2,data)
    clear_httprequests()
clear_servers()
'''



