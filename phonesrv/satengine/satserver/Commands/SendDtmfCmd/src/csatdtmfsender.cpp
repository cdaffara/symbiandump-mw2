/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles SendDTMF command
*
*/


#include    "csatdtmfsender.h"
#include    "CSendDtmfHandler.h"
#include    "msatmultimodeapi.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatDtmfSender::CSatDtmfSender
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatDtmfSender::CSatDtmfSender( 
    CSendDtmfHandler& aDtmfHandler, MSatMultiModeApi& aPhone ) : 
    CActive( EPriorityStandard ),
    iDtmfHandler( aDtmfHandler ),
    iPhone( aPhone )
    {
    LOG( SIMPLE,
        "SENDDTMF: CSatDtmfSender::CSatDtmfSender calling - exiting" )
    CActiveScheduler::Add( this );
    }

// Destructor
CSatDtmfSender::~CSatDtmfSender()
    {
    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::~CSatDtmfSender calling" )

    Cancel();

    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::~CSatDtmfSender exiting" )
    }

// -----------------------------------------------------------------------------
// CSatDtmfSender::SendToneString
// Sends DTMF tones during active, connected voice calls.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatDtmfSender::SendToneString( const TDesC& aTones )
    {
    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::SendToneString calling" )

    if ( !IsActive() )
        {
        LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::SendToneString set active" )
        iPhone.SendDTMFTones( iStatus, aTones );
        SetActive();
        }

    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::SendToneString exiting" )
    }

// -----------------------------------------------------------------------------
// CSatDtmfSender::DoCancel
// Cancels the DTMF sending request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatDtmfSender::DoCancel()
    {
    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::DoCancel calling" )

    iPhone.CancelAsyncRequest( EMobilePhoneSendDTMFTones );

    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatDtmfSender::RunL
// Informs observer about result of DTMF sending.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatDtmfSender::RunL()
    {
    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::RunL calling" )

    iDtmfHandler.SendDtmfComplete( iStatus.Int() );

    LOG( SIMPLE, "SENDDTMF: CSatDtmfSender::RunL exiting" )
    }

