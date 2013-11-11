/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles ProvideLocalInfo command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatSystemState.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "MSatEventMonitorContainer.h"
#include    "CProvideLocalInfoHandler.h"
#include    "SatLog.h"

const TInt8 KLangDescrSize( 2 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::CProvideLocalInfoHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProvideLocalInfoHandler::CProvideLocalInfoHandler() :
    CSatCommandHandler(),
    iLocalInfoData(),
    iLocalInfoPckg( iLocalInfoData ),
    iLocalInfoRsp(),
    iLocalInfoRspPckg( iLocalInfoRsp )
    {
    LOG( SIMPLE,
        "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
            CProvideLocalInfoHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProvideLocalInfoHandler* CProvideLocalInfoHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::NewL calling" )

    CProvideLocalInfoHandler* self = new( ELeave ) CProvideLocalInfoHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    // ConstructL not needed.
    CleanupStack::Pop( self );

    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::NewL exiting" )
    return self;
    }

// Destructor
CProvideLocalInfoHandler::~CProvideLocalInfoHandler()
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        CProvideLocalInfoHandler calling" )

    Cancel();

    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        ~CProvideLocalInfoHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::ClientResponse
// -----------------------------------------------------------------------------
//
void CProvideLocalInfoHandler::ClientResponse()
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        ClientResponse calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::DoCancel
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CProvideLocalInfoHandler::DoCancel()
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        DoCancel calling" )

    iUtils->USatAPI().NotifyLocalInfoCancel();

    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::IssueUSATRequest
// -----------------------------------------------------------------------------
//
void CProvideLocalInfoHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iLocalInfoData) RSat::TLocalInfoV3();

    iUtils->USatAPI().NotifyLocalInfo( aStatus, iLocalInfoPckg );

    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::CommandAllowed
// -----------------------------------------------------------------------------
//
TBool CProvideLocalInfoHandler::CommandAllowed()
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::CommandAllowed \
        calling - exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::NeedUiSession
// -----------------------------------------------------------------------------
//
TBool CProvideLocalInfoHandler::NeedUiSession()
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::NeedUiSession \
        calling - exiting" )
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::HandleCommand
// -----------------------------------------------------------------------------
//
void CProvideLocalInfoHandler::HandleCommand()
    {
    LOG( SIMPLE,
        "PROVIDELOCALINFO: CProvideLocalInfoHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::ELocalInfoExecuting );

    if ( RSat::KProvideLocalInfoLanguage == iLocalInfoData.iInfoType )
        {
        LOG( SIMPLE,"PROVIDELOCALINFO: CProvideLocalInfoHandler::\
        HandleCommand KProvideLocalInfoLanguage" )

        // Get new display text language from SAT SystemState.
        TInt language( iUtils->SystemState().DisplayTextLanguage() );

        // Maximum size in ISO 639-1 language is 2.
        TBuf<KLangDescrSize> languageISO( KNullDesC );

        // Verify language.
        // If it is automatic, language preferred by SIM is used
        TRAPD( err, iUtils->EventMonitors()->LanguageSettingL( language ) );
        LOG2( NORMAL, 
        "PROVIDELOCALINFO: CProvideLocalInfoHandler::HandleCommand err %i", 
        err )
        if ( KErrNone == err )
            {
            LOG2( NORMAL, 
            "PROVIDELOCALINFO: CProvideLocalInfoHandler::HandleCommand \
            language: %i", language )
            // If language is automatic, language = 0
            if ( language )
                {
                // Event monitor container has the function for resolving
                // language string
                iUtils->EventMonitors()->GetLanguageString(
                    language, languageISO );
                }
            }
        
        // Check the language string and build the response
        if ( languageISO != KNullDesC )
            {
            LOG( NORMAL, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
                HandleCommand Success" )
            iLocalInfoRsp.iGeneralResult = RSat::KSuccess;
            iLocalInfoRsp.iInfoType = RSat::KLocalInformation;
            iLocalInfoRsp.iAdditionalInfo.Copy( languageISO );
            }
        else
            {
            LOG( NORMAL, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
                HandleCommand UnableToProcessCmd" )
            iLocalInfoRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iLocalInfoRsp.iInfoType = RSat::KNoAdditionalInfo;
            iLocalInfoRsp.iAdditionalInfo.Zero();
            }
        }
    else
        {
        LOG( NORMAL, "PROVIDELOCALINFO: CProvideLocalInfoHandler::\
                HandleCommand TypeNotUnderstood" )
        iLocalInfoRsp.iGeneralResult = RSat::KCmdTypeNotUnderstood;
        iLocalInfoRsp.iInfoType = RSat::KNoAdditionalInfo;
        iLocalInfoRsp.iAdditionalInfo.Zero();
        }

    iLocalInfoRsp.SetPCmdNumber( iLocalInfoData.PCmdNumber() );
    TerminalRsp( RSat::ELocalInfo, iLocalInfoRspPckg );

    LOG( SIMPLE,
        "PROVIDELOCALINFO: CProvideLocalInfoHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CProvideLocalInfoHandler::UiLaunchFailed
// -----------------------------------------------------------------------------
//
void CProvideLocalInfoHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "PROVIDELOCALINFO: CProvideLocalInfoHandler::UiLaunchFailed \
        calling-exiting" )
    }
