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
* Description:   XDM Protocol
*
*/




#ifndef __XDMPROTOCOL__
#define __XDMPROTOCOL__

#include <ecom.h>
#include <e32base.h>
#include "XdmEngine.h"
#include "XdmDocument.h"
#include "XdmProtocolInfo.h"
    
//Idle timeout value for the protocol implementation in seconds
const TInt KDefaultIdleTimeout          = 10;

/**
* A helper class for  parameter passing from
* the XDM engine to the protocol implementation
*/
NONSHARABLE_CLASS( TXdmProtocolParams )
    {
    public:
        
        /**
        * C++ constructor
        *
        * @param CXdmEngine& Engine handle
        * @param CXdmProtocolInfo Protocol info handle
        * @return TXdmProtocolParams
        */
        TXdmProtocolParams( const CXdmEngine& aXdmEngine,
                            const CXdmProtocolInfo& aProtocolInfo ) :
                            iXdmEngine( aXdmEngine ),
                            iProtocolInfo( aProtocolInfo ){}
            
                            
    public:  //Data
    
        const CXdmEngine&               iXdmEngine;
        const CXdmProtocolInfo&         iProtocolInfo;             
    };
    
//FORWARD DECLARATION
class CXdmDocument;
class CXdmDirectory;
class CXdmDocumentNode;

//CLASS DECLARATION
NONSHARABLE_CLASS( CXdmProtocol ) : public CBase
    {
    public:
        
        /**
        * Symbian OS constructor.
        *
        * @param CXdmEngine& Reference to the engine
        * @param CXdmProtocolInfo& Protocol information
        * @return CXdmProtocol*
        */
        static CXdmProtocol* NewL( const CXdmEngine& aXdmEngine,
                                   const CXdmProtocolInfo& aProtocolInfo );
        
        /**
        * C++ destructor.
        *
        */
	    virtual ~CXdmProtocol();
	
	public:
        
        /**
        * Initialise the media used to transfer XDM data
        *
        * @param TInt Idle timeout
        * @param TRequestStatus& Request status of the caller
        * @return void
        */
        virtual void InitTransferMedium( TInt aIdleTimeout, TRequestStatus& aStatus ) = 0;
        
        /**
        * Is the transfer medium ready
        *
        * @return TBool Is the medium ready
        */
        virtual TBool IsTransferAvailable() const = 0;
                                                                       
        /**
        * Cancel initialisation of the transfer media.
        *
        * @return void
        */
        virtual void CancelTransferMediumInit() = 0;
        
        /**
        * Create a new document node.
        *
        * @return CXdmDocumentNode* 
        */
        virtual CXdmDocumentNode* CreateDocumentNodeL() = 0;
        
        /**
        * Create a new document model
        *
        * @param TDesC& Name of the new document model
        * @param TXdmDocType Type of the new document
        * @return CXdmDocument*
        */
        virtual CXdmDocument* CreateDocumentL( const TDesC& aDocumentName,
                                               const TXdmDocType aDocumentType ) = 0;
        
        /**
        * Create a new directory model
        *
        * @param TDesC& Directory path
        * @return CXdmDirectory*
        */
        virtual CXdmDirectory* CreateDirectoryL( const TDesC& aDirectoryPath ) = 0;
    
    protected:

        /**
        * C++ constructor for derived classes.
        *
        * @return CXdmProtocol
        */      
        CXdmProtocol();

    private:  //Data

        TUid                        iDestructorKey;
    };

// ----------------------------------------------------
// CXdmProtocol::CXdmProtocol
// 
// ----------------------------------------------------
//
inline CXdmProtocol::CXdmProtocol()
	{
	}

// ----------------------------------------------------
// CXdmProtocol::~CXdmProtocol
// 
// ----------------------------------------------------
//
inline CXdmProtocol::~CXdmProtocol()
	{
	REComSession::DestroyedImplementation( iDestructorKey );
    }

// ----------------------------------------------------
// CXdmProtocol::NewL
// Create a new protocol implementation
// ----------------------------------------------------
//
inline CXdmProtocol* CXdmProtocol::NewL( const CXdmEngine& aXdmEngine,
                                         const CXdmProtocolInfo& aProtocolInfo )
    {
    TAny* ptr = NULL;
    TXdmProtocolParams params( aXdmEngine, aProtocolInfo );
    ptr = REComSession::CreateImplementationL( 
          aProtocolInfo.ProtocolUid(), _FOFF( CXdmProtocol,
          iDestructorKey ), ( TAny* )&params );
    return ptr != NULL ? reinterpret_cast<CXdmProtocol*>( ptr ) : NULL;
    }
    
#endif      //__XDMPROTOCOL__
            
// End of File
