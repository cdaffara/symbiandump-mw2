/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Processes requestor related data.
*
*/


// INCLUDE FILES
#include "lpdrequestorprocessor.h"
#include "contactresolversession.h"

#include "locverifierdlgdebug.h"
#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_cposcontactrequestor.h>
#include <lbs/epos_cposservicerequestor.h>
#include <epos_cposprivacynotifier.h>


// Unnamed namespace for local definitions

const TInt KRequestorsGranularity = 5;
#ifdef _DEBUG
_LIT( KPanicText, "CLpdRequestorProcessor" );
enum TPanicCode
    {
    KLpdErrGeneral = 1
    };
#endif


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::CLpdRequestorProcessor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdRequestorProcessor::CLpdRequestorProcessor() 
: iRequestors( KRequestorsGranularity)
    {
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdRequestorProcessor* CLpdRequestorProcessor::NewL()
    {
    CLpdRequestorProcessor* self = new( ELeave ) CLpdRequestorProcessor;
    // Nothing to do in the Second Phase Constructor
    return self;
    }
    
// Destructor
CLpdRequestorProcessor::~CLpdRequestorProcessor()
    {
    iRequestors.ResetAndDestroy();
    iRequestors.Close();
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::ReadRequestorsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CLpdRequestorProcessor::ReadRequestorsL(
    const CPosPrivacyNotifier& aPosNotifier )
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::ReadRequestorsL" );
    __ASSERT_DEBUG( aPosNotifier.CurrentRequest() != KPosNullQNRequestId,
                    HandleAssertErrorL() );

    this->ResetAndDestroyRequestors();
    
    TBool allRequestorIdsValid = ETrue;
   
    // Let's read the requestors associated with the current request:
    TInt nofRequestors = aPosNotifier.RequestorCountL();

    for ( TInt i = 0; i < nofRequestors; i++ )
        {
        CPosRequestor* requestor = aPosNotifier.RequestorLC( i );
        
        if ( RequestorIdValid( *requestor ) )
            { // mark that some requestor id is invalid
            allRequestorIdsValid = EFalse;
            }
               
        User::LeaveIfError( iRequestors.Append( requestor ) );
        CleanupStack::Pop( requestor ); // iRequestors owns now the requestor
        }

    if ( nofRequestors == 0 )
        {
        allRequestorIdsValid = EFalse; // zero requestors == invalid
        }

     RContactResolverSession resSession;
     TInt err = resSession.Connect();
    
     // If any error in connection,dont leave.Use un-resolved requestors
     if( KErrNone == err )
     	{
     	resSession.ResolveRequestorsL(iRequestors);
     	}
     resSession.Close();
       
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::ReadRequestorsL" );
    return allRequestorIdsValid;
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::ResetAndDestroyRequestors
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestorProcessor::ResetAndDestroyRequestors()
    {
    iRequestors.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::RequestorIdValid
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CLpdRequestorProcessor::RequestorIdValid(
    const CPosRequestor& aRequestor )
    {
    return( aRequestor.RequestorIdFormat() != CPosRequestor::EIdFormatUnknown );
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::HandleAssertErrorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestorProcessor::HandleAssertErrorL() const
    {
    #ifdef _DEBUG
        User::Panic( KPanicText, KLpdErrGeneral );
    #else
        User::Leave( KErrCorrupt );
    #endif
    }
// end of file
