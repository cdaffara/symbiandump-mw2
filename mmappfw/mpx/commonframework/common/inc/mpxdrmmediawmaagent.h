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
* Description:  Wma DRM media agent
*
*/



#ifndef CMPXDRMMEDIAWMAAGENT_H
#define CMPXDRMMEDIAWMAAGENT_H

#include <caf/data.h>
#include <caf/manager.h>
#include <caf/rightsmanager.h>
#include "mpxdrmmediaagent.h"


// DATA TYPES
enum TMPXWmaDrmLicenseType
    {
    EMPXWmaDrmUnlimited,
    EMPXWmaDrmTime,
    EMPXWmaDrmCount,
    EMPXWmaDrmDuration,
    EMPXWmaDrmTimeCount
    };

// FORWARD DECLARATIONS
class CMPXMedia;

/**
* CMPXDrmMediaWmaAgent class declaration
* 
* @lib mpxcommon.lib
* @since S60 3.1
*/
NONSHARABLE_CLASS(CMPXDrmMediaWmaAgent) : public CMPXDrmMediaAgent
    {
public:     // Constructors and Destructors
    /**
    *  Two-phase constructor
    *
    *  @return object created
    */
    static CMPXDrmMediaWmaAgent* NewL();

    /**
    *  Two-phase constructor
    *
    *  @return object created
    */
    static CMPXDrmMediaWmaAgent* NewLC();

    /**
    *  Destructor
    */
    virtual ~CMPXDrmMediaWmaAgent();

public:     // New Functions
    /**
    *  From MMPXDrmMediaAgent
    *  Initialize using a filename
    *
    *  @param aFileName file name
    */
    void InitL(const TDesC& aFileName);

    /**
    *  From MMPXDrmMediaAgent
    *  Initialize using a file handle
    *
    *  @param aFile file handle
    */
    void InitL(RFile& aFile);

    /**
    *  From MMPXDrmMediaAgent
    *  Gets media object
    *
    *  @param aAttributes attributes to retreive
    *  @return CMPXMedia object with the specified attributes
    */
    const CMPXMedia& GetMediaL(TUint aAttributes);

    /**
    *  From MMPXDrmMediaAgent
    *  Consumes the rights for the current media according
    *  to the specified consume type
    *
    *  @param aType Type of consumption to execute
    */
    void ConsumeL(TDrmConsumeType aType);

    /**
    *  Closes and uninitializes the agent and cleans up member variables
    *
    *  @return void
    */
    void Close();

private:    // Constructors
    /**
    *  C++ default constructor.
    */
    CMPXDrmMediaWmaAgent();

    /**
    * 2nd phase contructor
    */
    void ConstructL();
    
private:    // New Functions
    /**
    * Creates a new media object
    */
    void CreateMediaL();

    /**
    * Gets the rights status
    */
    void GetRightsStatusL();

    /**
    * Gets the rights type attribute if not already obtained
    */
    void GetRightsTypeL();

    /**
    * Gets the count attribute if not already obtained
    */
    void GetCountL();

    /**
    * Gets the start time attribute if not already obtained
    */
    void GetStartTimeL();

    /**
    * Gets the end time attribute if not already obtained
    */
    void GetEndTimeL();

    /**
    * Gets the interval attribute if not already obtained
    */
    void GetIntervalL();

    /**
    * Gets the sending allowed attribute if not already obtained
    */
    void GetSendingAllowedL();

    /**
    * Gets the can set automated attribute if not already obtained
    */
    void GetCanSetAutomatedL();
    
    /**
    * Get Rights Description for WMDRM protected files
    * @return Error code:
    *           KErrNone:       Description returned.
    *           KErrNotFound:   No description available.
    **/
    TInt GetWmdrmRightsDescriptionL();

    /**
    * Retrieve and format TTime for WMDRM protected files
    * @param aTimeTypeDelimeter Time delimeter, starttime or endtime.
    * @param aTime Time.
    * @return Error code:
    *           KErrNone:       Time returned.
    *           KErrNotFound:   Not time available for given delimeter.
    **/
    TInt GetWmdrmTimeL( const TDesC& aTimeTypeDelimeter, TTime& aTime );

    /**
    * Gets whether the DRM object is about to expire or not
    */
    void GetAboutToExpireL();

private:    // Data
    HBufC* iFileName;   // owned
    CManager* iManager; // owned
    CRightsManager* iRightsManager; // owned
    HBufC* iRightsDescription;  // owned
    };


#endif // CMPXDRMMEDIAWMAAGENT_H
