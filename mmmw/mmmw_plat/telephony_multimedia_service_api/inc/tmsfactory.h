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

#ifndef TMS_FACTORY_H
#define TMS_FACTORY_H

#include <w32std.h>
#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCall;
class TMSFormat;
class TMSEffect;
class TMSBuffer;
class TMSSource;
class TMSSink;
class TMSFactoryImpl;
class TMSVer;
class TMSGlobalRouting;
class TMSDTMF;
class TMSRTPSession;
class TMSRingTone;
class TMSInbandTone;

/**
 * TMSFactory class
 *
 * This class serves as a factory crating all necessary TMS components.
 *
 * Usage:
 * <code>
 * TMSFactory* iFactory;
 * TMSCall* iCall;
 * gint err;
 * err = TMSFactory::CreateFactory(iFactory);
 * err = iFactory->CreateCall(TMS_CALL_IP, iCall);
 * ...
 * err = iFactory->DeleteCall(iCall);
 * delete iFactory;
 * ...
 * </code>
 *
 * @lib tmsapi.lib
 *
 */
class TMSFactory
    {
public:
    /**
     * Returns pointer reference to created TMS Factory object. The specified
     * API version will allow using different TMS API variants.
     *
     * @param  tmsfactory
     *      Created Factory object.
     *
     * @param  ver
     *      API version which should be used to create TMS components.
     *
     * @return
     *      TMS_RESULT_SUCCESS if factory is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if factory creation failed due to
     *      insufficient of memory.
     *      TMS_RESULT_VERSION_NOT_SUPPORTED if requested API version is not
     *      supported.
     *
     */
    IMPORT_C static gint CreateFactory(TMSFactory*& tmsfactory, TMSVer& ver);

    /**
     * Destructor
     *
     * Deletes the TMS Factory.
     *
     */
    IMPORT_C virtual ~TMSFactory();

    /**
     * Creates TMS Call of the given type with context ID.
     *
     * @param  ctype
     *      Call type to be created.
     *      Supported call types:
     *          TMS_CALL_CS
     *          TMS_CALL_ECS
     *          TMS_CALL_IP
     *
     * @param  tmscall
     *      Created Call object.
     *
     * @param  ctxid
     *      The Context ID to be assigned to the call. The default is
     *      DEFAULT_CALL_CONTEXT.
     *
     * @return
     *      TMS_RESULT_SUCCESS if call is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if call creation failed due to
     *      insufficient memory.
     *      TMS_RESULT_CALL_TYPE_NOT_SUPPORTED if calltype is not supported.
     *      TMS_RESULT_INVALID_ARGUMENT if the context id is not valid.
     *      TMS_RESULT_FATAL_ERROR if there is any other error.
     *
     */
    IMPORT_C gint CreateCall(TMSCallType ctype, TMSCall*& tmscall,
            guint ctxid = DEFAULT_CALL_CONTEXT);

    /**
     * Deletes TMS Call.
     *
     * @param  tmsct
     *      The call to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if call is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if the call is not valid.
     *
     */
    IMPORT_C gint DeleteCall(TMSCall*& tmsct);

    /**
     * Is the given calltype supported by the TMS API.
     *
     * @param  ctype
     *      The call type to be validated.
     *
     * @param  flag
     *      Indicates wheter the call type is supported or not.
     *
     * @return
     *      TMS_RESULT_SUCCESS if operation was successful.
     *      TMS_RESULT_FATAL_ERROR if operation failed.
     *
     */
    IMPORT_C gint IsCallTypeSupported(TMSCallType ctype, gboolean& flag);

    /**
     * Returns supported codec format types.
     *
     * @param  strmtype
     *      Stream type: Uplink or Downlink
     *
     * @param  fmtlist
     *      Format vector in which the supported formats are stored
     *
     * @return
     *      TMS_RESULT_SUCCESS if operation was successful.
     *      TMS_RESULT_FATAL_ERROR if operation failed.
     *
     */
    IMPORT_C gint GetSupportedFormats(const TMSStreamType strmtype,
            FormatVector& fmtlist);

    /**
     * Creates Format object of the given format type.
     *
     * @param  fmttype
     *      Format type to be created.
     *
     * @param  tmsfmt
     *      Crated Format object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if format is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if format creation failed due to
     *      insufficient of memory.
     *      TMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED if format type is not
     *      supported.
     *
     */
    IMPORT_C gint CreateFormat(TMSFormatType fmttype, TMSFormat*& tmsfmt);

    /**
     * Deletes Format object.
     *
     * @param  tmsfmt
     *      Format to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if format is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if the format is not valid.
     *
     */
    IMPORT_C gint DeleteFormat(TMSFormat*& tmsfmt);

    /**
     * Creates Effect object of the given type.
     *
     * @param  tmseffecttype
     *      Effect type to be created.
     *
     * @param  tmseffect
     *      Created Effect object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if effect is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if effect creation failed due to
     *      insufficient memory.
     *      TMS_RESULT_EFFECT_TYPE_NOT_SUPPORTED if the effect type is not
     *      supported.
     *
     */
    IMPORT_C gint CreateEffect(TMSEffectType tmseffecttype,
            TMSEffect*& tmseffect);

    /**
     * Deletes Effect object.
     *
     * @param  tmseffect
     *      Effect object to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if effect is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if the effect is not valid.
     */
    IMPORT_C gint DeleteEffect(TMSEffect*& tmseffect);

    /**
     * Creates Buffer object of the given type and size.
     * The memory is allocated on the heap.
     *
     * @param  buffertype
     *      Buffer type to be created.
     *
     * @param  size
     *      Size of the buffer to be created.
     *
     * @param  tmsbuffer
     *      Created buffer object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if buffer is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if buffer creation failed due to
     *      insufficient memory.
     *      TMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED if the buffer type is not
     *      supported.
     *
     */
    IMPORT_C gint CreateBuffer(TMSBufferType buffertype, guint size,
            TMSBuffer*& tmsbuffer);

    /**
     * Deletes the Buffer object.
     *
     * @param  tmsbuffer
     *      Buffer to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if buffer is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if the buffer is not valid.
     */
    IMPORT_C gint DeleteBuffer(TMSBuffer*& tmsbuffer);

    /**
     * Creates the Source object of the given type.
     *
     * @param  srctype
     *      Source type to be created.
     *
     * @param  tmssrc
     *      Crated source object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if source is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if source creation failed due to
     *      lack of memory.
     *      TMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED if the source type is not
     *      supported.
     *
     */
    IMPORT_C gint CreateSource(TMSSourceType srctype, TMSSource*& tmssrc);

    /**
     * Deletes the Source object.
     *
     * @param  tmssrc
     *      Source to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if source is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if the source is not valid.
     *
     */
    IMPORT_C gint DeleteSource(TMSSource*& tmssrc);

    /**
     * Creates the Sink object of the given TMSSinkType.
     *
     * @param  sinktype
     *      Sink type to be created.
     *
     * @param  tmssink
     *      Created sink object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if sink is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if sink creation failed due to
     *      lack of memory.
     *      TMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED if the sink type is not
     *      supported.
     *
     */
    IMPORT_C gint CreateSink(TMSSinkType sinktype, TMSSink*& tmssink);

    /**
     * Deletes the Sink object.
     *
     * @param  tmssink
     *      The sink to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if sink is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if the sink is not valid.
     */
    IMPORT_C gint DeleteSink(TMSSink*& tmssink);

    /**
     * Creates TMSGlobalRouting object.
     *
     * @param  globrouting
     *      Created global routing object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if global routing object is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if global routing creation failed
     *      due to lack of memory.
     *      TMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED if global routing is not
     *      supported.
     *
     */
    IMPORT_C gint CreateGlobalRouting(TMSGlobalRouting*& globrouting);

    /**
     * Deletes TMSGlobalRouting object.
     *
     * @param  globrouting
     *      Global routing object to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if global routing object is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if the global routing object is invalid.
     */
    IMPORT_C gint DeleteGlobalRouting(TMSGlobalRouting*& globrouting);

    /**
     * Create DTMF player object.
     *
     * @param  streamtype
     *     Stream ID to be used to create the DTMF player.
     *     TMS_STREAM_UPLINK
     *     TMS_STREAM_DOWNLINK
     *
     * @param  dtmf
     *      Created DTMF player.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if DTMF player creation failed due
     *      to insufficient memory.
     *      TMS_RESULT_FATAL_ERROR if an error occured.
     *      TMS_REASON_EMERGENCY_CALL_ONGOING if emergency call is active.
     *      TMS_REASON_PERMISSION_DENIED if permission is denied.
     *
     */
    IMPORT_C gint CreateDTMF(TMSStreamType streamtype, TMSDTMF*& dtmf);

    /**
     * Delete DTMF player object.
     *
     * @param  dtmf
     *      DTMF player to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INVALID_ARGUMENT if the DTMF player is not valid.
     *
     */
    IMPORT_C gint DeleteDTMF(TMSDTMF*& dtmf);

    /**
     * FEATURE IS CURRENTLY NOT SUPPORTED
     *
     * Creates TMSRTPSession object.
     *
     * @param  rtpsession
     *      RTP Session object to be created.
     *
     * @param  sdp
     *      Session description protocol message.
     *
     * @param  mode
     *      Buffer processing mode: either PULL or PUSH.
     *
     * @return
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED
     */
    IMPORT_C gint CreateRTPSession(TMSRTPSession*& rtpsession, guint8* sdp,
            guint mode);

    /**
     * FEATURE IS CURRENTLY NOT SUPPORTED
     *
     * Deletes TMSRTPSession object.
     *
     * @param  rtpsession
     *      RTP Session object to be deleted.
     *
     * @return
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED
     */
    IMPORT_C gint DeleteRTPSession(TMSRTPSession*& rtpsession);

    /**
     * Creates TMSRingTone player object for audio ring tone.
     *
     * @param  rt
     *      RingTone player object to be created.
     *
     * @return
     *      TMS_RESULT_SUCCESS when object is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY when object creation failed due to
     *      insufficient memory.
     */
    IMPORT_C gint CreateRingTonePlayer(TMSRingTone*& rt);

    /**
     * Creates TMSRingTone player object for video ring tone.
     *
     * @param  rt
     *      Video RingTone player object to be created.
     *
     * @param window
     *      A handle to window to which video should be rendered.
     *
     * @param scrid
     *      An ID of the screen to which video should be rendered.
     *
     * @return
     *      TMS_RESULT_SUCCESS when object is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY when object creation failed due to
     *      insufficient memory.
     */
    IMPORT_C gint CreateRingTonePlayer(TMSRingTone*& rt, RWindow& window,
            gint scrid);

    /**
     * Deletes TMSRingTone object.
     *
     * @param  rt
     *      RingTone player object to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS when object is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if RingTone player object is invalid.
     */
    IMPORT_C gint DeleteRingTonePlayer(TMSRingTone*& rt);

    /**
     * Creates TMSInbandTone player object.
     *
     * @param  inbandtone
     *      InbandTone player object to be created.
     *
     * @return
     *      TMS_RESULT_SUCCESS when object is created successfully.
     *      TMS_RESULT_INSUFFICIENT_MEMORY when object creation failed due to
     *      insufficient memory.
     */
    IMPORT_C gint CreateInbandTonePlayer(TMSInbandTone*& inbandtone);

    /**
     * Deletes TMSInbandTone object.
     *
     * @param  inbandtone
     *      InbandTone player object to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS when object is deleted successfully.
     *      TMS_RESULT_INVALID_ARGUMENT if InbandTone player object is invalid.
     */
    IMPORT_C gint DeleteInbandTonePlayer(TMSInbandTone*& inbandtone);

private:
    /**
     * Constructor
     */
    TMSFactory(TMSVer& ver);

private:
    TMSFactoryImpl* impl;
    };

} //namespace TMS

#endif //TMS_FACTORY_H
