#include "common.h"
#include "integration.h"

zwlevel_t lev;
uint8_t cnt;
uint8_t color_refresh_count;
uint8_t component_id[NO_OF_COLOR_COMP] = { 0, 1, 2, 3, 4 };	// color components type of array[5]

extern request_t requestdummy;
extern request_t request;//  request :This is Global structure used for recieving the request from Device Manager
extern hl_appl_ctx_t appl_ctx;
extern uint32_t homeid;
extern response_t discovery_struct;	//  discovery_struct :This is Global structure used for filling the discovering device data
extern uint8_t discovery_node_id;//  Global Variable :Storing the newly added node id, using all call back for checking b/w discovering node and observe node
/**
 interface_get - Search for the required interface
 @param[in]  net         network handle
 @param[out] intf_d   required interface found
 @return  0 on success; otherwise negative number
 */
int interface_get(zwnet_p net, uint8_t node_id, uint8_t ep_id, uint16_t cmd_cls,
		zwifd_t *intf_d) {
	int result;
	zwifd_t intf;
	result = zwnet_get_if_by_id(net, node_id, ep_id, cmd_cls, &intf);
	*intf_d = intf;
	return result;
}

/*This function finds the Command class of the devices*/
uint16_t command_class_find(uint8_t ep_id, uint16_t cls) {
	int cls_count = 0;
	syslog(LOG_DEBUG, "ZW: command_class_find:ep_id:%d  cls:%d", ep_id, cls);

	for (cls_count = 0; cls_count < NO_OF_INTERFACES; cls_count++) {
		if (discovery_struct.ep[ep_id].intf[cls_count].cmd_cls == cls) {
			syslog(LOG_DEBUG, "ZW: command_class_find: %d", cls_count);
			return cls_count;
		}
	}
	syslog(LOG_DEBUG, "ZW: Null command_class_find returned");
	return;
}

/**
 hl_battery_rpt_cb - Battery report callback function gets the battery level details, constructs the response structure and sends the response to DeviceManager
 @param[in] ifd  interface handle
 @param[in] level   Battery level
 @param[in] ts    Time stamp. If this is zero, the callback has no data and hence other
 parameter values should be ignored.
 */
static void hl_battery_rpt_fn_cb(zwifd_p ifd, uint8_t level, time_t ts) {
	int result;
	syslog(LOG_DEBUG, "ZW: Battery report called \n");
	zwepd_t epd;
	response_t response = { 0 };
	response.operation = requestdummy.operation;
	response.home_id = homeid;
	response.msgid = requestdummy.msgid;

	if (ts == 0) {
		syslog(LOG_DEBUG, "ZW: Battery report: no cached data");
		return;
	}

	if ((discovery_node_id != ifd->nodeid) && (requestdummy.operation != CONSIGN)) {
		syslog(LOG_DEBUG, "ZW: Inside discovery_node_id-sensor: %d ",
				requestdummy.operation);
		goto def;
	}

	switch (requestdummy.operation) {
	case COMMISSIONING:
	case UPDATE:
		syslog(LOG_DEBUG, "ZW: Battery Level: %d", level);
		discovery_struct.ep[ifd->epid].ep_id = ifd->epid;
		discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
				ifd->cls)].property_value.ch = level;
		print_discovery_struct();
		break;

		def: default:
		syslog(LOG_DEBUG, "ZW: In battery callback");
		response.operation = NODE_STATUS;
		response.err_code = ERR_BATTERY_STS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch = level;
		syslog(LOG_DEBUG, "ZW: Battery Level-OBSERVE :%d", level);
		response_DM(response);
		break;
	}
}

/**
 hl_snsr_rep_cb -   		Report callback,To get the sensors data.
 @param[in]  ifd     		Interface handle
 @param[in]  data   		Sensor data  
 @param[in]  ts    		Time stamp. If this is zero, the callback has no data and hence other parameter values should be ignored.
 */

static void hl_snsr_rep_cb(zwifd_p ifd, zwsensor_p data, time_t ts) {
	float f = CONVERSION_VALUE, conversion_buffer = 0, sensor_buffer = 0;
	int size_count = 0;
	int result;
	zwepd_t epd;
	response_t response = { 0 };


	syslog(LOG_DEBUG, "ZW: S2_Sensor ");

	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW: No end point found for hl_snsr_rep_cb returns error: %d", result);
	}
	response.home_id = homeid;
	response.msgid = requestdummy.msgid;

	syslog(LOG_DEBUG, "ZW: Sensor report cb TIME STAMP IS %d", ts);
	syslog(LOG_DEBUG,
			"ZW: Sensor type of CC COMMAND_CLASS_SENSOR_MULTILEVEL(49): %d:",
			data->type);
	syslog(LOG_DEBUG,
			"ZW: Sensor precision of CC COMMAND_CLASS_SENSOR_MULTILEVEL(49): %d:",
			data->precision);
	syslog(LOG_DEBUG,
			"ZW: Sensor Unit of CC COMMAND_CLASS_SENSOR_MULTILEVEL(49): %d:",
			data->unit);
	syslog(LOG_DEBUG,
			"ZW: Sensor size of CC COMMAND_CLASS_SENSOR_MULTILEVEL(49): %d",
			data->size);

	if (data->precision == PRECISION_ZERO) {
		for (size_count = 0; size_count <= data->size; size_count++)
			syslog(LOG_DEBUG,
					"ZW: The Total Sensor Data size(index_size)is: %d",
					data->data[size_count]);

		if (data->type == LUX && data->data[0] > 0) {
			sensor_buffer = data->data[0];
			syslog(LOG_DEBUG, "ZW: LUX Sensor Data with data[0]: %f",
					sensor_buffer);
		}

		else if (data->type == LUX && data->data[1] >= 0) {
			for (size_count = 0; size_count <= data->size; size_count++)
				syslog(LOG_DEBUG, "ZW: LUX Sensor Data : %d",
						data->data[size_count]);

			conversion_buffer = (data->data[0] << LEFT_SHIFT_EIGHT_BITS)
					| (data->data[1]);
			sensor_buffer = conversion_buffer / CONVERSION_VALUE;
			syslog(LOG_DEBUG,
					"ZW: LUX Sensor value to dm with PRECISION_ZERO: %f",
					sensor_buffer);
		} else if (data->type == TEMPERATURE && data->data[0] > 0) {

			if (data->data[1] > 0) {
				conversion_buffer = data->data[0];
				sensor_buffer = conversion_buffer / DIVIDE_BY_TEN;
				syslog(LOG_DEBUG,
						"ZW: Temperature Sensor data with data[0] with Precision=0: %f",
						sensor_buffer);
			} else {
				for (size_count = 0; size_count <= data->size; size_count++)
					syslog(LOG_DEBUG,
							"ZW: Temperature Sensor data with data[0]: %d",
							data->data[size_count]);
				if(data->unit==0)
				{
					sensor_buffer = data->data[0];
					syslog(LOG_DEBUG,
							"ZW: Temperature Sensor data with data[0] with Precision=0: %f",
							sensor_buffer);
				}
				
				else if(data->unit==1)
				{	sensor_buffer= (data->data[0] - 32) * 5/9;
					syslog(LOG_DEBUG,
							"ZW: Temperature Sensor data (when UNIT=1) data[0] with Precision=0: %f",
							sensor_buffer);
				}
			}
		} else if (data->type == TEMPERATURE && data->data[1] > 0) {
			for (size_count = 0; size_count <= data->size; size_count++)
				syslog(LOG_DEBUG, "ZW: The Sensor Data with data[0]: %d",
						data->data[size_count]);

			conversion_buffer = (data->data[1] << LEFT_SHIFT_FIVE_BITS);
			sensor_buffer = conversion_buffer / DIVIDE_BY_HUNDRED;
			syslog(LOG_DEBUG,
					"ZW: Temperature Sensor value to dm with  with Precision Zero: %f",
					sensor_buffer);
		}
	}

	else if (data->precision == PRECISION_ONE) {
		for (size_count = 0; size_count <= data->size; size_count++)
			syslog(LOG_DEBUG, "ZW: The Sensor Data with data[0]: %d",
					data->data[size_count]);

		conversion_buffer = (data->data[0] << LEFT_SHIFT_EIGHT_BITS)
				| (data->data[1]);
		sensor_buffer = conversion_buffer / DIVIDE_BY_TEN;
		syslog(LOG_DEBUG, "ZW: Sensor Value with Precision=1: %f ",
				sensor_buffer);
	}

	else {
		for (size_count = 0; size_count <= data->size; size_count++)
			syslog(LOG_DEBUG, "ZW: The Sensor Data is %d",
					data->data[size_count]);

		conversion_buffer = (data->data[0] << LEFT_SHIFT_EIGHT_BITS)
				| (data->data[1]);
		sensor_buffer = conversion_buffer / DIVIDE_BY_HUNDRED;
		syslog(LOG_DEBUG, "ZW: Sensor Value with Precision=2: %f ",
				sensor_buffer);
	}

	if ((discovery_node_id != ifd->nodeid) && (requestdummy.operation != CONSIGN)) {
		syslog(LOG_DEBUG, "ZW:\nInside discovery_node_id-sensor: %d ",
				requestdummy.operation);
		goto def;
	}
	switch (requestdummy.operation) {
	case COMMISSIONING:
	case UPDATE:
		syslog(LOG_DEBUG, "ZW: Sensor data COMMISSIONING: %f ", sensor_buffer);
		discovery_struct.ep[ifd->epid].ep_id = ifd->epid;
		discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
				ifd->cls)].property_value.ch = 0;
		break;

		def: default:
		response.operation = NODE_STATUS;
		response.err_code = OP_SUCCESS;
		response.node_id = ifd->nodeid;
		response.msgid = 0;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].no_of_types = 1;
		response.ep[EP_INDEX].intf[CC_INDEX].EVENT[0].type = data->type;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch = sensor_buffer;
		syslog(LOG_DEBUG, "ZW: Sensor data default: %f ", sensor_buffer);
		response_DM(response);
		//bzero(&response, sizeof(response_DM));
		memset(&response,0,sizeof(response_DM));
		break;
	}
}

/**
 hl_rep_alrm_evt_fn_cb -   	To get the alarm events.
 @param[in]  ifd     		Interface handle
 @param[in]  ztype   		Sensor types  
 @param[in]  evt_len   		Sensor events length 
 @param[in]  *evt         	Sensor events
 @param[in]  valid		Cache valid ot not
 */

void hl_rep_alrm_evt_fn_cb(zwifd_p ifd, uint8_t ztype, uint8_t evt_len,
		uint8_t *evt, int valid) {

	int result;
	char events_count = 0;
	zwepd_t epd;

	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW:no end point found for hl_rep_alrm_evt_fn_cb returns error: %d", result);
	}

	syslog(LOG_DEBUG, "ZW: Alarm Sensor type is: %d", ztype);

	if (!valid) {
		syslog(LOG_ERR, "ZW: No cache for supported alarm sensor types");
		return;
	}

	syslog(LOG_DEBUG, "ZW: Alarm Sensor Event Length is: %d", evt_len);

	if (evt_len > 0) {
		for (uint8_t t = 0; t < evt_len; t++)
			syslog(LOG_DEBUG, "ZW: Alarm Sensors Events:%d", evt[t]);

		for (events_count = 0; events_count < NO_EVENTS; events_count++) {
			if (discovery_struct.ep[ifd->epid].intf[command_class_find(
					ifd->epid, ifd->cls)].EVENT[events_count].type == 0xFF) {
				discovery_struct.ep[ifd->epid].intf[command_class_find(
						ifd->epid, ifd->cls)].EVENT[events_count].type = ztype;
				discovery_struct.ep[ifd->epid].intf[command_class_find(
						ifd->epid, ifd->cls)].EVENT[events_count].no_of_events =
						evt_len;
				syslog(LOG_DEBUG, "ZW: no_of_events: %d",
						discovery_struct.ep[ifd->epid].intf[command_class_find(
								ifd->epid, ifd->cls)].EVENT[events_count].no_of_events);
				syslog(LOG_DEBUG,
						"ZW: ifd->epid:%d   command_class_find(ifd->epid,ifd->cls:%d  events_count:%d",
						ifd->epid, command_class_find(ifd->epid, ifd->cls),
						events_count);

				for (uint8_t a = 0; a < evt_len; a++)
					discovery_struct.ep[ifd->epid].intf[command_class_find(
							ifd->epid, ifd->cls)].EVENT[events_count].evt[a] =
							evt[a];

				break;
			}
		}

	}
	print_discovery_struct();
}

/**
 hl_sensor_sup_get_cb -   	Supported get callback to get type information.
 @param[in]  ifd     		Interface handle
 @param[in]  type_len   		Sensor types length 
 @param[in]   *type         	Sensor types
 @param[in]  valid		Cache valid ot not
 */

static void hl_sensor_sup_get_cb(zwifd_p ifd, uint8_t type_len, uint8_t *type,
		int valid) {
	response_t response = { 0 };
	int result;
	uint8_t t;
	char type_count;
	zwepd_t epd;

	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW:no end point found for hl_sensor_sup_get_cb returns error: %d", result);
	}

	response.home_id = homeid;
	response.operation = COMMISSIONING;
	response.msgid = requestdummy.msgid;

	syslog(LOG_DEBUG, "ZW: Sensor Supported cb valid:%d", valid);

	if (!valid) {
		syslog(LOG_DEBUG, "ZW: No cache for supported sensor types ");
		return;
	}

	syslog(LOG_DEBUG,
			"ZW: Sensor Type Length of CC COMMAND_CLASS_SENSOR_MULTILEVEL(49): %d",
			type_len);
	discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid, ifd->cls)].no_of_types =
			type_len;

	syslog(LOG_DEBUG, "ZW: Type Length is-49: %x ep: %d cmd_cls: %d",
			discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
					ifd->cls)].no_of_types, ifd->epid, ifd->cls);

	syslog(LOG_DEBUG, "ZW: ep: %d,interface: %d", ifd->epid,
			command_class_find(ifd->epid, ifd->cls));
	if (type_len > 0) {
		for (t = 0; t < type_len; t++) {
			if (type[t] >= ZW_SENSOR_TYPE_RF_SGN) {
				type[t] = 0;
			}
		}

		for (type_count = 0; type_count < NO_TYPES; type_count++)
			syslog(LOG_DEBUG,
					"ZW: Z-wave Sensor Types of CC COMMAND_CLASS_SENSOR_MULTILEVEL(49):%d",
					type[t]);

		for (type_count = 0; type_count < NO_TYPES; type_count++) {
			if (discovery_struct.ep[ifd->epid].intf[command_class_find(
					ifd->epid, ifd->cls)].EVENT[type_count].type == 0xFF)
				discovery_struct.ep[ifd->epid].intf[command_class_find(
						ifd->epid, ifd->cls)].EVENT[type_count].type =
						type[type_count];

		}
		print_discovery_struct();
	}
}

/**
 hl_alrm_sup_cb -   		Supported callback to get alarm type information.
 @param[in]  ifd     		Interface handle
 @param[in]  ztype_len   	Sensor types length 
 @param[in]  *ztype         	Sensor types
 @param[in]  valid		Cache valid ot not
 */
static void hl_alrm_sup_cb(zwifd_p ifd, uint8_t have_vtype, uint8_t ztype_len,
		uint8_t *ztype, int valid) {
	int result;
	syslog(LOG_DEBUG, "ZW: Alarm supported get callback called");

	if (!valid) {
		syslog(LOG_ERR, "ZW: No cache for supported alarm sensor types");
		return;
	}

	syslog(LOG_DEBUG,
			"ZW: Sensor Type Length of CC COMMAND_CLASS_ALARM(113): %d",
			ztype_len);

	discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid, ifd->cls)].no_of_types =
			ztype_len;
	syslog(LOG_DEBUG, "ZW: Type Length is-113: %x ep: %d cmd_cls: %d",
			discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
					ifd->cls)].no_of_types, ifd->epid, ifd->cls);

	if (ztype_len > 0) {
		for (uint8_t t = 0; t < ztype_len; t++) {
			if (ztype[t] >= ZW_ALRM_GAS)
				ztype[t] = 0;

			result = zwif_alrm_sup_evt_get(ifd, ztype[t], hl_rep_alrm_evt_fn_cb,
					0);
			syslog(LOG_DEBUG,
					"ZW: Sensor Types of CC COMMAND_CLASS_ALARM(113):%d",
					ztype[t]);
		}
	}
}

/**
 hl_switch_report_cb - Switch report callback function gets the switch device type details, constructs the response structure and sends the response to DeviceManager
 @param[in] ifd     Switch interface handle
 @param[in] on   0 – off  1 – on
 @param[in] ts    Time stamp. If this is zero, the callback has no data and hence other
 parameter values should be ignored.
 */
void hl_switch_report_cb(zwifd_p ifd, zwswitch_p val, time_t ts) {
	int result;
	response_t response = { 0 };
	response.home_id = homeid;
	zwepd_t epd;

	syslog(LOG_DEBUG, "ZW: Switch report called");

	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW:no end point found hl_switch_report_cb return error : %d", result);
	}

	if (ts == 0) {
		syslog(LOG_ERR, "ZW: Binary switch report: no cached data");
		return;
	}

	if ((discovery_node_id != ifd->nodeid) && (requestdummy.operation != CONSIGN)) {
		syslog(LOG_DEBUG, "ZW: Inside discovery_node_id-switch: %d", requestdummy.operation);
		goto def;
	}

	switch (requestdummy.operation) {
	case COMMISSIONING:
	case UPDATE:
		syslog(LOG_DEBUG, "ZW: COMMISSIONING Switch state :%d", val->curr_val);
		discovery_struct.ep[ifd->epid].ep_id = ifd->epid;
		discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
				ifd->cls)].property_value.ch = val->curr_val;

			syslog(LOG_DEBUG, "ZW:Kalyan COMMISSIONING  %d %d %d",ifd->epid,command_class_find(ifd->epid,
				ifd->cls),val->curr_val);
		print_discovery_struct();
		break;

	case CONSIGN:
		response.operation = requestdummy.operation;
		response.msgid = requestdummy.msgid;
		response.err_code = OP_SUCCESS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch =val->curr_val;
		syslog(LOG_DEBUG, "ZW: Switch state :%d", val->curr_val);
		response_DM(response);
		memset(&requestdummy,0,sizeof(requestdummy));
		break;

		def: default:
		response.err_code = OP_SUCCESS;
		response.operation = NODE_STATUS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch = val->curr_val;
		syslog(LOG_DEBUG, "ZW: default-Switch state :%d", val->curr_val);
		response_DM(response);
		break;

	}
}

/**
 hl_dlck_op_rpt_cb - Door Lock Report callback function
 @param[in] zwifd_p   Door lock interface handle
 @param[in] zwdlck_op_p   operation status
 @param[in] time_t  Time stamp. If this is zero, the callback has no data and hence other parameter values should be ignored.
 */
void hl_dlck_op_rpt_cb(zwifd_p ifd, zwdlck_op_p op_sts, time_t ts) {
	syslog(LOG_DEBUG, "ZW: Door Lock report called");
	response_t response = { 0 };

	int result;
	zwepd_t epd;
	response.home_id = homeid;
	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW:no end point found, API return error : %d", result);
	}

	if (ts == 0) {
		syslog(LOG_ERR,
				"ZW: Door Lock report: No cached data Report get: modem");
		return;
	}

	if ((discovery_node_id != ifd->nodeid) && (requestdummy.operation != CONSIGN)) {
		syslog(LOG_DEBUG, "ZW: Inside discovery_node_id-switch: %d", requestdummy.operation);
		goto def;
	}

	switch (requestdummy.operation) {
	case COMMISSIONING:
	case UPDATE:
		syslog(LOG_DEBUG, "ZW: Lock mode: %d", op_sts->mode);

		discovery_struct.ep[ifd->epid].ep_id = ifd->epid;
		discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
				ifd->cls)].property_value.ch = op_sts->mode;
		print_discovery_struct();
		break;

	case CONSIGN:
		syslog(LOG_DEBUG, "ZW: In door lock callback");
		response.operation = requestdummy.operation;
		response.msgid = requestdummy.msgid;
		response.err_code = OP_SUCCESS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch = op_sts->mode;
		syslog(LOG_DEBUG, "ZW: Lock mode : %d \n", op_sts->mode);
		response_DM(response);
		memset(&requestdummy,0,sizeof(requestdummy));
		break;

		def: default:
		syslog(LOG_DEBUG, "ZW: In door lock callback");
		response.operation = NODE_STATUS;
		response.err_code = OP_SUCCESS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch = op_sts->mode;
		syslog(LOG_DEBUG, "ZW: Lock mode: %d", op_sts->mode);
		response_DM(response);
		break;
	}
}

/**
 hl_color_report_cb - color switch report callback function
 @param[in] zwifd_p   Interface handle
 @param[in] zwcolor_p   Color component data
 @param[in] time_t  Time stamp. If this is zero, the callback has no data and hence other parameter values should be ignored.
 */
void hl_color_report_cb(zwifd_p ifd, zwcolor_p data, time_t ts) {
	response_t response = { 0 };
	//static int color_id = 0;
	int result, i, s;
	zwepd_t epd;
	syslog(LOG_DEBUG, "ZW: color report called");

	response.home_id = homeid;
	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW: No end point found, returns error: %d", result);
	}

	if (ts == 0) {
		syslog(LOG_ERR, "ZW: Color switch report - no cached data");
		return;
	}

	//color_id = color_id >= NO_OF_COLOR_COMP ? 0 : color_id; /* color_id means Number of color components, if color_id greater than 5 than we are making it to zero*/

	if ((discovery_node_id != ifd->nodeid) && (requestdummy.operation != CONSIGN)) {
		syslog(LOG_DEBUG, "ZW:Inside discovery_node_id-color: %d", requestdummy.operation);
		goto def;
	}

	switch (requestdummy.operation) {
	case COMMISSIONING:
	case UPDATE:
		syslog(LOG_DEBUG, "ZW: Inside color callback COMMISSIONING");
		/*component_value[color_id] = data->value;
		color_id++;

		if ((color_id % NO_OF_COLOR_COMP) == 0) {

			for (i = 0; i < NO_OF_COLOR_COMP; i++) {
				discovery_struct.ep[ifd->epid].intf[command_class_find(
						ifd->epid, ifd->cls)].property_value.arr[i] =
						component_value[i];
			}
		}*/
		if (data->id == 0) {
			discovery_struct.ep[ifd->epid].intf[command_class_find(
					ifd->epid, ifd->cls)].property_value.arr[data->id] = 0;
		}
		else if (data->id == 1) {
			discovery_struct.ep[ifd->epid].intf[command_class_find(
					ifd->epid, ifd->cls)].property_value.arr[data->id] = 0;
		}
		else if (data->id == 2) {
			discovery_struct.ep[ifd->epid].intf[command_class_find(
					ifd->epid, ifd->cls)].property_value.arr[data->id] =
					215;
		}
		else if (data->id == 3) {
			discovery_struct.ep[ifd->epid].intf[command_class_find(
					ifd->epid, ifd->cls)].property_value.arr[data->id] =
					185;
		}
		else {
			discovery_struct.ep[ifd->epid].intf[command_class_find(
					ifd->epid, ifd->cls)].property_value.arr[data->id] = 15;
		}
		print_discovery_struct();
		break;
		def: default:
		syslog(LOG_DEBUG, "ZW: Color default");
		break;
	}
}

/**
 hl_level_rpt_cb - Report callback to get level information.
 @param[in] ifd     Interface handle
 @param[in] level   Level
 @param[in] ts    Time stamp. If this is zero, the callback has no data and hence other
 parameter values should be ignored.
 */
void hl_level_rpt_cb(zwifd_p ifd, zwlevel_dat_p val, time_t ts) {
	syslog(LOG_DEBUG, "ZW: level report called");
	response_t response = { 0 };
	response.home_id = homeid;
	int result;
	zwepd_t epd;

	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW:no end point found for hl_level_rpt_cb returns error: %d", result);
	}

	if (ts == 0) {
		syslog(LOG_ERR, "ZW: level report: no cached data ");
		return;
	}

	if ((discovery_node_id != ifd->nodeid) && (requestdummy.operation != CONSIGN)) {
		syslog(LOG_DEBUG, "ZW: Inside discovery_node_id-level: %d", requestdummy.operation);
		goto def;
	}

	syslog(LOG_DEBUG, "ZW:REQUEST OPERATION is : %d", requestdummy.operation);
	switch (requestdummy.operation) {
	case COMMISSIONING:
	case UPDATE:
		syslog(LOG_DEBUG, "ZW: Level state: %d", val->curr_val);
		discovery_struct.ep[ifd->epid].ep_id = ifd->epid;
		discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
				ifd->cls)].property_value.ch = val->curr_val;
		print_discovery_struct();
		break;

	case CONSIGN:
		syslog(LOG_DEBUG, "ZW: Inside level switch callback");
		response.operation = requestdummy.operation;
		response.msgid = requestdummy.msgid;
		response.err_code = OP_SUCCESS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch = val->curr_val;
		syslog(LOG_DEBUG, "ZW: Level state-CONSIGN: %d",val->curr_val);
		response_DM(response);
		memset(&requestdummy,0,sizeof(requestdummy));
		break;

	
		def: default:
		response.operation = NODE_STATUS;
		response.err_code = OP_SUCCESS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch = val->curr_val;
		syslog(LOG_DEBUG, "ZW: Level state-Observe: %d", val->curr_val);
		response_DM(response);
		break;
	}
}

/**
 hl_rep_alrm_fn_cb - 	Report callback to get alarm information.
 @param[in] ifd     	Interface handle
 @param[in] alarm_info   Alarm Events information
 @param[in] ts    	Time stamp. If this is zero, the callback has no data and hence other
 parameter values should be ignored.
 */

void hl_rep_alrm_fn_cb(zwifd_p ifd, zwalrm_p alarm_info, time_t ts) {
	response_t response = { 0 };
	int result;
	zwepd_t epd;
	syslog(LOG_DEBUG, "ZW: Alarm report called");

	response.home_id = homeid;
	result = zwif_get_ep(ifd, &epd);
	if (result != 0) {
		syslog(LOG_ERR, "ZW: No end point found for hl_rep_alrm_fn_cb returns error: %d", result);
	}

	if (ts == 0) {
		syslog(LOG_ERR, "ZW: Alarm report: no cached data");
		//return;
	}

	if ((discovery_node_id != ifd->nodeid) && (requestdummy.operation != CONSIGN)) {
		syslog(LOG_DEBUG, "ZW: Inside discovery_node_id-n Alarm: %d", requestdummy.operation);
		goto def;
	}
	switch (requestdummy.operation) {
	case COMMISSIONING:
	case UPDATE:
		syslog(LOG_DEBUG, "ZW: Alarm event COMMISSIONING123: %d",
				alarm_info->ex_event);

		discovery_struct.ep[ifd->epid].ep_id = ifd->epid;
		discovery_struct.ep[ifd->epid].intf[command_class_find(ifd->epid,
				ifd->cls)].property_value.ch = 0;
		print_discovery_struct();
		break;
		def: default:
		response.operation = NODE_STATUS;
		response.err_code = OP_SUCCESS;
		response.node_id = ifd->nodeid;
		response.no_of_ep = 1;
		response.ep[EP_INDEX].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = ifd->epid;
		response.ep[EP_INDEX].generic_dc = epd.generic;
		response.ep[EP_INDEX].specific_dc = epd.specific;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = ifd->cls;
		response.ep[EP_INDEX].intf[CC_INDEX].no_of_types = 1;
		response.ep[EP_INDEX].intf[CC_INDEX].EVENT[0].type =
				alarm_info->ex_type;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = CHAR_TYPE;
		response.ep[EP_INDEX].intf[CC_INDEX].property_value.ch =
				alarm_info->ex_event;
		syslog(LOG_DEBUG, "ZW: Alarm event default: %d", alarm_info->ex_event);
		response_DM(response);
		//bzero(&response, sizeof(response_DM));
		memset(&response,0,sizeof(response_DM));
		break;
	}
}

/**
 hl_color_sup_report_cb - Report callback to get supported barrier color components.
 @param[in] zwifd_p    Interface handle
 @param[in] comp_cnt   Number of color components in comp_id array
 @param[in] comp_id    color component id (see COL_SW_COMP_ID_XXX ) array
 @param[in] valid      Validity of the report. If 1 the report is valid; else this report contains no data and should be ignored
 */
static void hl_color_sup_report_cb(zwifd_p ifd, uint8_t comp_cnt,
		uint8_t *comp_id, int valid) {
	int i;
	if (!valid) {
		syslog(LOG_DEBUG, "ZW: No cache for supported color components");
		return;
	}

	if (comp_cnt > 0) {
		syslog(LOG_DEBUG, "ZW: Color component count is: %d", comp_cnt);
		cnt = comp_cnt;

		syslog(LOG_DEBUG, "ZW: Supported color component types:");
		for (i = 0; i < comp_cnt; i++) {
			syslog(LOG_DEBUG, "ZW: Color component id: %d", comp_id[i]);
			component_id[i] = comp_id[i];
		}
	}
}

/**
 get_value - This function sets the callbacks and is used to get the state of the devices using the Command Class. (Get Operation)
 @param[in] intf   Interface descriptor, this structure contains Command Class information.
 */
void get_value(zwifd_t *intf) {
	uint16_t command_class;
	int result;
	char i = 0;

	command_class = intf->cls;

	switch (command_class) {
	case COMMAND_CLASS_SWITCH_MULTILEVEL:
		result = zwif_level_rpt_set(intf, hl_level_rpt_cb);

		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_level_report_set returns error: %d ",
					result);

		sleep(1);
		result = zwif_level_get(intf, ZWIF_GET_BMSK_CACHE);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_level_report_get returns error: %d",
					result);

		break;

	case COMMAND_CLASS_SWITCH_BINARY:

		result = zwif_switch_rpt_set(intf, hl_switch_report_cb);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_switch_report_set returns error: %d",
					result);

		sleep(1);
		result = zwif_switch_get(intf, ZWIF_GET_BMSK_CACHE);
		syslog(LOG_DEBUG, "ZW: zwif_switch_get is: %d", result);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_switch_report_get returns error: %d",
					result);

		break;

	case COMMAND_CLASS_SWITCH_COLOR:
		result = zwif_color_sw_rpt_set(intf, hl_color_report_cb);

		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_color_switch_report_set returns error: %d",
					result);

		sleep(1);
		for (i = 0; i < NO_OF_COLOR_COMP; i++) {
			result = zwif_color_sw_get(intf, i, ZWIF_GET_BMSK_CACHE);
			if (result != 0)
				syslog(LOG_ERR,
						"ZW: zwif_color_switch_report_get returns error: %d",
						result);
		}
		break;

	case COMMAND_CLASS_DOOR_LOCK:

		result = zwif_dlck_op_rpt_set(intf, hl_dlck_op_rpt_cb);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_door_lock_set  returns error: %d",
					result);

		sleep(1);
		result = zwif_dlck_op_get(intf, ZWIF_GET_BMSK_CACHE);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_dlck_op_get return error: %d",
					result);

		break;

	case COMMAND_CLASS_ALARM:

		result = zwif_alrm_rpt_set(intf, hl_rep_alrm_fn_cb);
		syslog(LOG_DEBUG, "ZW: zwif_alarm_rpt_set : %d", result);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_alarm_rpt_set returns error: %d",
					result);

		sleep(1);
		result = zwif_alrm_sup_get(intf, hl_alrm_sup_cb, 0);
		syslog(LOG_DEBUG, "ZW: Result is zwif_alarm_sup_get: %d", result);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: Result is zwif_alarm_sup_get returns error:");
		result = zwif_alrm_get(intf, 0, ZW_ALRM_LOCK, ZW_ALRM_EVT_WINDOW_DOOR_OPEN, ZWIF_GET_BMSK_CACHE | ZWIF_GET_BMSK_LIVE );
		syslog(LOG_DEBUG, "ZW: Result is zwif_alrm_get: %d", result);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: Result is zwif_alrm_get returns error:");
		break;

	case COMMAND_CLASS_SENSOR_MULTILEVEL:
		result = zwif_sensor_rpt_set(intf, hl_snsr_rep_cb); /*FOR SESNORS*/
		syslog(LOG_DEBUG, "ZW: zwif_sensor_multilevel_report_set:  %d", result);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_sensor_multilevel_report_set returns error:");

		sleep(1);
		result = zwif_sensor_sup_get(intf, hl_sensor_sup_get_cb, 0);
		syslog(LOG_DEBUG, "ZW: Result is zwif_sensor_sup_get %d", result);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_sensor_multilevel_report_set returns error:");
		result=zwif_sensor_get(intf,ZW_SENSOR_TYPE_TEMP, ZW_SENSOR_UNIT_TEMP_CEL, ZWIF_GET_BMSK_CACHE);
		syslog(LOG_DEBUG, "ZW: Result is zwif_sensor_get %d", result);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_sensor_get returns error:");
		break;

	case COMMAND_CLASS_BATTERY:

		result = zwif_battery_rpt_set(intf, hl_battery_rpt_fn_cb);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_battery_rpt_set return error: %d",
					result);

		sleep(1);
		result = zwif_battery_get(intf,
				ZWIF_GET_BMSK_CACHE | ZWIF_GET_BMSK_LIVE);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_battery_get return error:");

		break;
	}
}

/**
 dm_value - This function sets the callbacks and is used to get the state of the devices using the Command Class. (Get Operation)
 @param[in] intf   Interface descriptor, this structure contains Command Class information.
 */
void dm_value(zwifd_t *intf) {
	uint16_t command_class;
	command_class = intf->cls;
	int result;
	int i = 0;

	syslog(LOG_DEBUG, "ZW: Inside get value");

	switch (command_class) {
	case COMMAND_CLASS_SWITCH_MULTILEVEL:
		result = zwif_level_rpt_set(intf, hl_level_rpt_cb);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_level_report_set return error: %d",
					result);

		sleep(1);
		result = zwif_level_get(intf, ZWIF_GET_BMSK_CACHE | ZWIF_GET_BMSK_LIVE);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_level_report_get returns error: %d",
					result);

		break;

	case COMMAND_CLASS_SWITCH_BINARY:
		result = zwif_switch_rpt_set(intf, hl_switch_report_cb);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_switch_report_set returns error: %d",
					result);

		result = zwif_switch_get(intf,
				ZWIF_GET_BMSK_CACHE | ZWIF_GET_BMSK_LIVE);
		syslog(LOG_DEBUG, "ZW: zwif_switch_get is: %d", result);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_switch_report_get returns error: %d",
					result);

		break;

	case COMMAND_CLASS_SWITCH_COLOR:
		result = zwif_color_sw_rpt_set(intf, hl_color_report_cb);

		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_color_switch_report_set returns error: %d",
					result);

		sleep(1);
		for (i = 0; i < NO_OF_COLOR_COMP; i++) {
			result = zwif_color_sw_get(intf, i,
					ZWIF_GET_BMSK_CACHE | ZWIF_GET_BMSK_LIVE);
			if (result != 0)
				syslog(LOG_ERR,
						"ZW: zwif_color_switch_report_get returns error: %d",
						result);

		}
		break;

	case COMMAND_CLASS_DOOR_LOCK:
		result = zwif_dlck_op_rpt_set(intf, hl_dlck_op_rpt_cb);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_door_lock_set return error: %d",
					result);

		sleep(1);
		result = zwif_dlck_op_get(intf,
				ZWIF_GET_BMSK_CACHE | ZWIF_GET_BMSK_LIVE);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_dlck_op_get return error: %d",
					result);

		break;

	case COMMAND_CLASS_ALARM:
		result = zwif_alrm_rpt_set(intf, hl_rep_alrm_fn_cb);
		syslog(LOG_DEBUG, "ZW: zwif_alarm_rpt_set : %d\n", result);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_alarm_rpt_set returns error : %d",
					result);

		break;

	case COMMAND_CLASS_SENSOR_MULTILEVEL:
		result = zwif_sensor_rpt_set(intf, hl_snsr_rep_cb); //FOR SESNORS
		syslog(LOG_DEBUG, "ZW: zwif_sensor_multilevel_report_set: %d",
				result);
		if (result != 0)
			syslog(LOG_ERR,
					"ZW: zwif_sensor_multilevel_report_set return error: %d",
					result);

		break;

	case COMMAND_CLASS_BATTERY:
		result = zwif_battery_rpt_set(intf, hl_battery_rpt_fn_cb);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_battery_rpt_set returns error: %d",
					result);

		sleep(1);
		result = zwif_battery_get(intf,
				ZWIF_GET_BMSK_CACHE | ZWIF_GET_BMSK_LIVE);
		if (result != 0)
			syslog(LOG_ERR, "ZW: zwif_battery_get return error: %d",
					result);
		break;

	}
}

/**
*
Refresh function is used to get the present status of the node. However, this feature is not available for end to end operation.
Hence, commenting this function for time being. 
*
*/
/**
 refresh_node - This function is used to perform GET operation on devices using Command Class. (Get Operation)
 */
 /*
void refresh_node() {
	color_refresh_count = 0;
	int result;
	zwifd_t intf_d;
	response_t response = { 0 };
	response.home_id = homeid;
	response.operation = REFRESH;
	result = interface_get(appl_ctx.zwnet, request.node_id, request.ep_id,
			request.cmd_cls, &intf_d);

	if (result == 0) {
		switch (request.cmd_cls) {
		case COMMAND_CLASS_SWITCH_BINARY:
			syslog(LOG_DEBUG, "ZW: success switch binary");
			result = zwif_switch_rpt_set(&intf_d, hl_switch_report_cb);
			syslog(LOG_DEBUG, "ZW: zwif_switch_rpt_set result is: %d",
					result);
			if (result == 0) {
				result = zwif_switch_get(&intf_d, ZWIF_GET_BMSK_CACHE);
				syslog(LOG_DEBUG, "ZW: zwif_switch_get result is: %d",
						result);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_switch_get  returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
			} else {
				syslog(LOG_ERR,
						"ZW: zwif_switch_report_set returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;

				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response_DM(response);
				bzero(&response, sizeof(response));
			}
			break;

		case COMMAND_CLASS_SWITCH_MULTILEVEL:
			syslog(LOG_DEBUG, "ZW: success switch multilevel");
			result = zwif_level_rpt_set(&intf_d, hl_level_rpt_cb);
			if (result == 0) {
				result = zwif_level_get(&intf_d, ZWIF_GET_BMSK_CACHE);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_level_get returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
			} else {
				syslog(LOG_ERR,
						"ZW: zwif_level_report_set returns error: %d \n",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response_DM(response);
				bzero(&response, sizeof(response));
			}
			break;

		case COMMAND_CLASS_SWITCH_COLOR:
			syslog(LOG_DEBUG, "ZW: success switch color");
			result = zwif_color_sw_rpt_set(&intf_d, hl_color_report_cb);
			if (result == 0) {
				for (color_refresh_count = 0;
						color_refresh_count < NO_OF_COLOR_COMP;
						color_refresh_count++) {
					syslog(LOG_DEBUG, "ZW: index of color_refresh_count: %d",
							color_refresh_count);
					result = zwif_color_sw_get(&intf_d, color_refresh_count,
							ZWIF_GET_BMSK_CACHE);
					if (result != 0) {
						syslog(LOG_ERR,
								"ZW: zwif_color_switch_report_get returns error: %d",
								result);
						response.err_code = result;
						response.msgid = request.msgid;
						response.node_id = request.node_id;
						response.ep[EP_INDEX].ep_id = request.ep_id;
						response.ep[EP_INDEX].generic_dc = request.generic_dc;
						response.ep[EP_INDEX].specific_dc = request.specific_dc;
						response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
								request.cmd_cls;
					}
				}
			} else {
				syslog(LOG_ERR,
						"ZW: color switch report set returns error");
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;

				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response_DM(response);
				bzero(&response, sizeof(response));
			}
			break;

		case COMMAND_CLASS_DOOR_LOCK:
			result = zwif_dlck_op_rpt_set(&intf_d, hl_dlck_op_rpt_cb);
			if (result == 0) {
				result = zwif_dlck_op_get(&intf_d, ZWIF_GET_BMSK_CACHE);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_door_lock_set returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
			} else {
				syslog(LOG_ERR,
						"ZW: door lock report set return error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response_DM(response);
				bzero(&response, sizeof(response));
			}
			break;

		case COMMAND_CLASS_ALARM:
			result = zwif_alrm_rpt_set(&intf_d, hl_rep_alrm_fn_cb);
			syslog(LOG_DEBUG, "ZW: Result is zwif_alrm_rpt_set %d", result);
			if (result != 0) {
				syslog(LOG_ERR,
						"ZW: zwif_alrm_rpt_set returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;

				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response_DM(response);
				bzero(&response, sizeof(response));
			}
			switch (request.type) {
			case ZW_ALRM_SMOKE:
				result = zwif_alrm_get(&intf_d, 0, ZW_ALRM_SMOKE,
						ZW_ALRM_EVT_CO, ZWIF_GET_BMSK_CACHE);
				syslog(LOG_DEBUG, "ZW: zwif_alrm_get result is: %d", result);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_alrm_get returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
				break;

			case ZW_ALRM_GAS:
				result = zwif_alrm_get(&intf_d, 0, ZW_ALRM_GAS,
						ZW_ALRM_EVT_COMBUST_GAS_DET, ZWIF_GET_BMSK_CACHE);
				syslog(LOG_DEBUG, "ZW: zwif_alrm_get Result is: %d", result);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_alrm_get returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
				break;

			case ZW_ALRM_LOCK:
				result = zwif_alrm_get(&intf_d, 0, ZW_ALRM_LOCK,
						ZW_ALRM_EVT_WINDOW_DOOR_OPEN, ZWIF_GET_BMSK_CACHE);
				syslog(LOG_DEBUG, "ZW: zwif_alrm_get result is: %d", result);

				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_alrm_get returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
				break;

			default:
				result = zwif_alrm_sup_get(&intf_d, hl_alrm_sup_cb, 0);
				syslog(LOG_DEBUG, "ZW: zwif_alrm_sup_get: %d", result);
				break;
			}
			break;
		case COMMAND_CLASS_SENSOR_MULTILEVEL:
			result = zwif_sensor_rpt_set(&intf_d, hl_snsr_rep_cb); //FOR SESNORS
			syslog(LOG_DEBUG, "ZW: Result is zwif_sensor_rpt_set: %d", result);

			if (result != 0) {
				syslog(LOG_ERR,
						"ZW: zwif_sensor_rpt_set returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response_DM(response);
				bzero(&response, sizeof(response));
			}

			switch (request.type) {
			case ZW_SENSOR_TYPE_TEMP:
				result = zwif_sensor_get(&intf_d, ZW_SENSOR_TYPE_TEMP,
						ZW_SENSOR_UNIT_TEMP_CEL, ZWIF_GET_BMSK_CACHE);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_sensor_get returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
				break;

			case ZW_SENSOR_TYPE_LUM:
				result = zwif_sensor_get(&intf_d, ZW_SENSOR_TYPE_LUM,
						ZW_SENSOR_UNIT_LUM_PERC, ZWIF_GET_BMSK_CACHE);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_sensor_get  returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
				break;

			case ZW_SENSOR_TYPE_HUMD:
				result = zwif_sensor_get(&intf_d, ZW_SENSOR_TYPE_HUMD,
						ZW_SENSOR_UNIT_HUMD_PERC, ZWIF_GET_BMSK_CACHE);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_sensor_get returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
				break;
			default:
				result = zwif_sensor_sup_get(&intf_d, hl_sensor_sup_get_cb, 0);
				syslog(LOG_DEBUG, "ZW: Result is zwif_sensor_sup_get: %d",
						result);
				break;
			}
			break;

		case COMMAND_CLASS_BATTERY:
			syslog(LOG_DEBUG, "ZW: success battery");
			result = zwif_battery_rpt_set(&intf_d, hl_battery_rpt_fn_cb);
			if (result == 0) {
				result = zwif_battery_get(&intf_d, ZWIF_GET_BMSK_CACHE);

				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_battery_get returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response_DM(response);
					bzero(&response, sizeof(response));
				}
			} else {
				syslog(LOG_ERR,
						"ZW: Battery report set returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response_DM(response);
				bzero(&response, sizeof(response));
			}
			break;

		default:
			syslog(LOG_ERR, "ZW: Invalid request cmd class: %d",
					request.cmd_cls);
			response.err_code = ERR_INVALID_CMD_CLS;
			response.msgid = request.msgid;
			response.node_id = request.node_id;
			response.ep[EP_INDEX].ep_id = request.ep_id;
			response.ep[EP_INDEX].generic_dc = request.generic_dc;
			response.ep[EP_INDEX].specific_dc = request.specific_dc;
			response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
			response_DM(response);
			bzero(&response, sizeof(response));
		}
	} else {
		syslog(LOG_ERR, "ZW: Searching for interface returns error: %d",
				result);
		response.err_code = result;
		response.msgid = request.msgid;
		response.node_id = request.node_id;
		response.ep[EP_INDEX].ep_id = request.ep_id;
		response.ep[EP_INDEX].generic_dc = request.generic_dc;
		response.ep[EP_INDEX].specific_dc = request.specific_dc;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
		response_DM(response);
		bzero(&response, sizeof(response));
	}
}
*/
/**
 control_node - Control a node in the  network using Command Class. (Set Operation)
 */
void control_node() {
	int result;
	char choice;

	zwifd_t intf_d;
	response_t response = { 0 };
	response.home_id = homeid;
	response.operation = CONSIGN;
	syslog(LOG_DEBUG, "ZW: contrl node:");
	result = interface_get(appl_ctx.zwnet, request.node_id, request.ep_id,
			request.cmd_cls, &intf_d);
	syslog(LOG_DEBUG, "ZW: contrl node123:");
	if (result == 0) {
		switch (request.cmd_cls) {
		case COMMAND_CLASS_SWITCH_BINARY:
			result = zwif_switch_rpt_set(&intf_d, hl_switch_report_cb);
			if (result != 0) {
				syslog(LOG_ERR, "ZW: zwif_switch_rpt_set return error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}

			result = zwif_switch_set(&intf_d, request.property_value.ch,NULL,NULL);
			syslog(LOG_DEBUG, "ZW: Result of zwif_switch_set: %d", result);
			if (result != 0) {
				syslog(LOG_ERR, "ZW: zwif_switch_set returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}
			break;

		case COMMAND_CLASS_SWITCH_MULTILEVEL:
			if (request.specific_dc == 0x01 || request.specific_dc == 0x04) {
				result = zwif_level_rpt_set(&intf_d, hl_level_rpt_cb);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_level_report_set returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.no_of_ep = 1;
					response.ep[0].no_of_intf = 1;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
					response_DM(response);
					//bzero(&response, sizeof(response));
					memset(&request,0,sizeof(request));
				}
				result = zwif_level_set(&intf_d, request.property_value.ch, 0,
						NULL, NULL);
				syslog(LOG_DEBUG, "ZW: Result of zwif_level_set: %d", result);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_level_set returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.no_of_ep = 1;
					response.ep[0].no_of_intf = 1;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
					response_DM(response);
					//bzero(&response, sizeof(response));
					memset(&request,0,sizeof(request));
				}
			} else if (request.specific_dc == 0x07) {
				if (request.property_value.ch == 99) {
					result = zwif_level_rpt_set(&intf_d, hl_level_rpt_cb);
					if (result != 0) {
						syslog(LOG_ERR,
								"ZW: zwif_motor_control_set returns error: %d",
								result);
						response.err_code = result;
						response.msgid = request.msgid;
						response.node_id = request.node_id;
						response.no_of_ep = 1;
						response.ep[0].no_of_intf = 1;
						response.ep[EP_INDEX].ep_id = request.ep_id;
						response.ep[EP_INDEX].generic_dc = request.generic_dc;
						response.ep[EP_INDEX].specific_dc = request.specific_dc;
						response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
								request.cmd_cls;
						response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
						response_DM(response);
						//bzero(&response, sizeof(response));
						memset(&request,0,sizeof(request));
					}
					result = zwif_level_stop(&intf_d);
					syslog(LOG_DEBUG, "ZW: zwif_level_stop result is: %d\n",
							result);
					if (result != 0) {
						syslog(LOG_ERR,
								"ZW: zwif_motor_control_set returns error: %d",
								result);
						response.err_code = result;
						response.msgid = request.msgid;
						response.node_id = request.node_id;
						response.no_of_ep = 1;
						response.ep[0].no_of_intf = 1;
						response.ep[EP_INDEX].ep_id = request.ep_id;
						response.ep[EP_INDEX].generic_dc = request.generic_dc;
						response.ep[EP_INDEX].specific_dc = request.specific_dc;
						response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
								request.cmd_cls;
						response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
						response_DM(response);
						//bzero(&response, sizeof(response));
						memset(&request,0,sizeof(request));
					}
				} else {
					lev.pri_level = 20;
					lev.pri_ignore_lvl = 0;
					lev.sec_dir = 3;
					lev.sec_step = 0xFF;
					lev.dur = 0;
					lev.pri_dir = request.property_value.ch;
					result = zwif_level_rpt_set(&intf_d, hl_level_rpt_cb);
					syslog(LOG_DEBUG, "ZW: Level_rpt_set result is: %d",
							result);
					if (result != 0) {
						syslog(LOG_ERR,
								"ZW: zwif_motor_controller_set returns error: %d",
								result);
						response.err_code = result;
						response.msgid = request.msgid;
						response.node_id = request.node_id;
						response.no_of_ep = 1;
						response.ep[0].no_of_intf = 1;
						response.ep[EP_INDEX].ep_id = request.ep_id;
						response.ep[EP_INDEX].generic_dc = request.generic_dc;
						response.ep[EP_INDEX].specific_dc = request.specific_dc;
						response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
								request.cmd_cls;
						response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
						response_DM(response);
						//bzero(&response, sizeof(response));
						memset(&request,0,sizeof(request));
					}

					result = zwif_level_start(&intf_d, &lev);
					syslog(LOG_DEBUG, "ZW: zwif_level_start result is: %d",
							result);
					if (result != 0) {
						syslog(LOG_ERR,
								"ZW: zwif_motor_controller_set returns error: %d",
								result);
						response.err_code = result;
						response.msgid = request.msgid;
						response.node_id = request.node_id;
						response.no_of_ep = 1;
						response.ep[0].no_of_intf = 1;
						response.ep[EP_INDEX].ep_id = request.ep_id;
						response.ep[EP_INDEX].generic_dc = request.generic_dc;
						response.ep[EP_INDEX].specific_dc = request.specific_dc;
						response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
								request.cmd_cls;
						response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
						response_DM(response);
						//bzero(&response, sizeof(response));
						memset(&request,0,sizeof(request));
					}
				}
			}
			break;

		case COMMAND_CLASS_SWITCH_COLOR:
			result = zwif_color_sw_rpt_set(&intf_d, hl_color_report_cb);
			if (result == 0) {
				result = zwif_color_sw_sup_get(&intf_d, hl_color_sup_report_cb,
						1);
				sleep(1);
				syslog(LOG_DEBUG, "ZW: result zwif_color_sw_sup_get: %d",
						result);
				result = zwif_color_sw_set(&intf_d, cnt, component_id,
						request.property_value.arr, 0);
				syslog(LOG_DEBUG, "ZW: result zwif_color_sw_set: %d",
						result);
				if (result != 0) {
					syslog(LOG_ERR,
							"ZW: zwif_color_set returns error: %d",
							result);
					response.err_code = result;
					response.msgid = request.msgid;
					response.node_id = request.node_id;
					response.no_of_ep = 1;
					response.ep[0].no_of_intf = 1;
					response.ep[EP_INDEX].ep_id = request.ep_id;
					response.ep[EP_INDEX].generic_dc = request.generic_dc;
					response.ep[EP_INDEX].specific_dc = request.specific_dc;
					response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls =
							request.cmd_cls;
					response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
					response_DM(response);
					//bzero(&response, sizeof(response));
					memset(&request,0,sizeof(request));
				}
			} else {
				syslog(LOG_ERR, "ZW: color report set returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}
			break;

		case COMMAND_CLASS_DOOR_LOCK:
			result = zwif_dlck_op_rpt_set(&intf_d, hl_dlck_op_rpt_cb);
			syslog(LOG_DEBUG, "ZW: result of  zwif_dlck_op_rpt_set: %d",
					result);
			if (result != 0) {
				syslog(LOG_ERR,
						"ZW: zwif_door_lock_set return error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}
			result = zwif_dlck_op_set(&intf_d, request.property_value.ch, NULL,
					NULL);
			syslog(LOG_DEBUG, "ZW: result of zwif_dlck_op_set: %d", result);
			if (result != 0) {
				syslog(LOG_ERR,
						"ZW: zwif_door_lock_set return error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}
			break;

		case COMMAND_CLASS_ALARM:
			result = zwif_alrm_rpt_set(&intf_d, hl_rep_alrm_fn_cb);

			if (result != 0) {
				syslog(LOG_ERR,
						"ZW: zwif_alrm_rpt_set returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}
			result = zwif_alrm_get(&intf_d, 0, ZW_ALRM_GAS,
					ZW_ALRM_EVT_COMBUST_GAS_DET, ZWIF_GET_BMSK_CACHE);
			syslog(LOG_DEBUG, "ZW: zwif_alrm_get result is %d", result);

			if (result != 0) {
				syslog(LOG_ERR, "ZW: zwif_alrm_get returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}

			result = zwif_alrm_get(&intf_d, 0, ZW_ALRM_LOCK,
					ZW_ALRM_EVT_WINDOW_DOOR_OPEN, ZWIF_GET_BMSK_CACHE);
			syslog(LOG_DEBUG, "ZW: zwif_alrm_get result is: %d", result);

			if (result != 0) {
				syslog(LOG_ERR, "ZW: zwif_alrm_get returns error: %d",
						result);
				response.err_code = result;
				response.msgid = request.msgid;
				response.node_id = request.node_id;
				response.no_of_ep = 1;
				response.ep[0].no_of_intf = 1;
				response.ep[EP_INDEX].ep_id = request.ep_id;
				response.ep[EP_INDEX].generic_dc = request.generic_dc;
				response.ep[EP_INDEX].specific_dc = request.specific_dc;
				response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
				response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
				response_DM(response);
				//bzero(&response, sizeof(response));
				memset(&request,0,sizeof(request));
			}
			break;

		default:
			syslog(LOG_ERR, "ZW: Invalid command class: %d", choice);
			response.err_code = ERR_INVALID_CMD_CLS;
			response.msgid = request.msgid;
			response.node_id = request.node_id;
			response.ep[EP_INDEX].ep_id = request.ep_id;
			response.ep[EP_INDEX].generic_dc = request.generic_dc;
			response.ep[EP_INDEX].specific_dc = request.specific_dc;
			response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
			response_DM(response);
			bzero(&response, sizeof(response));
		}
	} else {
		syslog(LOG_ERR, "ZW: Searching for interface returns error: %d",
				result);
		response.err_code = result;
		response.msgid = request.msgid;
		response.node_id = request.node_id;
		response.no_of_ep = 1;
		response.ep[0].no_of_intf = 1;
		response.ep[EP_INDEX].ep_id = request.ep_id;
		response.ep[EP_INDEX].generic_dc = request.generic_dc;
		response.ep[EP_INDEX].specific_dc = request.specific_dc;
		response.ep[EP_INDEX].intf[CC_INDEX].cmd_cls = request.cmd_cls;
		response.ep[EP_INDEX].intf[CC_INDEX].property_type = request.property_type;
		response_DM(response);
		//bzero(&response, sizeof(response));
		memset(&request,0,sizeof(request));
	}
}

