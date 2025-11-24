/*
 * fota.c
 *
 *  Created on: 21 Jan 2019
 *      Author: Priyanka Polumatla
 */

#include "fota.h"

void generate_request_id(char *request_id, char *device_id) {
    time_t seconds;

    time(&seconds);
    snprintf(request_id, strlen(device_id) + 14 + 1, "%sFOTA%ld", device_id, seconds);
}

