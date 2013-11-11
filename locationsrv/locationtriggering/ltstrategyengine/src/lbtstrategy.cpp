/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Location Acquisition Strategy 
*
*/



#include "lbtstrategy.h"
#include "lbtstrategyengine.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtStrategy::NewL
// 
// -----------------------------------------------------------------------------
//
CLbtStrategy* CLbtStrategy::NewL( TAny* aConstructionParameters )
    {
    CLbtStrategy* self = new ( ELeave ) CLbtStrategy();
    CleanupStack::PushL( self );
    self->ConstructL( aConstructionParameters );
    CleanupStack::Pop();
    return self;
    }

    
// -----------------------------------------------------------------------------
// CLbtStrategy::~CLbtStrategy
// 
// -----------------------------------------------------------------------------
//
CLbtStrategy::~CLbtStrategy()
    {
    delete iStrategyEngine;
    }


// -----------------------------------------------------------------------------
// CLbtStrategy::StartSupervision
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategy::StartSupervision()
    {
    iStrategyEngine->StartSupervision();
    }


// -----------------------------------------------------------------------------
// CLbtStrategy::StopSupervision
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategy::StopSupervision()
    {
    iStrategyEngine->StopSupervision();
    }


// -----------------------------------------------------------------------------
// CLbtStrategy::TriggeringSupervisionSettingsChanged
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategy::TriggeringSupervisionSettingsChanged()
    {
    iStrategyEngine->SettingsChanged();
    }


// -----------------------------------------------------------------------------
// CLbtStrategy::TriggerFired
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategy::TriggerFired( CLbtGeoAreaBase::TGeoAreaType aAreaType,TLbtTriggerId aId, 
                                            const TPositionInfo& aPosInfo )
    {
    TRAP_IGNORE( TriggerFiredL(aAreaType,aId, aPosInfo ) );
    }


// -----------------------------------------------------------------------------
// CLbtStrategy::ReportTriggeringSupervisionStatus
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategy::ReportTriggeringSupervisionStatus( 
                        const TLbtStrategySupervisionDynamicInfo& aDynInfo )
    {
    SetTriggeringSupervisionDynamicInfo( aDynInfo );
    }
    

// -----------------------------------------------------------------------------
// CLbtStrategy::RetrieveTriggeringSettings
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategy::RetrieveTriggeringSettings( 
                        TLbtStrategyTriggeringSupervisionSettings& aSettings ) 
    {
    GetTriggeringSupervisionSettings( aSettings );
    }
    

// -----------------------------------------------------------------------------
// CLbtStrategy::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategy::ConstructL( TAny* aConstructionParameters )
    {
    BaseConstructL( aConstructionParameters );
    iStrategyEngine = CLbtStrategyEngine::NewL( aConstructionParameters, this );
    }
    

// -----------------------------------------------------------------------------
// CLbtStrategy::CLbtStrategy
// 
// -----------------------------------------------------------------------------
//
CLbtStrategy::CLbtStrategy() : 
                CLbtStrategyBase()
    {
    }
