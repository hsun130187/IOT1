#include<stdio.h>
#include<curl/curl.h>

#define OK   0
#define INIT_ERR 1
#define REQ_ERR  2

//#define URL    "http://localhost:80"
#define URL    "https://postb.in/1561923673161-1031376444734"
int main(void){
	char* postdata="this is a test";
	
	
	CURL  *curl;
	CURLcode res;
	curl=curl_easy_init();
	if(curl){
		curl_easy_setopt(curl,CURLOPT_URL,URL);
		
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS, postdata);

		
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
