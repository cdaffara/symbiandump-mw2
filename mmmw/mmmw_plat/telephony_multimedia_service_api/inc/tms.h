/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMS_H
#define TMS_H

// Include files
#include <glib.h>
#include <vector>

namespace TMS {

// FORWARD DECLARATION
class TMSFormat;

// Result codes
#define TMS_RESULT_SUCCESS                      ((gint)0x00000000)
#define TMS_RESULT_ALREADY_EXIST                ((gint)0x00000001)
#define TMS_RESULT_DOES_NOT_EXIST               ((gint)0x00000002)
#define TMS_RESULT_NULL_ARGUMENT                ((gint)0x00000003)
#define TMS_RESULT_INVALID_ARGUMENT             ((gint)0x00000004)
#define TMS_RESULT_INVALID_STATE                ((gint)0x00000005)
#define TMS_RESULT_UNINITIALIZED_OBJECT         ((gint)0x00000006)
#define TMS_RESULT_INSUFFICIENT_MEMORY          ((gint)0x00000007)
#define TMS_RESULT_GENERAL_ERROR                ((gint)0x00000008)
#define TMS_RESULT_FATAL_ERROR                  ((gint)0x00000009)
#define TMS_RESULT_OPERATION_CANCELLED          ((gint)0x0000000A)
#define TMS_RESULT_ILLEGAL_OPERATION            ((gint)0x0000000B)
#define TMS_RESULT_BUFFER_LATENCY_ERROR         ((gint)0x0000000C)
#define TMS_RESULT_CALL_TYPE_NOT_SUPPORTED      ((gint)0x0000000D)
#define TMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED    ((gint)0x0000000E)
#define TMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED    ((gint)0x0000000F)
#define TMS_RESULT_SINK_TYPE_NOT_SUPPORTED      ((gint)0x00000010)
#define TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED    ((gint)0x00000011)
#define TMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED    ((gint)0x00000012)
#define TMS_RESULT_EFFECT_TYPE_NOT_SUPPORTED    ((gint)0x00000013)
#define TMS_RESULT_VERSION_NOT_SUPPORTED        ((gint)0x00000014)
#define TMS_RESULT_FORMAT_TYPE_UNSPECIFIED      ((gint)0x00000015)
#define TMS_RESULT_FEATURE_NOT_SUPPORTED        ((gint)0x00000016)

// Reasons codes
#define TMS_REASON_CLIENT_INITIATED             ((gint)0x00000000)
#define TMS_REASON_DEVICE_IN_USE                ((gint)0x00000001) // Device not available
#define TMS_REASON_DEVICE_TAKEN                 ((gint)0x00000002) // Preemption
#define TMS_REASON_INSUFFICIENT_MEMORY          ((gint)0x00000003) // Memory
#define TMS_REASON_PERMISSION_DENIED            ((gint)0x00000004) // Client app does not have perission
#define TMS_REASON_EMERGENCY_CALL_ONGOING       ((gint)0x00000005) // Already ongoing emergency call
#define TMS_REASON_EMERGENCY_CALL_STARTED       ((gint)0x00000006) // Preemption because of emergency call
#define TMS_REASON_SOURCE_NULL                  ((gint)0x00000007) // Could be checked on client side
#define TMS_REASON_SOURCE_NOT_SUPPORTED         ((gint)0x00000008)
#define TMS_REASON_FORMAT_NULL                  ((gint)0x00000009) // Could be checked on client side
#define TMS_REASON_EFFECT_NOT_SUPPORTED         ((gint)0x0000000A)
#define TMS_REASON_SINK_NULL                    ((gint)0x0000000B) // Could be checked on client side
#define TMS_REASON_SINK_NOT_SUPPORTED           ((gint)0x0000000C)
#define TMS_REASON_CALL_NOT_ACTIVE              ((gint)0x0000000D)

// Event codes
#define TMS_EVENT_STREAM_STATE_CHANGED          ((gint)0x00000000)
#define TMS_EVENT_STREAM_STATE_CHANGE_ERROR     ((gint)0x00000001)
#define TMS_EVENT_SOURCE_FILL_BUFFER            ((gint)0x00000002)
#define TMS_EVENT_SOURCE_PROCESSED_BUFFER       ((gint)0x00000003)
#define TMS_EVENT_SINK_PROCESS_BUFFER           ((gint)0x00000004)
#define TMS_EVENT_EFFECT_VOL_CHANGED            ((gint)0x00000005)
#define TMS_EVENT_EFFECT_GAIN_CHANGED           ((gint)0x00000006)
#define TMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED ((gint)0x00000007)
#define TMS_EVENT_ROUTING_OUTPUT_CHANGED        ((gint)0x00000008)
#define TMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE   ((gint)0x00000009)
#define TMS_EVENT_RINGTONE_OPEN_COMPLETE        ((gint)0x0000000A)
#define TMS_EVENT_RINGTONE_PLAY_COMPLETE        ((gint)0x0000000B)
#define TMS_EVENT_RINGTONE_DEINIT_COMPLETE      ((gint)0x0000000C)
#define TMS_EVENT_DTMF_TONE_STARTED             ((gint)0x0000000D)
#define TMS_EVENT_DTMF_TONE_STOPPED             ((gint)0x0000000E)
#define TMS_EVENT_INBAND_TONE_STARTED           ((gint)0x0000000F)
#define TMS_EVENT_INBAND_TONE_STOPPED           ((gint)0x00000010)

// Stream states
typedef gint TMSStreamState;
#define TMS_STREAM_UNINITIALIZED                ((gint)0x00000000)
#define TMS_STREAM_INITIALIZED                  ((gint)0x00000001)
#define TMS_STREAM_PAUSED                       ((gint)0x00000002)
#define TMS_STREAM_STARTED                      ((gint)0x00000003)

#define DEFAULT_CALL_CONTEXT                    ((gint)1)

// TMS object types
typedef gint TMSStreamType;
#define TMS_STREAM_UPLINK                       ((gint)0)
#define TMS_STREAM_DOWNLINK                     ((gint)1)

typedef gint TMSBufferType;
#define TMS_BUFFER_MEMORY                       ((gint)2)

typedef gint TMSSourceType;
#define TMS_SOURCE_CLIENT                       ((gint)3)
#define TMS_SOURCE_MODEM                        ((gint)4)
#define TMS_SOURCE_MIC                          ((gint)5)

typedef gint TMSSinkType;
#define TMS_SINK_CLIENT                         ((gint)6)
#define TMS_SINK_MODEM                          ((gint)7)
#define TMS_SINK_SPEAKER                        ((gint)8)

typedef gint TMSEffectType;
#define TMS_EFFECT_GLOBAL_VOL                   ((gint)9)
#define TMS_EFFECT_GLOBAL_GAIN                  ((gint)10)
#define TMS_EFFECT_VOLUME                       ((gint)11)
#define TMS_EFFECT_GAIN                         ((gint)12)

typedef gint TMSFormatType;
#define TMS_FORMAT_PCM                          ((gint)13)
#define TMS_FORMAT_AMR                          ((gint)14)
#define TMS_FORMAT_G711                         ((gint)15)
#define TMS_FORMAT_G729                         ((gint)16)
#define TMS_FORMAT_ILBC                         ((gint)17)

typedef gint TMSRingToneType;
#define TMS_RINGTONE_DEFAULT                    ((gint)0)
#define TMS_RINGTONE_FILE                       ((gint)1)
#define TMS_RINGTONE_SEQUENCE                   ((gint)2)
#define TMS_RINGTONE_BEEP_ONCE                  ((gint)3)
#define TMS_RINGTONE_SILENT                     ((gint)4)
#define TMS_RINGTONE_UNSECURE_VOIP              ((gint)5)

typedef gint TMSRTPSessionType;
#define TMS_SESSION_RTP_PULL                    ((gint)0)
#define TMS_SESSION_RTP_PUSH                    ((gint)1)

// TMS vectors
typedef std::vector<TMSFormat*> FormatVector;
typedef std::vector<guint> BitRateVector;
typedef std::vector<guint> OutputVector;

// Call types
typedef gint TMSCallType;
#define TMS_CALL_CS                             ((gint)0)
#define TMS_CALL_ECS                            ((gint)1)
#define TMS_CALL_IP                             ((gint)2)
#define TMS_CALL_RTP                            ((gint)3)

// Codec modes
typedef gint TMSG711CodecMode;
#define TMS_G711_CODEC_MODE_ALAW                ((gint)0)
#define TMS_G711_CODEC_MODE_MULAW               ((gint)1)

typedef gint TMSILBCCodecMode;
#define TMS_ILBC_CODEC_MODE_20MS_FRAME          ((gint)0)
#define TMS_ILBC_CODEC_MODE_30MS_FRAME          ((gint)1)

// Audio output types
typedef gint TMSAudioOutput;
#define TMS_AUDIO_OUTPUT_NONE                   ((gint)0)
#define TMS_AUDIO_OUTPUT_PUBLIC                 ((gint)1)
#define TMS_AUDIO_OUTPUT_PRIVATE                ((gint)2)
#define TMS_AUDIO_OUTPUT_HANDSET                ((gint)3)
#define TMS_AUDIO_OUTPUT_LOUDSPEAKER            ((gint)4)
#define TMS_AUDIO_OUTPUT_WIRED_ACCESSORY        ((gint)5)
#define TMS_AUDIO_OUTPUT_ACCESSORY              ((gint)6)
#define TMS_AUDIO_OUTPUT_ETTY                   ((gint)7)
#define TMS_AUDIO_OUTPUT_NOT_ACTIVE             ((gint)8)

typedef gint TMSInbandToneType;
#define TMS_INBAND_USER_BUSY                    ((gint)0)
#define TMS_INBAND_RADIO_PATH_NOT_AVAIL         ((gint)1)
#define TMS_INBAND_CONGESTION                   ((gint)2)
#define TMS_INBAND_SPECIAL_INFO                 ((gint)3)
#define TMS_INBAND_REORDER                      ((gint)4)
#define TMS_INBAND_REMOTE_ALEARTING             ((gint)5)
#define TMS_INBAND_CALL_WAITING                 ((gint)6)
#define TMS_INBAND_DATA_CALL                    ((gint)7)
#define TMS_INBAND_NO_SEQUENCE                  ((gint)8)
#define TMS_INBAND_BEEP_SEQUENCE                ((gint)9)

// Structure signalling callback notifications
struct TMSSignalEvent
    {
    guint type;
    guint reason;
    gpointer event_data;
    gpointer user_data;
    gint curr_state;
    gint prev_state;
    };

// Structure for global volume effect change event
struct TMSVolumeEventChangeData
    {
    guint level;
    TMSAudioOutput output;
    gboolean output_changed;
    };

} //namespace TMS

#endif // TMS_H

// End of file
