// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// @file
// This contains Class CSendAs2SetMessageCharset.
// 
//

#ifndef __T_SENDAS2_MESSAGE_SEND_SETCHARSET_H__
#define __T_SENDAS2_MESSAGE_SEND_SETCHARSET_H__


// epoc include
#include <sendas2.h>
#include <csendasaccounts.h>
#include <csendasmessagetypes.h>
#include <mtmuids.h>
#include <e32cons.h>    
#include <msvids.h>
#include <gsmuelem.h>
#include "msvtestutilsbase.h"

GLREF_D RTest test; 

class CDummyObserver;
/**
CSendAs2SetMessageCharset is a class that publicly inherits from CBase. 
This class demonstrates- creating a session, connecting to
sendAs server to create a message, displaying the list of SendAs accounts present,
*/
class CSendAs2SetMessageCharset : public CBase
{
public:
    static CSendAs2SetMessageCharset* NewL();
    ~CSendAs2SetMessageCharset();
    void StartL();
    void Connect();
    void CreateL(RSendAsMessage& aMessage);
    void DisplayAccountL();
    void SendL(RSendAsMessage& aMessage);
    void Delete();
    
    void TestOOMForUCS2(RSendAsMessage& aMessage);
    void TestOOMFor8Bit(RSendAsMessage& aMessage);
    void TestOOMFor7Bit(RSendAsMessage& aMessage);
   
 	void TestSet8BitCharsetSendMessageL(RSendAsMessage &message);
 	void TestSet7BitCharsetSendMessageL(RSendAsMessage &message);
 	void TestSet16BitUnicodeCharsetSendMessageL(RSendAsMessage &message);
    
private:
    CSendAs2SetMessageCharset();
    void ConstructL();
    void CleanMessageFolderL();

private:
    /** Pointer to the console interface */
    CConsoleBase*           iConsole;
    RSendAs                 iSendAs;
    /** Pointer to Session observer used to report whatever notification it receives */ 
    CDummyObserver*         iObserver;
    /** Pointer to the channel of communication between a client thread and the Message Server */
    CMsvSession*            iSession;
    /** Pointer to a particular Message Server entry */
    CMsvEntry*              iEntry;
    /** Pointer to the array of entry IDs */
    CMsvEntrySelection*     iSelection;
 };

#endif /*__T_SENDAS2_MESSAGE_SEND_SETCHARSET_H__*/
