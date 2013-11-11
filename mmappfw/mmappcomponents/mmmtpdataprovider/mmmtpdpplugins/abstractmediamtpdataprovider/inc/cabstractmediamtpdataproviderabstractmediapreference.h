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


#ifndef CABSTRACTMEDIAMTPDATAPROVIDERABSTRACTMEDIAPREFERENCE_H
#define CABSTRACTMEDIAMTPDATAPROVIDERABSTRACTMEDIAPREFERENCE_H

// INCLUDES
#include "crequestprocessor.h"

// CLASS DECLARATION
class MMmMtpDpConfig;

enum TMTPAbstractMediaReference
    {
    EMTPAbstractMediaRefOrdinary,
    EMTPAbstractMediaRefAll
    };

/**
* Defines abstract media data provider WMPReportSyncOnlyPlaylists request processor
*/
class CAbstractMediaMtpDataProviderAbstractMediaPreference : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    */
    static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Two-phase construction method
    */
    static CAbstractMediaMtpDataProviderAbstractMediaPreference* NewLC( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );

    /**
    * Destructor.
    */
    ~CAbstractMediaMtpDataProviderAbstractMediaPreference();

protected:
    // from CRequestProcessor
    void ServiceL();

private:
    /**
    * Standard c++ constructor
    */
    CAbstractMediaMtpDataProviderAbstractMediaPreference( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );

    /**
    * EPOC default constructor for performing 2nd stage construction
    */
    void ConstructL();

    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERABSTRACTMEDIAPREFERENCE_H
