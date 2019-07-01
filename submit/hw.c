#include<stdio.h>
#include<curl/curl.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
/* Compilation Commands
[x86]:
gcc -c hw.c -o hw.o
gcc -o hw hw.o -L/usr/lib/x86_64-linux-gnu -lcurl -lpthread
[arm]:
${BUILDROOT_HOME}/output/host/usr/bin/arm-linux-gcc --sysroot=${BUILDROOT_HOME}/output/staging  -c hw.c -o hw.o
${BUILDROOT_HOME}/output/host/usr/bin/arm-linux-gcc --sysroot=${BUILDROOT_HOME}/output/staging  -o hw hw.o  -lcurl -uClibc -lc
*/

#define OK    0
#define INIT_ERR  1
#define REQ_ERR   2
//This is the httpGET method.
void httpGET(CURL *curl){
	//printf("GET\n");
	curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
}
//This method will send HTTP POST.
void httpPOST(CURL *curl, char * postdata){
	//printf("POST\n");
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postdata);
}
//This is the read method used for httpPUT;
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  curl_off_t nread;
 
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */ 
  retcode = fread(ptr, size, nmemb, stream);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
 
  return retcode;
}
//This executes HTTP PUT;
void httpPUT(CURL *curl, struct stat file_info, FILE *hd_src){
	//printf("PUT\n");
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_PUT, 1L);
	
	curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);
}
//This executes HTTP DELETE;
void httpDELETE(CURL *curl){
	printf("delete");
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
}
//This will show the help information;
void help_doc(){
	printf("\n This is a C program that sends HTTP requests to some url.\n");
	printf("This program support the following command line options.\n");
	printf("-h  or --help will displace the help information about this program.\n");
	printf("-u  or --url tells the URL of the server that receives our HTTP requests.\n");
	printf("-g  or --get is to send a HTTP GET request to server.\n");
	printf("-p or --put is to send a HTTP PUT request to server. Note that the filename must be specified.\n");
	printf("-o or --post is to send a HTTP POST request to the server. Note that the posted message must be specified.\n");
	printf("-d or --delete is to send a HTTP DELTEE request to the server.\n");
	printf("The following are some example input:\n");
	printf("./hw -g --url https://postb.in/1562009753806-4109143195673\n");
	printf("./hw -p command.txt --url https://postb.in/1562009753806-4109143195673\n");
	printf("./hw -o This --url https://postb.in/1562009753806-4109143195673\n");
	printf("./hw -d --url https://postb.in/1562009753806-4109143195673\n");

}
int main(int argc, char **argv){
	CURL *curl;
	CURLcode res;
	char *url="url";//This is the string to store the url. 
	char*filename="file";//This is the string representing the filename to put. 
	char*postmessage="message";//This is the message we want to post to the server. 
	int operation=10;//error operation. I use an integer to represent the type of operation. 
	//0: help   1:POST  2:GET  3:PUT   4: DELETE
	int index=1;//The index of the command line arguments.
	FILE * hd_src;
	struct stat file_info;
	//The following while loop is used to parse the command line options. In Linux, getopt and getopt_long() method can be used to deal with command
	//line efficiently. But I am not familiar with that method, so I use the following method. 
	while(index<argc){
		if(strcmp(argv[index],"-h")==0 || strcmp(argv[index],"--help")==0){
			operation=0;
			break;
		}
		else if(strcmp(argv[index],"-o")==0 || strcmp(argv[index],"--post")==0){
			if(index <= argc-2){
				postmessage = argv[index+1];//get the postmessage;
					operation = 1;//POST message;
			}
			else {
				operation=10;//error
				printf("bad input\n");
			}
		}
		else if(strcmp(argv[index],"-g")==0 || strcmp(argv[index],"--get")==0){
			operation=2;//GET
		}
		else if(strcmp(argv[index],"-p")==0 || strcmp(argv[index],"--put")==0){
			if(index <= argc-2){
				operation=3;//PUT
				filename=argv[index+1];
				stat(filename,&file_info);
				hd_src=fopen(filename,"rb");
			}else{
				printf("bad input\n");	
			}
			
		}
		else if(strcmp(argv[index],"-d")==0 || strcmp(argv[index],"--delete")==0){
			operation=4;//DELET
		}
		else if(strcmp(argv[index],"-u")==0 || strcmp(argv[index],"--url")==0){
			url=argv[index+1];
		}
		//else operation=10;
		index++;
	}
	if(operation==10){
		return INIT_ERR;
	}
	if(operation==0){
		help_doc();
		return 0;//not sure;
	}
	
	curl=curl_easy_init();
	if(curl){
		printf("url is %s",url);
		curl_easy_setopt(curl, CURLOPT_URL, url);//specify the URL
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);//This is for arm. 
		printf("success");
		if(operation==10){
			printf("bad input");
			return INIT_ERR;
		} 
		if(operation==1){
			httpPOST(curl,postmessage);
			
		}
		if(operation==2){
			httpGET(curl);
		}
		if(operation==3){
			httpPUT(curl,file_info,hd_src);
		}
		if(operation==4){
			httpDELETE(curl);
		}
		res=curl_easy_perform(curl);
		long http_information=0;
		curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_information);//get the http information. 
		printf("%s %ld\n","\nRESPONSE CODE IS: ", http_information);
		if(res!=CURLE_OK){
			printf(" REQ_ERR");
			return REQ_ERR;
		}	
		curl_easy_cleanup(curl);	
	}else{
		printf("error");
		return INIT_ERR;
	}
	
	return OK;
	
}
