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
* Description:   XDM Engine
*
*/




#ifndef __XDMENGINE__
#define __XDMENGINE__

// INCLUDES
#include "e32base.h"
#include "XdmDocument.h"
#include "XdmDirectory.h"

enum TXdmEnginePanic
    {
    EDocNodeNull = 0,
    ENodeNotEmpty,
    EDDocModelNotFound,
    EDirModelNotFound,
    EIllegalPosition,
    EIndexOutOfBounds,
    };
    
//CONSTANTS
_LIT( KXdmEngLogFile,                                  "XdmEngine" );
_LIT8( KGeneralEqualsAndQuotes,                        "=\""  );
_LIT8( KGeneralQuotes,                                 "\""  );
_LIT8( KGeneralSpace,                                  " ");
const TInt KXdmEngLogBufferMaxSize		               = 2000;
const TInt KXdmEngShutdownDelay		                   = 10;

//FORWARD DECLARATIONS
class CDesC16Array;
class CXdmProtocol;
class CXdmDocument;
class CXdmDirectory;
class CXdmLogWriter;
class CXdmProtocolInfo;

// CLASS DECLARATION
class CXdmEngine : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Create an instance of the Engine object
        * @param CXdmProtocolInfo& Reference to a protocol info object
        * @return CXdmEngine*
        */     
        IMPORT_C static CXdmEngine* NewL( const CXdmProtocolInfo& aProtocolInfo );
        
        /**
        * Fetch the IDs of the XDM Settings which have been
        * configured to this device.
        *
        * @param RArray<TInt> On return, contains the TInt type IDs
        *                     which correspond to the names stored
        *                     in the returning array object.
        * @return CDesC16Array* The names of the settings
        */
        IMPORT_C static CDesC16Array* XdmSettingsLC( RArray<TInt>& aSettingIds );
        
        /**
        * Create a new document model
        *
        * @param TDesC8& Name of the document, including path
        * @param TDesC8& Application usage - an AUID
        *
        * NOTE! It is important to keep in mind that the Xdm Engine
        *       DOES NOT maintain the documents created on this engine
        *       object. It is the responsibility of the client application
        *       to deallocate the documents it creates by calling
        *       CXdmEngine::DeleteDocumentModelL( model ). The engine
        *       does keep track of the documents that are constructed
        *       on it, but it does not own them in the sense which would
        *       suggest deallocation of the memory reserved for them.
        *
        * @return CXdmDocument instance
        */     
        IMPORT_C CXdmDocument* CreateDocumentModelL( const TDesC& aDocumentName,
                                                     const TXdmDocType aDocumentType );

        /**
        * Create a new document model
        *
        * @param CXdmDocument* The document model to delete
        * @return void 
        */     
        IMPORT_C void DeleteDocumentModelL( const CXdmDocument* aDocument );
        
        /**
        * Create a new document model
        *
        * @param CXdmDocument* The document model to delete
        * @return void 
        */     
        IMPORT_C CXdmDirectory* CreateDirectoryModelL( const TDesC& aDirectoryPath );
        
        /**
        * Create a new document model
        *
        * @param CXdmDocument* The document model to delete
        * @return void 
        */     
        IMPORT_C void DeleteDirectoryModelL( const CXdmDirectory* aDocument );
        
        /**
        * Create a new element.
        *
        * The returning CXdmDocumentNode object does not yet 
        * belong to any document. Therefore it is the responsibility
        * of the client application to deallocate the memory
        * reserved for the element.
        *
        * The new element can be "attached" to a document by using
        * AppendToModelL() or ReplaceInModelL(). It is important to
        * note that even after calling either of these methods, the
        * ownership of the object does not change.
        *
		* @return CXdmDocumentNode* 
        */
        IMPORT_C CXdmDocumentNode* CreateDocumentNodeL();
        
        /**
        * Update the changes to the specified document model
        *
        * @param The document to be updated
        * @param TRequestStatus Request status of the caller
        * @return void
        */     
        IMPORT_C void UpdateL( CXdmDocument* aDocument,
                               TRequestStatus& aStatus );
        
        /**
        * Update the contents of the specified directory model
        *
        * @param The directory to be updated
        * @param TRequestStatus Request status of the caller
        * @return void
        */     
        IMPORT_C void UpdateL( TRequestStatus& aStatus,
                               CXdmDirectory* aDirectory,
                               TDirUpdatePhase aUpdatePhase );


        /**
        * Cancel an update operation on a document
        *
        * @param CXdmDocument The document whose update
        *                     is being cancelled
        * @return void
        */     
        IMPORT_C void CancelUpdate( CXdmDocument* aDocument );
        
        /**
        * Cancel an update operation on a directory
        *
        * @param CXdmDirectory The directory whose update
        *                      is being cancelled
        * @return void
        */     
        IMPORT_C void CancelUpdate( CXdmDirectory* aDirectory );
        
        /**
        * Get the list of documents created on this instance
        *
        * @return RPointerArray The list of document models
        */     
        IMPORT_C const RPointerArray<CXdmDocument>& DocumentCollection() const;
        
        /**
        * Get the list of directories created on this instance
        *
        * @return RPointerArray The list of document models
        */     
        IMPORT_C const RPointerArray<CXdmDirectory>& DirectoryCollection() const;
        
        /**
        * Destructor.
        */      
        IMPORT_C virtual ~CXdmEngine();

    public:   //New functions 
        
        /**
        * Return the protocol this engine uses
        *
        * @return CXdmProtocol* The protocol instance
        */ 
        CXdmProtocol* XdmProtocol() const;

        /**
        * Logging function
        *
        * @param TRefByValue<const TDesC8> aFmt,...
        * @return void
        */ 
        void WriteToLog( TRefByValue<const TDesC8> aFmt,... ) const;
    
    public:  //Static
     
        /**
        * Convert data from UNICODE to UTF-8
        *
        * @param TDesC& UNICODE descriptor
        * @return HBufC8* Data converted to UTF-8
        */ 
        static HBufC8* ConvertToUTF8L( const TDesC& aUnicodeDesc );

    private:
        
        /**
        * C++ default constructor is private
        * @return void
        */      
        CXdmEngine();
        
        /**
        * Symbian OS second-phase constructor
        * @param CXdmProtocolInfo& Protocol information
        * @return void
        */ 
        void ConstructL( CXdmProtocolInfo& aProtocolInfo );

        /**
        * From CActive.
        * @return void
        */
		void RunL();

        /**
        * From CActive.
        * @return void
        */
		void DoCancel();
        
    private: //Data

        enum TXdmEngineState
            {
            EXdmEngineIdle = 0,
            EXdmEngineStartDocUpdate,
            EXdmEngineStartDirUpdate
            };
        
        CXdmProtocol*                           iXdmProtocol;
        CXdmDocument*                           iCurrentDoc;
        CXdmDirectory*                          iCurrentDir;
        TRequestStatus*                         iClientStatus;
        RPointerArray<CXdmDocument>             iDocumentQueue;
        RPointerArray<CXdmDocument>             iDocUpdateQueue;
        RPointerArray<CXdmDirectory>            iDirUpdateQueue;
        RPointerArray<CXdmDirectory>            iDirectoryQueue;
        CXdmLogWriter*                          iLogWriter;
    };

#endif


// End of File
