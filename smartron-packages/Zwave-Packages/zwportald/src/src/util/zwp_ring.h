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
* @file    zwp_ring.h
*
* @brief   Ring (cyclic doubly linked list) implementation
* @details Utility functions for ring (cyclic doubly linked list) data structure.
*
* @author  Kumara
*
* @version 1.0 - 2014-03-11
* - Initial version
*/

#ifndef ZWP_RING_H_
#define ZWP_RING_H_

#include <apr_ring.h>

#include "zwp_apr.h"
#include "zwp_status.h"

/**
 * <b> ZWP_RING_ENTRY(elem) </b>
 *
 * The Ring Element
 *
 * A ring element struct is linked to the other elements in the ring
 * through its ring entry field, e.g.
 * <pre>
 *      struct my_element_t {
 *          ZWP_RING_ENTRY(my_element_t) link;
 *          int foo;
 *          char *bar;
 *      };
 * </pre>
 *
 * An element struct may be put on more than one ring if it has more
 * than one ZWP_RING_ENTRY field. Each ZWP_RING_ENTRY has a corresponding
 * ZWP_RING_HEAD declaration.
 *
 * @warning For strict C standards compliance you should put the ZWP_RING_ENTRY
 * first in the element struct unless the head is always part of a larger
 * object with enough earlier fields to accommodate the offsetof() used
 * to compute the ring sentinel below. You can usually ignore this caveat.
 */
#define ZWP_RING_ENTRY            APR_RING_ENTRY

/**
 * <b> ZWP_RING_HEAD(head, elem) </b>
 *
 * The Ring Head
 *
 * Each ring is managed via its head, which is a struct declared like this:
 * <pre>
 *      ZWP_RING_HEAD(my_ring_t, my_element_t);
 *      struct my_ring_t ring, *ringp;
 * </pre>
 *
 * This struct looks just like the element link struct so that we can
 * be sure that the typecasting games will work as expected.
 *
 * The first element in the ring is next after the head, and the last
 * element is just before the head.
 */
#define ZWP_RING_HEAD             APR_RING_HEAD

/**
 * <b> ZWP_RING_INIT(hp, elem, link) </b>
 *
 * Initialize a ring
 * @param         hp                The head of the ring
 * @param         elem              The name of the element struct
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 */
#define ZWP_RING_INIT             APR_RING_INIT

/**
 * <b> ZWP_RING_INSERT_HEAD(hp, nep, elem, link) </b>
 *
 * Insert the element nep into the ring before the first element
 *   (..hp.. becomes ..hp..nep..)
 * @param         hp                Head of the ring
 * @param         nep               Element to insert
 * @param         elem              The name of the element struct
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 */

#define ZWP_RING_INSERT_HEAD      APR_RING_INSERT_HEAD
/**
 * <b> ZWP_RING_INSERT_TAIL(hp, nep, elem, link) </b>
 *
 * Insert the element nep into the ring after the last element
 *   (..hp.. becomes ..nep..hp..)
 * @param         hp                Head of the ring
 * @param         nep               Element to insert
 * @param         elem              The name of the element struct
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 */

#define ZWP_RING_INSERT_TAIL      APR_RING_INSERT_TAIL
/**
 * <b> ZWP_RING_REMOVE(ep, link) </b>
 *
 * Remove a single element from a ring
 * @warning The unspliced element is left with dangling pointers at either end
 * @param         ep                Element to remove
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 */

#define ZWP_RING_REMOVE           APR_RING_REMOVE
/**
 * <b> ZWP_RING_UNSPLICE(ep1, epN, link) </b>
 *
 * Unsplice a sequence of elements from a ring
 * @warning The unspliced sequence is left with dangling pointers at either end
 * @param         ep1               First element in the sequence to unsplice
 * @param         epN               Last element in the sequence to unsplice
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 */
#define ZWP_RING_UNSPLICE         APR_RING_UNSPLICE

/**
 * <b> ZWP_RING_FIRST(hp) </b>
 *
 * The first element of the ring
 * @param         hp                The head of the ring
 */
#define ZWP_RING_FIRST            APR_RING_FIRST

/**
 * <b> ZWP_RING_NEXT(ep, link) </b>
 *
 * The next element in the ring
 * @param         ep                The current element
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 */
#define ZWP_RING_NEXT             APR_RING_NEXT

/**
 * <b> ZWP_RING_SENTINEL(hp, elem, link) </b>
 *
 * The Ring Sentinel
 *
 * This is the magic pointer value that occurs before the first and
 * after the last elements in the ring, computed from the address of
 * the ring's head.  The head itself isn't an element, but in order to
 * get rid of all the special cases when dealing with the ends of the
 * ring, we play typecasting games to make it look like one.
 *
 * Here is a diagram to illustrate the arrangements of the next and
 * prev pointers of each element in a single ring. Note that they point
 * to the start of each element, not to the ZWP_RING_ENTRY structure.
 *
 * <pre>
 *     +->+------+<-+  +->+------+<-+  +->+------+<-+
 *     |  |struct|  |  |  |struct|  |  |  |struct|  |
 *    /   | elem |   \/   | elem |   \/   | elem |  \
 * ...    |      |   /\   |      |   /\   |      |   ...
 *        +------+  |  |  +------+  |  |  +------+
 *   ...--|prev  |  |  +--|ring  |  |  +--|prev  |
 *        |  next|--+     | entry|--+     |  next|--...
 *        +------+        +------+        +------+
 *        | etc. |        | etc. |        | etc. |
 *        :      :        :      :        :      :
 * </pre>
 *
 * The ZWP_RING_HEAD is nothing but a bare ZWP_RING_ENTRY. The prev
 * and next pointers in the first and last elements don't actually
 * point to the head, they point to a phantom place called the
 * sentinel. Its value is such that last->next->next == first because
 * the offset from the sentinel to the head's next pointer is the same
 * as the offset from the start of an element to its next pointer.
 * This also works in the opposite direction.
 *
 * <pre>
 *        last                            first
 *     +->+------+<-+  +->sentinel<-+  +->+------+<-+
 *     |  |struct|  |  |            |  |  |struct|  |
 *    /   | elem |   \/              \/   | elem |  \
 * ...    |      |   /\              /\   |      |   ...
 *        +------+  |  |  +------+  |  |  +------+
 *   ...--|prev  |  |  +--|ring  |  |  +--|prev  |
 *        |  next|--+     |  head|--+     |  next|--...
 *        +------+        +------+        +------+
 *        | etc. |                        | etc. |
 *        :      :                        :      :
 * </pre>
 *
 * Note that the offset mentioned above is different for each kind of
 * ring that the element may be on, and each kind of ring has a unique
 * name for its ZWP_RING_ENTRY in each element, and has its own type
 * for its ZWP_RING_HEAD.
 *
 * Note also that if the offset is non-zero (which is required if an
 * element has more than one ZWP_RING_ENTRY), the unreality of the
 * sentinel may have bad implications on very perverse implementations
 * of C -- see the warning in ZWP_RING_ENTRY.
 *
 * @param         hp                The head of the ring
 * @param         elem              The name of the element struct
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 */
#define ZWP_RING_SENTINEL         APR_RING_SENTINEL

/**
 * <b> ZWP_RING_EMPTY(hp, elem, link) </b>
 *
 * Determine if a ring is empty
 * @param         hp                The head of the ring
 * @param         elem              The name of the element struct
 * @param         link              The name of the ZWP_RING_ENTRY in the element struct
 * @return         true or false
 */
#define ZWP_RING_EMPTY            APR_RING_EMPTY

#endif /* ZWP_RING_H_ */
