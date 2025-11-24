/** @copyright
© 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA 
DESIGNS, INC. AS A TRADE SECRET, AND IS NOT TO BE USED OR DISCLOSED EXCEPT AS 
PROVIDED Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. ALL 
RIGHTS RESERVED.
@par
NOTICE: ALL INFORMATION CONTAINED HEREIN IS CONFIDENTIAL AND/OR PROPRIETARY TO 
SIGMA DESIGNS AND MAY BE COVERED BY U.S. AND FOREIGN PATENTS, PATENTS IN 
PROCESS, AND ARE PROTECTED BY TRADE SECRET OR COPYRIGHT LAW. DISSEMINATION OR 
REPRODUCTION OF THE SOURCE CODE CONTAINED HEREIN IS EXPRESSLY FORBIDDEN TO 
ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS’ 
Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. THE COPYRIGHT 
NOTICE ABOVE IS NOT EVIDENCE OF ANY ACTUAL OR INTENDED PUBLICATION OF THE 
SOURCE CODE. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED 
INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR 
DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY 
DESCRIBE.
@par
THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY 
"AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY 
ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR 
COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S 
OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA 
PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL 
BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN 
COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE 
SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR 
DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL 
THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET, 
AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS 
INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE 
FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES. 
SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION 
OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES 
ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD 
PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE 
RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY 
CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING 
FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE 
EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO 
WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS 
SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR 
CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF 
PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF 
CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE), 
STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS 
REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE 
POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES 
OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR 
ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND 
FITNESS FOR A PARTICULAR PURPOSE.
@par
The Sigma Program  is not fault-tolerant and is not designed, manufactured or 
intended for use or resale as on-line control equipment in hazardous 
environments requiring fail-safe performance, such as in the operation of 
nuclear facilities, aircraft navigation or communication systems, air traffic 
control, direct life support machines, or weapons systems, in which the failure 
of the Sigma Program, or Company Applications created using the Sigma Program, 
could lead directly to death, personal injury, or severe physical or 
environmental damage ("High Risk Activities").  Sigma and its suppliers 
specifically disclaim any express or implied warranty of fitness for High Risk 
Activities.Without limiting Sigma’s obligation of confidentiality as further 
described in the Z-Wave Controller Development Kit Limited License Agreement, 
Sigma has no obligation to establish and maintain a data privacy and 
information security program with regard to Company’s use of any Third Party 
Service Environment or Cloud Service Environment. For the avoidance of doubt, 
Sigma shall not be responsible for physical, technical, security, 
administrative, and/or organizational safeguards that are designed to ensure 
the security and confidentiality of the Company Content or Company Application 
in any Third Party Service Environment or Cloud Service Environment that 
Company chooses to utilize.
*/
/** 
@file   zwscn_persistence_format.c - Tag Length Value (TLV) format utility functions implementation.
  
  
@author Arif Mohammad
  
@version    1.0 2014-01-23  Initial release
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "zwscn_persistence_format.h"
#include "zwscn_util_message_log.h"

/**
zwscn_tlv_tag_rd - Read a tag and its content
@param[in]      file     node info file handler 
@param[out]     tag      tag container on success 
@return		Zero on success; otherwise negative error number.
@post       Caller must free the memory allocated to tag on success
*/
int zwscn_tlv_tag_rd(FILE *file, zwscn_tlv_tag_p tag_cont)
{
    uint16_t    tag_id;
    uint16_t    len;
    uint16_t    padding;
/*
    zwscn_tlv_tag_p     tag_cont;
*/

	if(!tag_cont){
		return ZWSCN_TLV_ERR_PARAM;
	}
	
    if (fread(&tag_id, ZWSCN_TLV_TAG_ID_SIZE, 1, file) == 0)
    {
        return ZWSCN_TLV_ERR_FILE_EOF;
    }

    if (fread(&len, ZWSCN_TLV_TAG_LEN_SIZE, 1, file) == 0)
    {
        return ZWSCN_TLV_ERR_FILE_EOF;
    }

    padding = len & 0x01;   //padding to even number of bytes

/*
    //Allocate memory for the data
    tag_cont = (zwscn_tlv_tag_p) calloc(1, sizeof(zwscn_tlv_tag_t) + len + padding);

    if (!tag_cont)
    {
        return ZWSCN_TLV_ERR_MEMORY;
    }
*/

    if (fread(tag_cont->data, len + padding, 1, file) == 0)
    {
/*
        free(tag_cont);
*/
        return ZWSCN_TLV_ERR_FILE_EOF;
    }

    tag_cont->id = tag_id;
    tag_cont->len = len;

/*
    *tag = tag_cont;
*/

    return ZWSCN_TLV_ERR_NONE;
}


/**
zwscn_tlv_tag_wr - Write a tag and its content into a file
@param[in]      file     node info file handler 
@param[in]      tag      tag container 
@return		Zero on success; otherwise negative error number.
*/
int zwscn_tlv_tag_wr(FILE *file, zwscn_tlv_tag_p tag)
{

    if (fwrite(&tag->id, ZWSCN_TLV_TAG_ID_SIZE, 1, file) == 0)
    {
        return ZWSCN_TLV_ERR_FILE_WRITE;
    }

    if (fwrite(&tag->len, ZWSCN_TLV_TAG_LEN_SIZE, 1, file) == 0)
    {
        return ZWSCN_TLV_ERR_FILE_WRITE;
    }

    if (fwrite(tag->data, tag->len, 1, file) == 0)
    {
        return ZWSCN_TLV_ERR_FILE_WRITE;
    }

    //padding to even number of bytes
    if (tag->len & 0x01)
    {
        if (fputc(0, file) == EOF)
        {
            return ZWSCN_TLV_ERR_FILE_WRITE;
        }
    }

    return ZWSCN_TLV_ERR_NONE;
}

/**
zwscn_tlv_subtag_wr64 - Write a 64-bit sub-tag data into buffer
@param[in]      id          id of the sub-tag 
@param[in]      data        data to be written 
@param[in, out] buf         buffer to be written, the buf will be adjusted after the write
                            to point to the next empty location for subsequent write
@param[in, out] total_len   the number of bytes used for the sub-tag will be added to this total_len
@return
*/
void zwscn_tlv_subtag_wr64(uint8_t id, uint64_t data, uint8_t **buf, uint16_t *total_len)
{
    uint8_t     *subtag;
    uint8_t     subtag_len;

	subtag_len = (ZWSCN_TLV_SUBTAG_ID_SIZE + ZWSCN_TLV_SUBTAG_LEN_SIZE + ZWSCN_TLV_LEN_64);
	if(((*total_len) + subtag_len) >= ZWSCN_TLV_SUBTAGS_SIZE_MAX) {
		zwscn_message_log_critical("ZWTLV: [ERROR] Subtag len is more than available memory in Tag buffer. Returning without writing...\n");
		return;
	}

    subtag = *buf;
    subtag[0] = id;
    subtag[1] = ZWSCN_TLV_LEN_64;
	subtag[2] = (data >> 56) & 0xFF;
    subtag[3] = (data >> 48) & 0xFF;
    subtag[4] = (data >> 40) & 0xFF;
	subtag[5] = (data >> 32) & 0xFF;
    subtag[6] = (data >> 24) & 0xFF;
    subtag[7] = (data >> 16) & 0xFF;
    subtag[8] = (data >> 8) & 0xFF;
    subtag[9] = data & 0xFF;

    *buf = subtag + subtag_len;

    (*total_len) += subtag_len;

}

/**
zwscn_tlv_subtag_wr32 - Write a 32-bit sub-tag data into buffer
@param[in]      id          id of the sub-tag 
@param[in]      data        data to be written 
@param[in, out] buf         buffer to be written, the buf will be adjusted after the write
                            to point to the next empty location for subsequent write
@param[in, out] total_len   the number of bytes used for the sub-tag will be added to this total_len
@return
*/
void zwscn_tlv_subtag_wr32(uint8_t id, uint32_t data, uint8_t **buf, uint16_t *total_len)
{
    uint8_t     *subtag;
    uint8_t     subtag_len;

	subtag_len = (ZWSCN_TLV_SUBTAG_ID_SIZE + ZWSCN_TLV_SUBTAG_LEN_SIZE + ZWSCN_TLV_LEN_32);
	if(((*total_len) + subtag_len) >= ZWSCN_TLV_SUBTAGS_SIZE_MAX) {
		zwscn_message_log_critical("ZWTLV: [ERROR] Subtag len is more than available memory in Tag buffer. Returning without writing...\n");
		return;
	}

    subtag = *buf;
    subtag[0] = id;
    subtag[1] = 4;
    subtag[2] = data >> 24;
    subtag[3] = (data >> 16) & 0xFF;
    subtag[4] = (data >> 8) & 0xFF;
    subtag[5] = data & 0xFF;

    *buf = subtag + subtag_len;

    (*total_len) += subtag_len;

}


/**
zwscn_tlv_subtag_wr16 - Write a 16-bit sub-tag data into buffer
@param[in]      id          id of the sub-tag 
@param[in]      data        data to be written 
@param[in, out] buf         buffer to be written, the buf will be adjusted after the write
                            to point to the next empty location for subsequent write
@param[in, out] total_len   the number of bytes used for the sub-tag will be added to this total_len
@return
*/
void zwscn_tlv_subtag_wr16(uint8_t id, uint16_t data, uint8_t **buf, uint16_t *total_len)
{
    uint8_t     *subtag;
    uint8_t     subtag_len;

	subtag_len = (ZWSCN_TLV_SUBTAG_ID_SIZE + ZWSCN_TLV_SUBTAG_LEN_SIZE + ZWSCN_TLV_LEN_16);
	if(((*total_len) + subtag_len) >= ZWSCN_TLV_SUBTAGS_SIZE_MAX) {
		zwscn_message_log_critical("ZWTLV: [ERROR] Subtag len is more than available memory in Tag buffer. Returning without writing...\n");
		return;
	}

    subtag = *buf;
    subtag[0] = id;
    subtag[1] = 2;
    subtag[2] = data >> 8;
    subtag[3] = data & 0xFF;

    *buf = subtag + subtag_len;

    (*total_len) += subtag_len;

}


/**
zwscn_tlv_subtag_wr8 - Write a 8-bit sub-tag data into buffer
@param[in]      id          id of the sub-tag 
@param[in]      data        data to be written 
@param[in, out] buf         buffer to be written, the buf will be adjusted after the write
                            to point to the next empty location for subsequent write
@param[in, out] total_len   the number of bytes used for the sub-tag will be added to this total_len
@return
*/
void zwscn_tlv_subtag_wr8(uint8_t id, uint8_t data, uint8_t **buf, uint16_t *total_len)
{
    uint8_t     *subtag;
    uint8_t     subtag_len;

	subtag_len = (ZWSCN_TLV_SUBTAG_ID_SIZE + ZWSCN_TLV_SUBTAG_LEN_SIZE + ZWSCN_TLV_LEN_8);
	if(((*total_len) + subtag_len) >= ZWSCN_TLV_SUBTAGS_SIZE_MAX) {
		zwscn_message_log_critical("ZWTLV: [ERROR] Subtag len is more than available memory in Tag buffer. Returning without writing...\n");
		return;
	}

    subtag = *buf;
    subtag[0] = id;
    subtag[1] = 1;
    subtag[2] = data;

    *buf = subtag + subtag_len;

    (*total_len) += subtag_len;

}


/**
zwscn_tlv_subtag_wr - Write sub-tag data into buffer
@param[in]      id          id of the sub-tag 
@param[in]      data        data to be written 
@param[in]      data_len    size of data 
@param[in, out] buf         buffer to be written, the buf will be adjusted after the write
                            to point to the next empty location for subsequent write
@param[in, out] total_len   the number of bytes used for the sub-tag will be added to this total_len
@return
*/
void zwscn_tlv_subtag_wr(uint8_t id, uint8_t *data, uint16_t data_len, uint8_t **buf, uint16_t *total_len)
{
    uint8_t     *subtag;
    uint16_t     subtag_len;

    //Only write sub-tag if there is data
    if (data_len == 0)
    {
        return;
    }
	subtag_len = (ZWSCN_TLV_SUBTAG_WIDE_HEADER_LEN + data_len);
	if(((*total_len) + subtag_len) >= ZWSCN_TLV_SUBTAGS_SIZE_MAX) {
		zwscn_message_log_critical("ZWTLV: [ERROR] Subtag len is more than available memory in Tag buffer. Returning without writing...\n");
		return;
	}

    subtag = *buf;
    subtag[0] = id;
    subtag[1] = (data_len & 0xFF);
	subtag[2] = ((data_len >> 8) & 0xFF);
//	zwscn_message_log_debug("FILEWRITE subtag[1]=%u\n", subtag[1]);
//	zwscn_message_log_debug("FILEWRITE subtag[2]=%u\n", subtag[2]);
//	zwscn_message_log_debug("FILEWRITE data_len=%u\n", data_len);
    if (data_len == 1)
    {
        subtag[3] = *data;
    }
    else
    {
        memcpy(subtag + 3, data, data_len);
    }

    *buf = subtag + subtag_len;

    (*total_len) += subtag_len;

}

/**
zwscn_tlv_subtag_rd64 - Read a 64-bit sub-tag data from buffer
@param[in]      id          id of the sub-tag 
@param[out]     data        output data 
@param[in, out] buf         buffer to be read, the buf will be adjusted after the read
                            to point to the next location for subsequent read
@param[in, out] total_len   the number of bytes read for the sub-tag will be subtracted to this total_len
@return
*/
int zwscn_tlv_subtag_rd64(uint8_t id, uint64_t *data, uint8_t **buf, uint16_t *total_len) {
	uint8_t *subtag;

	subtag = *buf;
	if((*total_len) < ZWSCN_TLV_SUBTAG64_LEN_MIN) {
		return ZWSCN_TLV_ERR_SUBTAG_RD;
	}
	
	if ((subtag[0] != id) || (subtag[1] != ZWSCN_TLV_LEN_64)) {
		return ZWSCN_TLV_ERR_SUBTAG_MISMATCH;
	}

	(*data) = (((subtag[2] << 24) & 0xFF000000) | ((subtag[3] << 16) & 0x00FF0000) | ((subtag[4] << 8) & 0x0000FF00) | (subtag[5] & 0x000000FF));
	(*data) = ((*data) << 32);
	(*data) |= (((subtag[6] << 24) & 0xFF000000) | ((subtag[7] << 16) & 0x00FF0000) | ((subtag[8] << 8) & 0x0000FF00) | (subtag[9] & 0x000000FF));

	(*buf) += ZWSCN_TLV_SUBTAG64_LEN_MIN;
	(*total_len) -= ZWSCN_TLV_SUBTAG64_LEN_MIN;
	
	return ZWSCN_TLV_ERR_NONE;
}

/**
zwscn_tlv_subtag_rd32 - Read a 32-bit sub-tag data from buffer
@param[in]      id          id of the sub-tag 
@param[out]     data        output data 
@param[in, out] buf         buffer to be read, the buf will be adjusted after the read
                            to point to the next location for subsequent read
@param[in, out] total_len   the number of bytes read for the sub-tag will be subtracted to this total_len
@return
*/
int zwscn_tlv_subtag_rd32(uint8_t id, uint32_t *data, uint8_t **buf, uint16_t *total_len) {
	uint8_t *subtag;

	subtag = *buf;
	if((*total_len) < ZWSCN_TLV_SUBTAG32_LEN_MIN) {
		return ZWSCN_TLV_ERR_SUBTAG_RD;
	}
	
	if ((subtag[0] != id) || (subtag[1] != ZWSCN_TLV_LEN_32)) {
		return ZWSCN_TLV_ERR_SUBTAG_MISMATCH;
	}

	(*data) = ((subtag[2] << 24) & 0xFF000000) | ((subtag[3] << 16) & 0x00FF0000) | ((subtag[4] << 8) & 0x0000FF00) | (subtag[5] & 0x000000FF);

	(*buf) += ZWSCN_TLV_SUBTAG32_LEN_MIN;
	(*total_len) -= ZWSCN_TLV_SUBTAG32_LEN_MIN;
	
	return ZWSCN_TLV_ERR_NONE;
}

/**
zwscn_tlv_subtag_rd16 - Read a 16-bit sub-tag data from buffer
@param[in]      id          id of the sub-tag 
@param[out]     data        output data 
@param[in, out] buf         buffer to be read, the buf will be adjusted after the read
                            to point to the next location for subsequent read
@param[in, out] total_len   the number of bytes read for the sub-tag will be subtracted to this total_len
@return
*/
int zwscn_tlv_subtag_rd16(uint8_t id, uint16_t *data, uint8_t **buf, uint16_t *total_len) {
	uint8_t *subtag;

	subtag = *buf;
	if((*total_len) < ZWSCN_TLV_SUBTAG16_LEN_MIN) {
		return ZWSCN_TLV_ERR_SUBTAG_RD;
	}
	
	if((subtag[0] != id) ||  (subtag[1] != ZWSCN_TLV_LEN_16)) {
		return ZWSCN_TLV_ERR_SUBTAG_MISMATCH;
	}
	
	(*data) = ((subtag[2] << 8) & 0xFF00) | (subtag[3] & 0x00FF);

	(*buf) += ZWSCN_TLV_SUBTAG16_LEN_MIN;
	(*total_len) -= ZWSCN_TLV_SUBTAG16_LEN_MIN;
	
	return ZWSCN_TLV_ERR_NONE;
}

/**
zwscn_tlv_subtag_rd8 - Read a 8-bit sub-tag data from buffer
@param[in]      id          id of the sub-tag 
@param[out]     data        output data 
@param[in, out] buf         buffer to be read, the buf will be adjusted after the read
                            to point to the next location for subsequent read
@param[in, out] total_len   the number of bytes read for the sub-tag will be subtracted to this total_len
@return
*/
int zwscn_tlv_subtag_rd8(uint8_t id, uint8_t *data, uint8_t **buf, uint16_t *total_len) {
	uint8_t *subtag;

	subtag = *buf;
	if((*total_len) < ZWSCN_TLV_SUBTAG8_LEN_MIN) {
		return ZWSCN_TLV_ERR_SUBTAG_RD;
	}
	
	if((subtag[0] != id) ||  (subtag[1] != ZWSCN_TLV_LEN_8)) {
		return ZWSCN_TLV_ERR_SUBTAG_MISMATCH;
	}
	
	(*data) = subtag[2];

	(*buf) += ZWSCN_TLV_SUBTAG8_LEN_MIN;
	(*total_len) -= ZWSCN_TLV_SUBTAG8_LEN_MIN;
	
	return ZWSCN_TLV_ERR_NONE;
}

/**
zwscn_tlv_subtag_rd - Read a sub-tag data from buffer
@param[in]      id          id of the sub-tag 
@param[out]     data        output data 
@param[in]      data_len    maximum size of data to be read
@param[in, out] buf         buffer to be read, the buf will be adjusted after the read
                            to point to the next location for subsequent read
@param[in, out] total_len   the number of bytes read for the sub-tag will be subtracted to this total_len
@return
*/
int zwscn_tlv_subtag_rd(uint8_t id, uint8_t *data, uint16_t data_len, uint8_t **buf, uint16_t *total_len)
{
	uint8_t *subtag;
	uint16_t subtag_len;

	subtag = *buf;
	if((*total_len) < (ZWSCN_TLV_SUBTAG_WIDE_HEADER_LEN)) {
		return ZWSCN_TLV_ERR_SUBTAG_RD;
	}
	
	//subtag_len = subtag[1];
	subtag_len = ((subtag[1]) & 0xFF) | ((subtag[2] << 8) & 0xFF00);
//	zwscn_message_log_debug("FILEREAD data=%p\n", data);
//	zwscn_message_log_debug("FILEREAD subtag[1]=%u\n", subtag[1]);
//	zwscn_message_log_debug("FILEREAD subtag[2]=%u\n", subtag[2]);
//	zwscn_message_log_debug("FILEREAD subtag_len=%u\n", subtag_len);
//	zwscn_message_log_debug("FILEREAD data_len=%u\n", data_len);
	if((subtag[0] != id) ||  (subtag_len > data_len)) {
		zwscn_message_log_critical("Subtag: Error in reading subtag (id=%u len=%u data_len=%u)\n", subtag[0], subtag_len, data_len);
		return ZWSCN_TLV_ERR_SUBTAG_MISMATCH;
	}
	if((*total_len) < (ZWSCN_TLV_SUBTAG_WIDE_HEADER_LEN + subtag_len)) {
		zwscn_message_log_critical("ZWTLV: [ERROR] Subtag len is more than available memory in Tag buffer. Returning without reading...\n");
		return ZWSCN_TLV_ERR_SUBTAG_RD;
	}
	
	memset(data, 0, data_len); //zero out the buffer before copying
	memcpy(data, (subtag + ZWSCN_TLV_SUBTAG_WIDE_HEADER_LEN), subtag_len);
	
	(*buf) += ZWSCN_TLV_SUBTAG_WIDE_HEADER_LEN + subtag_len;
	(*total_len) -= ZWSCN_TLV_SUBTAG_WIDE_HEADER_LEN + subtag_len;
	
	return ZWSCN_TLV_ERR_NONE;
}
