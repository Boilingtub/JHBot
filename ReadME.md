
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
     - creates server to be used for listening 
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
