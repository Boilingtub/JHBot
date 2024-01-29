
# libJHBot
Online Bot with the ability to: \
receive Http requests from an online source. \
Proccess the http , can parse Json into a dictionary for dicision making. \
through Curl send a message to any other online destination \

# Compile from source
##  Dependencies
- gcc (version 12.2 recommended) **Compile-time**
- libcurl-devel **Compile-time**
- libcurl **Run-time**

### Linux
run compile.sh script from root directory of project
### Windows
run compile.bat script from root directory of project \
_**note : Please copy libJHBot.dll manually from objects/ to python_implementation/**_  \

# Python implementation
## Dependencies
Python V 3.12
## Functions available
- #### read_text_file( string file_path ) -> string
  - arguments : 
    - string (pointer to byte array) 
  - returns :
      - string (pointer to byte array)
   - description : 
     - simply reads a text file and return the contents of the file into a string
    - example:
      - ```
        file_content = read_text_file(b"path/to/text/file")
        ```
- #### write_text_file( string file_path , string data) -> Nothing
  - arguments : 
    - string (pointer to byte array) 
    - string (pointer to byte array)
  - returns :
      - Nothing
   - description : 
     - simply writes text to a text_file (does not append)
    - example:
      - ```
        write_text_file(b"path/to/text/file",b"string to write into text file")
        ```
- #### post_data(string URL, string array Headers[], int Header_count, string data) -> Integer
  - arguments : 
    - string (pointer to byte array) 
    - string array (array of pointer to byte array)
    - integer
    - string
  - returns :
      - Integer (simply and error code , if 0 success , if 1 failure)
   - description : 
     - sends Headers and data to URL
    - example:
      - ```
        Headers = char_p_array(b"Header 1",b"Header 2"....)
        post_data(b"http://url.to.send.to",Headers,len(Headers), data)
        ```
- #### create_new_listner_server(int domain, int service, int protocol, long interface, int port, int backlog) -> Integer
  - arguments : 
    - integer (socket.AF_INET)
    - integer (socket.SOCK_STREAM)
    - integer (0)
    - integer (socket.INADDR_ANY)
    - integer port (1 - 64738) can use any available
    - integer backlog (10)
  - returns :
      - Integer (used to adress specific server)
   - description : 
     - creates server to be used for listening to information entering from a specific port on the network
    - example:
      - ```
        listner_server = create_new_listner_server(socket.AF_INET, socket.SOCK_STREAM, 0, socket.INADDR_ANY, 80, 10)
        ```
- #### launch_listner_server(int select) -> Integer
  - arguments : 
    - integer (adresses server to be launched)
  - returns :
    - Integer (used to adress the received http request)
  - description : 
    - launches a server to listen to a specific port
  - example:
    - ```
      http_response = launch_listner_server(listner_server)
      ```
- #### parse_httprequest(string data) -> Integer
  - arguments : 
    - string (data that should be parsed)
  - returns :
    - Integer (used to adress the processed http request)
  - description : 
    - used to parse data received else where that is not from a listner_server
  - example:
    - ```
      #receive data form other source
      data = read_text_file(b"path/to/file")
      http_response = parse_httprequest(string data)
      ```
- #### httprequest_search(int select, string part, string field) -> string
  - arguments : 
    - integer (adresses http request to be searched)
    - string (part of request to be searched through. Usually only ever "request_line" , "header_fields" or "body")
    - string (field to search for , uses "**.**" notation,  if field is left empty function will return _NULL_ and print all fields in said part)
  - returns :
    - string (value of key searched for)
  - description : 
    - search for the value of a specific field in the Http_request
  - example:
    - ```
      message_text = httprequest_search(http_response,b"body",b"messages.body.text")
      ```
- #### create_whatsapp_message(string recipient_type , string to) -> Integer
  - arguments : 
    - String (type of recipient either "individual" or "group")
    - String (phone number if recipient_type is "individual" or group id if recipiet_type is "group")
  - returns :
    - Integer (used to adress the message that was created)
  - description : 
    - creates the base object of a whatsapp message, used with later functions such as Make_Text() to give message further features
  - example:
    - ```
      whatsapp_message = create_whatsapp_message(b"individual", b"2765876908")
      ```
- #### make_template_message(int message_index, string template_name, string language_code) -> Nothing
  - arguments : 
    - integer (adresses message to be made into a template message)
    - string (the name of th template that was made on metas official website for the app https://business.facebook.com/wa/manage/message-templates/?business_id=7278286912202252&waba_id=190578137475255&filters=%7B%22search_text%22%3A%22%22%2C%22tag%22%3A[]%2C%22language%22%3A[]%2C%22status%22%3A[%22APPROVED%22%2C%22IN_APPEAL%22%2C%22PAUSED%22%2C%22PENDING%22%2C%22REJECTED%22]%2C%22quality%22%3A[]%2C%22date_range%22%3A30%2C%22sort_direction%22%3A%22descending%22%2C%22sort_key%22%3A%22lastUpdatedTime%22%7Dhttps://developers.facebook.com/apps/3630546400492902/whatsapp-business/wa-dev-console/?business_id=7278286912202252)
  - returns :
    - Nothing
  - description : 
    - Make the message passed into a Template message. Warning: Once a message has been made into a type it cannot be changed. Rather make a new message
  - example:
    - ```
      message = create_whatsapp_message(b"individual",b"27058475493")
      make_template_message(message, b"hello_world", b"en_US")
      ```
- #### make_text_message(int message_index, string body) -> Nothing
  - arguments : 
    - integer (adresses message to be made into a text message)
  - returns :
    - Nothing
  - description : 
    - Make the message passed into a simple whatsapp text message. Warning: Once a message has been made into a type it cannot be changed. Rather make a new message
  - example:
    - ```
      message = create_whatsapp_message(b"individual",b"27058475493")
      make_text_message(message, b"text of message")     
       
       # do something else
      
      clear_messages()
      ```
- #### make_reply_message(integer message_index, string message_id) -> Nothing
  - arguments : 
    - integer (used to adress message to be made into a reply)
    - String (Id of the message that you wish to reply to)
  - returns :
    - Nothing
  - description : 
    - makes any message you pass in to this function into a reply message
  - example:
    - ```
      whatsapp_message = create_whatsapp_message(b"individual", b"2765876908")
      make_text_message(whatsapp_message, b"I whould please like to reply to you")
      make_reply_message(whatsapp_message,b"ABGGFlA5FpafAgo6tHcNmNjXmuSf"")
     ```

- #### create_section(string title, char_p_array options, integer option_count) -> Integer
  - arguments : 
    - string (title of the section in the menu)
    - char_p_array (the name of every option)
    - integer (the amount of options is this section)
  - returns :
    - integer (used to refer to the new section that was created)
  - description : 
    - creates a section that can be used to construct an action
  - example:
    - ```
             message = create_whatsapp_message(b"individual",b"27058475493")
      Quote_options = char_p_array(b"request a quote", b"call quotes       manager")
      information_options = char_p_array(b"request information", b"call human")
      Quote_section = create_section(b"quotes", Quote_options, len(Quote_options));
      information_section = create_section(b"information", information_options, len(information_options));
      Sections = int_p_array(Quote_section,information_section)
      actions = create_action_list(b"request", Sections, len(Sections))
      make_interactive_message(message,b"Get information",b"please select any one of the following options", b"thank you for contacting us", actions) 
      ```

- #### create_action(string title, int_p_array sections, integer section_count) -> integer
  - arguments : 
    - string (title of action button)
    - int_p_array (array of all the sections to be used)
    - integer (amount of sections to be used)
  - returns :
    - integer (used to refer to the newly created action)
  - description : 
    - create an action with multiple sections to be used in the creation of an interactive message
  - example:
    - ```
                   message = create_whatsapp_message(b"individual",b"27058475493")
      Quote_options = char_p_array(b"request a quote", b"call quotes       manager")
      information_options = char_p_array(b"request information", b"call human")
      Quote_section = create_section(b"quotes", Quote_options, len(Quote_options));
      information_section = create_section(b"information", information_options, len(information_options));
      Sections = int_p_array(Quote_section,information_section)
      actions = create_action_list(b"request", Sections, len(Sections))
      make_interactive_message(message,b"Get information",b"please select any one of the following options", b"thank you for contacting us", actions)
      ```

- #### make_interactive_message(int message_index , string header, string body, string footer, int action_index) -> Nothing
  - arguments : 
    - integer (index of message to be turned into a interactive message)
    - string (text of header)
    - string (text of body)
    - string (text of footer)
    - integer (index of action to be added to interactive message)
  - returns :
    - Nothing
  - description : 
    - Make the message passed into an interactive whatsapp message, can be interacted with. consists of and action wich consists of sections.
  - example:
    - ```
      message = create_whatsapp_message(b"individual",b"27058475493")
      Quote_options = char_p_array(b"request a quote", b"call quotes manager")
      information_options = char_p_array(b"request information", b"call human")
      Quote_section = create_section(b"quotes", Quote_options, len(Quote_options));
      information_section = create_section(b"information", information_options, len(information_options));
      Sections = int_p_array(Quote_section,information_section)
      actions = create_action_list(b"request", Sections, len(Sections))
      make_interactive_message(message,b"Get information",b"please select any one of the following options", b"thank you for contacting us", actions)     
      
      #do something else

      clear_messages()
      ```

- #### whatsapp_message_to_string(int message_index) -> string
  - arguments : 
    - integer (adresses message to be converted to a data string)
  - returns :
    - String (string data of message , used for sending whatsapp messages as data , use with pos_data message)
  - description : 
    - converts a whatsapp message to a string for whatsapp messages over a network
  - example:
    - ```
      message = create_whatsapp_message(b"type of recipient",b"phone number / group id of recipient")
      message_data = whatsapp_message_to_string(message)
      post_data(b"url.to.post.to", Headers, len(Headers), message_data)
      ```

- #### clear_httprequests() -> Nothing
  - arguments : 
    - Nothing
  - returns :
    - Nothing
  - description : 
    - clears all processed http requests, **PLEASE** make sure to use this function when done with all necessary http requests, helps to save on computer resources.
  - example:
    - ```
        #create and use a bunch of http requests
        clear_httprequests()
      ```

- #### clear_servers() -> Nothing
  - arguments : 
    - Nothing
  - returns :
    - Nothing
  - description : 
    - clears all processed clears all created listner servers **PLEASE** use this function after you have received all data you require from all active servers , helps to save on computer resources
  - example:
    - ```
        #create and use a bunch of listner servers
        clear_servers()
      ```

- #### clear_messages() -> Nothing
  - arguments : 
    - Nothing
  - returns :
    - Nothing
  - description : 
    - clears all created whatsapp messages, **PLEASE** use this function after you have send all necessary whatsapp messages , helps to save on computer resources 
  - example:
    - ```
        #create and use a bunch of whatsapp messages
        clear_messages()
      ```



## Examples
### Basic Structure

```
from libJHBotBindings import *
   
Headers = char_p_array(b"header1", b"header2");
listner_server = create_new_listner_server(socket.AF_INET,socket.SOCK_STREAM,0,socket.INADDR_ANY,80,10)
while(True):    
    response = launch_listner_server(listner_server)
      
    #decision making based on response behaviour
    str = httprequest_search(response,"part","field")
  
    #get data to send
    data = read_text_file(b"path/to/text/file")
   
    #send data to some link
    post_data(b"url.to.destination",Headers,2,data)
                  
    clear_httprequests() #clear processed httprequests 
 clear_servers() # clear servers that can be used;
```
### Generate Whatsapp message
```
    message = create_whatsapp_message("")
```
```

```
