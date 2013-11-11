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

#ifndef TMS_CALL_PROXY_H
#define TMS_CALL_PROXY_H

// INCLUDES
#include <e32base.h>
#include <tms.h>

namespace TMS {

// CONSTANTS

// FORWARD DECLARATIONS
class TMSProxy;
class TMSBuffer;

// TMSCallProxy class
class TMSCallProxy : public RSessionBase
    {
public:
    IMPORT_C TMSCallProxy();
    IMPORT_C virtual ~TMSCallProxy();
    IMPORT_C gint Connect();
    IMPORT_C gint CreateCall(const gint calltype);
    IMPORT_C gint CreateStream(const TMSCallType callType,
            const TMSStreamType strmType, gint& strmId);
    IMPORT_C gint InitStream(const TMSCallType callType,
            const TMSStreamType strmType, gint strmId,
            const TMSFormatType frmtType, RHandleBase* msgQueue,
            const gint retrytime);
    IMPORT_C gint StartStream(const TMSCallType callType,
            const TMSStreamType strmType, gint strmId, const gint retrytime);
    IMPORT_C gint PauseStream(const TMSCallType callType,
            const TMSStreamType strmType, gint strmId);
    IMPORT_C gint StopStream(const TMSCallType callType,
            const TMSStreamType strmType, gint strmId);
    IMPORT_C gint DeinitStream(const TMSCallType callType,
            const TMSStreamType strmType, gint strmId);
    IMPORT_C gint DeleteStream(const TMSCallType callType,
            const TMSStreamType strmType, gint& strmId);
    IMPORT_C void DeleteCall();
    IMPORT_C void Close();

    IMPORT_C gint BufferEmptied(TMSCallType callType,
            TMSStreamType strmType, gint strmId, TMSBuffer& buffer);
    IMPORT_C gint BufferFilled(TMSCallType callType,
            TMSStreamType strmType, gint strmId, TMSBuffer& buffer);
    IMPORT_C gint GetMaxVolume(guint& aVol);
    IMPORT_C gint SetVolume(const guint aVol);
    IMPORT_C gint GetVolume(guint& aVol);
    IMPORT_C gint GetMaxGain(guint& aGain);
    IMPORT_C gint SetGain(const guint aGain);
    IMPORT_C gint GetGain(guint& aGain);

    IMPORT_C gint GetSupportedBitRates(BitRateVector& aVector);
    IMPORT_C gint SetBitRate(const guint aBitrate);
    IMPORT_C gint GetBitRate(guint& aBitrate);
    IMPORT_C gint GetFormatCodecMode(const TMSFormatType aFmtType,
            const TMSStreamType aStrmType, gint& aMode);
    IMPORT_C gint SetFormatCodecMode(const TMSFormatType aFmtType,
            const TMSStreamType aStrmType, gint aMode);
    IMPORT_C gint GetCNG(const TMSFormatType aFmtType, gboolean& aCng);
    IMPORT_C gint SetCNG(const TMSFormatType aFmtType, gboolean aCng);
    IMPORT_C gint GetPlc(const TMSFormatType aFmtType, gboolean& aPlc);
    IMPORT_C gint SetPlc(const TMSFormatType aFmtType, gboolean aPlc);
    IMPORT_C gint GetVADMode(const TMSFormatType fmttype, gboolean& vad);
    IMPORT_C gint SetVADMode(const TMSFormatType fmttype, gboolean vad);

    IMPORT_C gint GetDataXferChunkHandle(TMSCallType callType,
            TMSStreamType strmType, gint strmId, guint32 key);

private:
    void GetSupportedBitRatesL(BitRateVector& aVector);

private:
    TMSProxy* iTMSProxy;
    };

} //namespace TMS

#endif // TMS_CALL_PROXY_H

// End of file
