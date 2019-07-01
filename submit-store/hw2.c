#include<stdio.h>
#include<curl/curl.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>


#define OK    0
#define INIT_ERR  1
#define REQ_ERR   2

void httpGET(CURL *curl){
	printf("GET\n");
	curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
}
void httpPOST(CURL *curl, char * postdata){
	printf("POST\n");
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postdata);
}
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
void httpPUT(CURL *curl, struct stat file_info, FILE *hd_src){
	printf("PUT\n");
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
	
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
		 curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);
}
void httpDELETE(CURL *curl){
	printf("delete");
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
}
void help_doc(){
	printf("help menu");
}
int main(int argc, char **argv){
	CURL *curl;
	CURLcode res;
	char *url="url";
	char*filename="file";
	char*postmessage="message";
	int operation=10;//error operation
	int index=1;
	FILE * hd_src;
	struct stat file_info;
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
		curl_easy_setopt(curl, CURLOPT_URL, url);
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
		curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_information);
		printf("%s %ld\n","RESPONSE CODE IS: ", http_information);
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
