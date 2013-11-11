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

#ifndef QTMS_CALL_H
#define QTMS_CALL_H

#include <qtms.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSCall;
}

namespace QTMS
{

// FORWARD DECLARATIONS
class QTMSStream;

/**
 * QTMSCall class
 *
 * This class instantiates QTMS call object of one of the following types:
 *  QTMS_CALL_CS
 *  QTMS_CALL_ECS
 *  QTMS_CALL_IP
 *
 * Each QTMS call contains at least 1 stream (uplink or downlink).
 *
 * Usage:
 * <code>
 * QTMSFactory *iFactory;
 * QTMSCall *iCall;
 * QTMSStream *iUplink;
 * QTMSStream *iDownlink;
 * gint err;
 * err = QTMSFactory::CreateFactory(iFactory);
 * err = iFactory->CreateCall(QTMS_CALL_IP, iCall);
 * err = iCall->CreateStream(QTMS_STREAM_UPLINK, iUplink);
 * err = iCall->CreateStream(QTMS_STREAM_DOWNLINK, iDownlink);
 * ...
 * err = iCall->DeleteStream(iDownlink);
 * err = iCall->DeleteStream(iUplink);
 * err = iFactory->DeleteCall(iCall);
 * delete iFactory;
 * ...
 * </code>
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSCall : public QObject
{
    Q_OBJECT
public:
    /**
     * Destructor
     *
     */
    virtual ~QTMSCall();

    /**
     * Return call type.
     *
     * This function can be called at any time.
     *
     * Possible call types are as follows:
     *  QTMS_CALL_CS,
     *  QTMS_CALL_ECS
     *  QTMS_CALL_IP
     *
     * @return
     *      Call type as indicated above.
     *
     */
    QTMSCallType GetCallType();

    /**
     * Get call context ID.
     *
     * This ID is passed during the creation of the call object.
     *
     * @param  ctxid
     *      Context ID.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_FATAL_ERROR if an error occured.
     *
     */
    gint GetCallContextId(guint& ctxid);

    /**
     * Create QTMS stream of the given type.
     *
     * @param  type
     *      Stream type to be created.
     *
     * @param  strm
     *      Created stream.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INSUFFICIENT_MEMORY if call creation failed due to
     *      insufficient memory.
     *      QTMS_RESULT_STREAM_TYPE_NOT_SUPPORTED if stream type is not
     *      supported.
     *      QTMS_RESULT_FATAL_ERROR if an error occured.
     *      QTMS_REASON_EMERGENCY_CALL_ONGOING if emergency call is active.
     *      QTMS_REASON_PERMISSION_DENIED if permission is denied.
     *
     */
    gint CreateStream(const QTMSStreamType type, QTMSStream*& strm);

    /**
     * Delete stream object.
     *
     * @param  strm
     *      Stream to be deleted.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INVALID_ARGUMENT if the stream is not valid.
     */
    gint DeleteStream(QTMSStream*& strm);

protected:
    /**
     * Constructor
     */
    QTMSCall();

protected:
    TMS::TMSCall* iTmsCall;
};

} //namespace QTMS

#endif //QTMS_CALL_H
// End of file

