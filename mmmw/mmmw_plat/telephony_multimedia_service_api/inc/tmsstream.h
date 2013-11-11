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

#ifndef TMS_STREAM_H
#define TMS_STREAM_H

// Include files
#include <glib.h>

namespace TMS {

// Forward declarations
class TMSStreamObserver;
class TMSSource;
class TMSSink;
class TMSEffect;
class TMSStreamBody;
class TMSFormat;
class TMSBuffer;

/**
 * TMSStream class
 *
 * TMSStream class represents either uplink or downlink stream associated with
 * a TMSCall.
 *
 * This class provides means for setting up and controlling telephony
 * multimedia streams associated with a Circuit Switching (CS) call or Packet
 * Switching (PS) such as IP call.
 *
 * TMSStream instances are created by TMSCall instance. Multiple TMSStream
 * instances (uplink and/or downlink streams) can be created. The TMS engine
 * does not limit the number of streams created per TMSCall object. However,
 * the combination of device policy and device run-time resources (both
 * software and hardware) may limit this use-case.
 *
 * TMSCall is a container which owns all its TMSStream instances. The client
 * has an option of creating and deleting individual streams. However, when a
 * TMSCall instance is deleted, all the streams associated with it are
 * automatically deleted as well.
 *
 * TMS ver 1.0.0.0:
 *  (1) An initialized stream will have one source, zero or one format,
 *  zero or more effects, and one or more sinks.
 *
 *  (2) In the initial version of the TMS implementation, the CS uplink and
 *  downlink streams will not control or communicate with the cellular modem.
 *  Also, the PS (IP) uplink and downlink streams will not handle network
 *  protocol. It is assumed that the TMS client will handle this functionality.
 *
 * States:
 *  (1) TMS_STREAM_UNINITIALIZED (initial state): The stream is alive but has
 *  not allocated all the resources it needs to function.
 *
 *  (2) TMS_STREAM_INITIALIZED: The stream has all the information required to
 *  acquire the media resources. Stream in this state will have most of the
 *  resources allocated, except the critical system resources, such as policy
 *  and/or hardware resources.
 *
 *  (3) TMS_STREAM_PAUSED: The stream has all the resources allocated, which may
 *  include critical system resources such as policy and/or hardware resources.
 *
 *  (4) TMS_STREAM_STARTED: The stream has all the resources allocated,
 *  including critical system resources such as policy and/or hardware resources
 *  and is active.
 *
 * Usage:
 * <code>
 * TMSFactory *iFactory;
 * TMSCall *iCall;
 * TMSStream *iUplink;
 * TMSStream *iDownlink;
 * gint err;
 *
 * err = TMSFactory::CreateFactory(iFactory);
 * err = iFactory->CreateCall(TMS_CALL_IP, iCall);
 * err = iCall->CreateStream(TMS_STREAM_UPLINK, iUplink);
 * err = iCall->CreateStream(TMS_STREAM_DOWNLINK, iDownlink);
 * // Configure CS uplink stream
 * iUplinkStream->AddObserver(*this);
 * iUplinkStream->AddSource(iMicSource);
 * iUplinkStream->AddSink(iModemSink);
 * iUplinkStream->AddEffect(iGainEffect); // To control mic gain
 * iUplinkStream->Init();
 * // Wait for state change callback
 * iUplinkStream->Play();
 * // Wait for state change callback
 * ...
 * // call terminated on the cell modem side, now signal multimedia system
 * iUplinkStream->Stop();
 * ...
 *  err = iCall->DeleteStream(iDownlink);
 *  err = iCall->DeleteStream(iUplink);
 *  err = iFactory->DeleteCall(iCall);
 * delete iFactory;
 *
 * </code>
 *
 * @lib tmsapi.lib
 *
 */
class TMSStream
    {
public:

    /**
     * Destructor
     */
    IMPORT_C ~TMSStream();

    /**
     * Add stream observer for this stream.
     *
     * Multiple observers can be added to a stream.
     *
     * This function can be called at any time. It is recommended to add
     * observer before calling Init() on stream. Otherwise, the observer may
     * miss a callback.
     *
     * A reference to the obsrvr is stored in the stream and TMSStreamEvent
     * function will be called when a stream event occurs. When stream is
     * deleted, all TMSStreamObserver references are automatically released.
     * The ownership of the obsrvr is still maintained by the client. Should
     * the client need to delete obsrvr before deleting stream, it should
     * remove all the obsrvr references from the stream by calling
     * RemoveObserver() method.
     *
     * @param  obsrvr
     *      Listener added to the stream.
     *
     * @param  user_data
     *      User data passed to the function.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is added successfully to the list.
     *      TMS_RESULT_ALREADY_EXIST if obsrvr is already in the list.
     *
     */
    IMPORT_C gint AddObserver(TMSStreamObserver& obsrvr, gpointer user_data);

    /**
     * Remove stream observer from this stream.
     *
     * This function can be called at any time. It is recommended to remove
     * observer after calling Deinit() on stream. Oterwise, the observer may
     * receive callback that is alread dispatched.
     *
     * @param  obsrvr
     *      Listener to be removed from the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is removed successfully from the
     *      stream.
     *      TMS_RESULT_DOES_NOT_EXIST if the obsrvr is not on the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSStreamObserver& obsrvr);

    /**
     * Add source for this stream.
     *
     * This function can be called only when stream is in
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After source is added to the stream successfully, ownership of the source
     * is vested in this stream. If the client wants to reuse the source, it
     * should call RemoveSource() to re-claim TMSSource ownership.
     *
     * @param  source
     *      Source object to be added to the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if source is added successfully to the stream.
     *      TMS_RESULT_NULL_ARGUMENT if source is a NULL.
     *      TMS_RESULT_ALREADY_EXIST if source has already been added to the
     *      stream.
     *      TMS_RESULT_INVALID_STATE if stream is not
     *      in the TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint AddSource(TMSSource* source);

    /**
     * Remove source from this stream.
     *
     * This function can be called only when stream is in
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After source is removed from the stream successfully, ownership of
     * source is re-claimed by the client.
     *
     * @param  source
     *      The source to remove.
     *
     * @return
     *      TMS_RESULT_SUCCESS if source is removed successfully from the
     *      stream.
     *      TMS_RESULT_NULL_ARGUMENT if source is a NULL.
     *      TMS_RESULT_DOES_NOT_EXIST if trying to remove the source that has
     *      not been added to stream.
     *      TMS_RESULT_INVALID_STATE if stream is not
     *      in the TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint RemoveSource(TMSSource* source);

    /**
     * Add sink to the stream.
     *
     * Multiple sinks of different types can be added to a stream.
     *
     * This function can be called only when stream is in
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After sink is added to the stream successfully, ownership of the sink is
     * vested in the stream. If the client wants to reuse sink, it should call
     * RemoveSink() to re-claim TMSSink ownership.
     *
     * @param  sink
     *      Data sink to be added to the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if sink is added successfully to the stream.
     *      TMS_RESULT_NULL_ARGUMENT if sink is a NULL.
     *      TMS_RESULT_ALREADY_EXIST if sink type is already added to the
     *      stream.
     *      TMS_RESULT_INVALID_STATE if stream is not
     *      in the TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint AddSink(TMSSink* sink);

    /**
     * Remove sink from the stream.
     *
     * This function can be called only when stream is in
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After sink is removed from the stream successfully, ownership of the
     * sink is re-claimed by the client.
     *
     * @param  sink
     *      Sink to removed from the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if sink is removed successfully from the stream.
     *      TMS_RESULT_NULL_ARGUMENT if sink is a NULL.
     *      TMS_RESULT_DOES_NOT_EXIST if trying to remove sink that has not
     *      been added to the stream.
     *      TMS_RESULT_INVALID_STATE if stream is not
     *      in the TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint RemoveSink(TMSSink* sink);

    /**
     * Set data format for the stream.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After format is added to the stream successfully, ownership of the
     * format is vested in this stream. If the client wants to reuse format,
     * it should call ResetFormat() to re-claim TMSFormat ownership.
     *
     * Note: In TMS Ver 1.0.0.0, setting data format for CS call is not
     * required.
     *
     * @param  format
     *      Data format to be set on the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if format is set successfully on the stream.
     *      TMS_RESULT_NULL_ARGUMENT if format is a NULL.
     *      TMS_RESULT_ALREADY_EXIST if format is already set on the stream.
     *      TMS_RESULT_INVALID_STATE if stream is not
     *      in the TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint SetFormat(TMSFormat* format);

    /**
     * Remove data format from the stream.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After format is removed from the stream successfully, ownership of
     * the format object is re-claimed by the client.
     *
     * @param  format
     *      Data format to be removed from the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if format is removed successfully from the
     *      stream.
     *      TMS_RESULT_NULL_ARGUMENT if format is a NULL.
     *      TMS_RESULT_DOES_NOT_EXIST if format is not currently set on the
     *      stream.
     *      TMS_RESULT_INVALID_STATE if stream is not in the
     *      TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint ResetFormat(TMSFormat* format);

    /**
     * Add an effect to the stream.
     *
     * Multiple effect objects of different types can be added to the stream.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After effect is added to the stream successfully, ownership of the
     * effect is vested in the stream. If the client wants to reuse the effect,
     * it shall call RemoveEffect() to re-claim TMSEffect ownership.
     *
     * @param  effect
     *      Stream effect to be added to the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if effect is added successfully to the stream.
     *      TMS_RESULT_NULL_ARGUMENT if effect is a NULL.
     *      TMS_RESULT_ALREADY_EXIST if effect type is already added to the
     *      stream.
     *      TMS_RESULT_INVALID_STATE if stream is not in the
     *      TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint AddEffect(TMSEffect* effect);

    /**
     * Remove effect from the stream.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_UNINITIALIZED state.
     *
     * After effect is removed from the stream successfully, ownership of the
     * effect is re-claimed by the client.
     *
     * @param  effect
     *      Stream effect to be removed from the stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if effect is removed successfully from the
     *      stream.
     *      TMS_RESULT_NULL_ARGUMENT if effect is a NULL.
     *      TMS_RESULT_DOES_NOT_EXIST if trying to remove an effect that has
     *      not been added to the stream.
     *      TMS_RESULT_INVALID_STATE if stream is not in the
     *      TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C gint RemoveEffect(TMSEffect* effect);

    /**
     * Get current state of the stream.
     *
     * This function can be called at any time.
     *
     * Possible states are:
     *  TMS_STREAM_UNINITIALIZED,
     *  TMS_STREAM_INITIALIZED,
     *  TMS_STREAM_PAUSED,
     *  TMS_STREAM_STARTED.

     * @return
     *      Stream's current state.
     *
     */
    IMPORT_C gint GetState();

    /**
     * Get stream ID.
     *
     * This function can be called at any time.
     *
     * @return
     *      Unique ID of the stream.
     *
     */
    IMPORT_C gint GetStreamId();

    /**
     * Get stream type.
     *
     * This function can be called at any time.
     *
     * The possible types are:
     *  TMS_STREAM_UPLINK
     *  TMS_STREAM_DOWNLINK
     *
     * @return
     *      Stream type indicating whether it is an uplink or downlink.
     *
     */
    IMPORT_C gint GetStreamType();

    /**
     * Trigger stream to transition to the initialized state.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_UNINITIALIZED state.
     *
     * If Init() is called when stream is already in initialized state, the
     * request will be ignored and the function will return TMS_RESULT_SUCCESS.
     *
     * Upon stream's successful transition to initialized state, the stream will
     * be in the TMS_STREAM_INITIALIZED state.
     *
     * Before stream can transition to initialized state the following
     * objects must be added to the stream:
     *      CS call: UPL: mic source and modem sink
     *      CS call: DNL: modem source and speaker sink
     *      IP call: UPL: mic source, codec format and client sink
     *      IP call: DNL: client source, codec format and speaker sink
     *
     * @param  retrytime
     *      Indicates (in seconds) for how long TMS should retry stream
     *      initialization in case of an error. When stream initialization
     *      fails within specified retry time, TMS will return
     *      TMS_EVENT_STREAM_STATE_CHANGE_ERROR. If set to 0, TMS will return
     *      TMS_EVENT_STREAM_STATE_CHANGE_ERROR immediately without retrying.
     *      If set to -1, TMS will keep retrying until user cancels by calling
     *      either Stop() or Deinit().
     *
     * @return
     *      Common return codes:
     *      TMS_RESULT_SUCCESS if stream transitioned to the initialized state.
     *      TMS_RESULT_INVALID_STATE if stream has not transitioned to the
     *      TMS_STREAM_INITIALIZED state.
     *      TMS_RESULT_FORMAT_TYPE_UNSPECIFIED (IP call only) when stream
     *      has no format attached to it.
     *      TMS_RESULT_UNINITIALIZED_OBJECT when stream has no sink or source
     *      element attached to it.
     *
     */
    IMPORT_C gint Init(gint retrytime = 0);

    /**
     * Trigger stream to transition to the paused state.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_UNINITIALIZED or TMS_STREAM_STARTED state.
     *
     * If Pause() is called when stream is already in paused state, the
     * request will be ignored and the function will return TMS_RESULT_SUCCESS.
     *
     * Upon stream's successful transition to the paused state, the stream will
     * be in the TMS_STREAM_PAUSED state.
     *
     * Note: In TMS Ver 1.0.0.0, pausing stream for CS call is not supported.
     *
     * @return
     *      Common return codes:
     *      TMS_RESULT_SUCCESS if stream successfully transitioned to the
     *      paused state.
     *      TMS_RESULT_INVALID_STATE if stream is not in the
     *      TMS_STREAM_INITIALIZED or TMS_STREAM_PAUSED state.
     *
     */
    IMPORT_C gint Pause();

    /**
     * Trigger stream to transition to the started state.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_INITIALIZED or TMS_STREAM_PAUSED state.
     *
     * If Start() is called when stream is already in the started state, the
     * request will be ignored and the function will return TMS_RESULT_SUCCESS.
     *
     * If Start() is called when stream is already in the initialized state, the
     * stream will implicitly pause, but the observer will only receive one
     * state change callback.
     *
     * Upon stream's successful transition to the started state, the stream will
     * be in the TMS_STREAM_STARTED state.
     *
     * @param  retrytime
     *      Indicates (in seconds) for how long TMS should attempt to start
     *      a stream in case of an error. When stream starting fails within
     *      specified retry time, TMS will return
     *      TMS_EVENT_STREAM_STATE_CHANGE_ERROR. If set to 0, TMS will return
     *      TMS_EVENT_STREAM_STATE_CHANGE_ERROR immediately without retrying.
     *      If set to -1, TMS will keep retrying until user cancels by calling
     *      either Stop() or Deinit().
     *
     * @return
     *      Common return codes:
     *      TMS_RESULT_SUCCESS if stream successfully transitioned to the
     *      started state.
     *      TMS_RESULT_INVALID_STATE if stream is not in the
     *      TMS_STREAM_INITIALIZED or TMS_STREAM_STARTED state.
     *
     */
    IMPORT_C gint Start(gint retrytime = 0);

    /**
     * Trigger stream to transition to the initialized state.
     *
     * This function can be called only when stream is in the
     * TMS_STREAM_STARTED or TMS_STREAM_PAUSED state.
     *
     * If Stop() is called when stream is already in the stopped state, the
     * request will be ignored and the function will return TMS_RESULT_SUCCESS.
     *
     * Upon stream's successful transition to the started state, the stream will
     * be in the TMS_STREAM_INITIALIZED state.
     *
     * @return
     *      Common return codes:
     *      TMS_RESULT_SUCCESS if stream successfully transitioned to the
     *      stopped state.
     *      TMS_RESULT_INVALID_STATE if stream is not in the
     *      TMS_STREAM_STARTED or TMS_STREAM_PAUSED state.
     *
     */
    IMPORT_C gint Stop();

    /**
     * Trigger stream to transition to un-initialized state.
     *
     * This function can be called only when stream is NOT in
     * TMS_STREAM_UNINITIALIZED state.
     *
     * If Deinit() is called when stream is already in un-initialized state, the
     * request will be ignored.
     *
     * Upon stream's successful transition to the un-initialized state, the
     * stream will be in the TMS_STREAM_UNINITIALIZED state.
     *
     */
    IMPORT_C void Deinit();

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSStream();

protected:
    TMSStreamBody *iBody;
    };

} //namespace TMS

#endif // TMS_STREAM_H

// End of file
