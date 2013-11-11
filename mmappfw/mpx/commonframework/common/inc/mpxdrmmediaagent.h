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
* Description:  Media agent base class
*
*/



#ifndef CMPXDRMMEDIAAGENT_H
#define CMPXDRMMEDIAAGENT_H

#include "mpxdrmmediautility.h"


// DATA TYPES

// FORWARD DECLARATIONS
class CMPXMedia;

/**
* CMPXDrmMediaAgent class declaration
* 
* @lib mpxcommon.lib
* @since S60 3.1
*/
NONSHARABLE_CLASS(CMPXDrmMediaAgent) : public CBase
    {
public:     // New Functions
    /**
    *  Initialize using a filename
    *
    *  @param aFileName file name
    */
    virtual void InitL(const TDesC& aFileName) = 0;

    /**
    *  Initialize using a file handle
    *
    *  @param aFile file handle
    */
    virtual void InitL(RFile& aFile) = 0;

    /**
    *  Gets media object
    *
    *  @param aAttributes attributes to retreive
    *  @return CMPXMedia object with the specified attributes
    */
    virtual const CMPXMedia& GetMediaL(TUint aAttributes) = 0;

    /**
    *  Consumes the rights for the current media according
    *  to the specified consume type
    *
    *  @param aType Type of consumption to execute
    */
    virtual void ConsumeL(TDrmConsumeType aType) = 0;
    
    /**
    *  Closes and uninitializes the agent and cleans up member variables
    *
    *  @return void
    */
    virtual void Close();

protected:    // New methods
    /**
    * Creates a new media object
    */
    virtual void CreateMediaL();

    /**
    * Gets the protected attribute if not already obtained
    */
    virtual void GetProtectedL();

protected:    // Data
    CMPXMedia* iMedia;  // owned
    CData* iData;   // owned
    };

#endif // CMPXDRMMEDIAAGENT_H
