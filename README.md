# ServerHLS - Broadband

<p align="center">
    <img src="https://i.imgur.com/mPb3Qbd.gif" width="180" alt="Politecnico di Milano"/>
</p>

## Overview
This project consists of the creation of a ServerHLS for testing use. 

The Flow.cpp file simulates the generation of the video stream, as the HLS format should be used for live video broadcasts. Instead, the Server.cpp file is responsible for managing connections; it was implemented using a forking mechanism that allows multiple connections to be managed simultaneously.

* Within the flow.cpp and server.cpp files is a brief explanation of how the code works
* Some chunks are a bit corrupted so the video may have some slight disturbance
* When they finish the chunks the flow automatically restarts from the first one, most players however require a refresh to return to view the video

<p align="center">
 <a href="https://github.com/PietroValente/ServerHLS-thesis/blob/main/image/overview.png"><img src="https://github.com/PietroValente/ServerHLS-thesis/blob/main/image/overview.png" alt="" width="100%"></a>
</p>

## Usability

1. Open a terminal in the folder
2. Compile the files:

   g++ server.cpp -o server  
   g++ flow.cpp -o flow  
  
3. Run on two different terminals(independent the order):

   ./server  
   ./flow
  
5. Access the address http://localhost:8080/index.m3u8
