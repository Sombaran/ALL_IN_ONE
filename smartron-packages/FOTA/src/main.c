/*
 * main.c
 *
 *  Modified on: 04 June 2019
 *      Author: Priyanka P, Praveen T, Rajiv Shankar
 */


#include "fota.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <signal.h>
#include <errno.h>
#include "fota-url.h"

#define HW_MODEL_STRING "T4011"
// Declaration of thread condition variable
pthread_cond_t download_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t upgrade_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t receive_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t thub_upgrade_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t internet_status_cond = PTHREAD_COND_INITIALIZER;
// declaring mutex
pthread_mutex_t download_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t upgrade_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receive_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thub_upgrade_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t status_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t internet_status_lock = PTHREAD_MUTEX_INITIALIZER;

//char fota_json_file_path[PATH_LEN] = "/root/downloads/fw/"; /* path where fota json file is saved */
/* This URL is moved to fota-url.h" file, as we need to change this URL as per build generated from
   QA, DEV , produciton & integration Branch */
/*
char fota_json_cdn_url[URL_LEN] = "https://d10fcns44yeyj9.cloudfront.net/json-file/";
*/


int create_socket_server(void);

int create_socket_server(void) {
    struct sockaddr_in servaddr;

    if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        syslog(LOG_CRIT, "[FOTA]: Socket creation failed %s", strerror(errno));
        return errno;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(FOTA_PORT);

    if (bind(server_fd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        syslog(LOG_DEBUG, "[FOTA]:Socket binding failed %s", strerror(errno));
        return errno;
    }
    syslog(LOG_DEBUG,
            "[FOTA]:Server socket binded successfully to IP (127.0.0.1) and Port (7771) \n");
    return SUCCESS;
}

int move_file(char *filename, char *path) {
    char tmp[6] = "/tmp/";
    char command[50];

    snprintf(command, strlen(tmp) + strlen(filename) + strlen(path) + strlen(filename) + 5,
            "mv %s%s %s", tmp, filename, path);
    syslog(LOG_DEBUG, "check %s", command);
    system(command);
    return SUCCESS;
}

void client_info(void) {

    memset(&ma_cliaddr, 0, sizeof(ma_cliaddr));

    ma_cliaddr.sin_family = PF_INET;
    ma_cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ma_cliaddr.sin_port = htons(MA_PORT);

    memset(&dm_cliaddr, 0, sizeof(dm_cliaddr));

    dm_cliaddr.sin_family = PF_INET;
    dm_cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    dm_cliaddr.sin_port = htons(DM_PORT);

    memset(&luci_cliaddr, 0, sizeof(luci_cliaddr));
    luci_cliaddr.sin_family      = PF_INET;
    luci_cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    luci_cliaddr.sin_port        = htons(LUCI_PORT);
}
#ifndef HAVE_STRCASESTR
char *strcasestr(const char *a, const char *b) {
    size_t l;
    char f[3];

    snprintf(f, sizeof(f), "%c%c", tolower(*b), toupper(*b));
    for (l = strcspn(a, f); l != strlen(a); l += strcspn(a + l + 1, f) + 1)
        if (strncasecmp(a + l, b, strlen(b)) == 0)
            return (a + l);
    return (NULL);
}
#endif


/**
 * @brief Function to convert string format version to type int form for easy comparision.
 * eg. 1.11.9 => 0001|0011|0009 => 000100110009.
 *
 * @parm string_version version number in string format.
 * @return atoi(str) 12 numbers long int format.
 */
int version_string_to_int_conv(char *string_version) {
    char *token = NULL;
    int i = 0, len;
    char *tmp = NULL;
    char str1[5] = "0", str2[5] = "\0", str[13] = "\0";

    tmp = (char *)malloc(strlen(string_version)+1);
    strcpy(tmp,string_version);
    
    while (token = strtok_r(tmp, ".", &tmp)) {
        len = strlen(token);
        snprintf(str2, len + 1, "%s", token);
        if (len < 4) {
            while (4 - len) {
                strncat(str1, str2, len);
                strncpy(str2, str1, len + 1);
                strncpy(str1, "0", 2);
                len++;
            }
            strncat(str, str2, strlen(str2) + 1);
        }
    }
    return atoi(str);
}

/**
 * @brief Function to compare version number in the int format mentioned in version_string_to_int_conv()
 *
 * @parm v1 First version.
 * @parm v2 Second version.
 * @return 0 If v1 = v2.
           1 If v1 > v2.
          -1 If v1 < v2.
 */
int version_comparison(int v1, int v2) {
    if (v1 > v2) {
        return 1;
    } else if (v1 < v2) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief This callback function gets called by libcurl as soon as there is
 *        data received that needs to be saved.
 *
 * Shows how the write callback function can be used to download data into a
 * chunk of memory instead of storing it in a file.
 * Size of that data is size multiplied with nmemb.
 *
 * @param contents Pointer to the delivered url data/content.
 * @param size
 * @param nmemb
 * @param userp Pointer to your callback function.
 * @return realsize Size of that data (size*nmemb).
 */
static size_t write_url_data_callback(void *contents, size_t size, size_t nmemb, void *userp) {

    size_t realsize = size * nmemb;
    url_data_type *mem = (url_data_type *) userp;

    mem->data = realloc(mem->data, mem->size + realsize + 1);
    if (mem->data == NULL) {
        /* out of data! */
        syslog(LOG_ERR, "[FOTA]: Not enough data (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;

    return realsize;
}

/**
 * @brief Parse header and looking for "filename=" in the "Content-Disposition" tag.
 *
 * @param cd "Content-Disposition" tag from the header.
 * @param name Name of file being downloaded.
 * @return -1 on filename not present in "Content-Disposition" header tag.
 *          0 on Success.
 */
static int get_name_from_cd_header(char const* const cd, char *name) {
    char const* const cdtag = "Content-Disposition:";
    char const* const key = "filename=";
    int ret = 0;
    char *val = NULL;
    /*
     * Example Content-Disposition: filename=name1367; charset=funny; option=strange
     */

    /* If filename is present */
    val = strcasestr(cd, key);
    if (!val) {
        ret = -1;
        return ret;
    }
    /* Move to value */
    val += strlen(key);
    /* Copy value as name */
    while (*val != '\0' && *val != ';') {
        *name++ = *val++;
    }
    *name = '\0';
    return ret;
}
/**
 * @brief Get name of the file being downloaded, from the url where it is being hosted.
 *
 * @param url URL from where we file is to be downloaded.
 * @param name Name of file being downloaded.
 * @return 0 on successful fetch of fota_json_file_path from url.
 */
static int get_name_from_url(char const* url, char *name) {
    int ret = 0;
    char const *u = url;

    /* Remove "http(s)://" */
    u = strstr(u, "://");
    if (u) {
        u += strlen("://");
    }
    u = strrchr(u, '/');
    /* Remove last '/' */
    u++;
    /* Copy value as name */
    snprintf(name, strlen(u) + 1, "%s", u);
    return ret;
}
/**
 * @brief Parse response header and check if "Content-Disposition" tag is present.
 *
 * @param hdr Points to the delivered response header.
 * @param size
 * @param nmemb
 * @param userdata Pointer to the url data.
 * @return cb Size of header data (size*nmemb).
 */
size_t dnld_header_parse(void *hdr, size_t size, size_t nmemb, void *userdata) {
    const size_t cb = size * nmemb;
    const char *hdr_str = hdr;
    dnld_params_type *dnld_params = (dnld_params_type*) userdata;
    char const* const cdtag = "Content-disposition:";

    /* Example:
     * Content-Type: text/html
     * Content-Disposition: filename=name1367; charset=funny; option=strange
     */
    if (strstr(hdr_str, cdtag)) {
        syslog(LOG_DEBUG, "[FOTA]: Has Content-disposition: %s\n", hdr_str);
    }

    if (!strncasecmp(hdr_str, cdtag, strlen(cdtag))) { ///FIXME: strncasecmp not required strcasecmp is enough.
        syslog(LOG_DEBUG, "[FOTA]: Found c-d: %s\n", hdr_str);
        int ret = get_name_from_cd_header(hdr_str + strlen(cdtag), dnld_params->dnld_remote_fname);
        if (ret) {
            syslog(LOG_DEBUG, "[FOTA]: Error: Bad or No Remote Name");
        }
    }
    return cb;
}
/**
 * @brief Writing download stream into a local file with name "fname".
 *
 * @param fname Name of the download file
 * @return fp File pointer to which download data is being written to.
 */
FILE* get_dnld_stream(char const* const fname) {
    char const* const pre = "/tmp/";
    char out[PATH_LEN];

    snprintf(out, sizeof(out), "%s%s", pre, fname);

    FILE *fp = fopen(out, "wb");
    if (!fp) {
        syslog(LOG_ERR, "[FOTA]: Could not create file %s\n", out);
    }
    return fp;
}
/**
 * @brief This callback function gets called by libcurl as soon as there is
 *        data received that needs to be saved.
 *
 * Shows how the write callback function can be used to download data and
 * store it in a file.
 * Size of that data is size multiplied with nmemb.
 *
 * @param buffer Pointer to the delivered url data/content.
 * @param sz
 * @param nmemb
 * @param userdata Pointer to your callback function.
 * @return On Success: It returns the count of the number of items successfully written to the file.
 *         On Error: It returns a number less than n.
 */
size_t write_cb(void *buffer, size_t sz, size_t nmemb, void *userdata) {
    int ret = 0;
    dnld_params_type *dnld_params = (dnld_params_type*) userdata;

    if (!dnld_params->dnld_remote_fname[0]) {
        ret = get_name_from_url(dnld_params->dnld_url, dnld_params->dnld_remote_fname);
    }

    if (!dnld_params->dnld_stream) {
        dnld_params->dnld_stream = get_dnld_stream(dnld_params->dnld_remote_fname);
    }
    ret = fwrite(buffer, sz, nmemb, dnld_params->dnld_stream);
    if (ret == (sz * nmemb)) {
        dnld_params->dnld_file_sz += ret;
    }
    return ret;
}
/**
 * @brief Function to download files from given URL's.
 *
 * @param url URL from where file is to be downloaded
 * @return On Success: SUCCESS.
 *         On Error  : CURLcodes.
 */
int dnld_file_from_url(char const* const url , char *path) {
    CURL *curl = NULL;
    CURLcode cerr = CURLE_OK;
    dnld_params_type dnld_params;
    double val;
    unsigned int retry_count = 0;
    unsigned int wait_time = 1; /* in seconds */
    long response;

    syslog(LOG_DEBUG, "[FOTA]: URL:\t%s\n", url);

    memset(&dnld_params, 0, sizeof(dnld_params));
    snprintf(dnld_params.dnld_url, sizeof(dnld_params.dnld_url), "%s", url); /// FIXME: check if it works

    curl = curl_easy_init();

    cerr = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "URL", cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_VERBOSE, VERBOSE);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "VERBOSE", cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "FOLLOWLOCATION",
                cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "TCP_KEEPALIVE",
                cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_MAXCONNECTS, 10L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n",
                "CURLOPT_MAXCONNECTS", cerr);
        return cerr;
    }
    /* complete connection within 60 seconds */
    cerr = curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1000);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n",
               "CURLOPT_LOW_SPEED_LIMIT", cerr);
        return cerr;
    }
    cerr = curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n",
               "CURLOPT_LOW_SPEED_TIME", cerr);
        return cerr;
    }
#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    cerr = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "SSL_VERIFYPEER", cerr);
        return cerr;
    }
    syslog(LOG_DEBUG, "[FOTA]: dnld_file_from_url SKIP_HOSTNAME_VERIFICATION\n");
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    cerr = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "SSL_VERIFYHOST", cerr);
        return cerr;
    }
    syslog(LOG_DEBUG, "[FOTA]: dnld_file_from_url SKIP_HOSTNAME_VERIFICATION\n");
#endif
    cerr = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dnld_header_parse);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "HEADER", cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_HEADERDATA, &dnld_params);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "HEADER DATA", cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "WR CB", cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dnld_params);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "WR Data", cerr);
        return cerr;
    }

    cerr = curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n", "HTTP Code >= 400",
                cerr);
        return cerr;
    }

    do {
        cerr = curl_easy_perform(curl);

        if (!cerr && !dnld_params.dnld_stream && !dnld_params.dnld_file_sz) {
            /*
             * We have received no data despite the transfer was successful
             * ==> force creation of an empty output file (if an output file
             * was specified)
             */
            get_name_from_url(url, dnld_params.dnld_remote_fname);
            if (dnld_params.dnld_remote_fname) {
                FILE *fp = get_dnld_stream(dnld_params.dnld_remote_fname);
                if (fp) {
                    fclose(fp);
                }
            }
        }
        syslog(LOG_DEBUG, "[FOTA]: Retry no: %d\n", retry_count);

        enum {
            RETRY_NO, RETRY_TIMEOUT, RETRY_SERVER_ERR, RETRY_CLIENT_ERR,
        } retry = RETRY_NO;

        if ((cerr == CURLE_OPERATION_TIMEDOUT) || (cerr == CURLE_COULDNT_RESOLVE_HOST)
                || (cerr == CURLE_COULDNT_CONNECT)) {
            retry = RETRY_TIMEOUT;
        } else if ((cerr == CURLE_OK) || cerr == CURLE_HTTP_RETURNED_ERROR) {
            /*
             * If curl returned OK or failonerror was enabled and it
             * returned due to such an error, check for HTTP transient
             * errors.
             */
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
            switch (response) {
                case 500: /* Internal Server Error */
                case 502: /* Bad Gateway */
                case 503: /* Service Unavailable */
                case 504: /* Gateway Timeout */
                    retry = RETRY_SERVER_ERR;
                    break;
                case 404: /* Not Found */
                case 408: /* Request Timeout */
                    retry = RETRY_CLIENT_ERR;
                    break;
            }
        }

        if (!retry) {
            break;
        }
        static const char * const m[] = {
        NULL, "Timeout error", "Server error", "Client error" };

        syslog(LOG_CRIT, "[FOTA]: Transient problem %s, CURL request failed with error %d\n", m[retry], cerr);

        retry_count++;

        if (retry_count < CURL_NUM_RETRIES_ON_ERR) {
            wait_time *= 2;
            sleep(wait_time);
        }

    } while (retry_count < CURL_NUM_RETRIES_ON_ERR);

    /* get response code to check if HTTP request was successful */
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);

    if (cerr != CURLE_OK) {
        syslog(LOG_CRIT, "[FOTA]: CURL request failed after %u retries with error %d\n",
                retry_count, cerr);
        return cerr;
    } else if (response != 200) {
        syslog(LOG_CRIT, "[FOTA]: CURL request failed with HTTP response %d", response);
        return HTTP_ERR;
    }

    /* check for total download time */
    cerr = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &val);
    if ((CURLE_OK == cerr) && (val > 0))
        syslog(LOG_INFO, "[FOTA]: Total download time: %.0f sec.\n", val);

    /* check for average download speed */
    cerr = curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &val);
    if ((CURLE_OK == cerr) && (val > 0))
        syslog(LOG_INFO, "[FOTA]: Average download speed: %.0f kbyte/sec.\n", val);

    /* Close file pointer if open */
    syslog(LOG_DEBUG, "[FOTA]: File name:\t%s\n", dnld_params.dnld_remote_fname);
    if (dnld_params.dnld_stream)
        fclose(dnld_params.dnld_stream);

    /* always cleanup */
    curl_easy_cleanup(curl);
    curl = NULL;
    syslog(LOG_DEBUG, "[FOTA]: File size :\t%lu\n", dnld_params.dnld_file_sz);
    move_file(dnld_params.dnld_remote_fname, path);

    return SUCCESS;
}

/**
 * @brief To extract data from downloaded CDN JSON file.
 *
 * @param path Absolute path of json file from where data is to extracted.
 * @param ith_image Position of the element in the JSON array from which attribute is to be extracted.
 * @param attribute Attribute to be extracted
 * @param value Value of the "attribute".
 * @return On Success: SUCCESS.
 *         On Error  : JSON_FILE_READ_ERR.
 */
int extract_dnld_json(char *path, size_t ith_image, char *attribute, char *value) {
    json_object *image_array = json_object_from_file(path);

    if (image_array != NULL) {
//        json_object_get_array(image_array);

        json_object *image = json_object_array_get_idx(image_array, ith_image); /// FIXE ME: error handling
        if (image != NULL) {
            json_object *attr_obj = json_object_object_get(image, attribute); /// FIXE ME: error handling
            if (attr_obj != NULL) {
                snprintf(value, strlen(json_object_get_string(attr_obj)) + 1, "%s", json_object_get_string(attr_obj)); //// FIXEME:
                syslog(LOG_DEBUG, "[FOTA]: Json key: %s: %s\n", attribute, value);
                /// FIX ME: Put the json objects to minimize memory usage
            } else {
                syslog(LOG_ERR, "[FOTA]: Error: JSON attribute (%s) is not present\n", attribute);
                return JSON_FILE_READ_ERR;
            }
        } else {
            syslog(LOG_ERR, "[FOTA]: Error: JSON Array (%d) is not present\n", ith_image);
            return JSON_FILE_READ_ERR;
        }
    } else {
        syslog(LOG_ERR, "[FOTA]: Error: json_object_from_file(%s) returned NULL: \n", path);
        return JSON_FILE_READ_ERR;
    }
    return SUCCESS;
}

/**
 * @brief To extract or update the local Dnld firmware and shifu version json file.
 *
 * @param path Absolute path of json file from where data is to extracted.
 * @param ith_image Not in use
 * @param attribute Attribute to be extracted
 * @param value Value of the "attribute".
 * @param b_write true : Write mode.
 *                false: Read mode.
 * @return On Success: SUCCESS.
 *         On Error  : JSON_FILE_READ_ERR.
 */
int extract_current_json(char *path, size_t ith_image, char *attribute, char *value, bool b_write) {
    json_object *jso = json_object_from_file(path);
    json_object *obj = NULL;
    if (jso != NULL) {
        obj = json_object_object_get(jso, attribute);
        if (obj != NULL)
            syslog(LOG_DEBUG, "[FOTA]: Json key: %s: %s", attribute, json_object_get_string(obj));
        else{
            syslog(LOG_DEBUG, "[FOTA]: Json key %s not present in file\t%s", attribute, path);
            return FAILURE;
        }

        if (b_write) {
            json_object *write_obj = json_object_new_string(value);
            json_object_object_add(jso, attribute, write_obj);

            int ret = json_object_to_file_ext(path, jso, JSON_C_TO_STRING_PRETTY);
            if (!ret) {
                syslog(LOG_DEBUG, "[FOTA]: Added %s value to JSON Document", attribute);
            } else {
                syslog(LOG_ERR, "[FOTA]: Failed to add %s value to JSON Document");
//                json_object_put(jso);
                return JSON_FILE_WRITE_ERR;
            }

        } else{
            snprintf(value, strlen(json_object_get_string(obj)) + 1, "%s", json_object_get_string(obj));
        }
    } else {
        syslog(LOG_ERR, "[FOTA]: FAIL: json_object_from_file (%s) is not present", path);
//        json_object_put(jso);
        return JSON_FILE_READ_ERR;
    }
    return SUCCESS;
}

/**
 * @brief Function to check validity of the Firmware image downloaded and set the value in dnld_firmware_info json
 *        to "valid" or "invalid" accordingly.
 *
 * @param path Path where firmware image is saved.
 */
int check_image_validity( char *image_name, char *config_file_path, char *cdn_json_file_path, char *dnld_files_path) {


    EVP_PKEY* pPubKey = NULL;
    EVP_MD_CTX* ctx = NULL;

    FILE* publickey_fp = NULL;
    FILE* image_fp = NULL;
    FILE* signature_fp = NULL;

    char *image_buffer = NULL, *signature_buffer = NULL;
    char created_at[CREATED_AT_LEN] = { 0 };
    char release_version[VERSION_LEN] = { 0 };
    char command[128] = {0};
    char image_path[URL_LEN] = {0};
    char signature_url[URL_LEN] = {0};
    char signature_name[FILENAME_LEN] = {0};

    int status = 0;
    int image_len;
    int signature_len;
    char magic[16];
    int is_sec_image = 0;

    sec_hdr_v1  sec_hdr;

    sprintf(image_path, "%s%s", dnld_files_path, image_name);
    syslog(LOG_DEBUG, "[FOTA]: image path is %s", image_path);

    /*Fetch data from image */
    image_fp = fopen(image_path, "rb");
    if (image_fp == NULL) {
        syslog(LOG_ERR, "[FOTA]: Failed to read image: %s", image_name);
        return MD5_FETCH_ERR;
    }

    fread(&sec_hdr, sizeof(sec_hdr), 1, image_fp);
    if ( !strncmp(sec_hdr.magic, "SMRTHUB", 7) ) {
        if(sec_hdr.hdr_version != 1) {
            syslog(LOG_ERR, "[FOTA]: Security header version %d is not supported", sec_hdr.hdr_version);
            return INVALID_DATA;
        } else {
            char release_version[VERSION_LEN] = { 0 };

            /* Check if model number in image header matches the target device */
            if( strncmp(sec_hdr.hw_model_str, HW_MODEL_STRING, sizeof(HW_MODEL_STRING))) {
                syslog(LOG_ERR, "[FOTA]: Model string inside the fw image didn't \
                    match to the target(%s). Model string in image is %s",
                    HW_MODEL_STRING, sec_hdr.hw_model_str);
                return FAILURE;
            }

            /* Check the version string inside the fw image to see if it higher then current
            version or not */
            status = extract_dnld_json(cdn_json_file_path, 0, "version", release_version);
            if (status) {
                syslog(LOG_ERR, "[FOTA]: Could not fetch present version from update json file.");
                pthread_mutex_unlock(&download_lock);
                return FAILURE;
            }
            syslog(LOG_DEBUG, "[FOTA]: release_version = [ %s ] sec_hdr.fw_ver_str = [ %s ] ",release_version,sec_hdr.fw_ver_str);
            if( version_string_to_int_conv(sec_hdr.fw_ver_str) != version_string_to_int_conv(release_version)) {
                syslog(LOG_ERR, "[FOTA]: FW image release version is not equal to header firmware version");
                return FAILURE;
            }

            image_len = sec_hdr.sig_offset;
            fseek(image_fp, sec_hdr.sig_offset, SEEK_SET);
            signature_len = 128;
            signature_buffer = malloc(signature_len);
            fread(signature_buffer, signature_len, 1, image_fp);

            {
                for(int i = 0; i<8; i++) {
                    syslog(LOG_INFO, "[FOTA]: image_len = 0x%08X", image_len);
                    syslog(LOG_INFO, "[FOTA]: %02X %02X %02X %02X %02X %02X %02X \
                    %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                        signature_buffer[i*16 + 0],  signature_buffer[i*16 + 1],
                        signature_buffer[i*16 + 2],  signature_buffer[i*16 + 3],
                        signature_buffer[i*16 + 4],  signature_buffer[i*16 + 5],
                        signature_buffer[i*16 + 6],  signature_buffer[i*16 + 7],
                        signature_buffer[i*16 + 8],  signature_buffer[i*16 + 9],
                        signature_buffer[i*16 + 10], signature_buffer[i*16 + 11],
                        signature_buffer[i*16 + 12], signature_buffer[i*16 + 13],
                        signature_buffer[i*16 + 14], signature_buffer[i*16 + 15]);
                }
            }

            /* Set the image file to read to firmware data */
            fseek(image_fp, 0, SEEK_SET);
        }
    } else {
        fseek(image_fp, 0, SEEK_END);
        image_len = ftell(image_fp);
        fseek(image_fp, 0, SEEK_SET);

        /*Fetch data from signature */

        /* Fetch release signature url from CDN json file */
        status = extract_dnld_json(cdn_json_file_path, 0, "signature", signature_url);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
            goto error;
        }
        get_name_from_url(signature_url, signature_name);

        memset(&command, 0, sizeof(command));
        sprintf(command, "%s%s", dnld_files_path, signature_name);
        syslog(LOG_DEBUG, "[FOTA]: Signature path is %s", command);

        signature_fp = fopen(command, "rb");
        if (signature_fp == NULL) {
            syslog(LOG_ERR, "[FOTA]: Failed to read signature: %s", signature_name);
            status = MD5_FETCH_ERR;
            goto error;
        }
        fseek(signature_fp, 0, SEEK_END);
        signature_len = ftell(signature_fp);
        fseek(signature_fp, 0, SEEK_SET);
        signature_buffer = malloc(signature_len);
        fread(signature_buffer, signature_len, 1, signature_fp);
        fclose(signature_fp);
    }

    if ((publickey_fp = fopen(PUBLIC_KEY, "rt"))
            && (pPubKey = PEM_read_PUBKEY(publickey_fp, NULL, NULL, NULL))) {
        syslog(LOG_ERR, "[FOTA]: Public key read.\n");
    } else {
        syslog(LOG_ERR, "[FOTA]: Cannot read \"public_key.pem\".\n");
	     status = MD5_FETCH_ERR;
        goto err_verify;
    }
    ctx = EVP_MD_CTX_create();
    if (ctx == NULL) {
        syslog(LOG_ERR, "[FOTA]: EVP_MD_CTX_create failed, error \n");
        status = MD5_COMP_FAIL;
        goto err_verify;
    }

    status = EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pPubKey);
    if (status != 1) {
        syslog(LOG_ERR, "[FOTA]: EVP_DigestVerifyInit failed, error\n");
        status = MD5_COMP_FAIL;
        goto err_EVP_XTX_destroy;
        /* failed */
    }

    /* Read FILE_READ_CHUNK_SZ at a time from the file whose signature has to be
    verified, this would reduce the memory allocation requirement from say arround
    50 MB to 4K as per the current FILE_READ_CHUNK_SZ */
    #define FILE_READ_CHUNK_SZ 4096
    image_buffer = malloc(FILE_READ_CHUNK_SZ);
    while ( image_len > FILE_READ_CHUNK_SZ) {
        fread(image_buffer, FILE_READ_CHUNK_SZ, 1, image_fp);

        status = EVP_DigestVerifyUpdate(ctx, image_buffer, FILE_READ_CHUNK_SZ);
        if (status != 1) {
            syslog(LOG_ERR, "[FOTA]: EVP_DigestVerifyUpdate failed, error\n");
	        status = MD5_COMP_FAIL;
            goto err_EVP_XTX_destroy;
        }

        image_len -= FILE_READ_CHUNK_SZ;
    }

    fread(image_buffer, image_len, 1, image_fp);
    status = EVP_DigestVerifyUpdate(ctx, image_buffer, image_len);
    if (status != 1) {
        syslog(LOG_ERR, "[FOTA]: EVP_DigestVerifyUpdate failed, error\n");
	    status = MD5_COMP_FAIL;
        goto err_EVP_XTX_destroy;
    }

    status = EVP_DigestVerifyFinal(ctx, (const unsigned char*) signature_buffer, signature_len);
    if (status != 1) {
        syslog(LOG_ERR, "[FOTA]: EVP_DigestVerifyFinal failed, error\n");
        status = MD5_COMP_FAIL;
        goto err_EVP_XTX_destroy;
        /* failed */
    }else {
        /* Fetch release version from fota json file */
        status = extract_dnld_json(cdn_json_file_path, 0, "version", release_version);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
            goto err_EVP_XTX_destroy;
        }
        syslog(LOG_DEBUG, "[FOTA]: Image downloaded is authentic, version: %s\n", release_version);
        /* Set validity_status value to valid in update file */
        status = extract_current_json(config_file_path, 0, "validity_status", "valid", true);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not set validity_status in update file.");
            goto err_EVP_XTX_destroy;
        }
        /* Set release version value in download firmware json file */
        status = extract_current_json(config_file_path, 0, "downloaded_version", release_version,
        true);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not set release version in update file.");
            goto err_EVP_XTX_destroy;
        }
        /* Extract created_at from fota json file */
        status = extract_dnld_json(cdn_json_file_path, 0, "created_at", created_at);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not extract created_at");
            goto err_EVP_XTX_destroy;
        }
        /* Set uploaded_at value in downloaded firmware json file */
        status = extract_current_json(config_file_path, 0, "uploaded_at", created_at, true);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not set release version in update file.");
            goto err_EVP_XTX_destroy;
        }
    }

    EVP_MD_CTX_destroy(ctx);
    fclose(publickey_fp);
    free(image_buffer);
    free(signature_buffer);
    return SUCCESS;

    err_EVP_XTX_destroy:
      EVP_MD_CTX_destroy(ctx);
      fclose(publickey_fp);
    err_verify:
      free(signature_buffer);
    error:
      free(image_buffer);
    
    if(image_fp) {
        fclose(image_fp);
        image_fp = NULL;
    }

    /* Set validity_status value in update file */
    extract_current_json(config_file_path, 0, "validity_status", "invalid", true);
    return status;
}

int num_of_things(char *db_file, char *manufacturer_name, char *model_id, char *hardware_version, int *row_count) {
    sqlite3 *db          = NULL;
    sqlite3_stmt *ppStmt = NULL;
    int status;
    int step;                                                       ///FIXME:
    char sql_stmt[128];                                             ///FIXME:

    status = sqlite3_open(END_DEVICE_DATABASE, &db);

    if (status != SQLITE_OK) {
        syslog(LOG_ERR, "[FOTA]:Cannot open %s database: %s\n", END_DEVICE_DATABASE, sqlite3_errmsg(db));
        sqlite3_close(db);
        return status;
    }
    sprintf(sql_stmt, "SELECT count(*) from FIRMWARE where MANUFACTURER_NAME=\"%s\" AND MODEL_ID=\"%s\" AND HARDWARE_VERSION=\"%s\"",
            manufacturer_name, model_id, hardware_version);          ///FIXME:
    status = sqlite3_prepare_v2(db, sql_stmt, -1, &ppStmt, 0);          ///FIXME:
    if (status != SQLITE_OK) {
        sqlite3_close(db);
        return status;
    }

    step = sqlite3_step(ppStmt);

    if (step == SQLITE_ROW)
        *row_count = atoi(sqlite3_column_text(ppStmt, 0));
    else {
        *row_count = 0;
        syslog(LOG_ERR, "[FOTA]: Cannot read the database\n");
    }
    sqlite3_finalize(ppStmt);
    sqlite3_close(db);
    return SUCCESS;

}

void split_end_device_id(char *end_device_id, char *manufacturer_id, char *model_id,
        char *hardware_version) {                   ///FIXME:
    char* token = NULL;
    char name[128];
    char *rest = name;

//    strcpy(name, end_device_id);
    strcpy(rest, end_device_id);

    token = strtok_r(rest, "_", &rest);
    strcpy(manufacturer_id, token);

    token = strtok_r(rest, "_", &rest);
    strcpy(model_id, token);

    token = strtok_r(rest, "_", &rest);
    strcpy(hardware_version, token);
}

int get_thingid(const char *device_file, int level, int version, int max_things) {

    int status;
    int count = 0;

    sqlite3 *db         = NULL;
    sqlite3_stmt *res   = NULL;
    char sql_stmt[256];

    char thing_id[64];
    char json_file_path[128]                = {0};
    char end_device_id[256]                 = {0};
    char manufacturer_name[FILENAME_LEN]    = {0};
    char hardware_version[VERSION_LEN]      = {0};
    char model_id[FILENAME_LEN]             = {0};

    json_object *jthingid = NULL;

    strncpy(end_device_id, device_file, strlen(device_file) - strlen(".json"));
    split_end_device_id(end_device_id, manufacturer_name, model_id, hardware_version);
    syslog(LOG_DEBUG, "[FOTA]: End device type MN: %s\tMI: %s\tHW: %s \n", manufacturer_name, model_id, hardware_version);

    sprintf(json_file_path, "/root/downloads/fw/enddevota/%s", device_file);      ///FIXME:

    status = sqlite3_open(END_DEVICE_DATABASE, &db);
    if (status != SQLITE_OK) {
        syslog(LOG_ERR, "[FOTA]: Cannot open database: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    sprintf(sql_stmt,
            "SELECT THING_ID FROM FIRMWARE where MANUFACTURER_NAME=\"%s\" AND MODEL_ID=\"%s\" AND HARDWARE_VERSION=\"%s\" AND FIRMWARE_VERSION < %d",
            manufacturer_name, model_id, hardware_version, version);
    status = sqlite3_prepare_v2(db, sql_stmt, -1, &res, 0);

    if (status != SQLITE_OK) {
        syslog(LOG_ERR, "[FOTA]: Failed to prepare statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    while (sqlite3_step(res) == SQLITE_ROW) {                                       ///FIXME:
        if (count <= max_things) {                                                  // Limiting the devices
            memset(&thing_id, 0, sizeof(thing_id));                                 ///FIXME:
            strncpy(thing_id, sqlite3_column_text(res, 0), strlen(sqlite3_column_text(res, 0)));
            jthingid = json_object_new_string(thing_id);
            json_object_array_add(jthingid_obj[level], jthingid);                   //storing thingids into json array
        } else
            break;
        count++;
    }
    sqlite3_finalize(res);
    status = sqlite3_close(db);
    if (status != SQLITE_OK)
        syslog(LOG_ERR, "[FOTA]: Database not closed");
    return SUCCESS;
}

int response_timer(void) {
    struct timespec timeToWait;
    struct timeval now;
    int status;

    gettimeofday(&now, NULL);

    timeToWait.tv_sec = now.tv_sec + (490);                                         /// Response timeout from DM
    timeToWait.tv_nsec = (now.tv_usec + 1000UL) * 1000UL;
    syslog(LOG_DEBUG, "[FOTA]: Waiting for a signal for a period of 490 secs");
    status = pthread_cond_timedwait(&receive_cond, &upgrade_lock, &timeToWait);
    if (status)
        syslog(LOG_CRIT, "[FOTA]: Response Timed out [%s]", strerror(errno));
    return status;
}

/**
 * @brief Function to do FCM push to Tronx.
 *
 * @param path FCM push URL
 */
int fcm_push(void) {

    char home_token[ENV_LEN] = { 0 };
    char device_id[ENV_LEN] = { 0 };

    char hometoken_hdtag[HEADER_FIELD_LEN] = "X-HomeToken: ";
    char deviceid_hdtag[HEADER_FIELD_LEN] = "X-DeviceId: ";
    char requestid_hdtag[HEADER_FIELD_LEN] = "X-RequestId: ";

    CURL *curl = NULL;
    CURLcode cerr = CURLE_OK;
    url_data_type chunk;
    struct curl_slist *headerlist = NULL;
    char changelog_string[80], changelog_string_full[CHANGELOG_STR_LEN] = { 0 };
    char base_url[URL_LEN] = { 0 };
    const char *resp_status = NULL;
    const char *resp_message = NULL;
    int status;
    char downloaded_version[VERSION_LEN] = { 0 };
    char uploaded_at[CREATED_AT_LEN] = { 0 };

    /* Get contents from changelog.md in string format */
    FILE *fp = popen("cat /root/downloads/fw/*.md", "r");

    if (fp == NULL) {
        syslog(LOG_ERR, "[FOTA]: Failed to do open changelog.md file\n");
    }

    while (fgets(changelog_string, sizeof(changelog_string), fp) != NULL) {
        strncat(changelog_string_full, changelog_string, strlen(changelog_string) + 1);
    }
    status = pclose(fp);
    syslog(LOG_DEBUG, "[FOTA]: Changelog full contents in string: %s\n", changelog_string_full);

    /* Fetching HomeToken and DeviceID NV */
    status = get_shifu_uci_param("HOME_TOKEN", home_token);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: No HOME_TOKEN NV\n");
        return ENV_ERR;
    }
    if (!strcmp(home_token, "NULL")) {
        syslog(LOG_ERR, "[FOTA]: HOME_TOKEN is NULL\n");
        return ENV_ERR;
    }
    status = get_fw_env("DEVICE_ID", device_id);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: No DEVICE_ID NV\n");
        return ENV_ERR;
    }
/// FIXE ME: What if Device ID value is incorrect?

    /* fetch release version from downloaded firmware json file */
    status = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "downloaded_version", downloaded_version, false);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch release version from update json file.");
        return JSON_FILE_READ_ERR;
    }
    syslog(LOG_DEBUG, "[FOTA]: Downloaded version %s\n", downloaded_version);

    /* fetch uploaded_at from downloaded firmware json file */
    status = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "uploaded_at", uploaded_at, false);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch uploaded_at from update json file.");
        return JSON_FILE_READ_ERR;
    }
    syslog(LOG_DEBUG, "[FOTA]: Firmware Uploaded at %s\n", uploaded_at);

    json_object *jobj = json_object_new_object();
    json_object *body = json_object_new_object();

    /* Creating a json string */
    json_object *jstring1 = json_object_new_string(changelog_string_full);
    json_object *jstring2 = json_object_new_string("SHIFU");
    json_object *jstring3 = json_object_new_string("FOTA");
    json_object *jstring4 = json_object_new_string("NOTIFY");
    json_object *jstring5 = json_object_new_string(downloaded_version);
    json_object *jstring6 = json_object_new_string(uploaded_at);

    json_object_object_add(body, "change_log", jstring1);
    json_object_object_add(body, "release_version", jstring5);
    json_object_object_add(body, "release_date", jstring6);

    /* Form the json object */
    json_object_object_add(jobj, "body", body);
    json_object_object_add(jobj, "domain", jstring2);
    json_object_object_add(jobj, "subdomain", jstring3);
    json_object_object_add(jobj, "verb", jstring4);

    /* Now printing the json object */
    syslog(LOG_DEBUG, "[FOTA]: The json object created: %s\n", json_object_to_json_string(jobj));

    /* FCM push url */
    status = get_shifu_uci_param("TRONX_URL", base_url);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: No TRONX_URL NV\n");
        return ENV_ERR;
    }
    strcat(base_url, FCM_PUSH_URL_PATH);
    syslog(LOG_DEBUG, "\n[FOTA]: FCM_PUSH URL: %s\n", base_url);

    chunk.data = malloc(1); /* will be grown as needed by realloc above */
    chunk.size = 0; /* no data at this point */
    /*
     * HTTPS POST to Tronx and get back response
     */
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    cerr = curl_easy_setopt(curl, CURLOPT_URL, base_url);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "URL", cerr);
    }

    cerr = curl_easy_setopt(curl, CURLOPT_VERBOSE, VERBOSE);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "VERBOSE", cerr);
    }

    cerr = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "FOLLOWLOCATION", cerr);
    }

#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    cerr = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "SSL_VERIFYPEER", cerr);
    }
    syslog(LOG_DEBUG, "[FOTA]: FCM PUSH SKIP_HOSTNAME_VERIFICATION\n");
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    cerr = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "SSL_VERIFYHOST", cerr);
    }
    syslog(LOG_DEBUG, "[FOTA]: FCM PUSH SKIP_HOSTNAME_VERIFICATION\n");
#endif

    snprintf(hometoken_hdtag, strlen(hometoken_hdtag) + strlen(home_token) + 1, "%s%s",
            hometoken_hdtag, home_token);
    syslog(LOG_DEBUG, "[FOTA]: X-HomeToken string: %s\n", hometoken_hdtag);

    snprintf(deviceid_hdtag, strlen(deviceid_hdtag) + strlen(device_id) + 1, "%s%s", deviceid_hdtag,
            device_id);
    syslog(LOG_DEBUG, "[FOTA]: X-DeviceId string: %s\n", deviceid_hdtag);

    generate_request_id(requestid_hdtag, device_id);
    syslog(LOG_DEBUG, "[FOTA]: X-RequestId string: %s\n", requestid_hdtag);

    headerlist = curl_slist_append(headerlist, hometoken_hdtag);
    headerlist = curl_slist_append(headerlist, requestid_hdtag);
    headerlist = curl_slist_append(headerlist, deviceid_hdtag);
    headerlist = curl_slist_append(headerlist, "Content-Type: application/json");
    headerlist = curl_slist_append(headerlist, "User-Agent:");
    headerlist = curl_slist_append(headerlist, "Accept:");

    /* set our custom set of headers */
    cerr = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "HTTPHEADER", cerr);
    }
    /* send all data to this function  */
    cerr = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_url_data_callback);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "WRITEFUNCTION", cerr);
    }
    /* we pass our 'chunk' struct to the callback function */
    cerr = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void * ) &chunk);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "WRITEDATA", cerr);
    }
    /* some servers don't like requests that are made without a user-agent field, so we provide one */
    cerr = curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "USERAGENT", cerr);
    }
    /* body of the POST method */
    cerr = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_to_json_string(jobj));
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "POSTFIELDS", cerr);
    }
    /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by itself */
    cerr = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,
            (long ) strlen(json_object_to_json_string(jobj)));
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: FCM PUSH failed with err %d\n", "POSTFIELDSIZE", cerr);
    }
    /* Perform the request, res will get the return code */
    cerr = curl_easy_perform(curl);
    /* Check for errors */
    if (cerr != CURLE_OK) {
        syslog(LOG_CRIT, "[FOTA]: FCM PUSH curl_easy_perform() failed: %d\n", cerr);
        return cerr;
    }
    /*
     * {chunk.data} points to a data block that is {chunk.size} bytes big and contains the remote file.
     */
    syslog(LOG_DEBUG, "[FOTA]: %lu bytes retrieved\n", chunk.size);
    syslog(LOG_DEBUG, "[FOTA]: %s\n", chunk.data);
    /* cleanup curl handle */
    curl_easy_cleanup(curl);
    curl = NULL;
    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    json_object *new_obj;

    new_obj = json_tokener_parse(chunk.data); //FIX ME: Error handling
    resp_status = json_object_get_string(json_object_object_get(new_obj, "status")); //FIX ME: Error handling, create json obj
    resp_message = json_object_get_string(json_object_object_get(new_obj, "msg")); //FIX ME: Error handling

    syslog(LOG_CRIT, "[FOTA]: STATUS: %s\nMESSAGE: %s\n", resp_status, resp_message);
    free(chunk.data);
}

/**
 * @brief Main Download function which performs Version check, Downloading of files, MD5SUM check and FCM push to Tronx.
 *
 * @return On Success: SUCCESS.
 *         On Error  : MD5_FETCH_ERR, MD5_COMP_FAIL, error codes from dnld_file_from_url, extract_dnld_json and extract_current_json.
 */
int DownloadFirmware(char *domain, char *cdn_filename, char *config_file_path, char *dnld_files_path, char * changelog_name) {

    pthread_mutex_lock(&download_lock);
    const char changelog_url[URL_LEN] = { 0 };
    const char download_url[URL_LEN] = { 0 };
    const char secimage_url[URL_LEN] = { 0 };
    const char signature_url[URL_LEN] = { 0 };
    char cdn_url[URL_LEN] = { 0 };
    char downloaded_version[VERSION_LEN] = { 0 };
    char release_version[VERSION_LEN] = { 0 };
    char current_version[VERSION_LEN] = { 0 };
    char image_name[FILENAME_LEN] = { 0 };
    char signature_name[FILENAME_LEN] = { 0 };

    char file_status[FLAG_LEN] = { 0 };
    char cdn_json_file_path[PATH_LEN] = {0};
    char command[COMMAND_LEN] = {0};

    int status = 0;
    int rv_inv = 0;
    int cv_inv = 0;
    int dv_inv = 0;
    int version_check = 0;
    char size[VERSION_LEN] = { 0 };
    float tmp_available_size = 0;
    float overlay_available_size = 0;
    float total_image_size = 0;

    bool download_flag = false;

    FILE * fp = NULL;

    snprintf(cdn_json_file_path, strlen(dnld_files_path) + strlen( cdn_filename ) + 1, "%s%s", dnld_files_path, cdn_filename);        ///FIXME: length has to be determined
//    strncat(fota_json_cdn_url, cdn_filename, URL_LEN - strlen(fota_json_cdn_url) - 1);
    snprintf(cdn_url, strlen(fota_json_cdn_url) + strlen(cdn_filename) + 1, "%s%s", fota_json_cdn_url, cdn_filename);

    syslog(LOG_DEBUG, "[FOTA]: Path to store CDN file: %s",cdn_json_file_path);
    syslog(LOG_DEBUG, "[FOTA]: Path of file with present firmware details: %s", config_file_path);
    syslog(LOG_DEBUG, "[FOTA]: CDN URL: %s\n: %s", cdn_url);

    /* download fota json to get changelog and image download urls */
    status = dnld_file_from_url(cdn_url, dnld_files_path);
    if (status) {
        syslog(LOG_CRIT, "[FOTA]: Fota json file download failed.");
        pthread_mutex_unlock(&download_lock);
        return status;
    }

    /* fetch release version from CDN json file */
    status = extract_dnld_json(cdn_json_file_path, 0, "version", release_version);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
        pthread_mutex_unlock(&download_lock);
        return JSON_FILE_READ_ERR;
    }
    
    /* Extract changelog url from fota json file */
    status = extract_dnld_json(cdn_json_file_path, 0, "change_log", changelog_url);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not extract changelog url");
        pthread_mutex_unlock(&download_lock);
        return JSON_FILE_READ_ERR;
    }

    get_name_from_url(changelog_url, changelog_name);

    /* Check if secimage_url is present and take an alternate path */
    status = extract_dnld_json(cdn_json_file_path, 0, "secimage_url", secimage_url);
    if (status != SUCCESS) {
        syslog(LOG_INFO, "[FOTA]: Could not find the secimage_url");

	    /* Extract download url from fota json file */
	    status = extract_dnld_json(cdn_json_file_path, 0, "download_url", download_url);
	    if (status) {
	        syslog(LOG_ERR, "[FOTA]: Could not extract url for image download");
	        pthread_mutex_unlock(&download_lock);
	        return JSON_FILE_READ_ERR;
	    }

	    get_name_from_url(download_url, image_name);
        /* Extract download url from fota json file */
        status = extract_dnld_json(cdn_json_file_path, 0, "signature", signature_url);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not extract url for image download");
            pthread_mutex_unlock(&download_lock);
            return JSON_FILE_READ_ERR;
        }

        get_name_from_url(signature_url, signature_name);

    } else {
        get_name_from_url(secimage_url, image_name);
    }

    /* fetch Shifu Hub present version from download firmware json file */ /////////FIXEME:
    status = extract_current_json(config_file_path, 0, "downloaded_version",
            downloaded_version, false);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch present version from update json file.");
        pthread_mutex_unlock(&download_lock);
        return JSON_FILE_READ_ERR;
    }

    rv_inv = version_string_to_int_conv(release_version);
    dv_inv = version_string_to_int_conv(downloaded_version);
    if (!strcmp(domain ,"SHIFU")){
        status = extract_current_json(SHIFU_VERSION_JSON_PATH, 0, "version", current_version, false);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch present version from update json file.");
            pthread_mutex_unlock(&download_lock);
            return JSON_FILE_READ_ERR;
        }
        syslog(LOG_DEBUG, "[FOTA]: Current SHIFU Firmware version %s", current_version);
        cv_inv = version_string_to_int_conv(current_version);
    }

    /* Version comparision between release version and downloaded firmware version*/
    version_check = version_comparison(rv_inv, dv_inv);

    if (version_check == 1) { /* Release version greater than previous downloaded version */
        download_flag = true;
    } else if (version_check == -1) { /* Release version less than previous downloaded version */
        syslog(LOG_DEBUG, "[FOTA]: SHIFU is up-to-date with %s", current_version);
        pthread_mutex_unlock(&download_lock);
        return UP_TO_DATE;
    } else { /* Release version equal to previous downloaded version */
        if (rv_inv == cv_inv) {
            /* Set validity_status value in update file */
            status = extract_current_json(config_file_path, 0, "validity_status", "null", true);
            if (status) {
                syslog(LOG_ERR,
                        "[FOTA]: Could not set validity_status in download firmware json file.");
                return JSON_FILE_WRITE_ERR;
            }
            syslog(LOG_DEBUG, "[FOTA]: SHIFU is up-to-date with %s", current_version);
            pthread_mutex_unlock(&download_lock);
            return UP_TO_DATE;
        }
        // Bug #19709:Start Error handling unwanted and unexpected deletion of FOTA related files from the system.
        else {
            /* If files get deleted during reboot (we have to re-download them) */
            memset(&command, 0, sizeof(command));
            sprintf(command, "ls %s%s %s%s 2> /dev/null ; echo $?", dnld_files_path, image_name, dnld_files_path, changelog_name);

            fp = popen(command, "r");
            if (fp == NULL) {
                syslog(LOG_CRIT, "[FOTA]: Image and/or changelog files are missing");
            }
            while (fgets(file_status, sizeof(file_status), fp) != NULL) {
            }
            pclose(fp);
            file_status[strlen(file_status) - 1] = '\0';

            /* If file_status is non zero it means some of the FOTA files are missing,
             * so we have to re-downlaod the FOTA files
             */
            if (atoi(file_status) != 0) {
                download_flag = true; /* executes the FOTA downloading process */
            }
        }
        // Bug #19709:End
    }

    /* Download changelog and image from the extracted urls */
    if (download_flag) {

        char * urls[3];
        int num_of_urls = 2;

        memset(&command, 0, sizeof(command));
        snprintf(command, "rm %s%s %s%s", dnld_files_path, image_name, dnld_files_path, changelog_name);

        fp = popen(command, "w");
        if (fp == NULL) {
            syslog(LOG_ERR, "[FOTA]: Could not remove image and changelog files\n");
        }
        status = pclose(fp);

        urls[0] = changelog_url;

        if (strlen(secimage_url)) {
           urls[1] = secimage_url;
        } else  {
            urls[1] = download_url;
            urls[2] = signature_url;
            num_of_urls = 3;
        }
        /** Check the Available space in /tmp and /overlay **/
        fp = popen("df -m | awk '/\\/tmp$/' | awk '{print $4}'", "r");
        if (fp == NULL) {
            syslog(LOG_ERR, "[FOTA]: Failed to find available space");
            pthread_mutex_unlock(&download_lock);
            return FAILURE;
        }
        else {
            fgets(size, sizeof(size), fp);
            pclose(fp);
            sscanf(size,"%f",&tmp_available_size);
            memset(size,0,sizeof(size));
            fp = popen("df -m | awk '/\\/overlay$/' | awk '{print $4}'", "r");
            if (fp == NULL) {
                syslog(LOG_ERR, "[FOTA]: Failed to find available space");
                pthread_mutex_unlock(&download_lock);
                return FAILURE;
            }
            else{
                fgets(size, sizeof(size), fp);
                pclose(fp);
                sscanf(size,"%f",&overlay_available_size);
                memset(size,0,sizeof(size));
                status = extract_dnld_json(cdn_json_file_path, 0, "size", size);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: Could not fetch image size from update json file.");
                    //pthread_mutex_unlock(&download_lock);
                    //return JSON_FILE_READ_ERR;
                }
                else{
                    sscanf(size,"%fMB",&total_image_size);
                    syslog(LOG_DEBUG, "[FOTA]: Available space = [%f]  Total_image_size = [%f] Overlay_available_size = [%f]",tmp_available_size,total_image_size,overlay_available_size );
                    if(!((tmp_available_size >(total_image_size+EXTRA_SPACE_TMP))&&(overlay_available_size > (total_image_size+EXTRA_SPACE_OVERLAY      )))){
                        syslog(LOG_ERR, "[FOTA]: Image size [%f] is larger than Available size",total_image_size);
                        pthread_mutex_unlock(&download_lock);
                        return FAILURE;
                    }
                }
            }
        }
        for (size_t i = 0; i < num_of_urls; i++) {
            syslog(LOG_INFO, "urls[i] = %s", urls[i]);
            /* downloads */
            status = dnld_file_from_url(urls[i], dnld_files_path);
            if (status) {
                syslog(LOG_CRIT, "[FOTA]: Download from %s failed", urls[i]);
                pthread_mutex_unlock(&download_lock);
                return status;
            }
        }
    }
    status = check_image_validity( image_name, config_file_path, cdn_json_file_path, dnld_files_path);
    if (status) {
        syslog(LOG_CRIT, "[FOTA]: Image downloaded is not authentic");
        memset(&command, 0, sizeof(command));
        sprintf(command, "rm %s%s %s%s %s%s", dnld_files_path, image_name, dnld_files_path, changelog_name, dnld_files_path, signature_name);   ///FIXME: snprintf
        syslog(LOG_DEBUG, "[FOTA]: Remove command %s, %d", command , strlen(command));

        fp = popen(command, "w");
        if (fp == NULL) {
            syslog(LOG_ERR, "[FOTA]: Could not remove image and changelog files\n");
        }
        pclose(fp);
        syslog(LOG_ERR, "[FOTA]: Image downloaded is not authentic end");
        pthread_mutex_unlock(&download_lock);
        return UP_TO_DATE;
    }

    /*
     * FCM push to Tronx regarding changelog contents
     */
    if (!strcmp(domain ,"SHIFU")){
        syslog(LOG_DEBUG, "[FOTA]: FCM PUSH Initialized\n");
        status = fcm_push();
        if (status) {
            syslog(LOG_CRIT, "[FOTA]: FCM PUSH Failed, RESPONSE CODE: %d \n", status);
        }
    }
    pthread_mutex_unlock(&download_lock);
    return SUCCESS;
}

void thub_upgrade_thread(void) {
    int ret = 0;
    char *update_timestamp = (char *) malloc(sizeof(char) * 16);
    time_t ltime;
    char json_filename[FILENAME_LEN] = { 0 };
    char cdn_json_file_path[PATH_LEN] ={0};
    char download_url[URL_LEN] = {0};
    char image_name[FILENAME_LEN];
    char buzzer_status[FLAG_LEN];
    char cmd[COMMAND_LEN];
    char secimage_url[URL_LEN] = {0};
    json_object *jsendto = NULL;
    json_object *jbody = NULL;
    
    while (1) {
        pthread_mutex_lock(&thub_upgrade_lock);
        pthread_cond_wait(&thub_upgrade_cond, &thub_upgrade_lock);
        buzzer_status[0] = '1';			//Default state of DM_VA_START_CHECK is 1
        jsendto = json_object_new_object(); 
        jbody = json_object_new_object();
        /* fetch Shifu Hub JSON path from version json file */
        memset(json_filename,0,sizeof(json_filename));
        ret = extract_current_json(SHIFU_VERSION_JSON_PATH, 0, "fota_filename", json_filename, false);
        if (ret) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch present Json filename from version json file.");
            goto END;
        }

        snprintf(cdn_json_file_path, strlen(THUB_DNLD_FILES_PATH) + strlen(json_filename) + 1, "%s%s", THUB_DNLD_FILES_PATH, json_filename);
        
        /* Check if secimage_url is present and take an alternate path */
        memset(secimage_url,0,sizeof(secimage_url));
        memset(download_url,0,sizeof(download_url));
        memset(image_name,0,sizeof(image_name));
        ret = extract_dnld_json(cdn_json_file_path, 0, "secimage_url", secimage_url);
        if ( ret != SUCCESS ) {
            /* Extract download url from fota json file */
            ret = extract_dnld_json(cdn_json_file_path, 0, "download_url", download_url);
            if (ret) {
                syslog(LOG_ERR, "[FOTA]: Could not extract url for image download");
                goto END;
            }
            get_name_from_url(download_url, image_name);
        } else {
            get_name_from_url(secimage_url, image_name);
        }
        
        ret = check_image_validity( image_name, DNLD_FIRMWARE_JSON_PATH, cdn_json_file_path, THUB_DNLD_FILES_PATH);
        if (ret) {
            syslog(LOG_ERR, "[FOTA]: Image validity check failed");
            goto END;
        }
        /* Set validity_status value in download firmware json file */
        ret = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "validity_status", "null", true);
        if (ret) {
            syslog(LOG_ERR, "[FOTA]: Could not set validity_status in download firmware json file.");
            goto END;
        }

        /* Fetching Security buzzer status from UCI parameter */
        ret = get_shifu_uci_param("DM_VA_START_CHECK", buzzer_status);
        if (ret) {
            syslog(LOG_ERR, "[FOTA]: No DM_VA_START_CHECK UCI parameter");
        }
        if (buzzer_status[0] == '0') {
            syslog(LOG_CRIT, "[FOTA]: Security buzzer is ON");
            json_object_object_add(jbody, "message", json_object_new_string("Security breach occurred, press kill switch to disable buzzer"));
            json_object_object_add(jsendto, "status", json_object_new_string("error"));
            json_object_object_add(jsendto, "body", jbody);
            json_object_object_add(jsendto, "status_code", json_object_new_int(430));
            goto SEND;
        }
        /* Get current unix timestamp */
        ltime = time(NULL);
        snprintf(update_timestamp, 16, "%ld", ltime);

        syslog(LOG_DEBUG, "[FOTA]: Timestamp at which Shifu is being updated: %s\n", update_timestamp);

        /* Set last_updated value in download firmware json file */
        ret = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "last_updated", update_timestamp, true);
        if (ret) {
            syslog(LOG_ERR, "[FOTA]: Could not set last_updated in download firmware json file.");
            free(update_timestamp);
            goto END;
        }
        free(update_timestamp);
        
        json_object_object_add(jbody, "message", json_object_new_string("Your t.hub firmware upgrade is initialized")); 
        json_object_object_add(jsendto, "status", json_object_new_string("success"));
        json_object_object_add(jsendto, "body", jbody);
        json_object_object_add(jsendto, "status_code", json_object_new_int(200));
        
        syslog(LOG_DEBUG, "[FOTA]: FOTA ACK to WS [ IP_ADDR= %s, PORT= %d ]  json=[%s]\n",
               inet_ntoa(req_cliaddr.sin_addr), ntohs(req_cliaddr.sin_port), json_object_to_json_string_ext(jsendto, JSON_C_TO_STRING_PRETTY));
        if (sendto(server_fd, json_object_to_json_string(jsendto), strlen(json_object_to_json_string(jsendto)) + 1,
            MSG_CONFIRM, (const struct sockaddr *) &req_cliaddr, sizeof(req_cliaddr)) < 0){
            syslog(LOG_ERR, "[FOTA]: Response to WS failed, Error Code: %s", strerror(errno));
            json_object_put(jsendto);
            jsendto = json_object_new_object();
            jbody = json_object_new_object();
            goto END;
        }
                   
        memset(cmd,0,COMMAND_LEN);
        sprintf(cmd,"/etc/fw/firmware_update %s",image_name);
        syslog(LOG_DEBUG, "[FOTA]: Executing [ %s ]",cmd);
        ret = system(cmd);
        syslog(LOG_DEBUG, "[FOTA]: Firmware update return %d\n", ret);
        if (ret == -1) {
            syslog(LOG_CRIT, "[FOTA]: Image is not present\n");
        }
        goto CONTINUE;

        END:    {
            json_object_object_add(jbody, "message", json_object_new_string("t.hub internal error"));
            json_object_object_add(jsendto, "status", json_object_new_string("error"));
            json_object_object_add(jsendto, "body", jbody);
            json_object_object_add(jsendto, "status_code", json_object_new_int(500));
        }

        SEND:{
            syslog(LOG_DEBUG, "[FOTA]: Send to WS [ IP_ADDR= %s, PORT= %d ]  json=%s",
                   inet_ntoa(req_cliaddr.sin_addr), ntohs(req_cliaddr.sin_port), json_object_to_json_string_ext(jsendto, JSON_C_TO_STRING_PRETTY));
            if (sendto(server_fd, json_object_to_json_string(jsendto), strlen(json_object_to_json_string(jsendto)) + 1,
                MSG_CONFIRM, (const struct sockaddr *) &req_cliaddr, sizeof(req_cliaddr)) < 0) {
                syslog(LOG_ERR, "[FOTA]: Check Response to WS failed, Error Code: %s", strerror(errno));
            }
        }

        CONTINUE:{
            pthread_mutex_lock(&status_lock);
            extract_current_json(FOTA_STATUS_JSON_PATH, 0, "upgrade_status", "0", true);
            pthread_mutex_unlock(&status_lock);
        }
        memset(&req_cliaddr, 0, sizeof(req_cliaddr));
        json_object_put(jsendto);
        pthread_mutex_unlock(&thub_upgrade_lock);
    }
}

void thing_upgrade_thread(void) {
    int status, array_len;
    sqlite3 *db                 = NULL;
    sqlite3_stmt *sql_stmt      = NULL;
    char stmt[LEN_256];
    char *thing_id              = NULL;

    json_object *jarray         = NULL;
    json_object *jsendto        = NULL;
    json_object *jclient        = NULL;
    json_object *jcommand       = NULL;
    json_object *jfirmware_path = NULL;
    json_object *jfirmware_ver  = NULL;
    json_object *jStatus        = NULL;
    json_object *jthing_id      = NULL;
    json_object *Thingid_obj    = NULL;

    char file_path[PATH_LEN];
    char end_device_json[LEN_128];
    char firmware_version[VERSION_LEN];
    char manufacturer_name[FILENAME_LEN];
    char hardware_version[VERSION_LEN];
    char model_id[FILENAME_LEN];

    while (1) {
        pthread_mutex_lock(&upgrade_lock);
        syslog(LOG_DEBUG, "[FOTA]: Waiting for thing upgrade signal");
        pthread_cond_wait(&upgrade_cond, &upgrade_lock);
        syslog(LOG_DEBUG, "[FOTA]: Thing upgrade signal received ");

        if (jthingid_list == NULL) {
            pthread_mutex_unlock(&upgrade_lock);
            continue;
        }
        array_len = json_object_array_length(jthingid_list);
        syslog(LOG_DEBUG, "[FOTA]: Number of thingids to upgrade: %d", array_len);

        while (array_len) {

            jarray = json_object_array_get_idx(jthingid_list, array_len - 1);
            thing_id = json_object_get_string(jarray);
            syslog(LOG_DEBUG, "[FOTA]: ThingID list: %s\tThingID for upgrade: %s", json_object_get_string(jthingid_list), thing_id);

            /*
             * Fetching End_device_Id based on thingid from database
             */
            ///FIXME: look for other logic
            status = sqlite3_open(END_DEVICE_DATABASE, &db);
            if (status != SQLITE_OK) {
                syslog(LOG_ERR, "[FOTA]: Cannot open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                pthread_mutex_unlock(&upgrade_lock);
                --array_len;
                continue;
            }
            sprintf(stmt, "SELECT HARDWARE_VERSION, MANUFACTURER_NAME, MODEL_ID FROM FIRMWARE where THING_ID=\"%s\"", thing_id);              ///FIXME: use snprintf

            status = sqlite3_prepare_v2(db, stmt, -1, &sql_stmt, 0);    ///FIXME: error case
            if (status != SQLITE_OK) {
                syslog(LOG_ERR, "[FOTA]: Failed to prepare statement: %s", sqlite3_errmsg(db));
                sqlite3_close(db);
                pthread_mutex_unlock(&upgrade_lock);
                --array_len;
                continue;
            }

            status = sqlite3_step(sql_stmt);          ///FIXME: error case
            if (status == SQLITE_ROW) {
                memset(hardware_version, 0, sizeof(hardware_version));
                memset(manufacturer_name, 0, sizeof(manufacturer_name));
                memset(model_id, 0, sizeof(model_id));
                strncpy(hardware_version,  sqlite3_column_text(sql_stmt, 0), strlen(sqlite3_column_text(sql_stmt, 0)));
                strncpy(manufacturer_name, sqlite3_column_text(sql_stmt, 1), strlen(sqlite3_column_text(sql_stmt, 1)));
                strncpy(model_id,          sqlite3_column_text(sql_stmt, 2), strlen(sqlite3_column_text(sql_stmt, 2)));
                syslog(LOG_DEBUG, "[FOTA]:THING_ID: %s\t\tHW: %s\tMN: %s\tMI: %s", thing_id,
                        hardware_version, manufacturer_name, model_id);
            } else {
                syslog(LOG_ERR, "[FOTA]: This THING_ID: %s is not present in database file, Error msg: %s", thing_id, sqlite3_errmsg(db));
                sqlite3_finalize(sql_stmt);
                sqlite3_close(db);
                pthread_mutex_unlock(&upgrade_lock);
                --array_len;
                continue;
            }

            sqlite3_finalize(sql_stmt);
            sqlite3_close(db);

            /*
             * End of Fetching from database
             */

            memset(end_device_json, 0, sizeof(end_device_json));
            sprintf(end_device_json, "/root/downloads/fw/enddevota/%s_%s_%s.json", manufacturer_name, model_id, hardware_version);
            syslog(LOG_DEBUG, "[FOTA]: Path of ED CDN config %s", end_device_json);

            memset(firmware_version, 0, sizeof(firmware_version));
            status = extract_dnld_json(end_device_json, 0, "version", firmware_version);
            if (status) {
                syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
            }
            memset(file_path, 0, sizeof(file_path));
            sprintf(file_path, "/root/downloads/fw/enddevota/%s_%s_%s_%s.ota", manufacturer_name, model_id, hardware_version, firmware_version);
            syslog(LOG_DEBUG, "[FOTA]: path is %s", file_path);

            jsendto = json_object_new_object();
            jclient = json_object_new_int(CLIENT_FOTA);
            jcommand = json_object_new_string("FOTA_REQ");
            jthing_id = json_object_new_string(thing_id);
            jfirmware_path = json_object_new_string(file_path);
            jfirmware_ver = json_object_new_string(firmware_version);
            json_object_object_add(jsendto, "client", jclient);
            json_object_object_add(jsendto, "command", jcommand);
            json_object_object_add(jsendto, "path", jfirmware_path);
            json_object_object_add(jsendto, "thing_id", jthing_id);
            json_object_object_add(jsendto, "version", jfirmware_ver);

            syslog(LOG_DEBUG, "[FOTA]: FOTA request to DM [ IP_ADDR= %s, PORT= %d ] for upgrading the end_device %s",
                    inet_ntoa(dm_cliaddr.sin_addr), ntohs(dm_cliaddr.sin_port), json_object_to_json_string_ext(jsendto, JSON_C_TO_STRING_PRETTY));

            if (sendto(server_fd, json_object_to_json_string(jsendto), strlen(json_object_to_json_string(jsendto)) + 1,
                    MSG_CONFIRM, (const struct sockaddr *) &dm_cliaddr, sizeof(dm_cliaddr)) < 0) {
                syslog(LOG_ERR, "[FOTA]: FOTA request to DM failed for ThingID: %s", thing_id);
                json_object_put(jsendto);
                pthread_mutex_unlock(&upgrade_lock);
                --array_len;
                continue;
            }
            status = response_timer();
            json_object_put(jsendto);
            --array_len;
        } //while loop end

        json_object_put(jthingid_list);
        if (fota_silent_flag == 1)
            pthread_cond_signal(&download_cond);
        pthread_mutex_unlock(&upgrade_lock);
    } //while loop end
}


void signal_handler_thread(void) {
    int status;
    int signal;

    char changelog_name[FILENAME_LEN]               = {0};
    char release_version[VERSION_LEN]               = {0};
    char created_at[CREATED_AT_LEN]                 = {0};
    char changelog_string[80]                       = {0};
    char changelog_string_full[CHANGELOG_STR_LEN]   = {0};
    char sys_command[COMMAND_LEN]                   = {0};
    char valid_status[FILENAME_LEN]                 = {0};
    char cdn_filename[FILENAME_LEN]                 = {0};
    char cdn_json_file_path[PATH_LEN]               = {0};
    const char changelog_url[URL_LEN]               = {0};
    char internet_status[4] = { 0 };
    
    char end_device_id[LEN_256];
    char end_device_json[LEN_128];
    char json_file_path[LEN_128];
    char end_dev_dnld_json[LEN_128];
    char manufacturer_name[FILENAME_LEN];
    char hardware_version[VERSION_LEN];
    char model_id[FILENAME_LEN];

    int num_of_devices;
    int num_of_things_list;
    int max_things;

    json_object *jsendto = NULL;
    json_object *jbody = NULL;
    json_object *obj = NULL;
    
    while (1) {

        memset(changelog_name,0,sizeof(changelog_name));
        memset(release_version,0,sizeof(release_version));
        memset(created_at,0,sizeof(created_at));

        syslog(LOG_DEBUG, "[FOTA]: Waiting for a SIGUSR1 or SIGUSR2 signal\n");
        status = sigwait(&set, &signal);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: sigwait failed \n");
            continue;
        }

        if (signal == SIGUSR1) {
            syslog(LOG_DEBUG, "[FOTA]: Signal received to check for end devices firmware update");
            json_object *jenddev_array;
            json_object *jdev_json;
            json_object *jfile;

            jfile = json_object_from_file(SHIFU_VERSION_JSON_PATH);
            jenddev_array = json_object_object_get( jfile, "end_device");
            num_of_devices = json_object_array_length(jenddev_array);
            max_things = MAX_THINGS - 1;        /*Limiting the number of end devices to be updated*/

            for (int i = 0; i < num_of_devices; i++) {
                jthingid_obj[i] = json_object_new_array();
                jdev_json = json_object_array_get_idx(jenddev_array, i);

                strcpy(end_device_json, json_object_get_string(jdev_json));

                /*Check if there are any devices onboarded to t.hub and then perform check for update*/
                memset(end_device_id, 0, sizeof(end_device_id));
                strncpy(end_device_id, end_device_json, strlen(end_device_json) - strlen(".json"));

                split_end_device_id(end_device_id, manufacturer_name, model_id, hardware_version);
                status = num_of_things(END_DEVICE_DATABASE, manufacturer_name, model_id, hardware_version, &num_of_things_list); // Fix Me
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: Failed to get thingIDs list: %d", status);             ///FIXME:
                    continue;
                }
                syslog(LOG_DEBUG, "[FOTA]: Number of things related to END DEVICE ID %s in database:\t%d", end_device_id, num_of_things_list);

                if (!num_of_things_list) {
                    syslog(LOG_DEBUG, "[FOTA]: No ThingID related to END DEVICE ID %s in database", end_device_id);
                    continue;
                }

                /*Initiate download of config file and firmware*/
                sprintf(end_dev_dnld_json, "%sdnld_%s", ED_CONFIG_FILE_PATH, json_object_get_string(jdev_json));
                syslog(LOG_DEBUG, "[FOTA]: Download initiated, %s %s", end_device_json, end_dev_dnld_json);
                status = DownloadFirmware( "END_DEVICE", end_device_json, end_dev_dnld_json, ED_DNLD_FILES_PATH, changelog_name);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: %s download failed = %d", end_device_json, status);
                    continue;
                }

                memset(&json_file_path, 0, sizeof(json_file_path));
                sprintf(json_file_path, "%s%s", ED_DNLD_FILES_PATH, end_device_json);
                memset(release_version,0,sizeof(release_version));
                status = extract_dnld_json(json_file_path, 0, "version", release_version);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: Could not fetch release version from CDN json file.");
                    continue;
                }
                int rv_inv = version_string_to_int_conv(release_version);
                status = get_thingid(end_device_json, i, rv_inv, max_things);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: ThingID fetch from database failed, Error:%d", status);
                    continue;
                }
                syslog(LOG_DEBUG, "[FOTA]: thingid_obj[%d]=\t%s", i, json_object_get_string(jthingid_obj[i]));

                if (!fota_silent_flag) {
                    /*sending Notify FCM*/
                } else {
                        pthread_mutex_lock(&download_lock);
                        jthingid_list = json_tokener_parse(json_object_get_string(jthingid_obj[i]));
                        num_of_things_list = json_object_array_length(jthingid_list);
                        if (num_of_things_list){
                            syslog(LOG_DEBUG, "[FOTA]: Things list to upgrade \t%s", json_object_get_string(jthingid_list));
                            pthread_cond_signal(&upgrade_cond);
                            syslog(LOG_DEBUG, "[FOTA]: Waiting on download condition variable");
                            pthread_cond_wait(&download_cond, &download_lock); ///FIXME: sigwait and pthread wait sync
                            syslog(LOG_DEBUG, "[FOTA]: Released upgrade condition variable");
                        }
                        pthread_mutex_unlock(&download_lock);
                        max_things = max_things - num_of_things_list;
                        if (!max_things)
                            break;
                } // if loop end - fota_silent_flag
            } //for loop end
            json_object_put(jenddev_array);
            json_object_put(jfile);
        } else if (signal == SIGUSR2) {
            
            /****** t.hub fota check and download  *******/
            syslog(LOG_DEBUG, "[FOTA]:Signal received to Check updates for t.hub\n");
            
            if(ntohs(req_cliaddr.sin_port) != 0){
                
                memset(valid_status,0,sizeof(valid_status));
                status = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "validity_status", valid_status, false);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: Could not write download_status to status json file, Error: %d", status);
                    goto ERR;
                }
                if(strcmp(valid_status,"valid")==0){
                    memset(cdn_filename,0,sizeof(cdn_filename));
                    status = extract_current_json(SHIFU_VERSION_JSON_PATH, 0, "fota_filename", cdn_filename, false);
                    if (status) {
                        syslog(LOG_ERR, "[FOTA]: Could not fetch present Json filename from version json file.");
                        goto ERR;
                    }
                    snprintf(cdn_json_file_path, strlen(THUB_DNLD_FILES_PATH) + strlen(cdn_filename) + 1, "%s%s", THUB_DNLD_FILES_PATH, cdn_filename);
                    /* Extract changelog url from fota json file */
                    status = extract_dnld_json(cdn_json_file_path, 0, "change_log", changelog_url);
                    if (status) {
                        syslog(LOG_ERR, "[FOTA]: Could not extract changelog url");
                        goto ERR;
                    }
                    get_name_from_url(changelog_url, changelog_name);
                    status = SUCCESS;
                }
                else{
                    status = UP_TO_DATE;
                }
            }
            else{
                status = get_shifu_uci_param("INTERNET_STATUS", internet_status);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: No INTERNET_STATUS NV\n");
                    continue;
                }
                if(strcmp(internet_status,"0") == 0)
                {
                    syslog(LOG_DEBUG, "[FOTA]: Check for update will be processed when internet is available");
                    pthread_cond_signal(&internet_status_cond);
                    continue;
                }
                /* executed by Cron job or boot-up time  */
                status = check_update_thub(changelog_name);
            }
            
            jsendto = json_object_new_object(); 
            jbody = json_object_new_object();
            
            if (status == SUCCESS ){
                memset(sys_command,0,sizeof(sys_command));
                /* Get contents from changelog.md in string format */
                sprintf( sys_command, "cat %s%s", THUB_DNLD_FILES_PATH, changelog_name );
                FILE *fp = popen( sys_command, "r");
                
                if (fp == NULL) {
                    syslog(LOG_ERR, "[FOTA]: Failed to do open changelog.md file\n");
                    goto ERR;
                }
                memset(changelog_string,0,sizeof(changelog_string));
                memset(changelog_string_full,0,sizeof(changelog_string_full));
                while (fgets(changelog_string, sizeof(changelog_string), fp) != NULL) {
                    strncat(changelog_string_full, changelog_string, strlen(changelog_string) + 1);
                }
                pclose(fp);
                
                /* fetch Shifu Hub present version from download firmware json file */ /////////FIXEME:
                status = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "downloaded_version", release_version, false);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: Could not fetch present version from update json file.");
                    goto ERR;
                }
                syslog(LOG_DEBUG, "[FOTA]: release_version %s\n", release_version);
                
                status = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "uploaded_at",
                                              created_at, false);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: Could not fetch present version from update json file.");
                    goto ERR;
                }
                json_object_object_add(jbody, "release_version", json_object_new_string(release_version));
                json_object_object_add(jbody, "release_date", json_object_new_string(created_at));
                json_object_object_add(jbody, "change_log", json_object_new_string(changelog_string_full));
                json_object_object_add(jbody, "update", json_object_new_boolean(1));
                json_object_object_add(jbody, "message", json_object_new_string("Update is available"));
                json_object_object_add(jsendto, "status", json_object_new_string("success"));
                json_object_object_add(jsendto, "body", jbody);
                json_object_object_add(jsendto, "status_code", json_object_new_int(200));
                goto EXIT;
                
            } else if ( status == UP_TO_DATE ){
                
                json_object_object_add(jbody, "update", json_object_new_boolean(0));
                json_object_object_add(jbody, "message", json_object_new_string("Your t.hub firmware is up to date"));
                json_object_object_add(jsendto, "status", json_object_new_string("success"));
                json_object_object_add(jsendto, "body", jbody);
                json_object_object_add(jsendto, "status_code", json_object_new_int(200));
                goto EXIT;
            }
           ERR:
                if(ntohs(req_cliaddr.sin_port) == 0)
                pthread_cond_signal(&internet_status_cond);
                json_object_object_add(jbody, "message", json_object_new_string("Please try again later"));
                json_object_object_add(jsendto, "status", json_object_new_string("failure"));
                json_object_object_add(jsendto, "body", jbody);
                json_object_object_add(jsendto, "status_code", json_object_new_int(500));
           
           EXIT:
            if(ntohs(req_cliaddr.sin_port) != 0)
            {
                syslog(LOG_DEBUG, "[FOTA]: Send to client [ IP_ADDR= %s, PORT= %d ]  json=%s",
                       inet_ntoa(req_cliaddr.sin_addr), ntohs(req_cliaddr.sin_port), json_object_to_json_string_ext(jsendto, JSON_C_TO_STRING_PRETTY));
                if (sendto(server_fd, json_object_to_json_string(jsendto), strlen(json_object_to_json_string(jsendto)) + 1,
                    MSG_CONFIRM, (const struct sockaddr *) &req_cliaddr, sizeof(req_cliaddr)) < 0) {
                    syslog(LOG_ERR, "[FOTA]: Check Response to WS failed, Error Code: %s", strerror(errno));
                    }
            }
            memset(&req_cliaddr, 0, sizeof(req_cliaddr));
            pthread_mutex_lock(&status_lock);
            status = extract_current_json(FOTA_STATUS_JSON_PATH, 0, "download_status", "0", true);
            if (status) {
                syslog(LOG_ERR, "[FOTA]: Could not write download_status to status json file, Error: %d", status);
            }
            pthread_mutex_unlock(&status_lock);
            json_object_put(jbody);
            json_object_put(jsendto);
        } else {
            syslog(LOG_DEBUG, "[FOTA]:Not a valid signal: %d\n", signal);
        }
    } // while loop end
}


int check_update_thub(char *changelog_name){
    char fota_status[FLAG_LEN] = {0};
    char cdn_filename[FILENAME_LEN] = {0};
    int status;
    pthread_mutex_lock(&status_lock);
    extract_current_json(FOTA_STATUS_JSON_PATH, 0, "download_status", "1", true);
    pthread_mutex_unlock(&status_lock);
    /* fetch Shifu Hub JSON path from version json file */
    status = extract_current_json(SHIFU_VERSION_JSON_PATH, 0, "fota_filename", cdn_filename, false);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch present CDN filename from version json file, Error: %d", JSON_FILE_READ_ERR);
        return status;
    }
    syslog(LOG_DEBUG, "[FOTA]: JSON filename %s\n", cdn_filename);
    
    status = DownloadFirmware("SHIFU", cdn_filename, DNLD_FIRMWARE_JSON_PATH, THUB_DNLD_FILES_PATH, changelog_name);
    return status;
}

int status_check()
{
    int ret;
    char down_status[10],upgrade_status[10];
    pthread_mutex_lock(&status_lock);
    ret = extract_current_json(FOTA_STATUS_JSON_PATH, 0, "download_status", down_status, false);
    if (ret) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch download_status from status json file, Error: %d", JSON_FILE_READ_ERR);
        pthread_mutex_unlock(&status_lock);
        return JSON_FILE_READ_ERR;
    }
    if (down_status[0] == '1') {             ///This checks if there is no download initiated to initiate
        syslog(LOG_ERR, "[FOTA]: Check is in progress");
        pthread_mutex_unlock(&status_lock);
        return CHECK_IN_PROGRESS;
    }
    ret = extract_current_json(FOTA_STATUS_JSON_PATH, 0, "upgrade_status", upgrade_status, false);
    if (ret) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch upgrade_status from status json file, Error: %d", JSON_FILE_READ_ERR);
        pthread_mutex_unlock(&status_lock);
        return JSON_FILE_READ_ERR;
    }
    if (upgrade_status[0] == '1') {             ///This checks if there is no download initiated to initiate
        syslog(LOG_ERR, "[FOTA]: Upgrade is in progress");
        pthread_mutex_unlock(&status_lock);
        return UPGRADE_IN_PROGRESS;
    }
    pthread_mutex_unlock(&status_lock);
    return SUCCESS;
}

void receive_from_thread(void) {
    int bytes_received = 0, i, command_len = 0;
    char thing_id[64];
//    char status[64];        ///FIXME: Not required when FCM status and socket status are in sync
    char *command = NULL, *domain = NULL, *verb = NULL, *request_version = NULL;
    char cdn_filename[FILENAME_LEN];
    int status;
    char cdn_json_file_path[PATH_LEN] ={0};
    char image_name[FILENAME_LEN];
    int len = sizeof(struct sockaddr_in);
    json_object *jreceive = NULL;
    json_object *jclient = NULL;
    json_object *jcommand = NULL;
    json_object *jsendto = NULL;
    json_object *jthingid = NULL;
    json_object *jbody = NULL, *jwsbody = NULL;
    json_object *jdomain = NULL;
    json_object *jverb = NULL;
    json_object *obj = NULL;
    json_object *jversion = NULL;
    
    char release_version[VERSION_LEN] = { 0 };
    char sys_command[COMMAND_LEN]={0};
    int client;
    int req_ver = 0;
    int cdn_ver = 0;

    while (1) {
	memset(release_version,0,sizeof(release_version));
	memset(sys_command,0,sizeof(sys_command));
	memset(buffer,0,sizeof(buffer));
	client = DEFAULT_VALUE;
	request_version = NULL;
        
        bytes_received = recvfrom(server_fd, (char *) buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
        if (bytes_received == 0)
            continue;

        buffer[bytes_received] = '\0';
        syslog(LOG_DEBUG, "[FOTA]: buffer is : %s", buffer);

        jreceive = json_tokener_parse(buffer);
	jclient = json_object_object_get(jreceive, "client");
        if (jclient != NULL) {//Redmine::22100
            client = json_object_get_int(jclient);
	    syslog(LOG_DEBUG, "[FOTA]: Received request from client = [ %d ]",client);
            jbody = json_object_object_get(jreceive, "body");
	    if (jbody != NULL) {
		jdomain = json_object_object_get(jbody, "domain");
		if (jdomain != NULL) {
		    domain = json_object_get_string(jdomain);
		}
		jverb = json_object_object_get(jbody, "verb");
		if (jverb != NULL)
		    verb = json_object_get_string(jverb);
                jversion = json_object_object_get(jbody, "version");
                if (jversion != NULL)
                    request_version = json_object_get_string(jversion);
	    }
	    jcommand = json_object_object_get(jreceive, "command");
	    if (jcommand != NULL) {
	      command_len = strlen(json_object_get_string(jcommand));
	      command = json_object_get_string(jcommand);
	      syslog(LOG_DEBUG, "[FOTA]: Command = [%s]", command);
	    }
	}

        switch (client) {
            case CLIENT_WS:
            case CLIENT_LUCI:
            {
                syslog(LOG_DEBUG, "[FOTA]: Received request from WS client");
                char fota_status[20];
                int array_len = 0, i = 0;
                
                jsendto = json_object_new_object();
                
                if (!strcmp(domain, "SHIFU")) {
                    json_object_object_add(jsendto, "client",  json_object_new_int(CLIENT_FOTA));
                    /** handling multiple request from client  **/
                    /*checking already any check is in progress*/
                    status=status_check();

                    if ( status == CHECK_IN_PROGRESS){
                        json_object_object_add(jbody, "message", json_object_new_string("Check for update in progress, please wait..."));
                        json_object_object_add(jsendto, "status", json_object_new_string("error"));
                        json_object_object_add(jsendto, "body", jbody);
                        json_object_object_add(jsendto, "status_code", json_object_new_int(405));
                        goto END;
                    } else if (status == UPGRADE_IN_PROGRESS){
                        json_object_object_add(jbody, "message", json_object_new_string("Firmware update in progress, Please wait..."));
                        json_object_object_add(jsendto, "status", json_object_new_string("error"));
                        json_object_object_add(jsendto, "body", jbody);
                        json_object_object_add(jsendto, "status_code", json_object_new_int(429));
                        goto END;
                    }
                    else if (status == SUCCESS){
                        memset(&req_cliaddr, 0, sizeof(req_cliaddr));
                        /*Fix for 22294,started here */
                        if (client == CLIENT_WS)
                            req_cliaddr=cliaddr;
                        else if (client == CLIENT_LUCI)
                            req_cliaddr=luci_cliaddr;
                        /** Ended here **/
                        if (!strcmp(verb, "check")) {
                            status = extract_current_json(FOTA_STATUS_JSON_PATH, 0, "download_status", "1", true);
                            if (status) {
                                syslog(LOG_ERR, "[FOTA]: Could not write download_status to status json file, Error: %d", status);
                                goto ERROR;
                            }
                            syslog(LOG_DEBUG, "[FOTA]: Sending signal (SIGUSR2) to signal handler thread ");
                            kill(getpid(),SIGUSR2);          
                            break;
                        }
                        else if (!strcmp(verb, "update")) {           ///Checks if verb is update
                            if (client == CLIENT_WS)
                            {
                                memset(cdn_filename,0,sizeof(cdn_filename));
                                status = extract_current_json(SHIFU_VERSION_JSON_PATH, 0, "fota_filename", cdn_filename, false);
                                if (status) {
                                    syslog(LOG_ERR, "[FOTA]: Could not fetch present Json filename from version json file.");
                                    goto ERROR;
                                }
                                snprintf(cdn_json_file_path, strlen(THUB_DNLD_FILES_PATH) + strlen(cdn_filename) + 1, "%s%s", THUB_DNLD_FILES_PATH, cdn_filename);
                                status = extract_dnld_json(cdn_json_file_path, 0, "version", release_version);
                                if (status) {
                                    syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
                                    goto ERROR;
                                }
                                
                                if(request_version == NULL)
                                {
                                    syslog(LOG_ERR, "[FOTA]: Not receive any version from client.");
                                    goto ERROR;
                                }
                                
                                cdn_ver = version_string_to_int_conv(release_version);
                                req_ver = version_string_to_int_conv(request_version);
                                if (cdn_ver > req_ver){
                                    syslog(LOG_ERR, "[FOTA]: Requested version lower than the released version.");
                                    json_object_object_add(jbody, "message", json_object_new_string("New firmware is available. Please try again..."));
                                    json_object_object_add(jsendto, "status", json_object_new_string("failure"));
                                    json_object_object_add(jsendto, "body", jbody);
                                    json_object_object_add(jsendto, "status_code", json_object_new_int(500));
                                    goto END;  
                                }
                                else if (cdn_ver < req_ver){
                                    syslog(LOG_ERR, "[FOTA]: Requested version higher than the released version.");
                                    goto ERROR;
                                }
                            }

                            status = extract_current_json(FOTA_STATUS_JSON_PATH, 0, "upgrade_status", "1", true);
                            if (status) {
                                syslog(LOG_ERR, "[FOTA]: Could not write upgrade_status to status json file, Error: %d", status);
                                goto ERROR;
                            }
                            syslog(LOG_ERR, "[FOTA]: Sending signal to Upgarde thread");
                            pthread_cond_signal(&thub_upgrade_cond);
                            break;
                            
                        }
                    }
                    else {
                        syslog(LOG_ERR, "[FOTA]: Error occured in status check");
                        goto ERROR;
                    }
                }               ///End of if loop - string compare SHIFU
                
                ERROR:
                json_object_object_add(jbody, "message", json_object_new_string("Please try again later"));
                json_object_object_add(jsendto, "status", json_object_new_string("failure"));
                json_object_object_add(jsendto, "body", jbody);
                json_object_object_add(jsendto, "status_code", json_object_new_int(500));
               
                END:
                if(client == CLIENT_WS) {
                    syslog(LOG_DEBUG, "[FOTA]: Send to WS [ IP_ADDR= %s, PORT= %d ]  json=%s",
                           inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), json_object_to_json_string_ext(jsendto, JSON_C_TO_STRING_PRETTY));
                    if (sendto(server_fd, json_object_to_json_string(jsendto), strlen(json_object_to_json_string(jsendto)) + 1,
                        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0) {
                        syslog(LOG_ERR, "[FOTA]: Check Response to WS failed, Error Code: %s", strerror(errno));
                        }
                } else if (client == CLIENT_LUCI) {
                    syslog(LOG_DEBUG, "[FOTA]: Send to LUCI [ IP_ADDR= %s, PORT= %d ]  json=%s",
                           inet_ntoa(luci_cliaddr.sin_addr), ntohs(luci_cliaddr.sin_port), json_object_to_json_string_ext(jsendto, JSON_C_TO_STRING_PRETTY));
                    if (sendto(server_fd, json_object_to_json_string(jsendto), strlen(json_object_to_json_string(jsendto)) + 1,
                        MSG_CONFIRM, (const struct sockaddr *) &luci_cliaddr, sizeof(luci_cliaddr)) < 0) {         
                        syslog(LOG_ERR, "[FOTA]: Check Response to LUCI failed, Error Code: %s", strerror(errno));
                        }
                }

                json_object_put(jsendto);
                break;
            }
            case CLIENT_DM: {
                
                syslog(LOG_DEBUG, "[FOTA]: Received request from DM client");
                if (strcmp(command, "FOTA_RES") == 0) {
                    
                    status = json_object_get_int(json_object_object_get(jreceive, "status"));
                    jthingid = json_object_object_get(jreceive, "thing_id");
                    
                    strcpy(thing_id, json_object_get_string(jthingid));
                    
                    if (!status) {
                        syslog(LOG_DEBUG, "[FOTA]: END_DEVICE upgraded successfully, ThingID is [%s]", thing_id);
                    } else {
                        syslog(LOG_DEBUG, "[FOTA]: END_DEVICE Upgrade failed for Thing_id = [%s], error code %d", thing_id, status);
                    }
                    pthread_cond_signal(&receive_cond);
                }
                break;
            } 
            default:
		syslog(LOG_CRIT, "[FOTA]: In default, received unknown request from client = [%d]",client);
                break;

        } // END of Switch case
        json_object_put(jreceive);
    } // END of While loop
} // END of receive_from_thread

void internet_status_thread()
{
    int status;
    char internet_status[4] = { 0 };
    while(1)
    {
        pthread_mutex_lock(&internet_status_lock);
        pthread_cond_wait(&internet_status_cond, &internet_status_lock);
        syslog(LOG_DEBUG, "[FOTA]: Internet status thread has received signal from Signal handler thread");
        pthread_mutex_unlock(&internet_status_lock);
        while(1)
        {
            status = get_shifu_uci_param("INTERNET_STATUS", internet_status);
            if (status) {
                syslog(LOG_ERR, "[FOTA]: No INTERNET_STATUS NV\n");
                break;
            }
            if(strcmp(internet_status,"1") == 0) {
                syslog(LOG_DEBUG, "[FOTA]: Internet has arrived");
                /* check if another check-for-update is in progress */
                status = status_check();
                if (status == SUCCESS) {
                    syslog(LOG_DEBUG, "[FOTA]: Signaling to check for update thread");
                    kill(getpid(), SIGUSR2);
                    break;
                }
            }
            sleep(10);
        }
    }
}

int main(int argc, char const * argv[]) {
    pthread_t client_receive_tid;
    pthread_t enddev_upgrade_request_tid;
    pthread_t enddev_download_request_tid;
    pthread_t thub_upgrade_tid;
    pthread_t internet_status_tid;
    char changelog_name[FILENAME_LEN];

    int status = 0;

    status = extract_current_json(FOTA_STATUS_JSON_PATH, 0, "download_status", "0", true);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not write download_status to status json file.");
    }

    status = extract_current_json(FOTA_STATUS_JSON_PATH, 0, "upgrade_status", "0", true);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not write download_status to status json file.");
    }

    create_socket_server();
    client_info();
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, NULL);

    DOWNLOAD_REQ: if (pthread_create(&enddev_download_request_tid, NULL, signal_handler_thread, NULL)) {
        syslog(LOG_ERR, "[FOTA]: signal_handler_thread failed \n");
        goto DOWNLOAD_REQ;
    }
    THUB_UPGRADE: if (pthread_create(&thub_upgrade_tid, NULL, thub_upgrade_thread, NULL)) {
        syslog(LOG_ERR, "[FOTA]: thub_upgrade_thread failed \n");
        goto THUB_UPGRADE;
    }
    UPGRADE_REQ: if (pthread_create(&enddev_upgrade_request_tid, NULL, thing_upgrade_thread, NULL)) {
        syslog(LOG_ERR, "[FOTA]: thing_upgrade_thread failed \n");
        goto UPGRADE_REQ;
    }
    CLIENT_RECV: if (pthread_create(&client_receive_tid, NULL, receive_from_thread, NULL)) {
        syslog(LOG_ERR, "[FOTA]: receive_from_thread failed \n");
        goto CLIENT_RECV;
    }
    INTERNET_STATUS: if (pthread_create(&internet_status_tid, NULL, internet_status_thread, NULL)) {
        syslog(LOG_ERR, "[FOTA]: internet_status_thread failed \n");
        goto INTERNET_STATUS;
    }
    kill(getpid(), SIGUSR2);
    pthread_join(enddev_download_request_tid, NULL);
    pthread_join(enddev_upgrade_request_tid, NULL);
    pthread_join(client_receive_tid, NULL);

}
