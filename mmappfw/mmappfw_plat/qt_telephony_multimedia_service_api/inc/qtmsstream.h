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

#ifndef QTMS_STREAM_H
#define QTMS_STREAM_H

// Include files
#include <qtms.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSStream;
}

namespace QTMS
{

// Forward declarations
class QTMSSource;
class QTMSSink;
class QTMSEffect;
class QTMSFormat;
class QTMSBuffer;

/**
 * QTMSStream class
 *
 * QTMSStream class represents either uplink or downlink stream associated with
 * a QTMSCall.
 *
 * This class provides means for setting up and controlling telephony
 * multimedia streams associated with a Circuit Switching (CS) call or Packet
 * Switching (PS) such as IP call.
 *
 * QTMSStream instances are created by QTMSCall instance. Multiple QTMSStream
 * instances (uplink and/or downlink streams) can be created. The QTMS engine
 * does not limit the number of streams created per QTMSCall object. However,
 * the combination of device policy and device run-time resources (both
 * software and hardware) may limit this use-case.
 *
 * QTMSCall is a container which owns all its QTMSStream instances. The client
 * has an option of creating and deleting individual streams. However, when a
 * QTMSCall instance is deleted, all the streams associated with it are
 * automatically deleted as well.
 *
 * QTMS ver 1.0.0.0:
 *  (1) An initialized stream will have one source, zero or one format,
 *  zero or more effects, and one or more sinks.
 *
 *  (2) In the initial version of the QTMS implementation, the CS uplink and
 *  downlink streams will not control or communicate with the cellular modem.
 *  Also, the PS (IP) uplink and downlink streams will not handle network
 *  protocol. It is assumed that the QTMS client will handle this functionality.
 *
 * States:
 *  (1) QTMS_STREAM_UNINITIALIZED (initial state): The stream is alive but has
 *  not allocated all the resources it needs to function.
 *
 *  (2) QTMS_STREAM_INITIALIZED: The stream has all the information required to
 *  acquire the media resources. Stream in this state will have most of the
 *  resources allocated, except the critical system resources, such as policy
 *  and/or hardware resources.
 *
 *  (3) QTMS_STREAM_PAUSED: The stream has all the resources allocated, which may
 *  include critical system resources such as policy and/or hardware resources.
 *
 *  (4) QTMS_STREAM_STARTED: The stream has all the resources allocated,
 *  including critical system resources such as policy and/or hardware resources
 *  and is active.
 *
 * Usage:
 * <code>
 * QTMSFactory *iFactory;
 * QTMSCall *iCall;
 * QTMSStream *iUplinkStream;
 * QTMSStream *iDownlinkStream;
 * gint err;
 *
 * err = QTMSFactory::CreateFactory(iFactory);
 * err = iFactory->CreateCall(QTMS_CALL_IP, iCall);
 * err = iCall->CreateStream(QTMS_STREAM_UPLINK, iUplinkStream);
 * err = iCall->CreateStream(QTMS_STREAM_DOWNLINK, iDownlinkStream);
 * // Configure CS uplink stream
 * connect(iUplinkStream,
 *         SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)),
 *         this,
 *         SLOT(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));
 * iUplinkStream->AddSource(iMicSource);
 * iUplinkStream->AddSink(iModemSink);
 * iUplinkStream->AddEffect(iGainEffect); // To control mic gain
 * iUplinkStream->Init();
 * // Wait for TMSStreamEvent signal
 * iUplinkStream->Play();
 * //Wait for TMSStreamEvent signal
 * ...
 * // call terminated on the cell modem side, now signal multimedia system
 * iUplinkStream->Stop();
 * ...
 *  err = iCall->DeleteStream(iDownlinkStream);
 *  err = iCall->DeleteStream(iUplinkStream);
 *  err = iFactory->DeleteCall(iCall);
 * delete iFactory;
 *
 * </code>
 *
 * @lib QTMSapi.lib
 *
 */

class QTMS_WRAPPER_DLL_EXPORT QTMSStream : public QObject
{
    Q_OBJECT
public:

    /**
     * Destructor
     */
    virtual ~QTMSStream();

    /**
     * Add source for this stream.
     *
     * This function can be called only when stream is in
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After source is added to the stream successfully, ownership of the source
     * is vested in this stream. If the client wants to reuse the source, it
     * should call RemoveSource() to re-claim QTMSSource ownership.
     *
     * @param  source
     *      Source object to be added to the stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if source is added successfully to the stream.
     *      QTMS_RESULT_NULL_ARGUMENT if source is a NULL.
     *      QTMS_RESULT_ALREADY_EXIST if source has already been added to the
     *      stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not
     *      in the QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint AddSource(QTMSSource* source);

    /**
     * Remove source from this stream.
     *
     * This function can be called only when stream is in
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After source is removed from the stream successfully, ownership of
     * source is re-claimed by the client.
     *
     * @param  source
     *      The source to remove.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if source is removed successfully from the
     *      stream.
     *      QTMS_RESULT_NULL_ARGUMENT if source is a NULL.
     *      QTMS_RESULT_DOES_NOT_EXIST if trying to remove the source that has
     *      not been added to stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not
     *      in the QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint RemoveSource(QTMSSource* source);

    /**
     * Add sink to the stream.
     *
     * Multiple sinks of different types can be added to a stream.
     *
     * This function can be called only when stream is in
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After sink is added to the stream successfully, ownership of the sink is
     * vested in the stream. If the client wants to reuse sink, it should call
     * RemoveSink() to re-claim QTMSSink ownership.
     *
     * @param  sink
     *      Data sink to be added to the stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if sink is added successfully to the stream.
     *      QTMS_RESULT_NULL_ARGUMENT if sink is a NULL.
     *      QTMS_RESULT_ALREADY_EXIST if sink type is already added to the
     *      stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not
     *      in the QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint AddSink(QTMSSink* sink);

    /**
     * Remove sink from the stream.
     *
     * This function can be called only when stream is in
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After sink is removed from the stream successfully, ownership of the
     * sink is re-claimed by the client.
     *
     * @param  sink
     *      Sink to removed from the stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if sink is removed successfully from the stream.
     *      QTMS_RESULT_NULL_ARGUMENT if sink is a NULL.
     *      QTMS_RESULT_DOES_NOT_EXIST if trying to remove sink that has not
     *      been added to the stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not
     *      in the QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint RemoveSink(QTMSSink* sink);

    /**
     * Set data format for the stream.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After format is added to the stream successfully, ownership of the
     * format is vested in this stream. If the client wants to reuse format,
     * it should call ResetFormat() to re-claim QTMSFormat ownership.
     *
     * Note: In QTMS Ver 1.0.0.0, setting data format for CS call is not
     * required.
     *
     * @param  format
     *      Data format to be set on the stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if format is set successfully on the stream.
     *      QTMS_RESULT_NULL_ARGUMENT if format is a NULL.
     *      QTMS_RESULT_ALREADY_EXIST if format is already set on the stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not
     *      in the QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint SetFormat(QTMSFormat* format);

    /**
     * Remove data format from the stream.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After format is removed from the stream successfully, ownership of
     * the format object is re-claimed by the client.
     *
     * @param  format
     *      Data format to be removed from the stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if format is removed successfully from the
     *      stream.
     *      QTMS_RESULT_NULL_ARGUMENT if format is a NULL.
     *      QTMS_RESULT_DOES_NOT_EXIST if format is not currently set on the
     *      stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not in the
     *      QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint ResetFormat(QTMSFormat* format);

    /**
     * Add an effect to the stream.
     *
     * Multiple effect objects of different types can be added to the stream.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After effect is added to the stream successfully, ownership of the
     * effect is vested in the stream. If the client wants to reuse the effect,
     * it shall call RemoveEffect() to re-claim QTMSEffect ownership.
     *
     * @param  effect
     *      Stream effect to be added to the stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if effect is added successfully to the stream.
     *      QTMS_RESULT_NULL_ARGUMENT if effect is a NULL.
     *      QTMS_RESULT_ALREADY_EXIST if effect type is already added to the
     *      stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not in the
     *      QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint AddEffect(QTMSEffect* effect);

    /**
     * Remove effect from the stream.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * After effect is removed from the stream successfully, ownership of the
     * effect is re-claimed by the client.
     *
     * @param  effect
     *      Stream effect to be removed from the stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if effect is removed successfully from the
     *      stream.
     *      QTMS_RESULT_NULL_ARGUMENT if effect is a NULL.
     *      QTMS_RESULT_DOES_NOT_EXIST if trying to remove an effect that has
     *      not been added to the stream.
     *      QTMS_RESULT_INVALID_STATE if stream is not in the
     *      QTMS_STREAM_UNINITIALIZED state.
     *
     */
    gint RemoveEffect(QTMSEffect* effect);

    /**
     * Get current state of the stream.
     *
     * This function can be called at any time.
     *
     * Possible states are:
     *  QTMS_STREAM_UNINITIALIZED,
     *  QTMS_STREAM_INITIALIZED,
     *  QTMS_STREAM_PAUSED,
     *  QTMS_STREAM_STARTED.

     * @return
     *      Stream's current state.
     *
     */
    gint GetState();

    /**
     * Get stream ID.
     *
     * This function can be called at any time.
     *
     * @return
     *      Unique ID of the stream.
     *
     */
    gint GetStreamId();

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
    gint GetStreamType();

    /**
     * Trigger stream to transition to the initialized state.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * If Init() is called when stream is already in initialized state, the
     * request will be ignored and the function will return QTMS_RESULT_SUCCESS.
     *
     * Upon stream's successful transition to initialized state, the stream will
     * be in the QTMS_STREAM_INITIALIZED state.
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
     *      QTMS_RESULT_SUCCESS if stream transitioned to the initialized state.
     *      QTMS_RESULT_INVALID_STATE if stream has not transitioned to the
     *      QTMS_STREAM_INITIALIZED state.
     *      QTMS_RESULT_FORMAT_TYPE_UNSPECIFIED (IP call only) when stream
     *      has no format attached to it.
     *      QTMS_RESULT_UNINITIALIZED_OBJECT when stream has no sink or source
     *      element attached to it.
     *
     */
    gint Init(gint retrytime = 0);

    /**
     * Trigger stream to transition to the paused state.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_UNINITIALIZED or QTMS_STREAM_STARTED state.
     *
     * If Pause() is called when stream is already in paused state, the
     * request will be ignored and the function will return QTMS_RESULT_SUCCESS.
     *
     * Upon stream's successful transition to the paused state, the stream will
     * be in the QTMS_STREAM_PAUSED state.
     *
     * Note: In QTMS Ver 1.0.0.0, pausing stream for CS call is not supported.
     *
     * @return
     *      Common return codes:
     *      QTMS_RESULT_SUCCESS if stream successfully transitioned to the
     *      paused state.
     *      QTMS_RESULT_INVALID_STATE if stream is not in the
     *      QTMS_STREAM_INITIALIZED or QTMS_STREAM_PAUSED state.
     *
     */
    gint Pause();

    /**
     * Trigger stream to transition to the started state.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_INITIALIZED or QTMS_STREAM_PAUSED state.
     *
     * If Start() is called when stream is already in the started state, the
     * request will be ignored and the function will return QTMS_RESULT_SUCCESS.
     *
     * If Start() is called when stream is already in the initialized state, the
     * stream will implicitly pause, but the observer will only receive one
     * state change callback.
     *
     * Upon stream's successful transition to the started state, the stream will
     * be in the QTMS_STREAM_STARTED state.
     *
     * @param  retrytime
     *      Indicates (in seconds) for how long TMS should attempt to start
     *      a stream in case of an error. When stream starting fails within
     *      specified retry time, TMS will return
     *      QTMS_EVENT_STREAM_STATE_CHANGE_ERROR. If set to 0, TMS will return
     *      QTMS_EVENT_STREAM_STATE_CHANGE_ERROR immediately without retrying.
     *      If set to -1, TMS will keep retrying until user cancels by calling
     *      either Stop() or Deinit().
     *
     * @return
     *      Common return codes:
     *      QTMS_RESULT_SUCCESS if stream successfully transitioned to the
     *      started state.
     *      QTMS_RESULT_INVALID_STATE if stream is not in the
     *      QTMS_STREAM_INITIALIZED or QTMS_STREAM_STARTED state.
     *
     */
    gint Start(gint retrytime = 0);

    /**
     * Trigger stream to transition to the initialized state.
     *
     * This function can be called only when stream is in the
     * QTMS_STREAM_STARTED or QTMS_STREAM_PAUSED state.
     *
     * If Stop() is called when stream is already in the stopped state, the
     * request will be ignored and the function will return QTMS_RESULT_SUCCESS.
     *
     * Upon stream's successful transition to the started state, the stream will
     * be in the QTMS_STREAM_INITIALIZED state.
     *
     * @return
     *      Common return codes:
     *      QTMS_RESULT_SUCCESS if stream successfully transitioned to the
     *      stopped state.
     *      QTMS_RESULT_INVALID_STATE if stream is not in the
     *      QTMS_STREAM_STARTED or QTMS_STREAM_PAUSED state.
     *
     */
    gint Stop();

    /**
     * Trigger stream to transition to un-initialized state.
     *
     * This function can be called only when stream is NOT in
     * QTMS_STREAM_UNINITIALIZED state.
     *
     * If Deinit() is called when stream is already in un-initialized state, the
     * request will be ignored.
     *
     * Upon stream's successful transition to the un-initialized state, the
     * stream will be in the QTMS_STREAM_UNINITIALIZED state.
     *
     */
    void Deinit();

    Q_SIGNALS:
    void TMSStreamEvent(const QTMSStream& stream, QTMSSignalEvent event);

protected:
    /**
     * Constructor
     */
    QTMSStream();

protected:
    TMS::TMSStream *iStream;
};

} //namespace QTMS

#endif // QTMS_STREAM_H
// End of file
