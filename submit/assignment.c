#include<stdio.h>
#include<curl/curl.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>


#define OK    0
#define INIT_ERR  1
#define REQ_ERR   2

void httpGET(CURL *curl){
	curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
}
void httpPOST(CURL *curl, char * postdata){
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
void httpPUT(CURL *CURL, struct stat file_info, FILE *hd_src){
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
	
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
		 curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);
}
void httpDELETE(CURL curl){
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
}
void help_doc(){
	printf("help menu");
}
int main(int argc, char **argv){
	char *url="url";
	char*filename="file";
	char*postmessage="message";
	int operation=10;//error operation
	int index=1;
	FILE * hd_src;
	struct stat file_info;
	while(index<argc){
		if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0){
			operation=0;
			break;
		}
		else if(strcmp(argv[i],"-o")==0 || strcmp(argv[i],"--post")==0){
			if(index <= argc-2){
				postmessage = argv[i+1];//get the postmessage;
					operation = 1;//POST message;
			}
			else {
				operation=10;//error
			}
		}
		else if(strcmp(argv[i],"-g")==0 || strcmp(argv[i],"--get")==0){
			operation=2;//GET
		}
		else if(strcmp(argv[i],"-p")==0 || strcmp(argv[i],"--put")==0){
			if(index <= argc-2){
				operation=3//PUT
				filename=argv[i+1];
				stat(filename,&file_info);
				hd_src=fopen(filename,"rb");
			}
			
		}
		else if(strcmp(argv[i],"-d")==0 || strcmp(argv[i],"--delete")==0){
			operation=4;//DELET
		}
		else operation=10;
		
	}
	if(operation==10){
		return INIT_ERR;
	}
	if(operation==0){
		help_doc();
		return 0;//not sure;
	}
	CURL *curl;
	CURLCode res;
	curl=curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		if(operation==10){
			printf("bad input");
			return INIT_ERR;
		} 
		if(operation==1){
			httpPost(curl,postmessage);
			
		}
		if(operation==2){
			httpGET(curl);
		}
		if(operation==3){
			httpPUT(curl,file_info,hd_src);
		}
		if(operation==4){
			httpDelete(curl);
		}
		res=curl_easy_perform(curl);
		long http_information=0;
		curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_information);
		print("%s %d\n","RESPONSE CODE IS: ", http_information);
		if(res!=CURLE_OK){
			print(" REQ_ERR");
			return REQ_ERR;
		}	
		curl_easy_cleanup(curl);	
	}else{
		print("error");
		return INIT_ERR;
	}
	
	return OK;
	
}
