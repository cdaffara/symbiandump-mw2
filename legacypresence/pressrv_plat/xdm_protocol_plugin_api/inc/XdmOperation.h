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
* Description:   XDM Operation
*
*/




#ifndef __XDMOPERATION__
#define __XDMOPERATION__

#include <e32base.h>

//These are instructions the document element
//should follow when an operation fails
const TInt KInsertConflict              = 5001;

/**** Option flags ****/

const TInt KXdmOption1                                         = 0x00000001;
const TInt KXdmOption2                                         = 0x00000002;
const TInt KXdmOption3                                         = 0x00000004;
const TInt KXdmOption4                                         = 0x00000008;
const TInt KXdmOption5                                         = 0x00000010;
const TInt KXdmOption6                                         = 0x00000020;
const TInt KXdmOption7                                         = 0x00000040;
const TInt KXdmOption8                                         = 0x00000080;

//The type of the operation
enum TXdmOperationType
    {
    EXdmDocument = 0,
    EXdmPartialDocument 
    };

//Completion Data    
class TXdmCompletionData
    {
    public:
    
        TInt        iCompletion;
        TInt        iHttpStatus;
        HBufC8*     iETag;
        HBufC8*     iStatusText;
        HBufC8*     iResponseData;     
    };

//Forward declaration
class CXdmDocumentNode;

//CLASS DECLARATION
NONSHARABLE_CLASS( MXdmOperation )
    {
    public:
        
        /**
        * Execute this operation synchronously
        *
        * @return void
        */      
        virtual void ExecuteL() = 0;
        
        /**
        * Execute this operation asynchronously.
        *
        * @param TRequestStatus& Request status of the caller
        * @param TUint The option flags
        * @return void
        */      
        virtual void ExecuteL( TRequestStatus& aStatus, TUint aOptions ) = 0;
        
        /**
        * Cancel this operation.
        *
        * @return void
        */      
        virtual void CancelOperation() = 0;
        
        /**
        * Get the result of this operation.
        * @return TInt Result of the operation
        */      
        virtual TInt Result() const = 0;

        /**
        * Get the completion data of this operation.
        * @return TXdmCompletionData Completion data
        */      
        virtual const TXdmCompletionData& CompletionData() const = 0;
        
        /**
        * Check the completion state of this operation.
        * 
        * @return TBool Has the operation been completed
        */      
        virtual TBool IsCompleted() const = 0;
        
        /**
        * Return the target node of this operation
        * 
        * @return CXdmDocumentNode* The target node
        */      
        virtual CXdmDocumentNode* TargetNode() const = 0;
        
        /**
        * Destructor
        *
        *@return void
        */      
        virtual void Destroy() = 0; 
    };
    
    

#endif      //__XCAPOPERATION__
            
// End of File
