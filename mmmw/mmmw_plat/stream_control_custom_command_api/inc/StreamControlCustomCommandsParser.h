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
* Description:  Header of StreamControlCustomCommandsParser interface.
*
*/


#ifndef STREAMCONTROLCUSTOMCOMMANDSCORE_H
#define STREAMCONTROLCUSTOMCOMMANDSCORE_H

#include <mmf/common/mmfcontroller.h>

//Interface UID and messages for the Stream Control API.
const TUid KUidIFStreamControlCustomCommands = {0x10207B3B};

enum TStreamControlCustomCommandMessage
    {
    EEnableEvents,
        EGetSeekingSupported,
        EGetRandomSeekingSupported
    };

// Mixin class to be derived from by controller plugins wishing to support
// state change event notification custom command.        
class MStreamControlCustomCommandImplementor
    {
    public:
        virtual TInt MscEnableEvents(TBool aEnable) = 0;
        virtual TInt MscGetSeekingSupported(TBool& aSupported) = 0;
        virtual TInt MscGetRandomSeekingSupported(TBool& aSupported) = 0;
    };

// Custom command parser class to be used by controller plugins wishing to
// support state change event. The controller plugin should create an object
// of this type and add it to the list of custom command parsers in the
// controller framework.
class CStreamControlCustomCommandParser : public CMMFCustomCommandParserBase
    {
    public:
        IMPORT_C static CStreamControlCustomCommandParser* NewL(
            MStreamControlCustomCommandImplementor& aImplementor );
        IMPORT_C ~CStreamControlCustomCommandParser();
        
        // Handles a request from the client. Called by the controller framework.
        void HandleRequest( TMMFMessage& aMessage );
    private:
        CStreamControlCustomCommandParser(MStreamControlCustomCommandImplementor& aImplementor);
        // Internal request handling methods.
        TInt DoHandleRequest(TMMFMessage& aMessage);
        TInt DoEnableEvents(TMMFMessage& aMessage);
        TInt DoGetSeekingSupported(TMMFMessage& aMessage);
        TInt DoGetRandomSeekingSupported(TMMFMessage& aMessage);
        
    private:
        MStreamControlCustomCommandImplementor& iImplementor;
    };

#endif // STREAMCONTROLCUSTOMCOMMANDSCORE_H

// End of file
