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
* Description:  
*
*/


#include "mmmtpdp_variant.hrh"

#ifndef CMMMTPDPPERFLOG_H
#define CMMMTPDPPERFLOG_H

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
#define PERFLOGSTART(AAA)                       iPerfLog->Start(AAA);
#define PERFLOGSTOP(AAA)                        iPerfLog->Stop(AAA);
#else
#define PERFLOGSTART(AAA)
#define PERFLOGSTOP(AAA)
#endif // defined(_DEBUG) || defined(MMMTPDP_PERFLOG)

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
#include <e32base.h>
#include <badesca.h>

// forward declaration
class CMmMtpDpPerfLog;

class TMmMtpDpOverflowHandler : public TDesOverflow
    {
public:
    inline void SetOwner( CMmMtpDpPerfLog* aOwner );
    inline void Overflow( TDes& aDes );

private:
    CMmMtpDpPerfLog* iOwner;

    };

class CMmMtpDpPerfLog : public CBase
    {
public:

    inline static CMmMtpDpPerfLog* NewL( const TDesC& aTitle );
    inline ~CMmMtpDpPerfLog();

    inline void Write( const TDesC& aText );
    inline void WriteFormat( TRefByValue<const TDesC> aFmt, ... );

    inline void Start( const TDesC& aDescription );
    inline void Stop( const TDesC& aDescription );

private:

    inline CMmMtpDpPerfLog();
    inline void ConstructL( const TDesC& aTitle );

private:

    TMmMtpDpOverflowHandler iOverflowHandler;

    HBufC16* iTitle;
    CDesC16ArrayFlat iDescription;
    CArrayFixFlat<TUint32> iStartTick;
    CArrayFixFlat<TUint64> iTotalTime;
    CArrayFixFlat<TUint32> iTotalUsage;

    TInt iNTickPeriod;
    TInt iLastIndex;

    };

#include "cmmmtpdpperflog.inl"

#endif // defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
#endif // CMMMTPDPPERFLOG_H
