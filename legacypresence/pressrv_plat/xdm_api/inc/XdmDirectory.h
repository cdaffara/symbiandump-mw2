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
* Description:   XDM directory
*
*/




#ifndef __XDMDIRECTORY__
#define __XDMDIRECTORY__

#include <e32base.h>
#include <XdmDocument.h>

enum TDirUpdatePhase
    {
    EDirPhaseIdle = 0,
    EUpdateDocumentList,
    ERefreshDocuments
    };

//FORWARD DECLARATION
class CXdmEngine;
class CXdmDocumentNode;
class CXdmOperationFactory;

class CXdmDirectory : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        IMPORT_C ~CXdmDirectory();
        
    public: //The abstract directory API
        
        /**
        * Return the number of documents in this directory
        * @return TInt Number of documents
        */
        virtual TInt DocumentCount() = 0;
        
        /**
        * Return the name of the specified document
        * @param TInt Index of the desired document
        * @return TPtrC Name of the specified document
        */
        virtual TPtrC Document( TInt aIndex ) const = 0;

        /**
        * Returns the path to this directory
		* @return TPtrC8 The path to the directory this object models 
        */
        virtual TPtrC DirectoryPath() const = 0;
        
        /*
        * Return the type of the specified document
        * @param TInt Index of the desired document
        * @return TXdmDocType Type of the document
        */
        virtual TXdmDocType DocumentTypeL( TInt aIndex ) const = 0;
    
    protected:
        
        /**
        * By default Symbian OS constructor is private.
        * @param TDesC& The directory path
        * @return void
        */
        IMPORT_C void BaseConstructL( const TDesC& aDirectoryPath );
        
        /**
        * C++ constructor is protected.
        */
		IMPORT_C CXdmDirectory( CXdmEngine& aXdmEngine );
		
    private:  //Methods which need to be accessible from the CXdmEngine
              //but NOT from the client application => friend class
        
        /**
        * Start updating the contents of this directory model
        * @param TRequestStatus& Request status of the client
        * @param TDirUpdatePhase Phase of the update
        * @return void
        */
        virtual void StartUpdateL() = 0;
        
        /**
        * Cancel a directory update
		* @return void 
        */
        virtual void CancelUpdate() = 0;
        
        /**
        * Save the client's request status and update status
        * @param TDirUpdatePhase The update phase
        * @param TRequestStatus The client's request status
		* @return void 
        */
        virtual void SaveRequestData( TDirUpdatePhase aUpdatePhase,
                                      TRequestStatus& aClientStatus ) = 0;
    
    protected:
        
        TInt                                    iFileCount;     
        HBufC*                                  iDirectoryPath;
        CXdmEngine&                             iXdmEngine;
        TDirUpdatePhase                         iUpdatePhase;
        friend class                            CXdmEngine;
    };

#endif      //__XDMDIRECTORY__
            
// End of File
