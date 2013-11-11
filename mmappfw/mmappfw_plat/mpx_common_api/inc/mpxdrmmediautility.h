/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Encapsulates DRM Media
*
*/



#ifndef CMPXDRMMEDIAUTILITY_H
#define CMPXDRMMEDIAUTILITY_H

#include <e32def.h>
#include <e32std.h>
#include <f32file.h>
#include <caf/content.h>
using namespace ContentAccess;

// CONSTANTS
_LIT( KMPXWDRMCafAgentName, "PV WMA CAF Agent" );
_LIT( KMPXOMACafAgentName, "OMA DRM Agent" );
_LIT( KMPXS60WMDRMCafAgent, "WM DRM Agent");

enum TDrmConsumeType
    {
    EMPXDrmConsumeNone,
    EMPXDrmConsumeStart,
    EMPXDrmConsumePause,
    EMPXDrmConsumeContinue,
    EMPXDrmConsumeFinish,
    EMPXDrmConsumeStartOrContinue
    };

// FORWARD DECLARATION
class CMPXMedia;
class CMPXDrmMediaAgent;


/**
* CMPXDrmMediaUtility class declaration.
* 
* @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(CMPXDrmMediaUtility) : public CBase
    {
private:
    enum TMPXDrmMediaAgents
        {
        EOmaDrmAgent,
        EWmaDrmAgent
        };

public:     // Constructors and Destructors
    /**
    *  Two-phase constructor.
    *
    *  @since S60 3.2.3
    *  @return object created
    */
    IMPORT_C static CMPXDrmMediaUtility* NewL();

    /**
    *  Two-phase constructor.
    *
    *  @since S60 3.2.3
    *  @return object created
    */
    IMPORT_C static CMPXDrmMediaUtility* NewLC();

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXDrmMediaUtility();
    
public:     // New Functions
    /**
    *  Initialize utility using a filename.
    *
    *  @since S60 3.2.3
    *  @param aFileName file name
    */
    IMPORT_C void InitL(const TDesC& aFileName);

    /**
    *  Initialize utility using a file handle.
    *
    *  @since S60 3.2.3
    *  @param aFile file handle
    */
    IMPORT_C void InitL(RFile& aFile);
    
    /**
    *  Gets media object with the given attributes.
    *
    *  @since S60 3.2.3
    *  @param aAttributes attributes to retreive
    *  @return CMPXMedia object with the specified attributes
    *          NULL if the utility has not been initialized with a DRM file
    */
    IMPORT_C const CMPXMedia* GetMediaL(TUint aAttributes);
    
    /**
    *  Consumes the rights for the current media according
    *  to the specified consume type.
    *
    *  @since S60 3.2.3
    *  @param aType Type of consumption to execute
    */
    IMPORT_C void ConsumeL(TDrmConsumeType aType);
    
    /**
    * Closes and unitializes the utility.
    *
    * @since S60 3.2.3
    * @return void
    */
    IMPORT_C void Close();
    
private:    // Constructors
    /**
    *  C++ default constructor.
    *
    *  @since S60 3.2.3
    */
    CMPXDrmMediaUtility();
    
    /**
    * 2nd phase contructor.
    *
    * @since S60 3.2.3
    */
    void ConstructL();
    
private:    // New Functions
    /**
    * Finds an agent to handle the content.
    * 
    * @since S60 3.2.3
    * @param aContent content to find agent for
    */
    void FindAgent( CContent& aContent );

private:  // New Functions

private:  // Data
    // Array of drm agents to use
    RPointerArray<CMPXDrmMediaAgent> iAgents;
    CMPXDrmMediaAgent* iCurrentAgent;
    TBool iConsumeStarted;
    CMPXMedia* iDefaultMedia;
    RFs iFs;
    };

#endif // CMPXDRMMEDIAUTILITY_H
