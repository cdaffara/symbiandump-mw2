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
* Description:    OMA Presence Settings Device Management Adapter
*
*/




#ifndef __PRESDMADAPTER_H__
#define __PRESDMADAPTER_H__

// Non-local includes
#include <e32base.h>
#include <smldmadapter.h>
#include <pressettingsapi.h>

// LITERALS
_LIT8(KPresDMDDFVersion, "1.0");

_LIT8(KPresDMIdentifier, "org.openmobilealliance/1.0/PRESENCE");
_LIT8(KPresDMNodeName, "OMA_PRESENCE");

// OMA Presence DM leaves
_LIT8(KPresDMName, "NAME");
_LIT8(KPresDMProviderId, "PROVIDER-ID");
_LIT8(KPresDMClientObjDataLim, "CLIENT-OBJ-DATA-LIMIT");
_LIT8(KPresDMContentSrvUri, "CONTENT-SERVER-URI");
_LIT8(KPresDMSrcThPublish, "SOURCE-THROTTLE-PUBLISH");
_LIT8(KPresDMMaxPresSubs, "MAX-NUMBER-OF-PRESENCE-SUBSCRIPTIONS");
_LIT8(KPresDMMaxPresSubsInLists, "MAX-NUMBER-OF-SUBSCRIPTIONS-IN-PRESENCE-LIST");
_LIT8(KPresDMServiceUriTemp, "SERVICE-URI-TEMPLATE");

// For holding additional S60 Presence leaves
_LIT8(KPresDMToConRef, "ToConRef");
_LIT8(KPresDMConRef, "ConRef");

// S60 additional leaves
_LIT8(KPresDMSIPProfile, "SIP-PROFILE");
_LIT8(KPresDMXDMSet, "XDM-SET");

// Descriptions
_LIT8(KPresDMNodeNameDescription, "This interior node acts as a placeholder for one or more accounts for a fixed node" );
_LIT8(KPresDMNameDescription, "User displayable name for the presence enabler");
_LIT8(KPresDMProviderIdDescription, "An identifier for the Presence application service access point");
_LIT8(KPresDMClientObjDataLimDescription, "Limits the size of the MIME object data to the specified value in bytes when PUBLISH and NOTIFY methods are used in the presence client");
_LIT8(KPresDMContentSrvUriDescription, "Defines the HTTP URI of the content server to be used for content indirection");
_LIT8(KPresDMSrcThPublishDescription, "Defines the minimum time interval (in seconds) between two consecutive publications from a Presence source");
_LIT8(KPresDMMaxPresSubsDescription, "Limits the total number of presence subscriptions");
_LIT8(KPresDMMaxPresSubsInListsDescription, "Limits the number of back-end subscriptions allowed for presence list");
_LIT8(KPresDMServiceUriTempDescription, "Defines the syntax of the service URI, in the form of a URI Template as specified in XDM Core Specification [XDMSPEC] subclause C.1");

_LIT8(KPresDMToConRefDescription, "Refers to a collection of connectivity definitions" );
_LIT8(KPresDMConRefDescription, "Indicates the linkage to connectivity parameters" );

_LIT8(KPresDMSIPProfileDescription, "SIP profile used");
_LIT8(KPresDMXDMSetDescription, "XDM settings set used");

_LIT8(KPresDmDynamicDescription, "Placeholder for one or more connectivity parameters");


// other constants
_LIT8(KPresDMTextPlain, "text/plain" );
_LIT8(KPresDMSeparator, "/" );
_LIT( KPresDMOpenBrack,    "(" );
_LIT( KPresDMCloseBrack,   ")" );

_LIT8(KPresDMSipLinkPrefix, "./SIP");
_LIT8(KPresDMSipIdPrefix, "SIPId");

_LIT8(KPresDMSipDMNode, "SIP"); // as specified in OMA SIP
_LIT8(KPresDMXdmDMNode, "OMA_XDM"); // as specified in OMA XDM

// CONSTANTS
const TInt KPresDMLogBufferMaxSize    = 2000;
const TInt KPresDMDefaultResultSize = 255;
const TInt KPresDMLevel = 2;
const TInt KPresDMHexLength = 8;
const TUint8 KPresDMObjectSizeWidth = 4;
const TUint8 KPresDMSeparatorDef = 0x2f; // forward slash
const TInt KPresDMMaxIntLength = 10;     // max length of 32bit integer
const TInt KPresSipIdLinkLength = 19;

/**
* The main class of the OMA Presence DM adapter. Handles
* requests of fetching and updating settings sets and 
* settings items.
*
* @since S60 3.2
*/
class CPresenceDMAdapter : public CSmlDmAdapter
    {
    public:
    
        /**
        * Symbian first phase constructor
        *
        * @since S60 3.2
        * @param MSmlDmCallback* aDmCallback
        * @return pointer to newly created CPresenceDMAdapter
        */
        static CPresenceDMAdapter* NewL( MSmlDmCallback* aDmCallback );

        /**
        * Destructor
        */
        ~CPresenceDMAdapter();
        
        // Adapter interface from CSmlDmAdapter, see CSmlDmAdapter
        void DDFVersionL(CBufBase& aDDFVersion);
        void DDFStructureL(MSmlDmDDFObject& aDDF);
        void UpdateLeafObjectL(const TDesC8& aUri, const TDesC8& aLUID, 
                                const TDesC8& aObject, const TDesC8& aType, 
                                TInt aStatusRef);
        void DeleteObjectL(const TDesC8& aUri, const TDesC8& aLUID, 
                            const TInt aStatusRef); 
        void FetchLeafObjectL(const TDesC8& aUri, const TDesC8& aLUID, 
                               const TDesC8& aType, const TInt aResultsRef, 
                               const TInt aStatusRef); 
        void ChildURIListL(const TDesC8& aUri, const TDesC8& aLUID, 
                            const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList, 
                            const TInt aResultsRef, const TInt aStatusRef);
        void AddNodeObjectL(const TDesC8& aUri, const TDesC8& aParentLUID, 
                             const TInt aStatusRef);
        void UpdateLeafObjectL(const TDesC8& aUri, const TDesC8& aLUID, 
                                RWriteStream*& aStream, const TDesC8& aType, 
                                const TInt aStatusRef);
        void FetchLeafObjectSizeL(const TDesC8& aUri, const TDesC8& aLUID, 
                                   const TDesC8& aType, const TInt aResultsRef, 
                                   const TInt aStatusRef);
        void ExecuteCommandL(const TDesC8& aUri, const TDesC8& aLUID, 
                              const TDesC8& aArgument, const TDesC8& aType, 
                              const TInt aStatusRef);
        void ExecuteCommandL(const TDesC8& aUri, const TDesC8& aLUID, 
                              RWriteStream*& aStream, const TDesC8& aType, 
                              const TInt aStatusRef);
        void CopyCommandL(const TDesC8& aTargetURI, const TDesC8& aTargetLUID, 
                           const TDesC8& aSourceURI, const TDesC8& aSourceLUID, 
                           const TDesC8& aType, TInt aStatusRef);
        void StartAtomicL();
        void CommitAtomicL();
        void RollbackAtomicL();
        TBool StreamingSupport(TInt& aItemSize);
        void StreamCommittedL();
        void CompleteOutstandingCmdsL();

    private:

        /**
         * Default constructor
         *
         * @since S60 3.2
         * @param   aDmCallback Pointer to callback interface 
         */
        CPresenceDMAdapter( MSmlDmCallback* aDmCallback );

        /**
        * Fetches an object from the given uri
        *
        * @since S60 3.2
        * @param aUri Uri to the fetched object
        * @param aObject The result is inserted to this buffer
        * @return Error code
        */
        CSmlDmAdapter::TError FetchObjectL(const TDesC8& aUri, CBufBase& aObject);

        /**
        * Updates the parameters of given DDF node
        *
        * @since S60 3.2
        * @param aNode The node to update.
        * @param aAccTypes Access types of the node.
        * @param aOccurrence Occurrance of the node.
        * @param aScope Scope of the node.
        * @param aFormat Format of the node.
        * @param aDescription A description of the node.   
        */
        void FillNodeInfoL(MSmlDmDDFObject& aNode,TSmlDmAccessTypes aAccTypes,
                            MSmlDmDDFObject::TOccurence aOccurrence, 
                            MSmlDmDDFObject::TScope aScope, 
                            MSmlDmDDFObject::TDFFormat aFormat,
                            const TDesC8& aDescription);

        /**
        * Get property from settings storage
        *
        * @since S60 3.2
        * @param aSetId unique id of the settings set
        * @param aProperty type of asked property
        * @param aObject returns property value
        * @return CSmlDmAdapter::EOk if successfull
        */
        CSmlDmAdapter::TError GetPropertyL( TInt aSetId, 
                                            TPresSettingsProperty aProperty, 
                                            CBufBase& aObject );

        /**
        * Create/update settings property
        *
        * @since S60 3.2
        * @param aSetId unique id of the settings set
        * @param aProperty type of property
        * @param aObject property value
        * @return CSmlDmAdapter::EOk if successfull
        */
        CSmlDmAdapter::TError UpdatePropertyL( TInt aSetId, 
                                               TPresSettingsProperty aProperty, 
                                               const TDesC8& aObject );
        
        /**
        * Compares strings
        *
        * @since S60 3.2
        * @param aLeft first string
        * @param aRight second string
        * @return True if the strings are identical
        */
        TBool Match( const TDesC8& aLeft, const TDesC8& aRight );
        
        /**
        * Finds uri based on SIP id, updates result
        *
        * @since S60 3.2
        * @param aSipID SIP ID
        * @param aObject object to be updated
        * @return Ok or Error
        */
        CPresenceDMAdapter::TError GetConRefFromSipIDL( TInt aSipID,
                                               CBufBase& aObject );
                                               
        /**
        * Finds uri based on XDM id, updates result
        *
        * @since S60 3.2
        * @param aXdmID XDM ID
        * @param aObject object to be updated
        * @return Ok or Error
        */
        CPresenceDMAdapter::TError GetConRefFromXdmIDL( TInt aXdmID,
                                               CBufBase& aObject );
                                               
        /**
        * Find SIP id based on uri
        *
        * @since S60 3.2
        * @param aUri uri of the wanted object
        * @return SIP id or KErrNotFound
        */  
        TInt GetSipIdFromConRefL(const TDesC8& aUri );
        
        /**
        * Find XDM id based on uri
        *
        * @since S60 3.2
        * @param aUri uri of the wanted object
        * @return XDM id or KErrNotFound
        */  
        TInt GetXdmIdFromConRefL(const TDesC8& aUri );
        
        /**
        * Finds whether the given persence id is valid or not
        *
        * @since S60 3.2
        * @param aSetId presence id to check
        * @return ETrue if valid
        */  
        TBool IsPresIDValidL(TInt aSetId);
        
        /**
        * Check presence settings duplicate names, provides alternate name
        *  if necessary
        *
        * @since S60 3.2
        * @param aSetId presence id
        * @param aSetName presence set name needs to be checked, 
        *   alternate name returns here
        * @return none
        */  
        void CheckDuplicateNameL(TDes& aSetName, const TInt& aSetId);
        
        /**
        * sets the proper length of presence settings discriptor items,
        * give shorter descriptor if necessary
        *
        * @since S60 3.2
        * @param aValue presence settings descriptor item to be evaluated
        * @param aProp which property this descriptor belongs to.
        * @return ETrue if name is validated, EFalse if it cant be validated
        */  
        TBool SetProperLength(TPresSettingsProperty aProp, TPtr& aValue); 
        
        /** 
        * Checks if there is ordinal numbering at the end of
        * descriptor and rips it off if there is.
        *
        * @param aName pointer to source descriptor
        * @return HBufC* containing the name ready for the numbering
        */
        HBufC* GetPrefixL( HBufC* aName );
        
        /**
        * 8 bit descriptor to integer
        *
        * @since S60 3.2
        * @param aLuid Descriptor to convert
        * @return TInt converted integer
        */
        TInt DesToInt( const TDesC8& aLuid ) const;
        
        /**
        * Integer to 8 bit descriptor, leaves pointer to stack
        *
        * @since S60 3.2
        * @param aLuid Integer to convert
        * @return HBufC8* 8 bit heap pointer to descriptor
        */        
        HBufC8* IntToDes8LC( const TInt aLuid ) const;
        
        /**
        * Integer to 16 bit descriptor, leaves pointer to stack
        *
        * @since S60 3.2
        * @param aLuid Integer to convert
        * @return HBufC* 16 bit heap pointer to descriptor
        */        
        HBufC* IntToDesLC( const TInt aLuid ) const;
        
        /**
        * 16 bit descriptor to 8 bit descriptor, leaves pointer to stack
        *
        * @since S60 3.2
        * @param aSource 16 bit descriptor to convert
        * @return HBufC8* pointer to 8 bit heap descriptor
        */        
        HBufC8* ConvertLC(const TDesC& aSource) const;
        
        /**
        * 8 bit descriptor to 16 bit descriptor, leaves pointer to stack
        *
        * @since S60 3.2
        * @param aSource 8 bit descriptor to convert
        * @return HBufC* pointer to 16 bit heap descriptor
        */        
        HBufC* ConvertLC(const TDesC8& aSource) const;
        
    private: // Data
    
        /**
        * Settings ID of presence set which is in cache
        */
        TInt iGetSetID;
        
        /**
        * Presence set which is in cache
        */
        TPresSettingsSet iGetSet;

    };

#endif // __PRESDMADAPTER_H__

// End of File
