#ifdef  BUILD_TYPE_RELEASE
char fota_json_cdn_url[URL_LEN] = "https://d10fcns44yeyj9.cloudfront.net/dev-user/json-file/";
#else
char fota_json_cdn_url[URL_LEN] = "https://d10fcns44yeyj9.cloudfront.net/dev-debug/json-file/";
#endif

