/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the implementation of the CCallAudioControl class.
*
*/

// INCLUDE FILES
#include "CallAudioControl.h"
#include "CallAudioControlImpl.h"
#include "CallAudioControlCommon.h"

// Two-phased constructor.
EXPORT_C CCallAudioControl* CCallAudioControl::NewL()
    {
	CAC_TRACE1(_L("CCallAudioControl::NewL"));
	CCallAudioControl* self = new (ELeave)CCallAudioControl;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 		  	
    return self;	
    }
    
CCallAudioControl::CCallAudioControl()
	{
	CAC_TRACE2(_L("CCallAudioControl[%x]:CCallAudioControl"),this);
	}    
// -----------------------------------------------------------------------------
// CCallAudioControl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCallAudioControl::ConstructL()
    {
	CAC_TRACE1(_L("CCallAudioControl::ConstructL"));  
    iImpl = CCallAudioControlImpl::NewL();
    }
       
// -----------------------------------------------------------------------------
// CMMFDevSoundAdaptation::DestructL
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CCallAudioControl::~CCallAudioControl()
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]CallAudioControl"),this); 
    delete iImpl;
    }

// ---------------------------------------------------------
// CCallAudioControl::AppendRoutingObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::AppendRoutingObserver(CRoutingObserver& aObserver)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]AppendRoutingObserver"),this); 
    return iImpl->AppendRoutingObserver(aObserver);
    }

// ---------------------------------------------------------
// CCallAudioControl::RemoveRoutingObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::RemoveRoutingObserver(CRoutingObserver& aObserver)
    {
    CAC_TRACE2(_L("CCallAudioControl[%x]RemoveRoutingObserver"),this); 
    return iImpl->RemoveRoutingObserver(aObserver);
    }

// ---------------------------------------------------------
// CCallAudioControl::AppendDownlinkVolumeObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::AppendDownlinkVolumeObserver(CDownlinkVolumeObserver& aObserver)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]AppendDownlinkVolumeObserver"),this); 
    return iImpl->AppendDownlinkVolumeObserver(aObserver);
    }

// ---------------------------------------------------------
// CCallAudioControl::RemoveDownlinkVolumeObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::RemoveDownlinkVolumeObserver(CDownlinkVolumeObserver& aObserver)
    {
    CAC_TRACE2(_L("CCallAudioControl[%x]RemoveDownlinkVolumeObserver"),this); 
    return iImpl->RemoveDownlinkVolumeObserver(aObserver);
    }
    
// ---------------------------------------------------------
// CCallAudioControl::AppendUplinkGainObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::AppendUplinkGainObserver(CUplinkGainObserver& aObserver)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]AppendUplinkGainObserver"),this); 
    return iImpl->AppendUplinkGainObserver(aObserver);
    }

// ---------------------------------------------------------
// CCallAudioControl::RemoveUplinkGainObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::RemoveUplinkGainObserver(CUplinkGainObserver& aObserver)
    {
    CAC_TRACE2(_L("CCallAudioControl[%x]RemoveUplinkGainObserver"),this); 
    return iImpl->RemoveUplinkGainObserver(aObserver);
    }
    
// ---------------------------------------------------------
// CCallAudioControl::GetAvaliableOutputs
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::GetAvailableOutputs(RArray<CCallAudioControl::TAudioOutput>& aAvailableOutputs)
    {
    CAC_TRACE2(_L("CCallAudioControl[%x]GetAvailableOutputs"),this); 
    return iImpl->GetAvailableOutputs(aAvailableOutputs);
    }
    
// ---------------------------------------------------------
// CCallAudioControl::GetOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::GetOutput(CCallAudioControl::TAudioOutput& aOutput)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]GetOutput"),this);
    return iImpl->GetOutput(aOutput);
    }
    
// ---------------------------------------------------------
// CCallAudioControl::SetOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::SetOutput(CCallAudioControl::TAudioOutput aOutput)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]SetOutput"),this);
    return iImpl->SetOutput(aOutput);
    }
    
// ---------------------------------------------------------
// CCallAudioControl::PreviousOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::PreviousOutput(CCallAudioControl::TAudioOutput& aPreviousOutput)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]PreviousOutput"),this);
    return iImpl->PreviousOutput(aPreviousOutput);
    }    
        
// ---------------------------------------------------------
// CCallAudioControl::GetMinDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::GetMinDownlinkVolume(TUint& aVolume)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]GetMinDownlinkVolume"),this);
    return iImpl->GetMinDownlinkVolume(aVolume);
    }  
    
// ---------------------------------------------------------
// CCallAudioControl::GetDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::GetDownlinkVolume(TUint& aVolume)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]GetDownlinkVolume"),this);
    return iImpl->GetDownlinkVolume(aVolume);
    }  
      
// ---------------------------------------------------------
// CCallAudioControl::GetMaxDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::GetMaxDownlinkVolume(TUint& aVolume)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]GetMaxDownlinkVolume"),this);
    return iImpl->GetMaxDownlinkVolume(aVolume);
    } 
       
// ---------------------------------------------------------
// CCallAudioControl::SetDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::SetDownlinkVolume(TUint aVolume)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]SetDownlinkVolume"),this);
    return iImpl->SetDownlinkVolume(aVolume);
    } 
    
// ---------------------------------------------------------
// CCallAudioControl::GetUplinkMute
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::GetUplinkMute(TBool& aMute)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]GetUplinkMute"),this);
    return iImpl->GetUplinkMute(aMute);
    } 
    
// ---------------------------------------------------------
// CCallAudioControl::SetUplinkMute
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CCallAudioControl::SetUplinkMute(TBool aMute)
    {
	CAC_TRACE2(_L("CCallAudioControl[%x]SetUplinkMute"),this);
    return iImpl->SetUplinkMute(aMute);
    } 
                                 
    
// End of file

