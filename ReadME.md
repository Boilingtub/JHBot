
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
