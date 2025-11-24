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
@file       zwscn_xml.c - xml reader writer API

                To provide xml reading and writing services

@author     Mafahir Fairoze

@version    1.0 2016-01-18  Initial release
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <glib.h>
#include <stdarg.h>
#include <inttypes.h>

#include "zwscn_scene_api.h"
#include "zip_api.h"
#include "zwscn_device_args.h"
#include "ZW_classcmd.h"
#include "zwscn_defines.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_time.h"
#include "zwscn_util_timer.h"
#include "zwscn_util_filesys.h"
#include "zwscn_util_config_file.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_util.h"
#include "zwscn_util_mem.h"
#include "zwscn_util_message_log.h"
#include "zwscn_scene.h"
#include "zwscn_setup_op.h"
#include "zwscn_xml.h"
#include "zwscn_xml.h"
#include <glib.h>
#include <string.h>

int asprintf(char **str, const char *fmt, ...) {
    int size = 0;
    va_list args, tmpa;
    va_start(args, fmt);
    va_copy(tmpa, args);
    size = vsnprintf(NULL, size, fmt, tmpa);
    va_end(tmpa);
    if (size < 0) {
        return -1;
    }
    *str = (char *) malloc(size + 1);
    if (NULL == *str) {
        return -1;
    }
    size = vsprintf(*str, fmt, args);
    va_end(args);
    return size;
}

typedef GQueue xml_stack;

static char strbuf[sizeof(uint32_t) * 10 + 1];

typedef struct xml_writer {
    FILE* handle;
    xml_writer_settings_t settings;
    xml_node_type_t node_type;
    xml_stack* stack;
} xml_writer_t;

typedef struct xml_writer_element_info {
    GString* name;
    uint8_t has_content;
} xml_writer_element_info_t;

xml_writer_element_info_t* xml_create_writer_element_info(char* name, uint8_t has_content) {
    xml_writer_element_info_t* info = calloc(1, sizeof(xml_writer_element_info_t));
    info->name = g_string_new(name);
    info->has_content = has_content;
    return info;
}

void xml_writer_element_info_free(xml_writer_element_info_t* info) {
    g_string_free(info->name, 1);
    info->name = NULL;
    free(info);
}

xml_writer_t* xml_create_writer(const char* filename, xml_writer_settings_t settings) {
    FILE* file = fopen(filename, "w");
    if (file) {
        xml_writer_t* writer = calloc(1, sizeof(xml_writer_t));
        writer->handle = file;
        writer->settings = settings;
        writer->stack = g_queue_new();
        if (writer->settings.encoding == XML_ENCODING_UTF_8) {
            fprintf(writer->handle, "\xEF\xBB\xBF"); //Byte Order Mark
        }
        if (!writer->settings.omit_xml_decleration) {
            fprintf(writer->handle, "<?xml version=\"1.0\"?>");
        }
        return writer;
    }
    return 0;
}

void xml_writer_close(xml_writer_t* writer) {
    xml_writer_write_end_all_element(writer);
    fputc('\n', writer->handle);
    fflush(writer->handle);
    fclose(writer->handle);
    writer->handle = NULL;
    while (!g_queue_is_empty(writer->stack)) {
        xml_writer_element_info_free(g_queue_pop_head(writer->stack));
    }
    g_queue_free(writer->stack);
    writer->stack = NULL;
    free(writer);
}

void xml_writer_complete(xml_writer_t* writer) {
    switch (writer->node_type) {
    case XML_NODE_TYPE_START_ELEMENT:
    case XML_NODE_TYPE_ATTRIBUTE:
        fprintf(writer->handle, ">");
        break;
    default:
        break;
    }
}

void xml_writer_indent(xml_writer_t* writer) {
    if (writer->settings.indent && writer->node_type != XML_NODE_TYPE_TEXT) {
        int depth = g_queue_get_length(writer->stack);
        fputc('\n', writer->handle);
        if (depth > 0) {
            fprintf(writer->handle, "%*s", depth, " ");
        }
    }
}

void xml_writer_write_start_element(xml_writer_t* writer, char* name) {
    xml_writer_element_info_t* info = g_queue_peek_head(writer->stack);
    xml_writer_complete(writer);
    if (!info || (info && !info->has_content)) {
        xml_writer_indent(writer);
    }
    g_queue_push_head(writer->stack, xml_create_writer_element_info(name, 0));
    fprintf(writer->handle, "<%s", name);
    writer->node_type = XML_NODE_TYPE_START_ELEMENT;
}

void xml_writer_write_end_all_element(xml_writer_t* writer) {
    while (!g_queue_is_empty(writer->stack)) {
        xml_writer_write_end_element(writer);
    }
}

void xml_writer_write_end_element(xml_writer_t* writer) {
    if (!g_queue_is_empty(writer->stack)) {
        xml_writer_element_info_t* info = g_queue_pop_head(writer->stack);
        if (writer->node_type == XML_NODE_TYPE_START_ELEMENT || writer->node_type == XML_NODE_TYPE_ATTRIBUTE) {
            fprintf(writer->handle, "/>");
        }
        else {
            xml_writer_complete(writer);
            if (!info->has_content) {
                xml_writer_indent(writer);
            }
            fprintf(writer->handle, "</%s>", info->name->str);
        }
        xml_writer_element_info_free(info);
        writer->node_type = XML_NODE_TYPE_NONE;
    }
}

void xml_writer_write_attribute(xml_writer_t* writer, char* name, char* value) {
    if (writer->node_type == XML_NODE_TYPE_START_ELEMENT || writer->node_type == XML_NODE_TYPE_ATTRIBUTE) {
        if (writer->settings.indent && writer->node_type == XML_NODE_TYPE_ATTRIBUTE && writer->settings.new_line_on_attributes) {
            xml_writer_element_info_t* info = g_queue_peek_head(writer->stack);
            size_t length = info->name->len;
            xml_writer_indent(writer);
            fprintf(writer->handle, "%*s", (int)length, " ");
        }
        fprintf(writer->handle, " %s=\"%s\"", name, value);
        writer->node_type = XML_NODE_TYPE_ATTRIBUTE;
    }
}

void xml_writer_write_comment(xml_writer_t* writer, char* comment) {
    xml_writer_complete(writer);
    writer->node_type = XML_NODE_TYPE_TEXT;
    fprintf(writer->handle, "<!--%s-->", comment);
}

void xml_writer_write_cdata(xml_writer_t* writer, char* cdata, size_t length) {
    size_t i;
    xml_writer_complete(writer);
    writer->node_type = XML_NODE_TYPE_TEXT;
    fprintf(writer->handle, "<![CDATA[");
    for (i = 0; i < length; ++i) {
        fprintf(writer->handle, "%s", xml_uint8_to_hexstring(strbuf, cdata[i]));
    }
    fprintf(writer->handle, "]]>");
}

void xml_writer_write_string(xml_writer_t* writer, char* str) {
    xml_writer_element_info_t* info = g_queue_peek_head(writer->stack);
    char* ch_p = str;
    info->has_content = 1;
    xml_writer_complete(writer);
    while (*ch_p != '\0') {
        char ch = *ch_p++;
        switch (ch) {
        case '&':
            fprintf(writer->handle, "&amp;");
            break;
        case '\\':
            fprintf(writer->handle, "&apos;");
            break;
        case '<':
            fprintf(writer->handle, "&lt;");
            break;
        case '>':
            fprintf(writer->handle, "&gt;");
            break;
        default:
            fputc(ch, writer->handle);
            break;
        }
    }
    writer->node_type = XML_NODE_TYPE_TEXT;
}

char* xml_uint64_to_string(char* buf, uint64_t value) {
    sprintf(buf, "%"PRIu64"", value);
    return buf;
}

char* xml_uint32_to_string(char* buf, uint32_t value) {
    sprintf(buf, "%u", value);
    return buf;
}

char* xml_int32_to_string(char* buf, int32_t value) {
    sprintf(buf, "%d", value);
    return buf;
}

char* xml_uint8_to_string(char* buf, uint8_t value) {
    sprintf(buf, "%hhu", value);
    return buf;
}

char* xml_uint8_to_hexstring(char* buf, uint8_t value) {
    sprintf(buf, "0x%hhx", value);
    return buf;
}

#define BUFFER_SIZE 1024
#define QNAME_LENGTH 255

typedef enum xml_token {
    XML_TOKEN_ANGBO = '<',
    XML_TOKEN_ANGBC = '>',
    XML_TOKEN_SLASH = '/',
    XML_TOKEN_SPACE = ' ',
    XML_TOKEN_TAB = '\t',
    XML_TOKEN_CR = '\r',
    XML_TOKEN_LF = '\n',
    XML_TOKEN_QUOTE = '\'',
    XML_TOKEN_DQUOTE = '\"',
    XML_TOKEN_EXCL = '!',
    XML_TOKEN_EQ = '=',
    XML_TOKEN_DASH = '-',
    XML_TOKEN_SQRBO = '[',
    XML_TOKEN_SQRBC = ']',
    XML_TOKEN_QM = '?',
    XML_TOKEN_COLON = ';',
    XML_TOKEN_UNDESCORE = '_',
    XML_TOKEN_AMP = '&',
    XML_TOKEN_EOF = EOF,
} xml_token_t;

typedef struct xml_reader_element_info {
    xml_node_type_t type;
    char* name;
} xml_reader_element_info_t;

typedef struct xml_reader {
    FILE* handle;
    xml_node_type_t node_type;
    xml_stack* stack;
    char node_name[QNAME_LENGTH];
    char* node_content;
    size_t node_content_len;
    char buffer[BUFFER_SIZE];
    size_t read_index;
    size_t write_index;
    size_t line_num;
    size_t column_num;
} xml_reader_t;

xml_exception_t xml_reader_next_token(xml_reader_t* reader);
size_t xml_reader_buffer_available(xml_reader_t* reader);
void xml_reader_fill_buffer(xml_reader_t* reader);
char xml_reader_read(xml_reader_t* reader);
char xml_reader_peek(xml_reader_t* reader, size_t count);
uint8_t xml_reader_iswhitespace(char c);
void xml_reader_skip_whitespace(xml_reader_t* reader);
size_t xml_reader_read_qname(xml_reader_t* reader, char str[QNAME_LENGTH]);
xml_exception_t xml_reader_parse_start_element(xml_reader_t* reader);
xml_exception_t xml_reader_parse_attribute(xml_reader_t* reader);
xml_exception_t xml_reader_parse_text(xml_reader_t* reader);
xml_exception_t xml_reader_parse_end_element(xml_reader_t* reader);
char xml_reader_parse_entity(xml_reader_t* reader);
xml_exception_t xml_reader_parse_comment(xml_reader_t* reader);
xml_exception_t xml_reader_parse_pi(xml_reader_t* reader);
xml_exception_t xml_reader_parse_cdata(xml_reader_t* reader);

xml_reader_t* xml_reader_new(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        xml_reader_t* reader = calloc(1, sizeof(xml_reader_t));
        reader->handle = file;
        reader->line_num = 1;
        reader->stack = g_queue_new();
        return reader;
    }
    return NULL;
}

void xml_reader_close(xml_reader_t* reader) {
    if (reader) {
        fclose(reader->handle);
        free(reader->node_content);
        g_queue_free(reader->stack);
        free(reader);
    }
}

void xml_reader_element_info_free(xml_reader_element_info_t* info){
    if(info){
        free(info->name);
        info->name = NULL;
        free(info);
    }
}

xml_exception_t xml_reader_next(xml_reader_t* reader) {
    xml_exception_t error = XML_EXCEPTION_ARGS;
    if (reader) {
        while (!(error = xml_reader_next_token(reader)) &&reader->node_type == XML_NODE_TYPE_NONE) {
            continue;
        }
    }
    return error;
}

xml_exception_t xml_reader_next_token(xml_reader_t* reader) {
    if (reader) {
        switch (xml_reader_peek(reader, 0)) {
        case XML_TOKEN_EOF:
            return XML_EXCEPTION_END_OF_FILE;
        case XML_TOKEN_ANGBO:
            switch (xml_reader_peek(reader, 1)) {
            case XML_TOKEN_QM:
                return xml_reader_parse_pi(reader);
                break;
            case XML_TOKEN_EXCL:
                switch (xml_reader_peek(reader, 2)) {
                case XML_TOKEN_SQRBO:
                    return xml_reader_parse_cdata(reader);
                    break;
                case XML_TOKEN_DASH:
                    return xml_reader_parse_comment(reader);
                }
                break;
            case XML_TOKEN_SLASH:
                return xml_reader_parse_end_element(reader);
            default:
                return xml_reader_parse_start_element(reader);
            }
            break;
        default:
            switch (reader->node_type) {
            case XML_NODE_TYPE_PI:
            case XML_NODE_TYPE_START_ELEMENT:
            case XML_NODE_TYPE_ATTRIBUTE:
                return xml_reader_parse_attribute(reader);
                break;
            default:
                return xml_reader_parse_text(reader);
                break;
            }
            break;
        }
    }
    return XML_EXCEPTION_ARGS;
}

const char* xml_error_to_string(xml_exception_t error) {
    switch (error) {
    case XML_EXCEPTION_ARGS:
        return "xml error in argument";
    case XML_EXCEPTION_PI:
        return "xml error in processing instruction";
    case XML_EXCEPTION_CDATA:
        return "xml error in CDATA section";
    case XML_EXCEPTION_COMMENT:
        return "xml error in comment";
    case XML_EXCEPTION_END_ELEMENT:
        return "xml error in end element";
    case XML_EXCEPTION_START_ELEMENT:
        return "xml error in start element";
    case XML_EXCEPTION_ATTRIBUTE:
        return "xml error in attribute";
    case XML_EXCEPTION_TEXT:
        return "xml error in text content";
    case XML_EXCEPTION_END_OF_FILE:
        return "end of file";
    case XML_EXCEPTION_MISMATCHED_TAG:
        return "mismatched tag";
    default:
        return "";
    }
}

size_t xml_reader_buffer_available(xml_reader_t* reader) {
    return ((BUFFER_SIZE + reader->write_index - reader->read_index) % BUFFER_SIZE);
}

void xml_reader_fill_buffer(xml_reader_t* reader) {
    if (reader) {
        while (!feof(reader->handle) && xml_reader_buffer_available(reader) < BUFFER_SIZE -1) {
            reader->buffer[reader->write_index] = fgetc(reader->handle);
            reader->write_index = (reader->write_index + 1) % BUFFER_SIZE;
        }
    }
}

char xml_reader_read(xml_reader_t* reader) {
    char ch = -1;
    if (reader) {
        if (xml_reader_buffer_available(reader) == 0) {
            xml_reader_fill_buffer(reader);
        }
        if (xml_reader_buffer_available(reader) > 0) {
            ch = reader->buffer[reader->read_index];
            reader->read_index = (reader->read_index + 1) % BUFFER_SIZE;
            if (ch == XML_TOKEN_LF) {
                ++reader->line_num;
                reader->column_num = 0;
            }
            else {
                ++reader->column_num;
            }
            return ch;
        }
    }
    return ch;
}

char xml_reader_peek(xml_reader_t* reader, size_t count) {
    char ch = -1;
    if (reader && count < BUFFER_SIZE) {
        if (xml_reader_buffer_available(reader) <= count) {
            xml_reader_fill_buffer(reader);
        }
        if (xml_reader_buffer_available(reader) > count) {
            ch = reader->buffer[(reader->read_index + count) % BUFFER_SIZE];
            return ch;
        }
    }
    return ch;
}
uint8_t xml_reader_iswhitespace(char c) {
    switch (c) {
    case XML_TOKEN_SPACE:
    case XML_TOKEN_TAB:
    case XML_TOKEN_CR:
    case XML_TOKEN_LF:
        return 1;
        break;
    default:
        return 0;
    }
}

void xml_reader_skip_whitespace(xml_reader_t* reader) {
    if (reader) {
        char ch;
        while ((ch = xml_reader_peek(reader, 0)) != XML_TOKEN_EOF) {
            switch (ch) {
            case XML_TOKEN_SPACE:
            case XML_TOKEN_TAB:
            case XML_TOKEN_CR:
            case XML_TOKEN_LF:
                xml_reader_read(reader);
                break;
            default:
                return;
            }
        }
    }
}

size_t xml_reader_read_qname(xml_reader_t* reader, char str[QNAME_LENGTH]) {
    size_t i = 0;
    if (reader) {
        char ch;
        memset(str, 0, QNAME_LENGTH);
        xml_reader_skip_whitespace(reader);
        ch = xml_reader_peek(reader, 0);
        if (isalpha(ch)) {
            do {
                str[i++] = xml_reader_read(reader);
            } while (i < QNAME_LENGTH && (isalnum(ch = xml_reader_peek(reader, 0)) || ch == XML_TOKEN_UNDESCORE));
        }
    }
    return i;
}

xml_exception_t xml_reader_parse_start_element(xml_reader_t* reader) {
    if (reader && xml_reader_read(reader) == XML_TOKEN_ANGBO) {
        reader->node_type = XML_NODE_TYPE_START_ELEMENT;
        if (xml_reader_read_qname(reader, reader->node_name)) {
            xml_reader_element_info_t* info = calloc(1, sizeof(xml_reader_element_info_t));
            info->type = XML_NODE_TYPE_START_ELEMENT;
            asprintf(&info->name, "%s", reader->node_name);
            g_queue_push_head(reader->stack, info);
            if (reader->node_content) {
                memset(reader->node_content, 0, reader->node_content_len);
            }
            return XML_EXCEPTION_NONE;
        }
    }
    return XML_EXCEPTION_START_ELEMENT;
}

xml_exception_t xml_reader_parse_attribute(xml_reader_t* reader) {
    if (reader) {
        char illegal_chars[] = { XML_TOKEN_ANGBO, XML_TOKEN_ANGBC, '\0' };
        uint32_t element_hash;
        xml_reader_skip_whitespace(reader);
        memset(reader->node_name, 0, QNAME_LENGTH);
        if (reader->node_content) {
            memset(reader->node_content, 0, reader->node_content_len);
        }
        reader->node_type = XML_NODE_TYPE_NONE;
        switch (xml_reader_peek(reader, 0)) {
        xml_reader_element_info_t* info;
        case XML_TOKEN_QM:
            while (!strrchr(illegal_chars, xml_reader_read(reader))) {
                continue;
            }
            info = g_queue_pop_head(reader->stack);
            element_hash = zwscn_util_str_hash(info->name);
            xml_reader_element_info_free(info);
            if (element_hash != zwscn_util_str_hash("pi")) {
                return XML_EXCEPTION_MISMATCHED_TAG;
            }
            return XML_EXCEPTION_NONE;
        case XML_TOKEN_SLASH:
            while (!strrchr(illegal_chars, xml_reader_read(reader))) {
                continue;
            }
            info = g_queue_pop_head(reader->stack);
            if (info->type != XML_NODE_TYPE_START_ELEMENT) {
                xml_reader_element_info_free(info);
                return XML_EXCEPTION_MISMATCHED_TAG;
            }
            reader->node_type = XML_NODE_TYPE_END_ELEMENT;
            sprintf(reader->node_name, "%s", info->name);
            xml_reader_element_info_free(info);
            return XML_EXCEPTION_NONE;
        case XML_TOKEN_ANGBC:
            while (!strrchr(illegal_chars, xml_reader_read(reader))) {
                continue;
            }
            info = g_queue_peek_head(reader->stack);
            if (info->type != XML_NODE_TYPE_START_ELEMENT) {
                return XML_EXCEPTION_MISMATCHED_TAG;
            }
            return XML_EXCEPTION_NONE;
        default:
            if (xml_reader_read_qname(reader, reader->node_name)) {
                reader->node_type = XML_NODE_TYPE_ATTRIBUTE;
                xml_reader_skip_whitespace(reader);
                if (xml_reader_read(reader) == XML_TOKEN_EQ) {
                    char quote;
                    xml_reader_skip_whitespace(reader);
                    if ((quote = xml_reader_read(reader)) == XML_TOKEN_QUOTE || quote == XML_TOKEN_DQUOTE) {
                        size_t i = 0;
                        char ch;
                        if (reader->node_content) {
                            memset(reader->node_content, 0, reader->node_content_len);
                        }
                        while ((ch = xml_reader_peek(reader, 0)) != quote && ch != XML_TOKEN_EOF && !strrchr(illegal_chars, ch)) {
                            if (!reader->node_content || i >= reader->node_content_len) {
                                reader->node_content = realloc(reader->node_content, reader->node_content_len + QNAME_LENGTH);
                                reader->node_content_len += QNAME_LENGTH;
                            }
                            if (reader->node_content && i < reader->node_content_len) {
                                switch (ch) {
                                case XML_TOKEN_AMP:
                                    if ((ch = xml_reader_parse_entity(reader)) > 0) {
                                        reader->node_content[i++] = ch;
                                    }
                                    break;
                                default:
                                    reader->node_content[i++] = xml_reader_read(reader);
                                    break;
                                }
                            }
                            else {
                                break;
                            }

                        }
                        if (xml_reader_peek(reader, 0) == quote) {
                            xml_reader_read(reader);
                            return XML_EXCEPTION_NONE;
                        }
                    }
                }
            }
            break;
        }

    }
    return XML_EXCEPTION_ATTRIBUTE;
}

xml_exception_t xml_reader_parse_text(xml_reader_t* reader) {
    if (reader) {
        char illegal_chars[] = { XML_TOKEN_ANGBO, XML_TOKEN_ANGBC, '\0' };
        size_t i = 0;
        char ch;
        memset(reader->node_name, 0, QNAME_LENGTH);
        xml_reader_skip_whitespace(reader);
        if (reader->node_content) {
            memset(reader->node_content, 0, reader->node_content_len);
        }
        reader->node_type = XML_NODE_TYPE_NONE;
        while ((ch = xml_reader_peek(reader, 0)) != XML_TOKEN_EOF && !strrchr(illegal_chars, ch)) {
            if (reader->node_type != XML_NODE_TYPE_TEXT) {
                reader->node_type = XML_NODE_TYPE_TEXT;
            }
            if (!reader->node_content || i >= reader->node_content_len) {
                reader->node_content = realloc(reader->node_content, reader->node_content_len + QNAME_LENGTH);
                reader->node_content_len += QNAME_LENGTH;
            }
            if (reader->node_content && i < reader->node_content_len) {
                switch (ch) {
                case XML_TOKEN_AMP:
                    if ((ch = xml_reader_parse_entity(reader)) > 0) {
                        reader->node_content[i++] = ch;
                    }
                    break;
                default:
                    reader->node_content[i++] = xml_reader_read(reader);
                    break;
                }
            }
        }
        while (i > 0) {
            if (xml_reader_iswhitespace(reader->node_content[i - 1])) {
                reader->node_content[--i] = '\0';
            }
            else {
                break;
            }
        }
        return XML_EXCEPTION_NONE;
    }
    return XML_EXCEPTION_TEXT;
}

xml_exception_t xml_reader_parse_end_element(xml_reader_t* reader) {
    if (reader) {
        if (xml_reader_read(reader) == XML_TOKEN_ANGBO) {
            if (xml_reader_read(reader) == XML_TOKEN_SLASH) {
                reader->node_type = XML_NODE_TYPE_END_ELEMENT;
                if (xml_reader_read_qname(reader, reader->node_name)) {
                    if (reader->node_content) {
                        memset(reader->node_content, 0, reader->node_content_len);
                    }
                    xml_reader_skip_whitespace(reader);
                    if (xml_reader_read(reader) == XML_TOKEN_ANGBC) {
                        uint32_t element_hash;
                        xml_reader_element_info_t* info = g_queue_pop_head(reader->stack);
                        element_hash = zwscn_util_str_hash(info->name);
                        xml_reader_element_info_free(info);
                        if (element_hash != zwscn_util_str_hash(reader->node_name)) {
                            return XML_EXCEPTION_MISMATCHED_TAG;
                        }
                        return XML_EXCEPTION_NONE;
                    }
                }

            }
        }
    }
    return XML_EXCEPTION_END_ELEMENT;
}

char xml_reader_parse_entity(xml_reader_t* reader) {
    int AMP = zwscn_util_str_hash("amp");
    int APOS = zwscn_util_str_hash("apos");
    int LT = zwscn_util_str_hash("lt");
    int GT = zwscn_util_str_hash("gt");
    if (reader) {
        char entity[QNAME_LENGTH] = "\0";
        if (xml_reader_peek(reader, 0) == XML_TOKEN_AMP) {
            xml_reader_read(reader);
            if (xml_reader_read_qname(reader, entity)) {
                uint32_t entity_hash = zwscn_util_str_hash(entity);
                if (xml_reader_peek(reader, 0) == XML_TOKEN_COLON) {
                    xml_reader_read(reader);
                    if(entity_hash == AMP){
                        return XML_TOKEN_AMP;
                    }
                    else if(entity_hash == APOS){
                        return XML_TOKEN_QUOTE;
                    }
                    else if(entity_hash == LT){
                        return XML_TOKEN_ANGBO;
                    }
                    else if(entity_hash == GT){
                        return XML_TOKEN_ANGBC;
                    }
                }
            }
        }
    }
    return -1;
}

xml_exception_t xml_reader_parse_comment(xml_reader_t* reader) {
    if (reader) {
        char buf[5] = { xml_reader_peek(reader, 0), xml_reader_peek(reader, 1), xml_reader_peek(reader, 2), xml_reader_peek(reader, 3), '\0', };
        if (strcmp(buf, "<!--") == 0) {
            size_t i = 0;
            char ch;
            xml_reader_read(reader);
            xml_reader_read(reader);
            xml_reader_read(reader);
            xml_reader_read(reader);
            memset(buf, 0, 5);
            memset(reader->node_name, 0, QNAME_LENGTH);
            if (reader->node_content) {
                memset(reader->node_content, 0, reader->node_content_len);
            }
            while ((ch = xml_reader_peek(reader, 0)) != XML_TOKEN_EOF) {
                if (reader->node_type != XML_NODE_TYPE_COMMENT) {
                    reader->node_type = XML_NODE_TYPE_COMMENT;
                }
                buf[0] = xml_reader_peek(reader, 0);
                buf[1] = xml_reader_peek(reader, 1);
                buf[2] = xml_reader_peek(reader, 2);
                buf[3] = '\0';
                if (strcmp(buf, "-->") == 0) {
                    xml_reader_read(reader);
                    xml_reader_read(reader);
                    xml_reader_read(reader);
                    break;
                }
                else {
                    if (!reader->node_content || i >= reader->node_content_len) {
                        reader->node_content = realloc(reader->node_content, reader->node_content_len + QNAME_LENGTH);
                        reader->node_content_len += QNAME_LENGTH;
                    }
                    if (reader->node_content && i < reader->node_content_len) {
                        reader->node_content[i++] = xml_reader_read(reader);
                    }
                }
            }
            if (strcmp(buf, "-->") == 0) {
                return XML_EXCEPTION_NONE;
            }

        }
    }
    return XML_EXCEPTION_COMMENT;
}

xml_exception_t xml_reader_parse_pi(xml_reader_t* reader) {
    if (reader && xml_reader_peek(reader, 0) == XML_TOKEN_ANGBO && xml_reader_peek(reader, 1) == XML_TOKEN_QM) {
        xml_reader_read(reader);
        xml_reader_read(reader);
        if (xml_reader_read_qname(reader, reader->node_name)) {
            reader->node_type = XML_NODE_TYPE_PI;
            xml_reader_element_info_t* info = calloc(1, sizeof(xml_reader_element_info_t));
            info->type = XML_NODE_TYPE_PI;
            asprintf(&info->name, "pi");
            g_queue_push_head(reader->stack, info);
            if (reader->node_content) {
                memset(reader->node_content, 0, reader->node_content_len);
            }
            return XML_EXCEPTION_NONE;
        }

    }
    return XML_EXCEPTION_PI;
}

xml_exception_t xml_reader_parse_cdata(xml_reader_t* reader) {
    if (reader) {
        size_t i = 0;
        char buf[10] = "\0";
        for (i = 0; i < 9; ++i) {
            buf[i] = xml_reader_peek(reader, i);
        }
        if (strcmp(buf, "<![CDATA[") == 0) {
            char ch;
            for (i = 0; i < 9; ++i) {
                xml_reader_read(reader);
            }
            memset(buf, 0, 5);
            memset(reader->node_name, 0, QNAME_LENGTH);
            if (reader->node_content) {
                memset(reader->node_content, 0, reader->node_content_len);
            }
            while ((ch = xml_reader_peek(reader, 0)) != XML_TOKEN_EOF) {
                if (reader->node_type != XML_NODE_TYPE_CDATA) {
                    reader->node_type = XML_NODE_TYPE_CDATA;
                }
                buf[0] = xml_reader_peek(reader, 0);
                buf[1] = xml_reader_peek(reader, 1);
                buf[2] = xml_reader_peek(reader, 2);
                buf[3] = '\0';
                if (strcmp(buf, "]]>") == 0) {
                    xml_reader_read(reader);
                    xml_reader_read(reader);
                    xml_reader_read(reader);
                    break;
                }
                else {
                    if (!reader->node_content || i >= reader->node_content_len) {
                        reader->node_content = realloc(reader->node_content, reader->node_content_len + QNAME_LENGTH);
                        reader->node_content_len += QNAME_LENGTH;
                    }
                    if (reader->node_content && i < reader->node_content_len) {
                        reader->node_content[i++] = xml_reader_read(reader);
                    }
                }
            }
            if (strcmp(buf, "]]>") == 0) {
                return XML_EXCEPTION_NONE;
            }
        }
    }
    return XML_EXCEPTION_CDATA;
}

xml_node_type_t xml_reader_node_type(xml_reader_t* reader) {
    return reader->node_type;
}
char* xml_reader_node_name(xml_reader_t* reader) {
    return reader->node_name;
}
char* xml_reader_node_content(xml_reader_t* reader) {
    return reader->node_content;
}

uint64_t xml_string_to_uint64(char* str) {
    return strtoull(str, 0, 10);
}

uint32_t xml_string_to_uint32(char* str) {
    return strtoul(str, 0, 10);
}

int32_t xml_string_to_int32(char* str) {
    return atoi(str);
}

uint8_t xml_string_to_uint8(char* str) {
    return atoi(str);
}

uint8_t xml_reader_skip_till(xml_reader_t* reader, xml_node_type_t type, char* node_name) {
    xml_exception_t error = XML_EXCEPTION_NONE;
    uint32_t node_name_hash = zwscn_util_str_hash(node_name);
    do {
        if (reader->node_type == type && node_name_hash == zwscn_util_str_hash(xml_reader_node_name(reader))) {
            return 1;
        }
    } while (!(error = xml_reader_next(reader)));
    return 0;
}

uint8_t xml_reader_skip_till_type(xml_reader_t* reader, xml_node_type_t type, xml_node_type_t till) {
    xml_exception_t error = XML_EXCEPTION_NONE;
    do {
        if (reader->node_type == type) {
            return 1;
        }
        else if(reader->node_type == till){
            return 0;
        }
    } while (!(error = xml_reader_next(reader)));
    return 0;
}
