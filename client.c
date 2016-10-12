#include "coap.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
/*
 * The response handler
 */ 

 #define Data_lengh 100


static void
message_temperature_handler(struct coap_context_t *ctx, const coap_endpoint_t *local_interface, 
                const coap_address_t *remote, coap_pdu_t *sent, coap_pdu_t *received, 
                const coap_tid_t id) 
{
	unsigned char* data;
	size_t         data_len;
	if (COAP_RESPONSE_CLASS(received->hdr->code) == 2) 
	{
		if (coap_get_data(received, &data_len, &data))
		{
			char templace[Data_lengh]={0}; //暫存器
			char Core0[Data_lengh]={0},Core1[Data_lengh]={0},Core2[Data_lengh]={0},Core3[Data_lengh]={0};
			int count=0;
			char *delim = "*";   //分割代表符號
  			char * pch;		//	strtok 用暫存指標
			
			//清空array
			memset(Core0,0,Data_lengh);
			memset(Core1,0,Data_lengh);
			memset(Core2,0,Data_lengh);
			memset(Core3,0,Data_lengh);

			sprintf(templace,"%s",data); // 將 unsigned char* 型態的 data 轉換成 char 
			pch = strtok(templace,delim); // 分割字串
  			
			/*存入各自的array*/
			while (pch != NULL)
  			{
				switch(count)
				{
					case 0:
						sprintf(Core0,"%s",pch);
					break;
					case 1:
						sprintf(Core1,"%s",pch);
					break;
					case 2:
						sprintf(Core2,"%s",pch);
					break;
					case 3:
						sprintf(Core3,"%s",pch);
					break;
				}
    			pch = strtok (NULL, delim);
				count++;
  			}
			printf("Core0: %s\n", Core0);
			printf("Core1: %s\n", Core1);
			printf("Core2: %s\n", Core2);
			printf("Core3: %s\n", Core3);
			printf("----------------------\n");
		}
	}
}

static void
message_information_handler(struct coap_context_t *ctx, const coap_endpoint_t *local_interface, 
                const coap_address_t *remote, coap_pdu_t *sent, coap_pdu_t *received, 
                const coap_tid_t id) 
{
	unsigned char* data;
	size_t         data_len;
	if (COAP_RESPONSE_CLASS(received->hdr->code) == 2) 
	{
		if (coap_get_data(received, &data_len, &data))
		{
			char templace[Data_lengh]={0}; //暫存器
			char Name[Data_lengh]={0},Cache[Data_lengh]={0},Mem[Data_lengh]={0};
			int count=0;
			char *delim = "*";   //分割代表符號
  			char * pch;		//	strtok 用暫存指標
			
			//清空array
			memset(Name,0,Data_lengh);
			memset(Cache,0,Data_lengh);
			memset(Mem,0,Data_lengh);

			sprintf(templace,"%s",data); // 將 unsigned char* 型態的 data 轉換成 char 
			pch = strtok(templace,delim); // 分割字串
  			
			/*存入各自的array*/
			while (pch != NULL)
  			{
				switch(count)
				{
					case 0:
						sprintf(Name,"%s",pch);
					break;
					case 1:
						sprintf(Cache,"%s",pch);
					break;
					case 2:
						sprintf(Mem,"%s",pch);
					break;
				}
    			pch = strtok (NULL, delim);
				count++;
  			}
			printf("Name: %s\n", Name);
			printf("Cache: %s\n", Cache);
			printf("Mem: %s\n", Mem);
			printf("----------------------\n");
		}
	}
}


int main(int argc, char* argv[])
{
	int chooserc,rcjudge = 0;
	const char*       server_uri;
	while(1)
	{
		/* 選擇 resource */
		printf("There exist 2 resource : \n 1. temperature \n 2. information \n please choose number : ");
		if(scanf("%d",&chooserc) == 1);
		switch(chooserc)
		{	
			case 1:
				server_uri= "coap://140.123.92.244/temperature";
				rcjudge = 0;
			break;
			case 2 :
				server_uri= "coap://140.123.92.244/information";
				rcjudge = 0;
			break;
			default :
				printf("There is no this resource\n");
				rcjudge = 1;
			break;
		}
		if(rcjudge == 0)
		{
			break;
		}
	}
	while(1)
	{
	coap_context_t*   ctx;
	coap_address_t    dst_addr, src_addr;
	static coap_uri_t uri;
	fd_set            readfds; 
	coap_pdu_t*       request;
	unsigned char     get_method = 1;
	/* Prepare coap socket*/
	coap_address_init(&src_addr);
	src_addr.addr.sin.sin_family      = AF_INET;
	src_addr.addr.sin.sin_port        = htons(0);
	src_addr.addr.sin.sin_addr.s_addr = inet_addr("0.0.0.0");
	ctx = coap_new_context(&src_addr);
	/* The destination endpoint */
	coap_address_init(&dst_addr);
	dst_addr.addr.sin.sin_family      = AF_INET;
	dst_addr.addr.sin.sin_port        = htons(5683);
	dst_addr.addr.sin.sin_addr.s_addr = inet_addr("140.123.92.244");
	/* Prepare the request */
	coap_split_uri(server_uri, strlen(server_uri), &uri);
	request            = coap_new_pdu();	
	request->hdr->type = COAP_MESSAGE_CON;
	request->hdr->id   = coap_new_message_id(ctx);
	request->hdr->code = get_method;
	coap_add_option(request, COAP_OPTION_URI_PATH, uri.path.length, uri.path.s);
	/* Set the handler and send the request */

	switch(chooserc)
	{
		case 1:
			coap_register_response_handler(ctx, message_temperature_handler);
		break;
		case 2:
			coap_register_response_handler(ctx, message_information_handler);
		break;
	}
	coap_send_confirmed(ctx, ctx->endpoint, &dst_addr, request);
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
	sleep(1);
	} 
  return 0;
}
