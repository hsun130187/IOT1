#include<stdio.h>
#include<curl/curl.h>

#define OK   0
#define INIT_ERR 1
#define REQ_ERR  2

#define URL    "http://localhost:80"

int main(void){
	char* postdata="this is a test";
	
	
	CURL  *curl;
	CURLcode res;
	curl=curl_easy_init();
	if(curl){
		curl_easy_setopt(curl,CURLOPT_URL,URL);
		//curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
		//curl_easy_setopt(curl, CURLOPT_POST, 1L);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strJsonData.size());
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS, postdata);

		//curl_easy_setopt (curl,CURLOPT_SSL_VERIFYPEER, 0L);

		//curl_easy_setopt (curl,CURLOPT_SSL_VERIFYHOST, 0L);

		//curl_easy_setopt (curl,CURLOPT_SSLCERT,"client.crt");

		//curl_easy_setopt (curl, CURLOPT_SSLCERTTYPE, "PEM");

		//curl_easy_setopt (curl, CURLOPT_SSLKEY,"client.key");

		//curl_easy_setopt (curl, CURLOPT_SSLKEYTYPE,"PEM");

		//curl_easy_setopt (curl,CURLOPT_TIMEOUT, 60L);

		//curl_easy_setopt (curl,CURLOPT_CONNECTTIMEOUT, 10L);
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
