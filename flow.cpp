#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

//This code updates the manifest.m3u8 file every x seconds (where x is the duration of a chunk) effectively simulating a live stream.
//All the chunks are actually already present in the simulations but they won't be present in a real live,
//for this reason the manifest is updated every x seconds inserting a new chunk as if it were actually created.

int main(){
        int i=7096037; //first chunk
        while(1){
                ofstream myfile;
                myfile.open ("chunks/manifest.m3u8");
                myfile << "" <<
                "#EXTM3U\n"
                "#EXT-X-VERSION:3\n"
                "#EXT-X-TARGETDURATION:2\n"
                "#EXT-X-MEDIA-SEQUENCE:"<<i<<"\n"
                "#EXTINF:1.920,\n"
                "manifest_3_"<<i<<".ts\n"
                "#EXTINF:1.920,\n"
                "manifest_3_"<<(i+1)<<".ts\n"
                "#EXTINF:1.920,\n"
                "manifest_3_"<<(i+2)<<".ts\n"
                "#EXTINF:1.920,\n"
                "manifest_3_"<<(i+3)<<".ts\n"
                "#EXTINF:1.920,\n"
                "manifest_3_"<<(i+4)<<".ts";
                myfile.close();
                i++;
                if(i+4==7096101){  //When the chunks run out it restarts from the first one.
                        i=7096037; //This happens in the manifest but in the players the video 
                                   //freezes and you have to reload the page.
                }
                usleep(2*1000000); //x = 2 secondi, #EXT-X-TARGETDURATION in manifest.m3u8 (chunk duration)
        }
}