/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: QT Bindings for TMS
 *
 */

#ifndef QTMS_H
#define QTMS_H

// Include files
#include <glib.h>
#include <vector>

namespace QTMS {

// FORWARD DECLARATION
class QTMSFormat;

// Result codes
#define QTMS_RESULT_SUCCESS                      ((gint)0x00000000)
#define QTMS_RESULT_ALREADY_EXIST                ((gint)0x00000001)
#define QTMS_RESULT_DOES_NOT_EXIST               ((gint)0x00000002)
#define QTMS_RESULT_NULL_ARGUMENT                ((gint)0x00000003)
#define QTMS_RESULT_INVALID_ARGUMENT             ((gint)0x00000004)
#define QTMS_RESULT_INVALID_STATE                ((gint)0x00000005)
#define QTMS_RESULT_UNINITIALIZED_OBJECT         ((gint)0x00000006)
#define QTMS_RESULT_INSUFFICIENT_MEMORY          ((gint)0x00000007)
#define QTMS_RESULT_GENERAL_ERROR                ((gint)0x00000008)
#define QTMS_RESULT_FATAL_ERROR                  ((gint)0x00000009)
#define QTMS_RESULT_OPERATION_CANCELLED          ((gint)0x0000000A)
#define QTMS_RESULT_ILLEGAL_OPERATION            ((gint)0x0000000B)
#define QTMS_RESULT_BUFFER_LATENCY_ERROR         ((gint)0x0000000C)
#define QTMS_RESULT_CALL_TYPE_NOT_SUPPORTED      ((gint)0x0000000D)
#define QTMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED    ((gint)0x0000000E)
#define QTMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED    ((gint)0x0000000F)
#define QTMS_RESULT_SINK_TYPE_NOT_SUPPORTED      ((gint)0x00000010)
#define QTMS_RESULT_STREAM_TYPE_NOT_SUPPORTED    ((gint)0x00000011)
#define QTMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED    ((gint)0x00000012)
#define QTMS_RESULT_EFFECT_TYPE_NOT_SUPPORTED    ((gint)0x00000013)
#define QTMS_RESULT_VERSION_NOT_SUPPORTED        ((gint)0x00000014)
#define QTMS_RESULT_FORMAT_TYPE_UNSPECIFIED      ((gint)0x00000015)
#define QTMS_RESULT_FEATURE_NOT_SUPPORTED        ((gint)0x00000016)

// Reasons codes
#define QTMS_REASON_CLIENT_INITIATED             ((gint)0x00000000)
#define QTMS_REASON_DEVICE_IN_USE                ((gint)0x00000001) // Device not available
#define QTMS_REASON_DEVICE_TAKEN                 ((gint)0x00000002) // Preemption
#define QTMS_REASON_INSUFFICIENT_MEMORY          ((gint)0x00000003) // Memory
#define QTMS_REASON_PERMISSION_DENIED            ((gint)0x00000004) // Client app does not have perission
#define QTMS_REASON_EMERGENCY_CALL_ONGOING       ((gint)0x00000005) // Already ongoing emergency call
#define QTMS_REASON_EMERGENCY_CALL_STARTED       ((gint)0x00000006) // Preemption because of emergency call
#define QTMS_REASON_SOURCE_NULL                  ((gint)0x00000007) // Could be checked on client side
#define QTMS_REASON_SOURCE_NOT_SUPPORTED         ((gint)0x00000008)
#define QTMS_REASON_FORMAT_NULL                  ((gint)0x00000009) // Could be checked on client side
#define QTMS_REASON_EFFECT_NOT_SUPPORTED         ((gint)0x0000000A)
#define QTMS_REASON_SINK_NULL                    ((gint)0x0000000B) // Could be checked on client side
#define QTMS_REASON_SINK_NOT_SUPPORTED           ((gint)0x0000000C)
#define QTMS_REASON_CALL_NOT_ACTIVE              ((gint)0x0000000D)

// Event codes
#define QTMS_EVENT_STREAM_STATE_CHANGED          ((gint)0x00000000)
#define QTMS_EVENT_STREAM_STATE_CHANGE_ERROR     ((gint)0x00000001)
#define QTMS_EVENT_SOURCE_FILL_BUFFER            ((gint)0x00000002)
#define QTMS_EVENT_SOURCE_PROCESSED_BUFFER       ((gint)0x00000003)
#define QTMS_EVENT_SINK_PROCESS_BUFFER           ((gint)0x00000004)
#define QTMS_EVENT_EFFECT_VOL_CHANGED            ((gint)0x00000005)
#define QTMS_EVENT_EFFECT_GAIN_CHANGED           ((gint)0x00000006)
#define QTMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED ((gint)0x00000007)
#define QTMS_EVENT_ROUTING_OUTPUT_CHANGED        ((gint)0x00000008)
#define QTMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE   ((gint)0x00000009)
#define QTMS_EVENT_RINGTONE_OPEN_COMPLETE        ((gint)0x0000000A)
#define QTMS_EVENT_RINGTONE_PLAY_COMPLETE        ((gint)0x0000000B)
#define QTMS_EVENT_RINGTONE_DEINIT_COMPLETE      ((gint)0x0000000C)
#define QTMS_EVENT_DTMF_TONE_STARTED             ((gint)0x0000000D)
#define QTMS_EVENT_DTMF_TONE_STOPPED             ((gint)0x0000000E)
#define QTMS_EVENT_INBAND_TONE_STARTED           ((gint)0x0000000F)
#define QTMS_EVENT_INBAND_TONE_STOPPED           ((gint)0x00000010)

// Stream states
typedef gint QTMSStreamState;
#define QTMS_STREAM_UNINITIALIZED                ((gint)0x00000000)
#define QTMS_STREAM_INITIALIZED                  ((gint)0x00000001)
#define QTMS_STREAM_PAUSED                       ((gint)0x00000002)
#define QTMS_STREAM_STARTED                      ((gint)0x00000003)

#define DEFAULT_CALL_CONTEXT                    ((gint)1)

// QTMS object types
typedef gint QTMSStreamType;
#define QTMS_STREAM_UPLINK                       ((gint)0)
#define QTMS_STREAM_DOWNLINK                     ((gint)1)

typedef gint QTMSBufferType;
#define QTMS_BUFFER_MEMORY                       ((gint)2)

typedef gint QTMSSourceType;
#define QTMS_SOURCE_CLIENT                       ((gint)3)
#define QTMS_SOURCE_MODEM                        ((gint)4)
#define QTMS_SOURCE_MIC                          ((gint)5)

typedef gint QTMSSinkType;
#define QTMS_SINK_CLIENT                         ((gint)6)
#define QTMS_SINK_MODEM                          ((gint)7)
#define QTMS_SINK_SPEAKER                        ((gint)8)

typedef gint QTMSEffectType;
#define QTMS_EFFECT_GLOBAL_VOL                   ((gint)9)
#define QTMS_EFFECT_GLOBAL_GAIN                  ((gint)10)
#define QTMS_EFFECT_VOLUME                       ((gint)11)
#define QTMS_EFFECT_GAIN                         ((gint)12)

typedef gint QTMSFormatType;
#define QTMS_FORMAT_PCM                          ((gint)13)
#define QTMS_FORMAT_AMR                          ((gint)14)
#define QTMS_FORMAT_G711                         ((gint)15)
#define QTMS_FORMAT_G729                         ((gint)16)
#define QTMS_FORMAT_ILBC                         ((gint)17)

typedef gint QTMSRingToneType;
#define QTMS_RINGTONE_DEFAULT                    ((gint)0)
#define QTMS_RINGTONE_FILE                       ((gint)1)
#define QTMS_RINGTONE_SEQUENCE                   ((gint)2)
#define QTMS_RINGTONE_BEEP_ONCE                  ((gint)3)
#define QTMS_RINGTONE_SILENT                     ((gint)4)
#define QTMS_RINGTONE_UNSECURE_VOIP              ((gint)5)

typedef gint QTMSRTPSessionType;
#define QTMS_SESSION_RTP_PULL                    ((gint)0)
#define QTMS_SESSION_RTP_PUSH                    ((gint)1)

// QTMS vectors
typedef std::vector<QTMSFormat*> FormatVector;
typedef std::vector<guint> BitRateVector;
typedef std::vector<guint> OutputVector;

// Call types
typedef gint QTMSCallType;
#define QTMS_CALL_CS                             ((gint)0)
#define QTMS_CALL_ECS                            ((gint)1)
#define QTMS_CALL_IP                             ((gint)2)
#define QTMS_CALL_RTP                            ((gint)3)

// Codec modes
typedef gint QTMSG711CodecMode;
#define QTMS_G711_CODEC_MODE_ALAW                ((gint)0)
#define QTMS_G711_CODEC_MODE_MULAW               ((gint)1)

typedef gint QTMSILBCCodecMode;
#define QTMS_ILBC_CODEC_MODE_20MS_FRAME          ((gint)0)
#define QTMS_ILBC_CODEC_MODE_30MS_FRAME          ((gint)1)

// Audio output types
typedef gint QTMSAudioOutput;
#define QTMS_AUDIO_OUTPUT_NONE                   ((gint)0)
#define QTMS_AUDIO_OUTPUT_PUBLIC                 ((gint)1)
#define QTMS_AUDIO_OUTPUT_PRIVATE                ((gint)2)
#define QTMS_AUDIO_OUTPUT_HANDSET                ((gint)3)
#define QTMS_AUDIO_OUTPUT_LOUDSPEAKER            ((gint)4)
#define QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY        ((gint)5)
#define QTMS_AUDIO_OUTPUT_ACCESSORY              ((gint)6)
#define QTMS_AUDIO_OUTPUT_ETTY                   ((gint)7)
#define QTMS_AUDIO_OUTPUT_NOT_ACTIVE             ((gint)8)

typedef gint QTMSInbandToneType;
#define QTMS_INBAND_USER_BUSY                    ((gint)0)
#define QTMS_INBAND_RADIO_PATH_NOT_AVAIL         ((gint)1)
#define QTMS_INBAND_CONGESTION                   ((gint)2)
#define QTMS_INBAND_SPECIAL_INFO                 ((gint)3)
#define QTMS_INBAND_REORDER                      ((gint)4)
#define QTMS_INBAND_REMOTE_ALEARTING             ((gint)5)
#define QTMS_INBAND_CALL_WAITING                 ((gint)6)
#define QTMS_INBAND_DATA_CALL                    ((gint)7)
#define QTMS_INBAND_NO_SEQUENCE                  ((gint)8)
#define QTMS_INBAND_BEEP_SEQUENCE                ((gint)9)

// Structure signalling callback notifications
struct QTMSSignalEvent
    {
    guint type;
    guint reason;
    gpointer event_data;
    gpointer user_data;
    gint curr_state;
    gint prev_state;
    };

// Structure for global volume effect change event
struct QTMSVolumeEventChangeData
    {
    guint level;
    QTMSAudioOutput output;
    gboolean output_changed;
    };

} //namespace QTMS

#endif // QTMS_H

// End of file
