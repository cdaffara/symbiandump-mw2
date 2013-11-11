/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/
// T_TelephonyAudioRouting.cpp

// INCLUDE FILES

#include "T_TelephonyAudioRouting.h"
#include "talogger.h"
#include "MTelephonyAudioRoutingObserver.h"


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::CTelephonyAudioRouting
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CStubTelephonyAudioRouting::CStubTelephonyAudioRouting( 
        MTelephonyAudioRoutingObserver& aObserver        
        ) : iObserver( aObserver ), iAvailableOutputsTArr(0, NULL, NULL)
    {    
    iOutput = CTelephonyAudioRouting::ENotActive;
    iPreviousOutput = CTelephonyAudioRouting::ENotActive;
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::ConstructL
// Symbian OS default constructor may leave.
// -----------------------------------------------------------------------------
//
void CStubTelephonyAudioRouting::ConstructL()
    {    
    TEFLOGSTRING( KTAOBJECT, "T_AudioH CTelephonyAudioRouting::ConstructL()" );    
    
    iAudioRoutingObserverCallBack = CIdle::NewL( CActive::EPriorityHigh );
        
    //iAvailableOutputs = new (ELeave) CArrayFixFlat<CTelephonyAudioRouting::TAudioOutput>(5);   
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CStubTelephonyAudioRouting* CStubTelephonyAudioRouting::NewL(
	MTelephonyAudioRoutingObserver& aObserver)
    {
    
    CStubTelephonyAudioRouting* telephonyAudioRouting;
    
    // Check thread local storage:
	if ( Dll::Tls() == NULL )
		{
		telephonyAudioRouting = new ( ELeave ) CStubTelephonyAudioRouting( aObserver );		
		CleanupStack::PushL( telephonyAudioRouting );
		telephonyAudioRouting->ConstructL();
		CleanupStack::Pop( telephonyAudioRouting );
	
		// Store a pointer to the new instance in thread local storage:
		TInt err = Dll::SetTls( static_cast<TAny*>( telephonyAudioRouting ) );
		if ( err == KErrNone )
			{
			return telephonyAudioRouting;
			}
		else
			{
			delete telephonyAudioRouting;
			User::Leave( err );
			return NULL;
			}
		}
	else // CStubTelephonyAudioRouting has been instantiated once already
		{		
		telephonyAudioRouting = static_cast<CStubTelephonyAudioRouting*>( Dll::Tls() );
		return telephonyAudioRouting;
		}
    }
    

// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::NewL
// Two-phased constructor. Leaves if the class does not exist in TLS because 
// this version does not take observer as parameter.
// -----------------------------------------------------------------------------
//
CStubTelephonyAudioRouting* CStubTelephonyAudioRouting::NewL()
    {
    
    CStubTelephonyAudioRouting* telephonyAudioRouting;
    
    // Check thread local storage:
	if ( Dll::Tls() == NULL )
		{
		User::Leave( KErrNotFound );
		return NULL;		
		}
	else // CStubTelephonyAudioRouting has been instantiated once already
		{		
		telephonyAudioRouting = static_cast<CStubTelephonyAudioRouting*>( Dll::Tls() );
		return telephonyAudioRouting;
		}
    }    
    

CStubTelephonyAudioRouting::~CStubTelephonyAudioRouting()     
{

Dll::SetTls( NULL ) ;
iAvailableOutputs.Close();

delete iAudioRoutingObserverCallBack;      
}  
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//    
const TArray<CTelephonyAudioRouting::TAudioOutput>& CStubTelephonyAudioRouting::AvailableOutputs()
	{
	iAvailableOutputsTArr = iAvailableOutputs.Array();
	return iAvailableOutputsTArr;
	}
	
	
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//    
CTelephonyAudioRouting::TAudioOutput CStubTelephonyAudioRouting::Output()
{
	return iOutput;
}
	

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  	
void CStubTelephonyAudioRouting::SetOutputL( CTelephonyAudioRouting::TAudioOutput aOutput)
{
	iPreviousOutput = iOutput;
	iOutput = aOutput;
	
	/* if callback has not been called already */
	iAudioRoutingObserverCallBack->Cancel();
	
	//Todo: call observers SetOutputComplete() 
	iAudioRoutingObserverCallBack->Start( TCallBack( DoSetOutputComplete, this ) );
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
TInt CStubTelephonyAudioRouting::DoSetOutputComplete( TAny* aAny )
{
	CStubTelephonyAudioRouting* self = 
        static_cast< CStubTelephonyAudioRouting* >( aAny );
    
   
    self->iObserver.SetOutputComplete( *self, KErrNone);   
    return KErrNone;	
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  	
void CStubTelephonyAudioRouting::CommitAvailableOutputsChanged()	
{
	iObserver.AvailableOutputsChanged( *this );   
	return;
}
	
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  	
CTelephonyAudioRouting::TAudioOutput CStubTelephonyAudioRouting::PreviousOutput()	
{
	return iPreviousOutput;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
TInt CStubTelephonyAudioRouting::SetShowNote( TBool aMode )
{
	iShowNote = aMode;
	return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
TInt CStubTelephonyAudioRouting::GetShowNote( TBool& aMode )
{
	aMode = iShowNote;
	return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
MTelephonyAudioRoutingObserver& CStubTelephonyAudioRouting::AudioRoutingObserver()
{
	return iObserver;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
void CStubTelephonyAudioRouting::AddAvailableOutput( TAudioOutput aOutput )
{
	iAvailableOutputs.Append(aOutput);
	return;	
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
void CStubTelephonyAudioRouting::ResetAvailableOutputs( )
{
	iAvailableOutputs.Reset();
	iAvailableOutputs.Append(CTelephonyAudioRouting::EHandset);
	iAvailableOutputs.Append(CTelephonyAudioRouting::ELoudspeaker);
	
	return;	
}

CStubAudioFactory* CStubAudioFactory::NewL()
    {
    return new( ELeave )CStubAudioFactory();
    }

CStubAudioFactory::CStubAudioFactory()
    {
    
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
CStubAudioToneUtility* CStubAudioToneUtility::NewL()
    {
    return new( ELeave )CStubAudioToneUtility();
    }
    
CStubAudioToneUtility::CStubAudioToneUtility()
    {
       
    }

void CStubAudioToneUtility::Reset()
    {
    iPrepareToPlayDesSequenceCalled = EFalse;
    iCancelPlayCallCount = 0;
    }

TBool CStubAudioToneUtility::IsPrepareToPlayDesSequenceCalled() const
    {
    return iPrepareToPlayDesSequenceCalled;
    }

TBool CStubAudioToneUtility::IsCancelPlayCalled() const
    {
    return iCancelPlayCallCount;
    }

TBool CStubAudioToneUtility::IsCancelPlayCalled( TInt aCallCount ) const
    {
    return aCallCount == iCancelPlayCallCount;
    }

void CStubAudioToneUtility::PrepareToPlayDesSequence(const TDesC8& /*aSequence*/)
    {
    iPrepareToPlayDesSequenceCalled = ETrue;
    }

TMdaAudioToneUtilityState CStubAudioToneUtility::State()
    {
    // T_CallHandling test "Test name: MO call, network changes alerting tone status between connecting and
    // connected state" is counting on that state is in EMdaAudioToneUtilityPlaying during the test.
    // 
    return EMdaAudioToneUtilityPlaying;
    }
void CStubAudioToneUtility::CancelPlay()
    {
    iCancelPlayCallCount++;
    }
TInt CStubAudioToneUtility::MaxVolume()
    {
    return 10;
    }

void CStubAudioToneUtility::SetVolume(
    TInt /*aVolume*/ )
    {
    
    }
void CStubAudioToneUtility::SetRepeats(
    TInt /*aRepeatNumberOfTimes*/, 
    const TTimeIntervalMicroSeconds& /*aTrailingSilence*/)
    {
    
    }

void CStubAudioToneUtility::SetPriority(
    TInt /*aPriority*/,
    TMdaPriorityPreference /*aPref*/)
    {
    
    }

void CStubAudioToneUtility::Play()
    {
    
    }

void CStubAudioToneUtility::SetVolumeRamp( 
    const TTimeIntervalMicroSeconds& /*aRampDuration*/)
    {
    
    }

void CStubAudioToneUtility::CancelPrepare()
    {
    
    }

// -----------------------------------------------------------------------------
// TStubAudioFactory::CreateTelephonyAudioRoutingL
// creates stub CTelephonyAudioRouting instance, defined in T_AudoHandlingMT.dll
// -----------------------------------------------------------------------------
//   
CTelephonyAudioRouting* CStubAudioFactory::CreateTelephonyAudioRoutingL( MTelephonyAudioRoutingObserver& aAudioRoutingObserver )      
    {
	return CStubTelephonyAudioRouting::NewL( aAudioRoutingObserver );
    }

CPEAudioToneUtility* CStubAudioFactory::CreateAudioToneUtilityL(
    MMdaAudioToneObserver& /*aToneObserver*/ )
    {
    iStubUtility = CStubAudioToneUtility::NewL();
    return iStubUtility;
    }

CStubAudioToneUtility* CStubAudioFactory::StubUtility() const
    {
    return iStubUtility;
    }
