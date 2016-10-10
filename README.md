# About
A example to transmission some data use libcoap
there exit a server.c file that can deliver:
 
* cpu core temperature
* cpu information (cpu name,cache size,memory size)

client use a plugin on firefox 
Copper : https://addons.mozilla.org/en-US/firefox/addon/copper-270430/
 
 #funtion
 Can use GET to get data , DELETE to delete the resource , POST to create resource 
 
#libcoap install
 
* git clone https://github.com/obgm/libcoap.git
* cd libcoap
* ./autogen.sh (if appear some warning, just follow the instructions to install)
* ./configure --disable-examples
* make
* sudo make install
* change configuration : /home/XXX/.bashrc --->  ADD " export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH " 

#kit install

##lm-sensors

* sudo apt-get install lm-sensors

##x86info

* sudo apt-get install x86info
    
#example

* git clone https://github.com/wifferlin/libcoap-basic.git
* cd libcoap-basic
* make
* ./server
* open firefox key in coap://127.0.0.1
