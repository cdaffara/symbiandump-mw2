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
* Description:   XDM Protocol operation factory
*
*/




#ifndef __XDMOPERATIONFACTORY__
#define __XDMOPERATIONFACTORY__

#include <ecom.h>
#include <e32base.h>
#include "XdmProtocol.h"

//Operation factory UIDs
const TInt KXcapOperationFactory            = 0x10207426; 
const TInt KLocalOperationFactory           = 0x10275088;


//FORWARD DECLARATION
class CXdmDocument;
class MXdmOperation;
class CXdmDocumentNode;

//CLASS DECLARATION
class CXdmOperationFactory : public CBase
    {
    public:
        
        /**
        * Symbian OS constructor
        * @param TInt UID of the operation factory
        * @return CXdmOperationFactory*
        */
        static CXdmOperationFactory* NewL( TInt aOperationFactory );
        
        /**
        * C++ destructor.
        */
	    virtual ~CXdmOperationFactory();
	
	public:
        
        /**
        * Create a fetch operation
        *
        * @param CXdmDocument& Target document
        * @param CXdmDocumentNode& Target element
        * @return MXdmOperation* The new operation
        */
        virtual MXdmOperation* FetchL( CXdmDocument& aTargetDocument,
                                       CXdmDocumentNode* aTargetNode ) const = 0;

        /**
        * Create an insert operation
        *
        * @param CXdmDocument& Target document
        * @param CXdmDocumentNode& Target element
        * @return MXdmOperation* The new operation
        */
        virtual MXdmOperation* InsertL( CXdmDocument& aTargetDocument,
                                        CXdmDocumentNode* aTargetNode ) const = 0;
        
        /**
        * Create a replace operation
        *
        * @param CXdmDocument& Target document
        * @param CXdmDocumentNode& New element
        * @param CXdmDocumentNode& Old element
        * @return MXdmOperation* The new operation
        */
        virtual MXdmOperation* ReplaceL( CXdmDocument& aOldDocument, 
                                         CXdmDocumentNode* aNewNode,
                                         CXdmDocumentNode* aOldNode ) const = 0;

        /**
        * Create an append operation
        *
        * @param CXdmDocument& Target document
        * @param CXdmDocumentNode& Target element
        * @return MXdmOperation* The new operation
        */
        virtual MXdmOperation* AppendL( CXdmDocument& aTargetDocument,
                                        CXdmDocumentNode* aTargetNode ) const = 0;

        /**
        * Create a delete operation
        *
        * @param CXdmDocument& Target document
        * @param CXdmDocumentNode& Target element
        * @return MXdmOperation* The new operation
        */
        virtual MXdmOperation* DeletionL( CXdmDocument& aTargetDocument,
                                          CXdmDocumentNode* aTargetNode ) const = 0;
    
    protected:

        /**
        * Symbian OS constructor for derived classes.
        *
        * @return CXdmOperationFactory
        */      
        CXdmOperationFactory();

    private:  //Data

        TUid                        iDestructorKey;
    };

// ----------------------------------------------------
// MXcapOperationFactory::MXcapOperationFactory
// 
// ----------------------------------------------------
//
inline CXdmOperationFactory::CXdmOperationFactory()
	{
	}

// ----------------------------------------------------
// CXdmProtocol::~CXcapOperationFactory
// 
// ----------------------------------------------------
//
inline CXdmOperationFactory::~CXdmOperationFactory()
	{
	REComSession::DestroyedImplementation( iDestructorKey );
    }

// ----------------------------------------------------
// CXdmProtocol::NewL
// Create a new operation factory
// ----------------------------------------------------
//
inline CXdmOperationFactory* CXdmOperationFactory::NewL( TInt aOperationFactory )
    {
    TAny* ptr = NULL;
    TUid KImplemantationUid = { aOperationFactory };
    ptr = REComSession::CreateImplementationL( 
          KImplemantationUid, _FOFF( CXdmOperationFactory, iDestructorKey ) ); 
    return reinterpret_cast<CXdmOperationFactory*>( ptr );
    
    
    }
    
#endif      //__XCAPOPERATIONFACTORY__


