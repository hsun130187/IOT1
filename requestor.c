#include<stdio.h>
#include<curl/curl.h>

#define OK   0
#define INIT_ERR 1
#define REQ_ERR  2

#define URL    "http://0.0.0.0:80"

int main(void){
	CURL  *curl;
	CURLcode res;
	curl=curl_easy_init();
	if(curl){
		curl_easy_setopt(curl,CURLOPT_URL,URL);
		curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
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
