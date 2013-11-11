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
* Description:  This is SUPL PSY location requestor
*
*/



// INCLUDE FILES
#include <e32cmn.h>
#include <centralrepository.h>
#include <npppsyinternalcrkeys.h>
#include "suplpsyrequestor.h"
#include "suplpsygenericinfouser.h"
#include "suplpsylogging.h"
#include "suplpsypanic.h"
#include "suplpsy.hrh"
#include <epos_suplterminalconstants.h>

// ===================== ==   ==   ==   == AL FUNCTIONS ===============================

//Constants
const TInt KGenericInfoIncreaseStep = 1024; //1k


// ==================== ==   ==   ==   == BER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::CSuplPsyRequestor
// C++default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSuplPsyRequestor::CSuplPsyRequestor( const TCallBack& aObserver ) 
        : CActive( EPriorityStandard ),
          iObserver( aObserver ),
          iFirstReq(ETrue),
          iTtnfFlag(EFalse),
          iPrevFixSuccess(EFalse)
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CSuplPsyRequestor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSuplPsyRequestor::ConstructL()
    {
    TRACESTRING( "CSuplPsyRequestor::ConstructL start" )
    
    //Connect to SUPL server
    User::LeaveIfError( iSuplServer.Connect() );
    
    //Construct iPositionInfo;
    iPositionInfo = ConstructGenericInfoL();

    CRepository *iRepository = CRepository::NewL(KCRUidNppPsy);

    //TInt keyValue;

    TInt ret = iRepository->Get(KPSYTimeToNextFix, iTtnf);

    delete iRepository;
    iRepository = NULL;
    
    TRACESTRING( "CSuplPsyRequestor::ConstructL end" )
    }

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::ConstructGenericInfoL
// -----------------------------------------------------------------------------
//
HPositionGenericInfo* CSuplPsyRequestor::ConstructGenericInfoL( 
    TInt aBufferSize ) const
    {
    HPositionGenericInfo* positionInfo = HPositionGenericInfo::NewL( aBufferSize );
    
    //Set all requested fields
    TInt count = SuplPsyGenericInfoUser::SupportedFieldsCount();
    for ( TInt i = 0; i < count; i++ )
        {
        positionInfo->SetRequestedField( 
            SuplPsyGenericInfoUser::SupportedFieldAt( i ) );
        }
    return positionInfo;
    }


// -----------------------------------------------------------------------------
// CSuplPsyRequestor::NewL
// Two - phased constructor.
// -----------------------------------------------------------------------------
//
CSuplPsyRequestor* CSuplPsyRequestor::NewL( 
        const TCallBack& aObserver )
    {
    CSuplPsyRequestor* self = new( ELeave ) CSuplPsyRequestor( aObserver );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CSuplPsyRequestor::~CSuplPsyRequestor
// Destructor
// -----------------------------------------------------------------------------
//
CSuplPsyRequestor::~CSuplPsyRequestor()
    {
    Cancel();
    iSuplSession.Close();
    iSuplServer.Close();
    delete iPositionInfo;
    }


// -----------------------------------------------------------------------------
// CSuplPsyRequestor::RunL
// -----------------------------------------------------------------------------
//
void CSuplPsyRequestor::RunL()
    {
    TRACESTRING2( "CSuplPsyRequestor::RunL status=%d", iStatus.Int() )
    //Call callback function
    if ( iStatus.Int() >= KErrNone )
        {
	        if (!iTtnfFlag)                
	            {                
										//Get position info
										HPositionGenericInfo* posInfo = ConstructGenericInfoL();
										// Set module id
										TPositionInfo* position = static_cast<TPositionInfo*> (posInfo );
										TUid implUid = { KSuplPsyImplUid };
										position->SetModuleId( implUid );
										
										TRACESTRING( "Calling SUPL FW GetPosition ... " )
										iStatus = iSuplSession.GetPosition( *posInfo );
										
										while ( iStatus.Int() == KErrPositionBufferOverflow )
										{
										//Not enough buffer allocated.Increase the buffer
										TInt bufferSize = posInfo->BufferSize();
										delete posInfo;
										posInfo = ConstructGenericInfoL( bufferSize + KGenericInfoIncreaseStep );
										iStatus = iSuplSession.GetPosition( *posInfo );
										}
	        
									            if ( iStatus.Int() >= KErrNone )
									                {
									                delete iPositionInfo;
									                iPositionInfo = posInfo;
									        
									                //Set module Id
									                iPositionInfo->SetModuleId( TUid::Uid( KSuplPsyImplUid ) );
									            
									                //Set time
									                TPosition pos;
									                iPositionInfo->GetPosition( pos );
									                TTime now;
									                now.UniversalTime();
									                pos.SetTime( now );
									                iPositionInfo->SetPosition( pos );
									                iPrevFixSuccess = ETrue;
									                }
									            else
									                {
									                delete posInfo;
									                iPrevFixSuccess = EFalse;
									                }
	            }
						else
						{
								iTtnfFlag = EFalse; 
								TRACESTRING( "Using previous position... " )
						}                    
        }
    iObserver.CallBack();
    TRACESTRING( "CSuplPsyRequestor::RunL end" )
    }

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void CSuplPsyRequestor::DoCancel()
    {
    if(iSuplSession.SubSessionHandle())
        {
        iSuplSession.CancelRunSession();
        }
    }

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::RunError
// -----------------------------------------------------------------------------
//
TInt CSuplPsyRequestor::RunError( TInt aError )
    {
    iStatus = aError;
    iObserver.CallBack();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::RequestLocation
// -----------------------------------------------------------------------------
//
void CSuplPsyRequestor::RequestLocation()
    {
    TRACESTRING( "CSuplPsyRequestor::RequestLocation start" )
    //Check if location request has been made already
    if ( IsActive() )
        {
        return;
        }
    
    // Check if the request is made withing TTNF,
    // if so return the previous computed position 
    // if it was success
                    
    if (!iFirstReq && iPrevFixSuccess)
         if (IsRequestInTtnf())
            {
            iTtnfFlag = ETrue;                
            iStatus = KRequestPending;
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
            TRACESTRING( "Interval is within TTNF, returning. Returning Previous Position." )
            return;
            }            
        
    //Open SUPL session if needed
    if ( iSuplSession.SubSessionHandle() == NULL )
        {
        TInt err = iSuplSession.Open( iSuplServer );

        //If can't open session call RunL in next active scheduler round
        if ( err != KErrNone )
            {
            iStatus = KRequestPending;
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, err );
            TRACESTRING2( "Could not open subsession:%d", err )
            return;
            }
        }
        
		TRACESTRING( "Calling SUPL FW RunSession... " )
    //Make location request
    iSuplSession.RunSession( iStatus, 
        KGpsSETAssisted | KAFLT | KECID | KEOTD | KOTDOA | KCID, 0, iFirstReq );
    if (iFirstReq)
        {            
        iFirstReq = EFalse;                
        }
    SetActive();
    TRACESTRING( "CSuplPsyRequestor::RequestLocation end" )
    }

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::GetRequestResult
// -----------------------------------------------------------------------------
//
TInt CSuplPsyRequestor::GetRequestResult()
    {
    return iStatus.Int();
    }

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::GetLocation
// -----------------------------------------------------------------------------
//
const HPositionGenericInfo& CSuplPsyRequestor::GetLocation()
    {
    return *iPositionInfo;
    }

// -----------------------------------------------------------------------------
// CSuplPsyRequestor::ResetLastPosition
// -----------------------------------------------------------------------------
//
void CSuplPsyRequestor::ResetLastPosition()
    {
//    iPositionInfo->ClearPositionData();
    }

TBool CSuplPsyRequestor::IsRequestInTtnf()
    {
    	
    iTtnfFlag = EFalse;  	
    
    TTime now;
    now.UniversalTime();

    TInt diff = 1000000;

    TPosition pos;

    iPositionInfo->GetPosition( pos );

    TTimeIntervalSeconds secs;

    now.SecondsFrom(pos.Time(), secs); 

    diff = diff * secs.Int();

		TRACESTRING2( "Time diff. for request...:%d", diff )
    if (diff <= iTtnf)
        return ETrue;            

    return EFalse;
    }        
//  End of File  
