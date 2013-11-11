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

#ifndef TMS_CALL_H
#define TMS_CALL_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSStream;
class TMSCallBody;

/**
 * TMSCall class
 *
 * This class instantiates TMS call object of one of the following types:
 *  TMS_CALL_CS
 *  TMS_CALL_ECS
 *  TMS_CALL_IP
 *
 * Each TMS call contains at least 1 stream (uplink or downlink).
 *
 * Usage:
 * <code>
 * TMSFactory *iFactory;
 * TMSCall *iCall;
 * TMSStream *iUplink;
 * TMSStream *iDownlink;
 * gint err;
 * err = TMSFactory::CreateFactory(iFactory);
 * err = iFactory->CreateCall(TMS_CALL_IP, iCall);
 * err = iCall->CreateStream(TMS_STREAM_UPLINK, iUplink);
 * err = iCall->CreateStream(TMS_STREAM_DOWNLINK, iDownlink);
 * ...
 * err = iCall->DeleteStream(iDownlink);
 * err = iCall->DeleteStream(iUplink);
 * err = iFactory->DeleteCall(iCall);
 * delete iFactory;
 * ...
 * </code>
 *
 * @lib tmsapi.lib
 *
 */
class TMSCall
    {
public:
    /**
     * Destructor
     *
     */
    IMPORT_C virtual ~TMSCall();

    /**
     * Return call type.
     *
     * This function can be called at any time.
     *
     * Possible call types are as follows:
     *  TMS_CALL_CS,
     *  TMS_CALL_ECS
     *  TMS_CALL_IP
     *
     * @return
     *      Call type as indicated above.
     *
     */
    IMPORT_C TMSCallType GetCallType();

    /**
     * Get call context ID.
     *
     * This ID is passed during the creation of the call object.
     *
     * @param  ctxid
     *      Context ID.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_FATAL_ERROR if an error occured.
     *
     */
    IMPORT_C gint GetCallContextId(guint& ctxid);

    /**
     * Create TMS stream of the given type.
     *
     * @param  type
     *      Stream type to be created.
     *
     * @param  strm
     *      Created stream.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INSUFFICIENT_MEMORY if call creation failed due to
     *      insufficient memory.
     *      TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED if stream type is not
     *      supported.
     *      TMS_RESULT_FATAL_ERROR if an error occured.
     *      TMS_REASON_EMERGENCY_CALL_ONGOING if emergency call is active.
     *      TMS_REASON_PERMISSION_DENIED if permission is denied.
     *      TMS_RESULT_INVALID_ARGUMENT if strm is not set to NULL.
     *      TMS_RESULT_ALREADY_EXIST if the same streamtype is created
     *      multiple times.
     *
     */
    IMPORT_C gint CreateStream(const TMSStreamType type, TMSStream*& strm);

    /**
     * Delete stream object.
     *
     * @param  strm
     *      Stream to be deleted.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INVALID_ARGUMENT if the stream is not valid.
     */
    IMPORT_C gint DeleteStream(TMSStream*& strm);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSCall();

protected:
    TMSCallBody* iBody;
    };

} //namespace TMS

#endif //TMS_CALL_H

// End of file

