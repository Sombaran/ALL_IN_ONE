/*
 * end_device.c
 *
 *  Created on: 15 Mar 2019
 *     Version: 1.0
 *      Author: Praveen Talari, Priyanka P
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <signal.h>
#include <errno.h>
#include "end_device.h"

// Declaration of thread condition variable
pthread_cond_t download_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t upgrade_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t receive_cond = PTHREAD_COND_INITIALIZER;

// declaring mutex
pthread_mutex_t download_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t upgrade_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receive_lock = PTHREAD_MUTEX_INITIALIZER;

int fcm_push(bool type, char *verb, json_object *thing_id, char *ed_file, char *ed_version,
        char *ed_status);
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

void client_info(void) {

    memset(&ma_cliaddr, 0, sizeof(ma_cliaddr));

    ma_cliaddr.sin_family = PF_INET;
    ma_cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ma_cliaddr.sin_port = htons(MA_PORT);

    memset(&dm_cliaddr, 0, sizeof(dm_cliaddr));

    dm_cliaddr.sin_family = PF_INET;
    dm_cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    dm_cliaddr.sin_port = htons(DM_PORT);
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
    json_object *obj;

    if (jso != NULL) {
        syslog(LOG_DEBUG, "[FOTA]: Fetched JSON file from\t%s\n", path);
        obj = json_object_object_get(jso, attribute);
        if (obj != NULL)
            syslog(LOG_DEBUG, "[FOTA]: %s: %s\n", attribute, json_object_get_string(obj));
        else
            syslog(LOG_DEBUG, "[FOTA]: Json key %s not present in file\t%s\n", attribute, path);

        if (b_write) {
            json_object *write_obj = json_object_new_string(value);
            json_object_object_add(jso, attribute, write_obj);

            int ret = json_object_to_file_ext(path, jso, JSON_C_TO_STRING_PRETTY);
            if (!ret) {
                syslog(LOG_DEBUG, "[FOTA]: Added %s value to JSON Document\n", attribute);
            } else {
                syslog(LOG_ERR, "[FOTA]: Failed to add %s value to JSON Document\n");
                json_object_put(jso);
                return JSON_FILE_WRITE_ERR;
            }
            json_object_put(jso);
        } else
            snprintf(value, strlen(json_object_get_string(obj)) + 1, "%s",
                    json_object_get_string(obj));
    } else {
        syslog(LOG_ERR, "[FOTA]: FAIL: json_object_from_file(%s) is not present \n", path);
        return JSON_FILE_READ_ERR;
    }
    return SUCCESS;
}

/**
 * @brief Writing download stream into a local file with name "fname".
 *
 * @param fname Name of the download file
 * @return fp File pointer to which download data is being written to.
 */
FILE* get_dnld_stream(char const* const fname) {
    char const* const pre = "/root/downloads/fw/enddevota/";
    char out[PATH_LEN];

    snprintf(out, sizeof(out), "%s%s", pre, fname);

    FILE *fp = fopen(out, "wb");
    if (!fp) {
        syslog(LOG_ERR, "[FOTA]: Could not create file %s\n", out);
    }
    return fp;
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

int dnld_file_from_url(char const* const url) {
    CURL *curl;
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
    cerr = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);
    if (cerr != CURLE_OK) {
        syslog(LOG_ERR, "[FOTA]: %s: dnld_file_from_url failed with err %d\n",
                "CURLOPT_CONNECTTIMEOUT", cerr);
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

        syslog(LOG_ERR, "Transient problem: %s\n", m[retry]);
        syslog(LOG_CRIT, "[FOTA]: CURL request failed with error %d\n", cerr);

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

    return SUCCESS;
}

/**
 * @brief To extract data from downloaded Fota JSON file.
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
        json_object_get_array(image_array);

        json_object *image = json_object_array_get_idx(image_array, ith_image); /// FIXE ME: error handling
        if (image != NULL) {
            json_object *attr_obj = json_object_object_get(image, attribute); /// FIXE ME: error handling
            if (attr_obj != NULL) {
                snprintf(value, strlen(json_object_get_string(attr_obj)) + 1, "%s",
                        json_object_get_string(attr_obj)); //// FIXEME:
                syslog(LOG_DEBUG, "[FOTA]: Attribute: %s: %s\n", attribute, value);
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
 * @brief Function to convert string format version to type int form for easy comparision.
 * eg. 1.11.9 => 0001|0011|0009 => 000100110009.
 *
 * @parm string_version version number in string format.
 * @return atoi(str) 12 numbers long int format.
 */
int version_string_to_int_conv(char *string_version) {
    char *token;
    int i = 0, len;
    char str1[5] = "0", str2[5] = "\0", str[13] = "\0";

    while (token = strtok_r(string_version, ".", &string_version)) {
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
 * @brief Function to check validity of the Firmware image downloaded and set the value in dnld_firmware_info json
 *        to "valid" or "invalid" accordingly.
 *
 * @param path Path where firmware image is saved.
 */
int check_image_validity(char *path, char *image_name, char *download_json) {

    char md5key[MD5_LEN] = { 0 }, md5sum[MD5_LEN] = { 0 }, created_at[CREATED_AT_LEN] = { 0 },
            release_version[VERSION_LEN] = { 0 };
    //char *command = "md5sum /root/downloads/fw/enddevota/*.img | awk '{print $1;}'";
    char command[128];
    int status = 0;
    FILE * fp;

    sprintf(command, "md5sum /root/downloads/fw/enddevota/%s*.ota | awk '{print $1;}'", image_name);
    syslog(LOG_DEBUG, "[FOTA]: command for md5sum %s\n", command);
    /* Fetch md5key from fota json file */
    status = extract_dnld_json(path, 0, "md5key", md5key);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch md5key from Fota json file.");
        return JSON_FILE_READ_ERR;
    }

    syslog(LOG_DEBUG, "[FOTA]: md5key %s\n", md5key);

    fp = popen(command, "r");

    if (fp == NULL) {
        syslog(LOG_ERR, "[FOTA]: Failed to do md5sum on firmware image\n");
        return MD5_FETCH_ERR;
    }

    while (fgets(md5sum, sizeof(md5sum), fp) != NULL) {
    }
    status = pclose(fp);
    md5sum[strlen(md5sum) - 1] = '\0';

    /* Comparing md5key sent though cloud and md5key generated on shifu to validate the downloaded image. */
    if (strcmp(md5key, md5sum)) {
        syslog(LOG_DEBUG, "[FOTA]: Invalid image downloaded\n");
        /* Set validity_status value to invalid in update file */
        extract_current_json(download_json, 0, "validity_status", "invalid", true);

        memset(&command, 0, sizeof(command));
        sprintf(command, "rm /root/downloads/fw/enddevota/%s.*", image_name);
        syslog(LOG_ERR, "[FOTA]: %s\n", command);

        fp = popen(command, "w");
        if (fp == NULL) {
            syslog(LOG_ERR, "[FOTA]: Failed to do remove files\n");
        }

        status = pclose(fp);
        return MD5_COMP_FAIL;

    } else {
        /* Fetch release version from fota json file */
        status = extract_dnld_json(path, 0, "version", release_version);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
            return JSON_FILE_READ_ERR;
        }
        syslog(LOG_DEBUG, "[FOTA]: Image is valid, %s\n", release_version);
        /* Set validity_status value to valid in update file */
        status = extract_current_json(download_json, 0, "validity_status", "valid", true);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not set validity_status in update file.");
            return JSON_FILE_WRITE_ERR;
        }
        /* Set release version value in download firmware json file */
        status = extract_current_json(download_json, 0, "downloaded_version", release_version,
        true);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not set release version in update file.");
            return JSON_FILE_WRITE_ERR;
        }
        /* Extract created_at from fota json file */
        status = extract_dnld_json(path, 0, "created_at", created_at);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not extract created_at");
            return JSON_FILE_READ_ERR;
        }
        /* Set uploaded_at value in downloaded firmware json file */
        status = extract_current_json(download_json, 0, "uploaded_at", created_at, true);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not set release version in update file.");
            return JSON_FILE_WRITE_ERR;
        }
        return SUCCESS;
    }
}

/**
 * @brief Function to compare version number in the int format mentioned in version_string_to_int_conv()
 *
 * @parm v1 First version.
 * @parm v2 Second version.
 * @return 0 If v1 = v2.
 *           1 If v1 > v2.
 *          -1 If v1 < v2.
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

int num_of_things(char *db_file, char *manufacturer_name, char *model_id, char *hardware_version,
        int *row_count) {
    sqlite3 *db;
    sqlite3_stmt *ppStmt;
    int status;
    int step;                                                       ///FIXME:
    char sql_stmt[128];                                             ///FIXME:

    status = sqlite3_open(END_DEVICE_DATABASE, &db);

    if (status != SQLITE_OK) {
        syslog(LOG_ERR, "[FOTA]:[%s] Cannot open database: %s\n", END_DEVICE_DATABASE,
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return status;
    }
    sprintf(sql_stmt,
            "SELECT count(*) from FIRMWARE where MANUFACTURER_NAME=\"%s\" AND MODEL_ID=\"%s\" AND HARDWARE_VERSION=\"%s\"",
            manufacturer_name, model_id, hardware_version);          ///FIXME:
    syslog(LOG_DEBUG, "[FOTA]: %s", sql_stmt);
    status = sqlite3_prepare_v2(db, sql_stmt, -1, &ppStmt, 0);          ///FIXME:
    if (status != SQLITE_OK) {
        sqlite3_close(db);
        return status;
    }

    step = sqlite3_step(ppStmt);

    if (step == SQLITE_ROW)                                 ///FIXME: while loop and sqlite3_reset
        *row_count = atoi(sqlite3_column_text(ppStmt, 0));
    else {
        *row_count = 0;
        syslog(LOG_ERR, "[FOTA]: Cannot read the database\n");
    }
    sqlite3_finalize(ppStmt);
    sqlite3_close(db);
    return SUCCESS;

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

void split_end_device_id(char *end_device_id, char *manufacturer_id, char *model_id,
        char *hardware_version) {                   ///FIXME:
    char* token;
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

int get_thingid(const char *device_file, int level) {

    int status;
    int thing_ids;

    sqlite3 *db;
    sqlite3_stmt *res;
    char *err_msg = 0;
    char sql_stmt[256];

    char thing_id[64];                                      ///FIXME:
    int version_check;
    char release_version[VERSION_LEN] = { 0 }, json_file_path[128], end_device_id[256];    ///FIXME:
    int rv_inv;

    memset(&end_device_id, 0, sizeof(end_device_id));
    strncpy(end_device_id, device_file, strlen(device_file) - strlen(".json"));
    memset(Hardware_version, 0, sizeof(Hardware_version));
    memset(Manufacturer_name, 0, sizeof(Manufacturer_name));
    memset(Model_id, 0, sizeof(Model_id));

    split_end_device_id(end_device_id, Manufacturer_name, Model_id, Hardware_version);
    syslog(LOG_DEBUG, "[FOTA]: %s %s %s \n", Manufacturer_name, Model_id, Hardware_version);

    status = num_of_things(END_DEVICE_DATABASE, Manufacturer_name, Model_id, Hardware_version,
            &thing_ids); // Fix Me
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Failed to get thingIDs list: %d", status);             ///FIXME:
    }

    if (!thing_ids) {
        syslog(LOG_DEBUG, "[FOTA]: No ThingID related to END DEVICE ID [ %s ] in database",
                end_device_id);
        return SUCCESS;                                                                  ///FIXME:
    }
    syslog(LOG_DEBUG, "[FOTA]: ThingIDs related to END DEVICE ID [ %s ] in database : %d",
            end_device_id, thing_ids);

//    for (j = 0; j < thing_ids; j++)

    while (thing_ids) {                                                             ///FIXME:
        status = sqlite3_open(END_DEVICE_DATABASE, &db);
        if (status != SQLITE_OK) {
            syslog(LOG_ERR, "[FOTA]: Cannot open database: %s", sqlite3_errmsg(db));
            sqlite3_close(db);
//            return status;
            continue;
        }

        sprintf(sql_stmt,
                "SELECT THING_ID, FIRMWARE_VERSION FROM FIRMWARE where MANUFACTURER_NAME=\"%s\" AND MODEL_ID=\"%s\" AND HARDWARE_VERSION=\"%s\"  LIMIT 1 OFFSET %d",
                Manufacturer_name, Model_id, Hardware_version, thing_ids - 1);
        status = sqlite3_prepare_v2(db, sql_stmt, -1, &res, 0);

        if (status != SQLITE_OK) {
            syslog(LOG_ERR, "[FOTA]: Failed to prepare statement: %s", sqlite3_errmsg(db));
            sqlite3_close(db);
            continue;
        }

        status = sqlite3_step(res);

        if (status == SQLITE_ROW) {                                                 ///FIXME:
            memset(&thing_id, 0, sizeof(thing_id));                                ///FIXME:
            memset(&Firmware_version, 0, sizeof(Firmware_version));              ///FIXME:

            strncpy(thing_id, sqlite3_column_text(res, 0), strlen(sqlite3_column_text(res, 0)));
            strncpy(Firmware_version, sqlite3_column_text(res, 1),
                    strlen(sqlite3_column_text(res, 1)));
            syslog(LOG_DEBUG, "[FOTA]: Thingid =[%s] Firmware_version=[%s]", thing_id,
                    Firmware_version);
        } else {
            syslog(LOG_ERR, "[FOTA]: unable to read DB\n");
            --thing_ids;
            sqlite3_finalize(res);
            sqlite3_close(db);
            continue;
        }
        sqlite3_finalize(res);
        sqlite3_close(db);

        sprintf(json_file_path, "/root/downloads/fw/enddevota/%s", device_file);      ///FIXME:
        status = extract_dnld_json(json_file_path, 0, "version", release_version);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
            --thing_ids;
            continue;                                                                  ///FIXME:
        }
        syslog(LOG_DEBUG, "[FOTA]: END DEVICE release version %s\n", release_version);

        rv_inv = version_string_to_int_conv(release_version);
        version_check = version_comparison(rv_inv, atoi(Firmware_version));
//        syslog(LOG_DEBUG, "[FOTA]:version_check ==[%d]\n", version_check);

        if (version_check == 1) { /* Release version greater than current version */
            json_object *jthingid;
            jthingid = json_object_new_string(thing_id);
            json_object_array_add(thingid_obj[level], jthingid); //storing thingids into json array
        } else
            syslog(LOG_DEBUG, "[FOTA]: END DEVICE [%s] ThingID [%s] up-to-date", end_device_id,
                    thing_id);
        --thing_ids;
    } //for loop end
    return SUCCESS;
}

int DownloadFirmware(char *device_json, char *download_json) {
    const char changelog_url[URL_LEN] = { 0 }, download_url[URL_LEN] = { 0 };
    char downloaded_version[VERSION_LEN] = { 0 }, release_version[VERSION_LEN] = { 0 },
            current_version[VERSION_LEN] = { 0 }, md5key[MD5_LEN] = { 0 }, md5sum[MD5_LEN] = { 0 },
            created_at[CREATED_AT_LEN] = { 0 }, json_filename[JSON_FILENAME_LEN] = { 0 },
            file_status[FLAG_LEN] = { 0 };
    FILE * fp;
    int status;
    char *err_msg = 0;
    char image[256]; //combination of manufacture_name_model_id_hw ver
    bool download_flag = false;
    int version_check;
    int cv_inv, rv_inv, dv_inv;

    strcpy(json_filename, device_json);                                 ///FIXME:
    syslog(LOG_DEBUG, "[FOTA]: JSON filename [ %s ]\n", json_filename);
    memset(&fota_json_file_path, 0, PATH_LEN);
    memset(&fota_end_device_json_cdn_url, 0, URL_LEN);

    sprintf(fota_json_file_path, "/root/downloads/fw/enddevota/%s", json_filename);        ///FIXME:
    sprintf(fota_end_device_json_cdn_url, "https://d10fcns44yeyj9.cloudfront.net/json-file/%s",
            json_filename);         // Fix Me

    syslog(LOG_DEBUG,
            "[FOTA]: Path of to save fota json file: %s\nPath of file with present firmware details: %s\nFota json URL: %s\n",
            fota_json_file_path, download_json, fota_end_device_json_cdn_url);

    /* download fota json to get changelog and image download urls */
    status = dnld_file_from_url(fota_end_device_json_cdn_url);
    if (status) {
        syslog(LOG_CRIT, "[FOTA]: Fota json file download failed.");
        return status;
    }
    /* fetch release version from fota json file */
    status = extract_dnld_json(fota_json_file_path, 0, "version", release_version);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
        return JSON_FILE_READ_ERR;
    }
    syslog(LOG_DEBUG, "[FOTA]: Release version %s\n", release_version);

    /* fetch Shifu Hub present version from download firmware json file */
    status = extract_current_json(download_json, 0, "downloaded_version", downloaded_version,
    false);
    if (status) {
        syslog(LOG_ERR, "[FOTA]: Could not fetch present version from update json file.");
        return JSON_FILE_READ_ERR;
    }
    syslog(LOG_DEBUG, "[FOTA]: last downloaded firmware version %s", downloaded_version);

    rv_inv = version_string_to_int_conv(release_version);
    dv_inv = version_string_to_int_conv(downloaded_version);

    /* comparing release_version and downloaded_version*/
    version_check = version_comparison(rv_inv, dv_inv);
    syslog(LOG_DEBUG, "[FOTA]: rv_inv = %d\tdv_inv = d", rv_inv, dv_inv);
    if (version_check == 1) { /* Release version greater than previous downloaded version */
        download_flag = true;
    } else {
        download_flag = false;
    }
    if (download_flag) {
        syslog(LOG_DEBUG, "[FOTA]: Downloading image and changelog");

        /* Extract changelog url from fota json file */
        status = extract_dnld_json(fota_json_file_path, 0, "change_log", changelog_url);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not extract changelog url");
            return JSON_FILE_READ_ERR;
        }
        syslog(LOG_DEBUG, "[FOTA]: END DEVICE change_log is %s", changelog_url);
        /* Extract download url from fota json file */
        status = extract_dnld_json(fota_json_file_path, 0, "download_url", download_url);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not extract url for image download");
            return JSON_FILE_READ_ERR;
        }
        syslog(LOG_DEBUG, "[FOTA]: END DEVICE download_url is %s", download_url);

        char *urls[] = { changelog_url, download_url };
        for (size_t i = 0; i < sizeof(urls) / sizeof(char *); i++) {
            /* downloads */
            status = dnld_file_from_url(urls[i]);
            if (status) {
                syslog(LOG_CRIT, "[FOTA]: Download from %s failed", urls[i]);
                return status;
            }
        } //for loop end
        memset(&image, 0, sizeof(image));
        strncpy(image, json_filename, strlen(json_filename) - strlen(".json"));

        status = check_image_validity(fota_json_file_path, image, download_json);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Image validity check failed");
            return status;
        }
    } //if end
    return SUCCESS;

}

int response_timer(void) {
    struct timespec timeToWait;
    struct timeval now;
    int status;

    gettimeofday(&now, NULL);

    timeToWait.tv_sec = now.tv_sec + (60 * 2);
    timeToWait.tv_nsec = (now.tv_usec + 1000UL) * 1000UL;

    //pthread_mutex_lock(&receive_lock);
    syslog(LOG_DEBUG, "[FOTA]: Waiting for a signal with specific period");
    status = pthread_cond_timedwait(&receive_cond, &upgrade_lock, &timeToWait);
    if (status)
        syslog(LOG_CRIT, "[FOTA]: Response Timed out [%s]", strerror(errno));
    //pthread_mutex_unlock(&receive_lock);
    return status;
}

void receive_from_thread(void) {
    int bytes_received = 0, i, command_len = 0;
    char thing_id[64];
    char status[64];            ///FIXME: Not required when FCM status and socket status are in sync
    char *command;

    int len = sizeof(struct sockaddr_in);

    json_object *jreceive;
    json_object *jclient;
    json_object *jcommand;
    json_object * jsendto;
    json_object *jthingid;

    int client;

    while (1) {

        bytes_received = recvfrom(server_fd, (char *) buffer, MAXLINE, MSG_WAITALL,
                (struct sockaddr *) &cliaddr, &len);
        if (bytes_received == 0)
            continue;

        buffer[bytes_received] = '\0';
        syslog(LOG_DEBUG, "[FOTA]: buffer = [%s]", buffer);

        jreceive = json_tokener_parse(buffer);
        syslog(LOG_DEBUG, "[FOTA]: DEBUG 1");
        jclient = json_object_object_get(jreceive, "client");
        jcommand = json_object_object_get(jreceive, "command");

        client = json_object_get_int(jclient);
        command_len = strlen(json_object_get_string(jcommand));
        command = json_object_get_string(jcommand);
        syslog(LOG_DEBUG, "[FOTA]: Command = [%s]", command);

        switch (client) {
            case CLIENT_MA:

                syslog(LOG_DEBUG, "[FOTA]:Received request from MA client");
                int array_len = 0, i = 0;
                json_object *obj;                                   ///FIXME: object put

                jsendto = json_object_new_object();
                json_object_object_add(jsendto, "client", json_object_new_int(CLIENT_FOTA));
                json_object_object_add(jsendto, "command", json_object_new_string("FOTA_ACK"));
                syslog(LOG_DEBUG, "[FOTA]: FOTA ACK to MA [ IP_ADDR= %s, PORT= %d ]  json=[%s]\n",
                        inet_ntoa(ma_cliaddr.sin_addr), ntohs(ma_cliaddr.sin_port),
                        json_object_get_string(jsendto));

                if (sendto(server_fd, json_object_to_json_string(jsendto),
                        strlen(json_object_to_json_string(jsendto)) + 1,
                        MSG_CONFIRM, (const struct sockaddr *) &ma_cliaddr, sizeof(ma_cliaddr))
                        < 0) {
                    syslog(LOG_ERR, "[FOTA]: FOTA ACK to MA failed, Error Code: %s",
                            strerror(errno));
                    json_object_put(jsendto);
                    free(command);
                    continue; // Ack failed, and so, goto recvfrom for FOTA_SILENT request from MA
                }
                json_object_put(jsendto);           ///FIXME: can jsendto re-used?

                if (strcmp(command, "FOTA_SILENT") == 0) {

                    fota_silent_flag = json_object_get_int(
                            json_object_object_get(jreceive, "status"));

                    if (fota_silent_flag == 1) {

                        pthread_mutex_lock(&upgrade_lock);
                        jthingid_list = json_object_new_array();
                        for (i = 0; i < MAX_END_DEVICE; i++) {

                            if (thingid_obj[i] == NULL) {
                                free(command);
                                continue;
                            }
                            array_len = json_object_array_length(thingid_obj[i]);
                            while (array_len) {

                                obj = json_object_array_get_idx(thingid_obj[i], array_len - 1);
                                json_object_array_add(jthingid_list,
                                        json_object_new_string(json_object_get_string(obj)));
                                --array_len;
                            }
                        }
                        syslog(LOG_DEBUG, "[FOTA]: Thingid list [%s]",
                                json_object_get_string(jthingid_list));
                        pthread_cond_signal(&upgrade_cond);
                        pthread_mutex_unlock(&upgrade_lock);
                    }
                } else if (strcmp(command, "FOTA_REQ") == 0) {

                    pthread_mutex_lock(&upgrade_lock);
                    jthingid_list = json_object_new_array();
                    jthingid = json_object_object_get(jreceive, "thing_id");
                    array_len = json_object_array_length(jthingid);
                    while (array_len) {

                        obj = json_object_array_get_idx(jthingid, array_len - 1);
                        json_object_array_add(jthingid_list,
                                json_object_new_string(json_object_get_string(obj)));
                        --array_len;
                    }
                    syslog(LOG_DEBUG, "[FOTA]: Thingid list [%s]",
                            json_object_get_string(jthingid_list));
                    pthread_cond_signal(&upgrade_cond);
                    pthread_mutex_unlock(&upgrade_lock);
                }

                break;

            case CLIENT_DM:

                syslog(LOG_DEBUG, "[FOTA]: Received request from DM client");
                if (strcmp(command, "FOTA_RES") == 0) {

                    int upgrade_status = json_object_get_int(
                            json_object_object_get(jreceive, "status"));
                    memset(&status, 0, sizeof(status));             // For UPGRADED FCM
                    sprintf(status, "%d", upgrade_status);
                    jthingid = json_object_object_get(jreceive, "thing_id");

                    strcpy(thing_id, json_object_get_string(jthingid));

                    if (!upgrade_status) {
                        syslog(LOG_DEBUG,
                                "[FOTA]: END_DEVICE upgraded successfully, ThingID is [%s]",
                                thing_id);
                    } else {
                        syslog(LOG_DEBUG,
                                "[FOTA]: END_DEVICE Upgrade failed for Thing_id = [%s], error code %d",
                                thing_id, upgrade_status);
                    }
                    fcm_push(THING, "UPGRADED", jthingid, NULL,
                            json_object_get_string(json_object_object_get(jreceive, "version")),
                            status);
                    pthread_cond_signal(&receive_cond);
                }
                break;

            default:
                break;
        }
        json_object_put(jreceive);
    }
}

void thing_upgrade_thread(void) {
    int status, array_len;
    sqlite3 *db;
    sqlite3_stmt *sql_stmt;
    char stmt[256];
    char *thing_id;

    json_object *jarray;
    json_object *jsendto;
    json_object *jclient;
    json_object *jcommand;
    json_object *jfirmware_path;
    json_object *jfirmware_ver;
    json_object *jStatus;
    json_object *jthing_id;
    json_object *Thingid_obj;

    char file_path[256];
    char end_device_json[128];

    while (1) {
        pthread_mutex_lock(&upgrade_lock);
        syslog(LOG_DEBUG, "[FOTA]: Waiting for UPGRADE signal");
        pthread_cond_wait(&upgrade_cond, &upgrade_lock);
        syslog(LOG_DEBUG, "[FOTA]:Upgrade signal received ");

        if (jthingid_list == NULL) {
            pthread_mutex_unlock(&upgrade_lock);
            continue;
        }
        array_len = json_object_array_length(jthingid_list);
        syslog(LOG_DEBUG, "[FOTA]: Number of thingids to upgrade [%d]\n", array_len);

        while (array_len) {

            jarray = json_object_array_get_idx(jthingid_list, array_len - 1);
            thing_id = json_object_get_string(jarray);
            syslog(LOG_DEBUG, "[FOTA]:Thingid [%s] [ %s ] [i=%d] \n",
                    json_object_get_string(jthingid_list), thing_id, array_len - 1);

            /*
             * Fetching End_device_Id based on thingid from database
             */
            ///FIXME: look for other logic
            status = sqlite3_open(END_DEVICE_DATABASE, &db);
            if (status != SQLITE_OK) {
                syslog(LOG_ERR, "[FOTA]: Cannot open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                continue;               ///FIXME: does array_len has to be decremented?
            }
            sprintf(stmt,
                    "SELECT FIRMWARE_VERSION, HARDWARE_VERSION, MANUFACTURER_NAME, MODEL_ID FROM FIRMWARE where THING_ID=\"%s\"",
                    thing_id);              ///FIXME: use snprintf

            status = sqlite3_prepare_v2(db, stmt, -1, &sql_stmt, 0);    ///FIXME: error case
            if (status != SQLITE_OK) {
                syslog(LOG_ERR, "[FOTA]: Failed to prepare statement: %s", sqlite3_errmsg(db));
//                        return status;            ///FIXME: error case
                sqlite3_close(db);
                continue;
            }

            status = sqlite3_step(sql_stmt);          ///FIXME: error case
            if (status == SQLITE_ROW) {

                memset(Firmware_version, 0, sizeof(Firmware_version));
                memset(Hardware_version, 0, sizeof(Hardware_version));
                memset(Manufacturer_name, 0, sizeof(Manufacturer_name));
                memset(Model_id, 0, sizeof(Model_id));

                strncpy(Firmware_version, sqlite3_column_text(sql_stmt, 0),
                        strlen(sqlite3_column_text(sql_stmt, 0)));
                strncpy(Hardware_version, sqlite3_column_text(sql_stmt, 1),
                        strlen(sqlite3_column_text(sql_stmt, 1)));
                strncpy(Manufacturer_name, sqlite3_column_text(sql_stmt, 2),
                        strlen(sqlite3_column_text(sql_stmt, 2)));
                strncpy(Model_id, sqlite3_column_text(sql_stmt, 3),
                        strlen(sqlite3_column_text(sql_stmt, 3)));

                syslog(LOG_DEBUG, "[FOTA]:THING_ID=%s FW=%s HW=%s MN=%s MI=%s\n", thing_id,
                        Firmware_version, Hardware_version, Manufacturer_name, Model_id);
            } else {
                syslog(LOG_ERR, "[FOTA]: This THING_ID[ %s ] is not present in database file: %s",
                        thing_id, sqlite3_errmsg(db));
                sqlite3_finalize(sql_stmt);
                sqlite3_close(db);
                pthread_mutex_unlock(&upgrade_lock);
                continue;
            }

            sqlite3_finalize(sql_stmt);
            sqlite3_close(db);

            /*
             * End of Fetching from database
             */

            memset(end_device_json, 0, sizeof(end_device_json));
            sprintf(end_device_json, "/root/downloads/fw/enddevota/%s_%s_%s.json",
                    Manufacturer_name, Model_id, Hardware_version);
            syslog(LOG_DEBUG, "[FOTA]: path is %s \n", end_device_json);

            memset(Firmware_version, 0, sizeof(Firmware_version));
            status = extract_dnld_json(end_device_json, 0, "version", Firmware_version);
            if (status) {
                syslog(LOG_ERR, "[FOTA]: Could not fetch release version from Fota json file.");
//                return JSON_FILE_READ_ERR;                ///FIXME: error case handle
            }
            memset(file_path, 0, sizeof(file_path));
            sprintf(file_path, "/root/downloads/fw/enddevota/%s_%s_%s_%s.ota", Manufacturer_name,
                    Model_id, Hardware_version, Firmware_version);
            syslog(LOG_DEBUG, "[FOTA]: path is %s", file_path);

            jsendto = json_object_new_object();
            jclient = json_object_new_int(CLIENT_FOTA);
            jcommand = json_object_new_string("FOTA_REQ");
            jthing_id = json_object_new_string(thing_id);
            jfirmware_path = json_object_new_string(file_path);
            jfirmware_ver = json_object_new_string(Firmware_version);
            json_object_object_add(jsendto, "client", jclient);
            json_object_object_add(jsendto, "command", jcommand);
            json_object_object_add(jsendto, "path", jfirmware_path);
            json_object_object_add(jsendto, "thing_id", jthing_id);
            json_object_object_add(jsendto, "version", jfirmware_ver);
            syslog(LOG_DEBUG,
                    "[FOTA]: Sending the FOTA request to DM [ IP_ADDR= %s, PORT= %d ] for upgrading the end_device %s\n",
                    inet_ntoa(dm_cliaddr.sin_addr), ntohs(dm_cliaddr.sin_port),
                    json_object_get_string(jsendto));
            if (sendto(server_fd, json_object_to_json_string(jsendto),
                    strlen(json_object_to_json_string(jsendto)) + 1,
                    MSG_CONFIRM, (const struct sockaddr *) &dm_cliaddr, sizeof(dm_cliaddr)) < 0) {
                syslog(LOG_ERR, "[FOTA]: FOTA request to DM failed with ThingID[ %S ] \n",
                        thing_id);
                json_object_put(jsendto);
                pthread_mutex_unlock(&upgrade_lock);
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

void download_thread(void) {
    int i;
    int num_of_devices;
    int status;
    int signal;

    char dnld_json[128];
    char device_json[128];
    char end_device_json[128];
    char file[128];
    char json_file_path[128];

    json_object *jenddev_array;
    json_object *jdev_json;
    json_object *jobj;

    while (1) {
        syslog(LOG_DEBUG, "[FOTA]: Waiting for a SIGUSR1 signal\n");
        status = sigwait(&set, &signal);
        if (status) {
            syslog(LOG_ERR, "[FOTA]: sigwait failed \n");
            continue;
        }

        if (signal == SIGUSR1) {
            syslog(LOG_DEBUG, "[FOTA]: Signal received from cron job\n");
            jobj = json_object_from_file(SHIFU_VERSION_JSON_PATH);
            jenddev_array = json_object_object_get(jobj, "end_device");
            num_of_devices = json_object_array_length(jenddev_array);

            for (i = 0; i < num_of_devices; i++) {
                thingid_obj[i] = json_object_new_array();
                jdev_json = json_object_array_get_idx(jenddev_array, i);
                strcpy(device_json, json_object_get_string(jdev_json));
                sprintf(dnld_json, "/etc/ota/dnld_%s", json_object_get_string(jdev_json));
                syslog(LOG_DEBUG, "[FOTA]:Download initiated, %s %s", device_json, dnld_json);
                status = DownloadFirmware(device_json, dnld_json);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: %s download failed = %d", device_json, status);
                    continue;
                }
                status = get_thingid(device_json, i);
                if (status) {
                    syslog(LOG_ERR, "[FOTA]: get_thingid failed = %d", status);
                    continue;
                }
                syslog(LOG_DEBUG, "[FOTA]: thingid_obj[%d]=%s", i,
                        json_object_get_string(thingid_obj[i]));

                if (!fota_silent_flag) {
                    /*sending Notify FCM*/
                    memset(&file, 0, sizeof(file));
                    strncpy(file, device_json, strlen(device_json) - strlen(".json"));
                    memset(Firmware_version, 0, sizeof(Firmware_version));
                    sprintf(json_file_path, "/root/downloads/fw/enddevota/%s", device_json);
                    status = extract_dnld_json(json_file_path, 0, "version", Firmware_version);
                    if (status) {
                        syslog(LOG_ERR,
                                "[FOTA]: Could not fetch release version from Fota json file.");
                        continue;
                    }
                    status = fcm_push(THING, "NOTIFY", thingid_obj[i], file, Firmware_version,
                    NULL);
                    if (status) {
                        syslog(LOG_ERR, "[FOTA]: Notify FCM push failed : %d", status);
                        continue;
                    }

                } else {
                    pthread_mutex_lock(&download_lock);
                    jthingid_list = json_tokener_parse(json_object_get_string(thingid_obj[i]));
                    syslog(LOG_DEBUG, "[FOTA]: thread list [%s]",
                            json_object_get_string(jthingid_list));
                    pthread_cond_signal(&upgrade_cond);
                    syslog(LOG_DEBUG, "[FOTA]: Waiting on download condition variable");
                    pthread_cond_wait(&download_cond, &download_lock); ///FIXME: sigwait and pthread wait sync
                    syslog(LOG_DEBUG, "[FOTA]: Released upgrade condition variable");
                    pthread_mutex_unlock(&download_lock);
                }
            } //for loop end
        } else {
            syslog(LOG_DEBUG, "[FOTA]:Signal received is %d\n", signal);
            continue;
        }
    } // while loop end
}

/**
 * @brief Function to do FCM push to Tronx.
 *
 * @param path FCM push URL
 */
int fcm_push(bool type, char *verb, json_object *thing_id, char *ed_file, char *ed_version,
        char *ed_status) {
    // ThingIds to be handled
    char home_token[ENV_LEN] = { 0 };
    char device_id[ENV_LEN] = { 0 };

    char hometoken_hdtag[HEADER_FIELD_LEN] = "X-HomeToken: ";
    char deviceid_hdtag[HEADER_FIELD_LEN] = "X-DeviceId: ";
    char requestid_hdtag[HEADER_FIELD_LEN] = "X-RequestId: ";

    CURL *curl;
    CURLcode cerr = CURLE_OK;
    url_data_type chunk;
    struct curl_slist *headerlist = NULL;

    char changelog_string[80], changelog_string_full[CHANGELOG_STR_LEN] = { 0 };
    char base_url[URL_LEN] = { 0 };

    const char *resp_status;
    const char *resp_message;
    int status;
    char command[64];

    char downloaded_version[VERSION_LEN] = { 0 };
    char uploaded_at[CREATED_AT_LEN] = { 0 };
    FILE *fp;

    if (!strcmp(verb, "NOTIFY")) {

        /* Get contents from changelog.md in string format */
        if (!type)
            fp = popen("cat /root/downloads/fw/enddevota/*.md", "r");
        else {
            /*           Get contents from changelog.md in string format*/
            sprintf(command, "cat /root/downloads/fw/enddevota/%s*.md", ed_file);
            fp = popen(command, "r");
        }

        if (fp == NULL) {
            syslog(LOG_ERR, "[FOTA]: Failed to do open changelog.md file\n");
        }

        while (fgets(changelog_string, sizeof(changelog_string), fp) != NULL) {
            strncat(changelog_string_full, changelog_string, strlen(changelog_string) + 1);
        }
        status = pclose(fp);
        syslog(LOG_DEBUG, "[FOTA]: Changelog full contents in string: %s\n", changelog_string_full);
    }

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

    if (!type) {
        /* fetch release version from downloaded firmware json file */
        status = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "downloaded_version",
                downloaded_version, false);                                         // Modify for ED
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch release version from update json file.");
            return JSON_FILE_READ_ERR;
        }
        syslog(LOG_DEBUG, "[FOTA]: Downloaded version %s\n", downloaded_version);

        /* fetch uploaded_at from downloaded firmware json file */
        status = extract_current_json(DNLD_FIRMWARE_JSON_PATH, 0, "uploaded_at", uploaded_at,
        false);       // Modify for ED
        if (status) {
            syslog(LOG_ERR, "[FOTA]: Could not fetch uploaded_at from update json file.");
            return JSON_FILE_READ_ERR;
        }
        syslog(LOG_DEBUG, "[FOTA]: Firmware Uploaded at %s\n", uploaded_at);
    }

    json_object *jobj = json_object_new_object();
    json_object *jbody = json_object_new_object();
    json_object *jstring1, *jstring2, *jstring3, *jstring4, *jstring5, *jstring6;
    /* Creating a json string */
    if (!strcmp(verb, "NOTIFY"))
        jstring1 = json_object_new_string(changelog_string_full);

    if (!type) {
        jstring2 = json_object_new_string("SHIFU");
        jstring5 = json_object_new_string(downloaded_version);
        jstring6 = json_object_new_string(uploaded_at);
    } else {
        jstring2 = json_object_new_string("END_DEVICE");
        jstring5 = json_object_new_string(ed_version);
    }

    jstring3 = json_object_new_string("FOTA");
    jstring4 = json_object_new_string(verb);

    if (strcmp(verb, "NOTIFY") == 0) {
        json_object_object_add(jbody, "change_log", jstring1);
    } else
        json_object_object_add(jbody, "status", ed_status);

    json_object_object_add(jbody, "release_version", jstring5);

    if (!type)
        json_object_object_add(jbody, "release_date", jstring6);
    else
        json_object_object_add(jbody, "thing_id", thing_id);

    /* Form the json object */
    json_object_object_add(jobj, "body", jbody);
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

int main(int argc, char const * argv[]) {
    pthread_t client_receive_tid;
    pthread_t enddev_upgrade_request_tid;
    pthread_t enddev_download_request_tid;

    create_socket_server();
    client_info();
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    DOWNLOAD_REQ: if (pthread_create(&enddev_download_request_tid, NULL, download_thread, NULL)) {
        syslog(LOG_ERR, "[FOTA]: waiting_for_client_req thread failed \n");
        goto DOWNLOAD_REQ;
    }
    UPGRADE_REQ: if (pthread_create(&enddev_upgrade_request_tid, NULL, thing_upgrade_thread,
    NULL)) {
        syslog(LOG_ERR, "[FOTA]: end_device_fw_upgrade thread failed \n");
        goto UPGRADE_REQ;
    }
    CLIENT_RECV: if (pthread_create(&client_receive_tid, NULL, receive_from_thread, NULL)) {
        syslog(LOG_ERR, "[FOTA]: waiting_for_client_req thread failed \n");
        goto CLIENT_RECV;
    }

    pthread_join(enddev_download_request_tid, NULL);
    pthread_join(enddev_upgrade_request_tid, NULL);
    pthread_join(client_receive_tid, NULL);

}

