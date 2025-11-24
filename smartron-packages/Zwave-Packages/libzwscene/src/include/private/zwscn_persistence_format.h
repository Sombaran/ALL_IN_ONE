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
@file   zwscn_persistence_format.h - Tag Length Value (TLV) format utility functions header file.
  
  
@author Arif Mohammad
  
@version    1.0 2014-01-23  Initial release
*/ 

#ifndef _ZWSCN_PERSISTENCE_FORMAT_H_
#define	_ZWSCN_PERSISTENCE_FORMAT_H_

/** @defgroup GroupPrivatePersistenceFormat Tag Length Value (TLV) data format for storing persistent data
 *  Functions for storing persistent data in Tag Length Value (TLV) data format
 *  @ingroup GroupPrivateGeneral
 */
///@{

#define ZWSCN_FILE_MAGIC_NUM	0x7EC3	/**< First two bytes magic identifier of the node information file*/
#define ZWSCN_FILEMODE_READ		1		/**< FILE Read mode */
#define ZWSCN_FILEMODE_WRITE	2		/**< FILE Write mode */

/** Tag size definition */
#define ZWSCN_TLV_TAG_ID_SIZE      2			/**< Tag identifier field size in bytes*/
#define ZWSCN_TLV_TAG_LEN_SIZE     2			/**< Tag length field size in bytes*/
#define ZWSCN_TLV_SUBTAG_ID_SIZE   1			/**< Sub-tag identifier field size in bytes*/
#define ZWSCN_TLV_SUBTAG_LEN_SIZE  1			/**< Sub-tag length field size in bytes*/
#define ZWSCN_TLV_SUBTAG_WIDE_LEN_SIZE  2		/**< Wide Sub-tag length field size in bytes*/
#define ZWSCN_TLV_SUBTAGS_SIZE_MAX (5*1024)     /**< Maximum subtags size, used for pre-allocate memory */

#define ZWSCN_TLV_LEN_64	8	/**< Length of 64bit tag */
#define ZWSCN_TLV_LEN_32	4	/**< Length of 32bit tag */
#define ZWSCN_TLV_LEN_16	2	/**< Length of 16bit tag */
#define ZWSCN_TLV_LEN_8		1	/**< Length of 8bit tag */
#define ZWSCN_TLV_SUBTAG_HEADER_LEN			(ZWSCN_TLV_SUBTAG_ID_SIZE + ZWSCN_TLV_SUBTAG_LEN_SIZE)				/**< Length of subtag header */
#define ZWSCN_TLV_SUBTAG_WIDE_HEADER_LEN	(ZWSCN_TLV_SUBTAG_ID_SIZE + ZWSCN_TLV_SUBTAG_WIDE_LEN_SIZE)	/**< Length of subtag wide header */
#define ZWSCN_TLV_SUBTAG64_LEN_MIN			(ZWSCN_TLV_SUBTAG_HEADER_LEN + ZWSCN_TLV_LEN_64)	/**< Minimum length of 64bit subtag */
#define ZWSCN_TLV_SUBTAG32_LEN_MIN			(ZWSCN_TLV_SUBTAG_HEADER_LEN + ZWSCN_TLV_LEN_32)	/**< Minimum length of 32bit subtag */
#define ZWSCN_TLV_SUBTAG16_LEN_MIN			(ZWSCN_TLV_SUBTAG_HEADER_LEN + ZWSCN_TLV_LEN_16)	/**< Minimum length of 16bit subtag */
#define ZWSCN_TLV_SUBTAG8_LEN_MIN			(ZWSCN_TLV_SUBTAG_HEADER_LEN + ZWSCN_TLV_LEN_8)		/**< Minimum length of 8bit subtag */

#define ZWSCN_TLV_ERR_NONE				0		/**< Error type None */
#define ZWSCN_TLV_ERR_MEMORY			-1		/**< Memory Error */
#define ZWSCN_TLV_ERR_SUBTAG_RD			-2		/**< Subtag read Error */
#define ZWSCN_TLV_ERR_SUBTAG_MISMATCH	-3		/**< Subtag mismatch Error */
#define ZWSCN_TLV_ERR_FILE_EOF			-4		/**< EoF file Error */
#define ZWSCN_TLV_ERR_FILE_WRITE		-5		/**< File write Error */
#define ZWSCN_TLV_ERR_PARAM				-6		/**< Parameter read Error */
#define ZWSCN_TLV_ERR_NOT_FOUND			-7		/**< Item not found Error */

#define ZWSCN_TLV_FALSE	0	/**< FALSE */
#define ZWSCN_TLV_TRUE	1	/**< TRUE */



/** Tag container */
typedef struct
{
    uint16_t    id;		    /**< Tag id */
    uint16_t    len;	    /**< The length of the data[] */
    uint8_t     data[1];    /**< Data storage place holder */
} zwscn_tlv_tag_t, *zwscn_tlv_tag_p; /**< Tag container */
/**
 * @brief	TLV tag writer function
 * @param[in] file	output file
 * @param[in] tag	tag to be written to the file
 * @return	  0 if success; else negative value on error
 */
int zwscn_tlv_tag_wr(FILE *file, zwscn_tlv_tag_p tag);
/**
 * @brief	TLV tag reader function
 * @param[in] file	input file
 * @param[out] tag	tag to read from the file
 * @return	  0 if success; else negative value on error
 */
int zwscn_tlv_tag_rd(FILE *file, zwscn_tlv_tag_p tag);
/**
 * @brief	Write 64bit subtag
 * @param[in] id			Subtage id
 * @param[in] data			Subt ag data
 * @param[out] buf			Tag buffer
 * @param[out] total_len 	Tag len
 */
void zwscn_tlv_subtag_wr64(uint8_t id, uint64_t data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Write 32bit subtag
 * @param[in] id			Subtage id
 * @param[in] data			Subtag data
 * @param[out] buf			Tag buffer
 * @param[out] total_len 	Tag len
 */
void zwscn_tlv_subtag_wr32(uint8_t id, uint32_t data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Write 16bit subtag
 * @param[in] id			Subtage id
 * @param[in] data			Subtag data
 * @param[out] buf			Tag buffer
 * @param[out] total_len 	Tag len
 */
void zwscn_tlv_subtag_wr16(uint8_t id, uint16_t data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Write 8bit subtag
 * @param[in] id			Subtage id
 * @param[in] data			Subtag data
 * @param[out] buf			Tag buffer
 * @param[out] total_len 	Tag len
 */
void zwscn_tlv_subtag_wr8(uint8_t id, uint8_t data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Write variable length subtag
 * @param[in] id			Subtage id
 * @param[in] data			Subtag data
 * @param[in] data_len		Subtag data length
 * @param[out] buf			Tag buffer
 * @param[out] total_len 	Tag len
 */
void zwscn_tlv_subtag_wr(uint8_t id, uint8_t *data, uint16_t data_len, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Read 64bit subtag
 * @param[in] id				Subtage id
 * @param[out] data				Subtag data
 * @param[in, out] buf			Tag buffer
 * @param[in, out] total_len	Tag len
 */
int zwscn_tlv_subtag_rd64(uint8_t id, uint64_t *data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Read 32bit subtag
 * @param[in] id				Subtage id
 * @param[out] data				Subtag data
 * @param[in, out] buf			Tag buffer
 * @param[in, out] total_len	Tag len
 */
int zwscn_tlv_subtag_rd32(uint8_t id, uint32_t *data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Read 16bit subtag
 * @param[in] id				Subtage id
 * @param[out] data				Subtag data
 * @param[in, out] buf			Tag buffer
 * @param[in, out] total_len	Tag len
 */
int zwscn_tlv_subtag_rd16(uint8_t id, uint16_t *data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Read 8bit subtag
 * @param[in] id				Subtage id
 * @param[out] data				Subtag data
 * @param[in, out] buf			Tag buffer
 * @param[in, out] total_len	Tag len
 */
int zwscn_tlv_subtag_rd8(uint8_t id, uint8_t *data, uint8_t **buf, uint16_t *total_len);
/**
 * @brief	Read variable length subtag
 * @param[in] id				Subtage id
 * @param[out] data				Subtag data
 * @param[in] data_len			Subtag data length
 * @param[in, out] buf			Tag buffer
 * @param[in, out] total_len	Tag len
 */
int zwscn_tlv_subtag_rd(uint8_t id, uint8_t *data, uint16_t data_len, uint8_t **buf, uint16_t *total_len);
///@}

#endif	/* _ZWSCN_PERSISTENCE_FORMAT_H_ */

