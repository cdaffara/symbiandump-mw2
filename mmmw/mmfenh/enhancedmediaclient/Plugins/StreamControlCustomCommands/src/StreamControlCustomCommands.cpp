/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Project file for EnhancedMediaClient Utility
*
*/


#include "StreamControlCustomCommands.h"
#include "StreamControlCustomCommandsParser.h"

// Client class to access state change event functionality.
EXPORT_C RStreamControlCustomCommands::RStreamControlCustomCommands( RMMFController& aController )
: RMMFCustomCommandsBase(aController, KUidIFStreamControlCustomCommands)
    {
    }

EXPORT_C TInt RStreamControlCustomCommands::EnableEvents(TBool aEnable)
    {
    TPckgBuf<TBool> configPackage(aEnable);
    return iController.CustomCommandSync(iDestinationPckg, 
        EEnableEvents, 
        configPackage,
        KNullDesC8);
    }

EXPORT_C TInt RStreamControlCustomCommands::GetSeekingSupported(TBool& aSupported)
    {
    TPckgBuf<TBool> configPackage;
    TInt status = iController.CustomCommandSync(iDestinationPckg, 
        EGetSeekingSupported,
        KNullDesC8,
        KNullDesC8,
        configPackage);
    if (status == KErrNone)
        {
        aSupported = configPackage();
        }
    return status;										 
    }

EXPORT_C TInt RStreamControlCustomCommands::GetRandomSeekingSupported(TBool& aSupported)
    {
    TPckgBuf<TBool> configPackage;
    TInt status = iController.CustomCommandSync(iDestinationPckg, 
        EGetRandomSeekingSupported,
        KNullDesC8, 
        KNullDesC8,
        configPackage);
    if (status == KErrNone)
        {
        aSupported = configPackage();
        }
    return status;										 
    }

// Custom command parser class to be used by controller plugins wishing to
// support state change event. The controller plugin should create an object
// of this type and add it to the list of custom command parsers in the
// controller framework.
EXPORT_C CStreamControlCustomCommandParser* CStreamControlCustomCommandParser::NewL(
                                                                                    MStreamControlCustomCommandImplementor& aImplementor )
    {
    return new (ELeave) CStreamControlCustomCommandParser(aImplementor);
    }

EXPORT_C CStreamControlCustomCommandParser::~CStreamControlCustomCommandParser()
    {
    }

CStreamControlCustomCommandParser::CStreamControlCustomCommandParser( 
                                                                     MStreamControlCustomCommandImplementor& aImplementor )
                                                                     : CMMFCustomCommandParserBase( KUidIFStreamControlCustomCommands ),
                                                                     iImplementor( aImplementor )
    {
    }

// Handles a request from the client. Called by the controller framework.
void CStreamControlCustomCommandParser::HandleRequest( TMMFMessage& aMessage )
    {
    TInt status = KErrNotSupported;
    if ( aMessage.Destination().InterfaceId() == KUidIFStreamControlCustomCommands )
        {
        status = DoHandleRequest( aMessage );
        }
    aMessage.Complete(status);
    }

// Internal request handling methods.
TInt CStreamControlCustomCommandParser::DoHandleRequest( TMMFMessage& aMessage )
    {
    TInt status = KErrNotSupported;
    switch (aMessage.Function())
        {
        case EEnableEvents:
            status = DoEnableEvents(aMessage);
            break;
        case EGetSeekingSupported:
            status = DoGetSeekingSupported(aMessage);
            break;
        case EGetRandomSeekingSupported:
            status = DoGetRandomSeekingSupported(aMessage);
            break;
        default:
            break;
        }
    return status;	
    }

TInt CStreamControlCustomCommandParser::DoEnableEvents(TMMFMessage& aMessage)
    {
    TInt status;
    TPckgBuf<TBool> pckg;
    TRAPD(err, aMessage.ReadData1FromClientL(pckg));
    if(!err)
        {
        status = iImplementor.MscEnableEvents(pckg());
        }
    else
        status = err;        
    
    return status;
    }

TInt CStreamControlCustomCommandParser::DoGetSeekingSupported(TMMFMessage& aMessage)
    {
    TInt status;
    TBool supported;
    status = iImplementor.MscGetSeekingSupported(supported);
    TPckgBuf<TBool> pckg(supported);
    TRAPD(err, aMessage.WriteDataToClientL(pckg));
    if(err)
        {
            status = err;
        }
    return status;
    }

TInt CStreamControlCustomCommandParser::DoGetRandomSeekingSupported(TMMFMessage& aMessage)
    {
    TInt status;
    TBool supported;
    status = iImplementor.MscGetRandomSeekingSupported(supported);
    TPckgBuf<TBool> pckg(supported);
    TRAPD(err, aMessage.WriteDataToClientL(pckg));
    if(err)
        {
            status = err;
        }
    return status;
    }


// End of file
