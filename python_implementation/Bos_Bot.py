from libJHBotBindings import *

Facebook_Url = b"https://graph.facebook.com/v17.0/149485694924975/messages"\

Autorization_Header = b"Authorization: Bearer EAAzl9nSKvWYBO4x2LQCZB5qxWysyADDzR4pwYUFyjC7E5vFIVgZBCdqBKlARoZCyjnt4ZBuSCWZAMoGv8pS3yj0H0PcE24cQmYOqrRIL85ZBGCZAnn83HcBEHqUMvyETW4ez9R2O1K1MuVZBVBeWjSpTOf0jOV5IvkWH9YM0G8jA0h1z6wqunXSTcRmCvZBu4JJXQHQsRPwdpjzNaefO6XM2rZAwkhXGUZD"

ContentType_Header = b"Content-Type: application/json"

Headers = char_p_array(Autorization_Header,ContentType_Header)

listner_server = create_new_listner_server(socket.AF_INET, socket.SOCK_STREAM, 0, socket.INADDR_ANY, 80 , 10)
while(True):
    http_request = launch_listner_server(listner_server)

    sender_phone_number = httprequest_search(http_request, b"body",b"")
    sender_message_body = httprequest_search(http_request,b"body",b"")

    if(sender_phone_number == "27769827148"):
        print("Jan-Hendrik sent a message")
    else:
        automatic_response_message = create_whatsapp_message("individual",sender_phone_number)
        make_template_message(automatic_response_message,"bcs_first","en_US")
        automatic_response_message_data = whatsapp_message_to_data(automatic_response_message)
        
        post_data(Facebook_Url,Headers,len(Headers),automatic_response_message_data)
    
    clear_messages()
    clear_httprequests()
