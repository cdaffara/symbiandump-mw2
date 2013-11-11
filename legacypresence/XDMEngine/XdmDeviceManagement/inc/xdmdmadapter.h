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
* Description:    DM OMA XDM Settings Adapter
 *
*/





#ifndef __XDMDMADAPTER_H__
#define __XDMDMADAPTER_H__

#include <e32base.h>
#include <smldmadapter.h>
#include <XdmSettingsApi.h>


// LITERALS
_LIT8( KXdmDmNodeName,      "OMA_XDM" );
_LIT8( KXdmDmAppId,         "APPID" );
_LIT8( KXdmDmName,          "NAME" );
_LIT8( KXdmDmProviderId,    "PROVIDER-ID" );
_LIT8( KXdmDmToConRef,      "ToConRef" );
_LIT8( KXdmDmConRef,        "ConRef" );
_LIT8( KXdmDmSip,           "SIP" );
_LIT8( KXdmDmToNapId,       "TO-NAPID" );
_LIT8( KXdmDmAP,            "AP" );
_LIT8( KXdmDmUri,           "URI" );
_LIT8( KXdmDmAAuthName,     "AAUTHNAME" );
_LIT8( KXdmDmAAuthSecret,   "AAUTHSECRET" );
_LIT8( KXdmDmAAuthType,     "AAUTHTYPE" );

_LIT8( KXdmDmNodeNameDescription,       "The interior object holds all OMA XDM objects" );
_LIT8( KXdmDmAppIdDescription,          "The identity of the application service" );
_LIT8( KXdmDmNameDescription,           "Application name which is to be displayed in the user's equipment" );
_LIT8( KXdmDmProviderIdDescription,     "Provides an identifier for the service provider" );
_LIT8( KXdmDmUriDescription,            "Defines the root of all XDM resources" );
_LIT8( KXdmDmAAuthNameDescription,      "Defines the user name for XDMC authentication using HTTP digest" );
_LIT8( KXdmDmAAuthSecretDescription,    "Defines the password for XDMC authentication using HTTP digest" );
_LIT8( KXdmDmAAuthTypeDescription,      "Defines the authentication type for XDMC authentication" );
_LIT8( KXdmDmDynamicDescription,        "Placeholder for one or more connectivity parameters" );
_LIT8( KXdmDmToConRefDescription,       "Refers to a collection of connectivity definitions" );
_LIT8( KXdmDmConRefDescription,         "Indicates the linkage to connectivity parameters" );
_LIT8( KXdmDmSipDescription,            "Reference to SIP DM Object" );
_LIT8( KXdmDmToNapIdDescription,        "Reference to the connection used for the XCAP traffic" );

_LIT8( KXdmDDFVersion,      "1.0" );
_LIT8( KXdmDmTextPlain,     "text/plain" );
_LIT8( KXdmDmUriDotSlash,   "./" );
_LIT8( KXdmDmSeparator,     "/" );
_LIT8( KXdmDmSipLinkPrefix, "./SIP" );
_LIT( KXdmDefaultId,        "-1" );
_LIT( KXdmDmStartBracket,   "(" );
_LIT( KXdmDmCloseBracket,   ")" );

_LIT( KXdmDmLogDir,         "Xdm" );
_LIT( KXdmDmLogFile,        "XdmDm.txt" );

// CONSTANTS
const TInt KXdmDmLogBufferMaxSize   = 2000;
const TInt KXdmDefaultResultSize    = 255;
const TInt KXdmDmLevel                = 2;
const TInt KXdmDmIdTableSize        = 16;
const TInt KXdmDmHexLength          = 8;
const TInt KXdmDmMaxIntLength       = 10;   // max length of 32bit integer
const TUint8 KXdmDmObjectSizeWidth  = 4;
const TUint8 KXdmDmSeparatorDef     = 0x2f; // forward slash


/**
* The main class of the OMA XDM DM adapter. Handles
* requests of fetching and updating settings sets and 
* settings items.
*/
class CXdmDMAdapter : public CSmlDmAdapter
    {
    public:
        // Adapter interface from CSmlDmAdapter
        static CXdmDMAdapter* NewL( MSmlDmCallback* aDmCallback );

        ~CXdmDMAdapter();
        
        void DDFVersionL( CBufBase& aDDFVersion );
        
        void DDFStructureL( MSmlDmDDFObject& aDDF );
        
        void UpdateLeafObjectL( const TDesC8& aUri, 
                                const TDesC8& aLUID, 
                                const TDesC8& aObject, 
                                const TDesC8& aType, 
                                TInt aStatusRef );
                                                        
        void DeleteObjectL( const TDesC8& aUri, 
                            const TDesC8& aLUID, 
                            const TInt aStatusRef ); 
                                              
        void FetchLeafObjectL( const TDesC8& aUri, 
                               const TDesC8& aLUID, 
                               const TDesC8& aType, 
                               const TInt aResultsRef, 
                               const TInt aStatusRef ); 
                                               
        void ChildURIListL( const TDesC8& aUri, 
                            const TDesC8& aLUID, 
                            const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList, 
                            const TInt aResultsRef, 
                            const TInt aStatusRef ); 
                                                       
        void AddNodeObjectL( const TDesC8& aUri, 
                             const TDesC8& aParentLUID, 
                             const TInt aStatusRef );
                             
        void UpdateLeafObjectL( const TDesC8& aUri, 
                                const TDesC8& aLUID, 
                                RWriteStream*& aStream, 
                                const TDesC8& aType, 
                                const TInt aStatusRef );   
                                                             
        void FetchLeafObjectSizeL( const TDesC8& aUri, 
                                   const TDesC8& aLUID, 
                                   const TDesC8& aType, 
                                   const TInt aResultsRef, 
                                   const TInt aStatusRef );
                                   
        void ExecuteCommandL( const TDesC8& aUri, 
                              const TDesC8& aLUID, 
                              const TDesC8& aArgument, 
                              const TDesC8& aType, 
                              const TInt aStatusRef );    
                                              
        void ExecuteCommandL( const TDesC8& aUri, 
                              const TDesC8& aLUID, 
                              RWriteStream*& aStream, 
                              const TDesC8& aType, 
                              const TInt aStatusRef );     
                                                     
        void CopyCommandL( const TDesC8& aTargetURI, 
                           const TDesC8& aTargetLUID, 
                           const TDesC8& aSourceURI, 
                           const TDesC8& aSourceLUID, 
                           const TDesC8& aType, 
                           TInt aStatusRef );   
                                                 
        void StartAtomicL();        
        void CommitAtomicL();
        void RollbackAtomicL();
        TBool StreamingSupport( TInt& aItemSize);
        void StreamCommittedL();
        void CompleteOutstandingCmdsL();

        /**
        * Log writing
        * DEBUG only
        */
 #ifdef _DEBUG
        static void WriteToLog( TRefByValue<const TDesC8> aFmt,... );  
 #endif


    private:

        /**
        * Default constructor
        * @param   aDmCallback Pointer to callback interface 
        */
        CXdmDMAdapter( MSmlDmCallback* aDmCallback );

        
        /**
        * Fetches an object from the given uri
        * @param aUri Uri to the fetched object
        * @param aObject The result is inserted to this buffer
        * @return Error code
        */
        CSmlDmAdapter::TError FetchObjectL( const TDesC8& aUri, 
                                            CBufBase& aObject,
                                            TInt aSettingsId );

        /**
        * Updates the parameters of given DDF node
        * @param aNode The node to update.
        * @param aAccTypes Access types of the node.
        * @param aOccurrence Occurrance of the node.
        * @param aScope Scope of the node.
        * @param aFormat Format of the node.
        * @param aDescription A description of the node.   
        */
        void FillNodeInfoL( MSmlDmDDFObject& aNode,
                            TSmlDmAccessTypes aAccTypes,
                            MSmlDmDDFObject::TOccurence aOccurrence, 
                            MSmlDmDDFObject::TScope aScope, 
                            MSmlDmDDFObject::TDFFormat aFormat,
                            const TDesC8& aDescription);

        /**
        * Get property from settings storage
        * @param aSettingsId unique id of the settings set
        * @param aProperty type of asked property
        * @param aObject returns property value
        * @return CSmlDmAdapter::EOk if successfull
        */
        CSmlDmAdapter::TError GetPropertyL( TInt aSettingsId, 
                                            TXdmSettingsProperty aProperty, 
                                            CBufBase& aObject );

        /**
        * Create/update settings property 
        * @param aSettingsId unique id of the settings set
        * @param aProperty type of property
        * @param aObject property value
        * @return CSmlDmAdapter::EOk if successfull
        */
        CSmlDmAdapter::TError UpdatePropertyL( TInt aSettingsId, 
                                               TXdmSettingsProperty aProperty, 
                                               const TDesC8& aObject );
        /**
        * Find IAP id based on uri
        * @param aUri uri of the wanted object
        * @return id or KErrNotFound
        */                                      
        TInt IapIdFromURIL( const TDesC8& aUri );
        
        /**
        * Find uri based on IAP id
        * @param aId value stored to xdm settings
        * @return uri or NULL
        */
        HBufC8* URIFromIapIdL( TInt aId );
        
        /**
        * Find SIP id based on uri
        * @param aUri uri of the wanted object
        * @return id or KErrNotFound
        */  
        TInt GetSipIdL( const TDesC8& aUri );
        
        /**
        * Finds uri based on SIP id, updates result
        * @param aSettingsId xdm settings id
        * @param aObject object to be updated
        * @return Ok or Error
        */
        CXdmDMAdapter::TError FetchSipConRefL( TInt aSettingsId,
                                               CBufBase& aObject );
                
        /**
        * Find settings id
        * @param aLUID luid of the wanted object
        * @param aUri uri of the wanted object
        * @return id or KErrNotFound
        */  
        TInt FindSettingsIdL( const TDesC8& aLUID, const TDesC8& aUri );
         
        /**
        * Checks if the settings name is already in use,
        * creates a unique name if needed
        * @param aName name of the settings
        * @return unique name
        */   
        HBufC* CheckExistingNamesLC( const TDesC8& aName );
        
        
        /**
        * Compares strings
        * @param aLeft first string
        * @param aRight second string
        * @return True if the strings are identical
        */
        TBool Match( const TDesC8& aLeft, const TDesC8& aRight );
        
        
        /**
        * Converts descriptor to int
        * @param aSource descriptor to be converted
        * @return TInt converted value
        */
        TInt DesToInt( const TDesC& aSource ) const;

        /**
        * Converts descriptor to int
        * @param aSource descriptor to be converted
        * @return TInt converted value
        */
        TInt DesToInt( const TDesC8& aSource ) const; 
        
        /**
        * Converts int to descriptor
        * @param aSource int to be converted
        * @return HBufC8* converted value
        */
        HBufC8* IntToDes8LC( const TInt aSource );
       
        /**
        * Converts 16-bit descriptor to 8-bit
        * @param aSource descriptor to be converted
        * @return HBufC8* converted value
        */
        HBufC8* ConvertLC( const TDesC& aSource );
        
        /**
        * Converts 8-bit descriptor to 16-bit
        * @param aSource descriptor to be converted
        * @return HBufC* converted value
        */
        HBufC*  ConvertLC( const TDesC8& aSource );
        
        
        
    private: // Data

    
    
    };

#endif // __XDMDMADAPTER_H__

// End of File
