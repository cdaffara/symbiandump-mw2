/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   XDM protocol information
*
*/




#ifndef __XDMPROTOCOLINFO__
#define __XDMPROTOCOLINFO__

#include <e32base.h>
#include "XdmCredentials.h"
    
//FORWARD DECLARATION
class CXdmDocument;

//CLASS DECLARATION
class CXdmProtocolInfo : public CBase
    {
    public:
        
        /**
        * Create a new protocol info object
        *
        * @param TInt ID of the access point to use
        * @param TUid The UID of the XDM protocol to use
        * @param TDesC& Address of the remote host
        * @return CXdmProtocolInfo The protocol info object
        */
        IMPORT_C static CXdmProtocolInfo* NewL( TInt aAccessPoint,
                                                TInt aProtocolUid,
                                                const TDesC& aRootLocation );
        
        /**
        * Create a new protocol info object
        *
        * @param TInt ID of the settings collection
        *
        * NOTE: In order to be able to use this overload
        *       of the constructor, the client must first 
        *       fetch the collection of XDM Settings through
        *       the static method XdmSettingsL() declared in
        *       CXdmEngine API. This method returns an array 
        *       of strings, which denote the names of the
        *       settings collections stored for XDM.
        *
        * @return CXdmProtocolInfo* The protocol info object
        */
        IMPORT_C static CXdmProtocolInfo* NewL( TInt aSettingsId );
                                                
        /**
        * Return the root location of the XDM documents
        *
        * @return TPtrC RootURI
        */
        IMPORT_C TPtrC Root() const;

        /**
        * Return the access point
        *
        * @return TInt Access point
        */
        IMPORT_C TInt AccessPoint() const;
        
        /**
        * Return the the UID of the protocol in use
        *
        * @return TUid UID of the protocol implementation
        */
        IMPORT_C TUid ProtocolUid() const;
        
        /**
        * Set appropriate credentials in case the used
        * resource requires authorization.
        *
        * @param TXdmCredentials Credentials for the user
        * @return void
        */
        IMPORT_C void SetCredentials( const TXdmCredentials aCredentials );

        /**
        * Enable/disable cache
        *
        * @param TBool Is cache in use or not
        * @return void
        */
        IMPORT_C void SetCacheUsage( TBool aCacheUsage );

        /**
        * Enable/disable secure transfer
        *
        * @param TBool Is the protocol secure
        * @return void
        */
        IMPORT_C void SetSecurity( TBool aSecure );

        /**
        * Is secure transfer enabled
        *
        * @return TBool Secure or not
        */
        IMPORT_C TBool IsSecure() const;
        
        /**
        * Check cache usage
        *
        * @return TBool Is cache in use
        */
        IMPORT_C TBool IsCacheEnabled() const;
        
        /**
        * Return the credentials
        *
        * @return TXdmCredentials& Credentials for the user
        */
        IMPORT_C const TXdmCredentials& Credentials() const;
        
        /**
        * Return the ID of the settings
        *
        * @return TInt SettingsID
        */
        IMPORT_C TInt SettingsID() const;

        /**
        * Destructor
        */
        IMPORT_C virtual ~CXdmProtocolInfo();

    private:
        
        /**
        * C++ constructor is private
        */
        CXdmProtocolInfo();
        
        /**
        * C++ constructor is private
        *
        * @param TInt UID of the protocol plugin
        */
        CXdmProtocolInfo( TInt aProtocolUid );
                                           
        /**
        * C++ constructor is private
        *
        * @param TInt ID of the access point in use
        * @param TInt UID of the protocol plugin
        */
        CXdmProtocolInfo( TInt aAccessPoint,
                          TInt aProtocolUid );

        /**
        * Symbian OS second-phase constructor
        *
        * @param TDesC& Root location
        * @return void
        */
        void ConstructL( const TDesC& aRootLocation );
        
        /**
        * Symbian OS non-leaving constructor
        *
        * @param TInt Settings ID
        * @return void
        */
        void Construct( TInt aSettingsId );

    private:  //Data
        
        TUid                           iProtocolUid;
        TInt                           iSettingsId;
        TInt                           iAccessPoint;
        TBool                          iSecure;
        TBool                          iUseCache;
        HBufC*                         iRootLocation;
        TXdmCredentials                iCredentials;
    };

#endif      //__XDMPROTOCOLINFO__
            
// End of File
