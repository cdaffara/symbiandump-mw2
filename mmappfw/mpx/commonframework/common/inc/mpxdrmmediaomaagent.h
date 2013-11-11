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
* Description:  OMA DRM media agent
*
*/



#ifndef CMPXDRMMEDIAOMAAGENT_H
#define CMPXDRMMEDIAOMAAGENT_H

#include "mpxdrmmediaagent.h"
#ifdef CAMESE_IN_DRM_UTILITY
#include <drmutilitytypes.h>
#endif

// FORWARD DECLARATIONS
class CMPXMedia;
#ifdef CAMESE_IN_DRM_UTILITY
class CDRMRightsConstraints;
class DRMCommon;

namespace DRM 
	{
	class CDrmUiHandling;
	}
#else
class CDRMHelper;
class CDRMHelperRightsConstraints;
#endif

/**
* CMPXDrmMediaOmaAgent class declaration
* 
* @lib mpxcommon.lib
* @since S60 3.1
*/
NONSHARABLE_CLASS(CMPXDrmMediaOmaAgent) : public CMPXDrmMediaAgent
    {
public:     // Constructors and Destructors
    /**
    *  Two-phase constructor
    *
    *  @return object created
    */
    static CMPXDrmMediaOmaAgent* NewL();

    /**
    *  Two-phase constructor
    *
    *  @return object created
    */
    static CMPXDrmMediaOmaAgent* NewLC();

    /**
    *  Destructor
    */
    virtual ~CMPXDrmMediaOmaAgent();

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
    CMPXDrmMediaOmaAgent();

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
    * Gets the rights details
    */
    void GetRightsDetailsL();

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
    * Gets the interval start time attribute if not already obtained
    */
    void GetIntervalStartTimeL();

    /**
    * Gets the accumlated time attribute if not already obtained
    */
    void GetAccumulatedTimeL();

    /**
    * Gets the can set automated attribute if not already obtained
    */
    void GetCanSetAutomatedL();

    /**
    * Gets the info url attribute if not already obtained
    */
    void GetHasInfoUrlL();

    /**
    * Gets the preview url attribute if not already obtained
    */
    void GetHasPreviewUrlL();
    
    /**
    * Gets whether the DRM object is about to expire or not
    */
    void GetAboutToExpireL();

private:    // Data
#ifdef CAMESE_IN_DRM_UTILITY
    DRM::CDrmUiHandling* iDrmHandler;       // owned
    DRMCommon* iDRMCommon;					// owned
    CDRMRightsConstraints* iRightsConstraints;    // owned
#else
    CDRMHelper* iDrmHelper; // owned
    CDRMHelperRightsConstraints* iRightsConstraints;    // owned
#endif

    RFile iFile;    // owned (duplicate)
    RFs iFs;
    };


#endif // CMPXDRMMEDIAOMAAGENT_H
