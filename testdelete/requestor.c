#include<stdio.h>
#include<curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <string>
//#include <iostream>
#define OK   0
#define INIT_ERR 1
#define REQ_ERR  2

#define URL    "http://localhost:8000"
using namespace std;
string g_buf = "";
size_t http_callback(void *buffer, size_t size, size_t count, void *user_p)
{
    g_buf += (char*)buffer;
    return size*count;
}
int main(void){
	CURL  *curl;
	CURLcode res;
	struct stat file_info;
    	double speed_upload, total_time;
    	FILE *fd;
	curl=curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl,CURLOPT_CUSTOMREQUEST,"DELETE");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&http_callback);
		curl_easy_setopt(curl,CURLOPT_URL,URL);
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"username=huancong&password=doyou");
		//curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
		res=curl_easy_perform(curl);
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
	return OK;
}
