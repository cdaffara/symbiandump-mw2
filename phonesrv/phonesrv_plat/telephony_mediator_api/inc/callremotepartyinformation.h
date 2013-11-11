/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call remote party information
*
*/


#ifndef CALLREMOTEPARTYINFORMATION_H
#define CALLREMOTEPARTYINFORMATION_H

#include <e32base.h>
#include <mcallremotepartyinfo.h> 

class RWriteStream;

/**  The category UID for the messages in this header file. */
const TUid KCatCallRemotePartyInformation = { 0x10207906 }; 

/**  Version number of this API */
const TInt KCallRemotePartyInformationVersionMajor( 1 );
const TInt KCallRemotePartyInformationVersionMinor( 0 );
const TInt KCallRemotePartyInformationVersionBuild( 0 );

/**
 * Holds single calls remote party information
 * 
 * @since S60 v5.1
 */
NONSHARABLE_CLASS( CCallRemotePartyInfo ) : public CBase, public MCallRemotePartyInfo
    {
public:
    
    inline static CCallRemotePartyInfo* NewL();
    inline static CCallRemotePartyInfo* NewLC();
    inline ~CCallRemotePartyInfo();
    
    inline void ExternalizeL( RWriteStream& aStream ) const;
    inline void InternalizeL( RReadStream& aStream );
    
    // Size of the descriptor required for this information
    // to be externalized
    inline TInt Size() const;
    
    inline void SetAddressL( const TDesC& aAddress );
    inline void SetMatchedNameL( const TDesC& aName );
    inline void SetNameFromNetworkL( const TDesC& aName );
    inline void SetCompanyNameL( const TDesC& aName );
    inline void SetCallIndex( TInt aCallIndex );
    inline void SetRemoteIdentity( TRemoteIdentityStatus aRemoteIdentity );
    inline void SetSATCall( TBool aSatCall );
    
// From base class MCallRemotePartyInfo
    inline const TDesC& Address() const;
    inline const TDesC& MatchedName() const;
    inline const TDesC& NameFromNetwork() const;
    inline const TDesC& CompanyName() const;    
    inline TInt CallIndex() const;  
    inline TRemoteIdentityStatus RemoteIdentity() const;
    inline TBool SATCall() const;
    inline TBool operator==(const MCallRemotePartyInfo& aRemoteInfo) const;
    
private:
    
    inline static HBufC* ReadDescriptorFromStreamLC( RReadStream& aStream );
    inline static void WriteDescriptorToStreamL( 
        RWriteStream& aStream,
        const TDesC& aDesc );
    
    inline CCallRemotePartyInfo();
    inline void ConstructL();
    
private: // Data
    
    HBufC* iMatchedName;
    HBufC* iNameFromNetwork;
    HBufC* iCompanyName;
    HBufC* iAddress;
    TInt iCallIndex;
    TBool iSATCall;
    TRemoteIdentityStatus iRemoteIdentity;
    };
    
/**
 * Holds all calls remote party information.
 *
 * @since S60 v5.1
 */
NONSHARABLE_CLASS( CCallRemotePartyInfos ) : public CBase
    {
public:
    
    inline static CCallRemotePartyInfos* NewL();
    inline static CCallRemotePartyInfos* NewLC();
    inline ~CCallRemotePartyInfos();
    
    /**
     * Adds remote party information
     * @param aInfo Information to be added. Ownership is taken. Must not be NULL.
     */
    inline void AddL( CCallRemotePartyInfo* aInfo );
    
    /**
     * Access to remote party infos
     */
    inline const RPointerArray<CCallRemotePartyInfo>& RemotePartyInfos() const;
    
    /**
     * Externalizes the remote party information to descriptor.
     */
    inline HBufC8* ExternalizeL() const;
    
    /**
     * Internalizes remote party information from data. Note that
     * the existing infos are removed before internalizing. So if leave
     * occurs the old data is lost.
     */
    inline void InternalizeL( const TDesC8& aData ); 
    
private:
    
    inline CCallRemotePartyInfos();
    
private: // Data
    
    RPointerArray<CCallRemotePartyInfo> iInfos;
    };

  
/**
 * Commands
 */
enum TCallRemotePartyInformationCommands
    {
    EGetCallRemotePartyInformation = 0
    };
    
/**
 * Events
 */
enum TCallRemotePartyInformationEvents
    {
    EChangesInRemotePartyInformation = 0
    };
   
#include <callremotepartyinformation.inl>



#endif // CALLREMOTEPARTYINFORMATION_H
