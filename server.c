#include "coap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


#define Data_lengh 50
#define Query_lengh 1000 

/*判斷資源是否啟用*/
int tempjudge = 1 ,infojudge = 1;

/*     讀取CPU的溫度         */
float CoreTemperature(char Core0[Data_lengh], char Core1[Data_lengh], char Core2[Data_lengh], char Core3[Data_lengh])  
{
  /*     for Core 0         */
  FILE *temp_core0;
  temp_core0 = popen("sensors | grep 'Core 0' | awk '{printf $3}'","r");
  if(fgets(Core0, Data_lengh, temp_core0)!=NULL);
  pclose(temp_core0);

/*     for Core 1         */
  FILE *temp_core1;
  temp_core1 = popen("sensors | grep 'Core 1' | awk '{printf $3}'","r");
  if(fgets(Core1, Data_lengh, temp_core1)!=NULL);
  pclose(temp_core1);

/*     for Core 2         */
  FILE *temp_core2;
  temp_core2 = popen("sensors | grep 'Core 2' | awk '{printf $3}'","r");
  if(fgets(Core2, Data_lengh, temp_core2)!=NULL);
  pclose(temp_core2);

/*     for Core 3         */
  FILE *temp_core3;
  temp_core3 = popen("sensors | grep 'Core 3' | awk '{printf $3}'","r");
  if(fgets(Core3, Data_lengh, temp_core3)!=NULL);
  pclose(temp_core3);
}


/*讀取 CPU 資訊*/
float Coreinfo(char Name[Data_lengh], char Cache[Data_lengh], char Mem[Data_lengh])
{

  /*   CPU processor name    */
  FILE *temp_name;
  temp_name = popen("x86info | grep 'Processor name string' | cut -d':' -f2 | awk {'printf $0'}","r");
  if(fgets(Name, Data_lengh, temp_name)!=NULL);
  pclose(temp_name);

  /*   CPU cache size    */
  FILE *temp_cache;
  temp_cache = popen("cat /proc/cpuinfo | grep 'cache size' | sed '2,$d' | awk '{printf $4}'","r");
  if(fgets(Cache, Data_lengh, temp_cache)!=NULL);
  pclose(temp_cache);

  /*   CPU memeory size    */
  FILE *temp_mem;
  temp_mem = popen(" cat /proc/meminfo | grep 'MemTotal' | awk '{printf $2}'","r");
  if(fgets(Mem, Data_lengh, temp_mem)!=NULL);
  pclose(temp_mem);
}


/*
 * The resource handler
 */

/*CPU 溫度 handler*/ 
static void
Coretemp_get_handler(coap_context_t *ctx, struct coap_resource_t *resource, 
              const coap_endpoint_t *local_interface, coap_address_t *peer, 
              coap_pdu_t *request, str *token, coap_pdu_t *response) 
{

	if(tempjudge == 1)
	{
		char Core0[Data_lengh]={0},Core1[Data_lengh]={0},Core2[Data_lengh]={0},Core3[Data_lengh]={0};
		CoreTemperature(Core0,Core1,Core2,Core3);

		strcat(Core0, Core1);
		strcat(Core0, Core2);
		strcat(Core0, Core3);

		const char* sent;
		sent=&Core0[0];
		unsigned char buf[3];
		const char* response_data     =sent;
		response->hdr->code           = COAP_RESPONSE_CODE(205);
		coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
		coap_add_data  (response, strlen(response_data), (unsigned char *)response_data);
	}
	else
	{
				response->hdr->code           = COAP_RESPONSE_CODE(404);
	}
	
}

static void
Coretemp_delete_handler(coap_context_t *ctx, struct coap_resource_t *resource, 
              const coap_endpoint_t *local_interface, coap_address_t *peer, 
              coap_pdu_t *request, str *token, coap_pdu_t *response) 
{
	if(tempjudge==1)
	{
		tempjudge = 0 ;
		response->hdr->code           = COAP_RESPONSE_CODE(202);
	}
	else
	{
		response->hdr->code           = COAP_RESPONSE_CODE(404);
	}
	

}
static void
Coretemp_post_handler(coap_context_t *ctx, struct coap_resource_t *resource, 
              const coap_endpoint_t *local_interface, coap_address_t *peer, 
              coap_pdu_t *request, str *token, coap_pdu_t *response) 
{
	if(tempjudge==0)
	{
		tempjudge = 1 ;
		response->hdr->code           = COAP_RESPONSE_CODE(201);
	}
	else
	{
		response->hdr->code           = COAP_RESPONSE_CODE(404);
	}
	

}

/*CPU 資訊 handler*/
static void
Coreinfo_get_handler(coap_context_t *ctx, struct coap_resource_t *resource, 
              const coap_endpoint_t *local_interface, coap_address_t *peer, 
              coap_pdu_t *request, str *token, coap_pdu_t *response) 
{

	if(infojudge == 1)
	{
		char Name[Data_lengh]={0},Cache[Data_lengh]={0},Mem[Data_lengh]={0};
		Coreinfo(Name,Cache,Mem);

		strcat(Name, Cache);
		strcat(Name, Mem);

		const char* sent;
		sent=&Name[0];
		unsigned char buf[3];
		const char* response_data     =sent;
		response->hdr->code           = COAP_RESPONSE_CODE(205);
		coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
		coap_add_data  (response, strlen(response_data), (unsigned char *)response_data);
	}
	else
	{
				response->hdr->code           = COAP_RESPONSE_CODE(404);
	}
	
}
static void
Coreinfo_delete_handler(coap_context_t *ctx, struct coap_resource_t *resource, 
              const coap_endpoint_t *local_interface, coap_address_t *peer, 
              coap_pdu_t *request, str *token, coap_pdu_t *response) 
{
	if(infojudge==1)
	{
		infojudge = 0 ;
		response->hdr->code           = COAP_RESPONSE_CODE(202);
	}
	else
	{
		response->hdr->code           = COAP_RESPONSE_CODE(404);
	}
	

}
static void
Coreinfo_post_handler(coap_context_t *ctx, struct coap_resource_t *resource, 
              const coap_endpoint_t *local_interface, coap_address_t *peer, 
              coap_pdu_t *request, str *token, coap_pdu_t *response) 
{
	if(infojudge==0)
	{
		infojudge = 1 ;
		response->hdr->code           = COAP_RESPONSE_CODE(201);
	}
	else
	{
		response->hdr->code           = COAP_RESPONSE_CODE(404);
	}
	

}



int main(int argc, char* argv[])
{
	coap_context_t*  ctx;
	coap_address_t   serv_addr;
	coap_resource_t* temp_resource;
	coap_resource_t* info_resource;
	fd_set           readfds;    
	/* Prepare the CoAP server socket */ 
	coap_address_init(&serv_addr);
	serv_addr.addr.sin.sin_family      = AF_INET;
	serv_addr.addr.sin.sin_addr.s_addr = INADDR_ANY;
	serv_addr.addr.sin.sin_port        = htons(5683); //default port
	ctx                                = coap_new_context(&serv_addr);
	if (!ctx) exit(EXIT_FAILURE);
	/* Initialize the temp resource */
	//初始化URL和resource的名稱 coap_resource_init(名稱,長度,flag);

		/*temperature resource*/
	temp_resource = coap_resource_init((unsigned char *)"temperature", 11, 0);
	coap_register_handler(temp_resource, COAP_REQUEST_GET, Coretemp_get_handler);
	coap_register_handler(temp_resource, COAP_REQUEST_DELETE, Coretemp_delete_handler);
	coap_register_handler(temp_resource, COAP_REQUEST_POST, Coretemp_post_handler);	
	coap_add_resource(ctx, temp_resource);

		/*info resource*/
	info_resource = coap_resource_init((unsigned char *)"information", 11, 0);
	coap_register_handler(info_resource, COAP_REQUEST_GET, Coreinfo_get_handler);
	coap_add_resource(ctx, info_resource);

	/*Listen for incoming connections*/
	while (1) {
		FD_ZERO(&readfds);
		FD_SET( ctx->sockfd, &readfds );
		int result = select( FD_SETSIZE, &readfds, 0, 0, NULL );
		if ( result < 0 ) /* socket error */
		{
			exit(EXIT_FAILURE);
		} 
		else if ( result > 0 && FD_ISSET( ctx->sockfd, &readfds )) /* socket read*/
		{	 
				coap_read( ctx );      
		} 
	}    
}
