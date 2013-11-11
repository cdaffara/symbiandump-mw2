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
* Description:  Active Object operation for serving server logic requests
*
*/



// INCLUDE FILES
#include <e32cmn.h>
#include <lbttriggerentry.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeoareabase.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerconditionarea.h>
#include <lbttriggerdynamicinfo.h>
#include <s32mem.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbterrors.h>
#include "lbtcreatetriggeraooperation.h"
#include "lbtaoobserver.h"
#include "lbtserverconsts.h"
#include "lbtglobal.h"
#include "lbtcontainertriggerentry.h"
#include "lbtcontainer.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtsettingsmanager.h"
#include "lbtlogger.h"

// CONSTANTS
const TInt KCircleRadiusCutoff(500);
const TReal KMinHysteresisArea(20);


// ===================== MEMBER FUNCTIONS =====================

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtCreateTriggerAOOperation* CLbtCreateTriggerAOOperation::NewL(MLbtAOObserver& aObserver,
															     const RMessage2& aMessage,
															     CLbtContainer& aContainer,
															     TInt aClientType,
															     CLbtSettingsManager& aSettingsManager )
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::NewL");
	CLbtCreateTriggerAOOperation* self = new (ELeave) CLbtCreateTriggerAOOperation(aObserver, 
																				   aMessage, 
																				   aContainer, 
																				   aClientType,
																				   aSettingsManager);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::~CLbtCreateTriggerAOOperation
// Destructor
// ---------------------------------------------------------------------------
//
CLbtCreateTriggerAOOperation::~CLbtCreateTriggerAOOperation()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::~CLbtCreateTriggerAOOperation");
	iContainer.RemoveObserver(this);
	// Call cancel operation
	if( IsActive() )
	    {
	    Cancel();
	    }
	delete iContainerTriggerEntry;
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::CLbtCreateTriggerAOOperation
// C++ Default constructor
// ---------------------------------------------------------------------------
//
CLbtCreateTriggerAOOperation::CLbtCreateTriggerAOOperation(MLbtAOObserver& aObserver,
														   const RMessage2& aMessage,
														   CLbtContainer& aContainer,
														   TInt aClientType,
														   CLbtSettingsManager& aSettingsManager)
	: CLbtAOOperationBase(aObserver, aMessage, aContainer, aClientType),
	  iSettingsManager(aSettingsManager)
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::CLbtCreateTriggerAOOperation");
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtCreateTriggerAOOperation::ConstructL()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::ConstructL");
	CActiveScheduler::Add(this);
	}
	
// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::RunL
// ---------------------------------------------------------------------------
//
void CLbtCreateTriggerAOOperation::RunL()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::RunL");	
	switch(iFunction)
		{
		case ELbtCreateTrigger:
			{
			if(iStatus.Int() == KErrNone)
				{
				LOGSTRING("Trigger created: \"%S\" ", &(iContainerTriggerEntry->TriggerEntry()->Name()));
				LOG1("Trigger Id:%d",iContainerTriggerEntry->TriggerEntry()->Id());
				LOG1("Trigger state:%d",iContainerTriggerEntry->TriggerEntry()->State());
				// For create trigger server has to write the trigger id back to the client
				TPckg<TUint32> triggerId(iContainerTriggerEntry->TriggerEntry()->Id());
				LbtGlobal::Write(iMessage, KParamTriggerId,triggerId);
				}
			LbtGlobal::RequestComplete(iMessage, iStatus.Int());			
			break;
			}
		default:
			{
			// Handle the completion of request. Ideally this should not execute
			LbtGlobal::RequestComplete(iMessage, iStatus.Int());
			break;
			}
		}

	iObserver.HandleOperationClosureL(this, iStatus.Int());
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtCreateTriggerAOOperation::DoCancel()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::DoCancel");
	// Cancel container notifications
	iContainer.CancelAsyncOperation(iAOIdentificationNum);
	iMessage.Complete( iStatus.Int() );
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::StartOperationL
// ---------------------------------------------------------------------------
//
void CLbtCreateTriggerAOOperation::StartOperationL()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::StartOperationL");
	switch(iFunction)
		{
		case ELbtCreateTrigger:
			{
			if( CheckTriggerCap() )
				{
				CreateTriggerL();
				}
			else
				{
				LbtGlobal::RequestComplete(iMessage, KLbtErrMaxEnabledValidTriggersLimitExceeded );
				iObserver.HandleOperationClosureL(this, KLbtErrMaxEnabledValidTriggersLimitExceeded );
				}
			break;
			}
		default:
			{
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::CheckTriggerCap()
// ---------------------------------------------------------------------------
//
TBool CLbtCreateTriggerAOOperation::CheckTriggerCap()
	{
	TUint enabledValidTriggerCount = iContainer.GetCountOfEnabledAndValidTriggers();
	
	if( iSettingsManager.EnabledValidTriggerCount() > enabledValidTriggerCount )
		{
		// More triggers can be created. Return true
		return ETrue;
		}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::CreateTriggerL()
// ---------------------------------------------------------------------------
//
void CLbtCreateTriggerAOOperation::CreateTriggerL()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::CreateTriggerL");
	// Copy the trigger entry object descriptor from the client IPC message
	HBufC8* triggerEntryBuffer = LbtGlobal::CopyClientBuffer8LC(iMessage, KParamTriggerEntry);
	
	if( triggerEntryBuffer == NULL )
		{
		iObserver.HandleOperationClosureL(this, KErrBadDescriptor);
		return;
		}
	
	TLbtTriggerCreationInfo triggerCreationInfo;
	
	TPckg<TLbtTriggerCreationInfo> des(triggerCreationInfo);
	LbtGlobal::Read(iMessage,2,des);

		
	// Read the buffer into a stream
	RDesReadStream stream(*triggerEntryBuffer);
	CleanupClosePushL(stream);
	
	// Create the trigger entry object based on the type of trigger
	CLbtTriggerEntry* trigger;
	if( triggerCreationInfo.iTriggerType== CLbtTriggerEntry::ETypeStartup)
		{
		trigger = CLbtStartupTrigger::NewLC();
		}
	else
		{
		trigger = CLbtSessionTrigger::NewLC();
		}
		
	// Internalize the trigger entry class since it is a "C" class
	trigger->InternalizeL(stream);	
		
	// Fill the extended trigger info required by Stratergy
	CLbtExtendedTriggerInfo::TLbtTriggerRectArea area;
		
	if(trigger->GetCondition()->Type() == CLbtTriggerConditionBase::ETriggerConditionArea)
		{
		// Get the condition area base
		CLbtTriggerConditionArea* conditionArea = 
				static_cast<CLbtTriggerConditionArea*>(trigger->GetCondition());
		
		// Check if the area is a circle
		if(conditionArea->TriggerArea()->Type() == CLbtGeoAreaBase::ECircle)
			{
			CLbtGeoCircle* circle = static_cast<CLbtGeoCircle*>(conditionArea->TriggerArea());
			
			// If the triggering area specified is less than the minimum triggering area
			// the return KErrTriggerAreaTooSmall
			
			if(circle->Radius() < iSettingsManager.MinimumTriggeringArea())
				{
				CleanupStack::PopAndDestroy( 3 ); // trigger,stream,triggerEntry
				LbtGlobal::RequestComplete(iMessage, KErrTriggerAreaTooSmall);
				iObserver.HandleOperationClosureL(this, KErrTriggerAreaTooSmall);
				return;
				}

            CalculateRectangularArea(area, circle);
			}
		}
		
	// Create the extended trigger info
	CLbtExtendedTriggerInfo* extendedTriggerInfo = 
							CLbtExtendedTriggerInfo::NewL(area,
														  CalculateHysteresisAreaForTrigger(*trigger),
														  EFalse,
														  KNullDesC8,
														  iMessage.SecureId(),
														  triggerCreationInfo.iFireOnCreation);
	CleanupStack::PushL(extendedTriggerInfo);
	
	TLbtTriggerDynamicInfo* dynInfo = new (ELeave) TLbtTriggerDynamicInfo;
	CleanupStack::PushL(dynInfo);
	dynInfo->iValidity = TLbtTriggerDynamicInfo::EValid;
	

		
	// Create trigger class of Container. Container takes ownership of trigger and all info sent to it
	iContainerTriggerEntry = CLbtContainerTriggerEntry::NewL(trigger, dynInfo, extendedTriggerInfo);
	
	// Register for container change event observer
    TLbtTriggerEventMask eventMask = 0;
    eventMask|= CLbtContainer::ELbtConTriggerCreated;
                
    iContainer.SetChangeObserver( this ,eventMask );
	
	// Creates the trigger in the container		
	iContainer.CreateTrigger(*iContainerTriggerEntry,iAOIdentificationNum,iStatus);
	SetActive();
	
	CleanupStack::Pop(5); //trigger, stream, triggerEntryBuffer, dynInfo and extendedTriggerInfo
	stream.Close();
	delete triggerEntryBuffer;
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::CancelCreateTrigger
// ---------------------------------------------------------------------------
//
void CLbtCreateTriggerAOOperation::CancelCreateTrigger()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::CancelCreateTrigger");
	Cancel();
	}
	
// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::GetTriggerId
// ---------------------------------------------------------------------------
//
TLbtTriggerId CLbtCreateTriggerAOOperation::GetTriggerId()
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::GetTriggerId");
	return iContainerTriggerEntry->TriggerEntry()->Id();
	}

// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::CalculateHysteresisAreaForTrigger
// ---------------------------------------------------------------------------
//
TInt CLbtCreateTriggerAOOperation::CalculateHysteresisAreaForTrigger(
    CLbtTriggerEntry& aTrigger)
	{
	FUNC_ENTER("CLbtCreateTriggerAOOperation::CalculateHysteresisAreaForTrigger");
	// The Hysteresis for trigger is calculated as per this formula
	// If radius <= 500 
	//      hysteresis = radius / 4.
	// Else
	//      hysteresis = radius / Ln(radius/10) ( Log to the base e)
	// After the formula 
	// if hysteresis < MinimumHysteresisRadius
	//      hysteresis is taken to be the minimumHysteresis radius.
	
	// The constant values have to be store in cenrep and read
	// from there.
	
	TReal hysteresis = 0;
	CLbtTriggerConditionBase* conditionBase = aTrigger.GetCondition();
	TReal radius = 0.0;
	
	// Fist find the type of the trigger
	if(conditionBase->Type() == 
	                    CLbtTriggerConditionBase::ETriggerConditionArea)
		{
		CLbtTriggerConditionArea* basicCondition = 
		                static_cast<CLbtTriggerConditionArea*>(conditionBase);
		if(basicCondition->TriggerArea()->Type() == CLbtGeoAreaBase::ECircle)
			{
			// The trigger area is circular. Apply hysteresis formula for a circular area
			CLbtGeoCircle* circle = 
			        static_cast<CLbtGeoCircle*>(basicCondition->TriggerArea());
			
			radius = circle->Radius();
			
			if(radius > KCircleRadiusCutoff)
				{
				TReal logRadius;				
				Math::Ln(logRadius, radius / 10 );
				hysteresis = radius / logRadius;
				}
			else
				{
				hysteresis = radius / 4;
				}
			}
		}
		
	if(hysteresis < KMinHysteresisArea)
		{
		hysteresis = KMinHysteresisArea;
		}

	return (hysteresis + radius);
	}


// ---------------------------------------------------------------------------
// CLbtCreateTriggerAOOperation::CalculateRectangularArea
// ---------------------------------------------------------------------------
//
void CLbtCreateTriggerAOOperation::CalculateRectangularArea(
    CLbtExtendedTriggerInfo::TLbtTriggerRectArea& aArea,
    CLbtGeoCircle* aCircle)
    {
    FUNC_ENTER("CLbtCreateTriggerAOOperation::CalculateRectangularArea");
	TCoordinate center = aCircle->Center();
	TCoordinate tmp = center;
	// Get the North Latitude of the ractangular area by moving north
	// (by using a bearing of 0).
	tmp.Move(0, aCircle->Radius());
	aArea.iTrigAreaNorthLat = tmp.Latitude();

	// From the previous point, Move west(by using a bearing of 270).
	// This gives the West Longitude.
	tmp.Move(270,aCircle->Radius());
	aArea.iTrigAreaWestLong = tmp.Longitude();
	

	// Reset tmp to the center
	tmp = center;

	// Get the South Latitude by moving south
	// (by using a bearing of 180).
	tmp.Move(180, aCircle->Radius());
	aArea.iTrigAreaSouthLat = tmp.Latitude();

	// From the previous point, Move east(by using a bearing of 90).
	// This gives the Eest Longitude.
	tmp.Move(90,aCircle->Radius());
	aArea.iTrigAreaEastLong = tmp.Longitude();
	
	return;
    }
// end of file
