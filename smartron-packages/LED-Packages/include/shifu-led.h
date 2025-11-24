
typedef enum {
    LED_OFF              	=    0,	     ///< LED OFF
    LED_VOLUME           	=    (1 << 0),     ///< Volume , (volume up and down same priority)
    LED_NO_INTERNET_CHECK_NOTI           	=    (1 << 1),     ///< LED_NO_INTERNET_CHECK_NOTIFY
    LED_ERROR			=    (1 << 2),     ///< Error
    LED_WAKEWORD         	=    (1 << 3),     ///< Wake word
    LED_ALARM		 	=    (1 << 4),     ///< Alarm
    LED_QUERY_PROCESSING 	=    (1 << 5),     ///< Query processing
    LED_QUERY_RESPONSE   	=    (1 << 6),     ///< Query Response
    LED_DEVICE_CONNECTED 	=    (1 << 7),     ///< Device Discovery
    LED_DEVICE_DISCOVERY 	=    (1 << 8),     ///< Device Control
    LED_CONNECTED_AS_ROUTER	=    (1 << 9),     ///< Connected as Router
    LED_MIC_MUTE         	=    (1 << 10),    ///< Mic mute
    LED_SETUP_MODE		=    (1 << 11),    ///< device registration mode
    LED_VOICE_VOLUME        =    (1 << 12),    ///< Volume change through voice
    LED_MICMUTE_OFF         =    (1 << 13),    ///< Mic UnMute after Mute
    LED_BLUTOOTH_DISCOVERY  =    (1 << 14),    ///< Blutooth Discovery
    LED_BLUTOOTH_CONNECTED  =    (1 << 15),    ///< Blutooth Connected
    LED_NETWORK_RESET       =    (1 << 16),    ///< Network Reset
    LED_WPS_MODE            =    (1 << 17),     ///< WPS Mode
    LED_WAKEWORD_OFF        =    (1 << 18),     ///< LED_WAKEWORD_OFF
    LED_DND                 =    (1 << 19),     ///< LED_DND
    LED_NOTIFY              =    (1 << 20),      ///< LED_NOTIFY
    LED_ANNOUNCEMENT           =    (1 << 21),      ///< LED_ANNOUNCEMENT
	LED_AVS_CONFIG			=		(1 << 22)		///< LED AVS CONFIG
}PATTERN;

#define LED_BOOTING "i2cset -y 0 0x18 0x47 0xFD 0xE8 0x00 0x00 0x00 0x00 0x78 0x00 0xff 0x00 0x00 0x00 0x50 0x20 i"

#define SHIFU_LED_PATTERN_START 1
#define SHIFU_LED_PATTERN_STOP  0

#define SERVER_ADDRESS	"127.0.0.1"	///< server address
#define SERVER_PORT	7891		///< listening port

struct led_struct{
	int pattern;	///< LED_XXXX
	int state;	///< SHIFU_LED_PATTERN_XXX (0 for Off and 1 for On)
	int value;	///< only for hardware contron module(for volume), remaining all set to 1
};

int shifu_led_init();	///< API to initialize socket interface7
int shifu_led_pattern_send(int fd,struct led_struct *);	///< API to send pattern data
