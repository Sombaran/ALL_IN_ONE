#ifndef _CONTROL_
#define _CONTROL_


#include "common.h"

/**
control_node - Controls a node in the  network using Command Class. (Set Operation)
*/
extern void control_node();

/**
hl_color_report_cb - color switch report callback function
@param[in] zwifd_p   Interface handle
@param[in] zwcolor_p   Color component data
@param[in] time_t  Time stamp. If this is zero, the callback has no data and hence other parameter values should be ignored.
*/
extern void hl_color_report_cb(zwifd_p ifd, zwcolor_p data, time_t ts);

/**
hl_level_rpt_cb - Report callback to get switch type information.
@param[in] ifd     Interface handle
@param[in] level   Level
@param[in] ts    Time stamp. If this is zero, the callback has no data and hence other
parameter values should be ignored.
*/
extern void hl_level_rpt_cb(zwifd_p ifd, uint8_t level, time_t ts);

#endif
