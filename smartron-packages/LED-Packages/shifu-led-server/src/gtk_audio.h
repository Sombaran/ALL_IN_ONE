#ifndef __GTK_AUDIO_H__
#define __GTK_AUDIO_H__

typedef enum {
	DSP_CX20921,
	DSP_CX20924,
	DSP_INVALID
} DSP_MODEL_t;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * Notes:
 *     In normal case, this API should not be used. The AGC function should
 *     be controlled by CX20921 firmware; turning this off may affect AEC
 *     performance.
 *
 *     The API is open for test purpose.
 */
int gtk_audio_set_mic_agc(int on);

/**
 * Notes:
 *     In normal case, this API should not be used. Inappropriate MIC gain
 *     control might affect AEC performance.
 *
 *     The API is open for test purpose.
 *
 * Levels:
 *    -96~48: -96 ~ 48 dB
 */
int gtk_audio_set_mic_db(int db);

/**
 * Notes:
 *     In normal case, this API should not be used. Inappropriate MIC gain
 *     control might affect AEC performance.
 *
 *     The API is open for test purpose.
 *
 * Levels:
 *        0: Mute (set to very low gain)
 *    4-100: -48 ~ 48 dB
 *    1-3  : set to -48 dB
 */
int gtk_audio_set_mic_vol(int vol);

/**
 * Notes:
 *     There will be the maximum gain allowed to make AEC performance good.
 *     This value is To-Be-Defined. Once it's updated, caling this API is
 *     fine but the volume should be limited to the To-Be-Defined value.
 *
 * Levels:
 *        0: Mute
 *    4-100: -96dB ~ 0dB
 *    1-3  : set to -96dB
 */
int gtk_audio_set_speaker_vol(int vol);

/**
 *	This API will set DAC to mute. CX20721.
 */
void gtk_audio_set_speaker_mute(int mute);


/* This API for cx2092x DOA function*/
unsigned int gtk_get_angle_of_arrival_4mic(unsigned int *p_volume);

/**
 * Notes:
 *	This API set diagonal mic distance to CX20924. If DSP chip is not CX20924, it does nothing and return.
 *
 * Input:
 *	Distance of diagonal mic distance of CX20924. Unit is mm.
 */
void gtk_audio_set_diagonal_mic_distance(int distance);

/**
 * Notes:
 *	This API return diagonal mic distance of CX20924.
 *
 * Return:
 *	Distance of diagonal mic distance of CX20924. Unit is mm.
 *	If DSP chip is not CX20924, it returns -1.
 */
int gtk_audio_get_diagonal_mic_distance(void);

#endif /* __GTK_AUDIO_H__ */

