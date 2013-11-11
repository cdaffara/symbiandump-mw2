/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call state information
*
*/


#ifndef CALLINFORMATION_H
#define CALLINFORMATION_H

#include <e32base.h>
#include <mcall.h>

/**  The category UID for the messages in this header file. */
const TUid KCatCallInformation = { 0x102078FD }; 

/**  Version number of this API */
const TInt KCallInformationVersionMajor( 1 );
const TInt KCallInformationVersionMinor( 0 );
const TInt KCallInformationVersionBuild( 0 );
  
#include <e32cmn.h>

/**
 *  Holds single calls information.
 *
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( TCallInfo ) : public MCall
    {
public:
    
    TInt CallIndex() const { return iIndex; }   
    
    CCPCall::TCallType CallType() const { return iType; }

    CCPCall::TCallState CallState() const { return iState; }
    
    TUint32 ServiceId() const { return iServiceId; }

    CCPCall::TCallDirection CallDirection() const { return iDirection; }
    
    TBool IsEmergency() const { return iEmergencyCall; }
    
    CCPCall::TCallState iState;
    CCPCall::TCallType iType;
    CCPCall::TCallDirection iDirection;
    TInt iIndex;    
    TUint32 iServiceId;
    TBool iEmergencyCall;
    };


/**
 *  Class holding information about calls.
 *  Can be packed for IPC purposes. 
 *  
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( CCallInfos ) : public CBase 
    {
public:
    
    /**
     * Two-phased constructor.     
     */
    static CCallInfos* NewL();
    
    /**
     * Two-phased constructor.     
     */
    static CCallInfos* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CCallInfos();
    
    /**
     * Adds information about a call.  
     *
     * @since S60 v5.1
     * @param aCallInfo Call information to be added.    
     */
    inline void AddL( TCallInfo& aCallInfo );
    
    /**
     * Returns information about all calls.
     *
     * @since S60 v5.1      
     * @return Reference to call info array
     */    
    inline const RPointerArray<TCallInfo>& CallInfos() const;
    
    /**
     * Packages this class for IPC transfer.
     *
     * @since S60 v5.1
     * @return Pointer to descriptor holding the class.
     */
    inline HBufC8* ExternalizeL() const;
    
    /**
     * Intializes the class from descriptor
     *
     * @since S60 v5.1
     * @param aCallInfos Reference to descriptor containg the class data.   
     */    
    inline void InternalizeL( const TDesC8& aCallInfos );
        
    
private:

    inline CCallInfos();
    
    
private:    
    RPointerArray<TCallInfo> iCallInfos;
    };

/**
 * Commands
 */
enum TCallInformationCommands
    {
    EGetAllCallStates = 0
    };
    
/**
 * Events
 */
enum TCallInformationEvents
    {
    EChangesInCallStates = 0
    };


#include "ccallinfos.inl"

#endif // CALLINFORMATION_H
