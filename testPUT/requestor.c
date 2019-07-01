#include<stdio.h>
#include<curl/curl.h>
#include <fcntl.h>
#include <sys/stat.h>

#define OK   0
#define INIT_ERR 1
#define REQ_ERR  2

//#define URL    "http://0.0.0.0:80"
#define URL    "https://postb.in/1561923673161-1031376444734"

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
int main(void){
	CURL  *curl;
	CURLcode res;
	FILE * hd_src;
	struct stat file_info;
	char *file;
	file="puttask.txt";
	stat(file, &file_info);
	hd_src = fopen(file, "rb");
	
	curl=curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
		curl_easy_setopt(curl,CURLOPT_URL,URL);
		//curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
		 curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);
		res=curl_easy_perform(curl);
		printf("success");
		if(res!=CURLE_OK){
			printf("req error");
			return REQ_ERR;
		}
		curl_easy_cleanup(curl);
		printf("success");
	}else{
		printf("error");
		return INIT_ERR;

	}
	fclose(hd_src);
	return OK;
}
