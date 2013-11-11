// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Generic File Parser:
// - Operator logo, OTA bitmap, parser 
// - Ringing Tones
// Still in a rudimentary shape, pending specifications and design from Symbian.
// At the present the parser can only parse an Operator Logo Message containing 
// an OTA bitmap.
// When ParseL is called the message is passed to the base class StoreL method,
// which converts the descriptor to 8bit format,ie binary,
// and then saves it as an attachment file. 
// Possibles: May have to implement a ProcessL fn to either save the file in a different directory
// or launch an application, after the user presses the accept button in the viewer.
// 
//


#include <msventry.h>
#include <biodb.h>
//
#include <gfp.h>


//
// Constructor
//
CMsvGenericFileParser::CMsvGenericFileParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
: CBaseScriptParser2(aRegisteredParserDll, aEntry, aFs), 
iState(0), iReport(0), iCompleted(0)
    {
    }

//
// Factory methods
//
EXPORT_C CMsvGenericFileParser* CMsvGenericFileParser::NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
    {
    CMsvGenericFileParser* self = new (ELeave) CMsvGenericFileParser(aRegisteredParserDll, aEntry, aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

void CMsvGenericFileParser::ConstructL()
    {
    CActiveScheduler::Add(this);
    }

CMsvGenericFileParser::~CMsvGenericFileParser()
    {
    if (iParsedFieldArray != NULL)
        {
        iParsedFieldArray->ResetAndDestroy();
        delete iParsedFieldArray;
        }
	if (iSmsBuf)
		delete iSmsBuf;
    }

void CMsvGenericFileParser::ParseL(TRequestStatus& aStatus, const TDesC& aSms)
    {
    TMsvEntry entry = iEntry.Entry();   //  Get the generic stuff
    iEntryId = entry.Id();              //  store the TMsvId
	iBioType= entry.iBioType;
    
    __ASSERT_DEBUG((entry.MtmData3() == 0 || entry.MtmData3() == 1),
                            User::Panic(_L("GXXP-DLL"),KErrGeneral));

    //  Already parsed....
    if(entry.MtmData3() == 1)
        {
        iReport = &aStatus;
        User::RequestComplete(iReport, KErrNone);
        }
    //  not parsed
    else if(entry.MtmData3() == 0)
        {
        if (iSettings!= NULL)
            {
            delete iSettings;
            iSettings = NULL;
            }
        iSettings = aSms.AllocL();         // Allocate new HBufC object
        ChangeStateL(EParseMessage);       //Don't set initial request -just parse it???
        aStatus = KRequestPending;
        iReport = &aStatus;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }


//
//  ProcessL() --   Currently, the parser doesn't actually use a  separate
//					 commit phase to save any information to persistant .
//                  store. This is done as part of the ParseL function.  Therefore  
//                  this function is superflous, and is only required to satisfy  
//					the base class inheritance requirements.  This may change...
//                  .. Complete  KErrNotSupported..
//
void CMsvGenericFileParser::ProcessL(TRequestStatus& aStatus)
    {
    iReport = &aStatus;
    User::RequestComplete(iReport, KErrNotSupported);
    }

void CMsvGenericFileParser::DoCancel()
    {
	User::RequestComplete(iReport,KErrCancel);
    }

void CMsvGenericFileParser::RunL()
    {
    iCompleted = iStatus.Int();
    if (iCompleted != KErrNone)
        {
        User::RequestComplete(iReport,iCompleted);
        return;
        }

    switch (iState)
        {
        case EParseMessage:
            TRAPD(err, ChangeStateL(ECompleteMessage));
            if(err != KErrNone)
                {
                User::RequestComplete(iReport, err);
                }
            break;
                        
        case ECompleteMessage:
            User::RequestComplete(iReport, KErrNone);
            break;

        default:
            break;
        }
    }

void CMsvGenericFileParser::ChangeStateL(TParseSession aState)
    {
    iState = aState;
    switch (iState)
        {                    
        case EParseMessage:
            ParseMessageL();
            break;

        case ECompleteMessage:
            CompleteMessageL();
            break;

        default:
            break;
        }
	RequestComplete(iStatus, KErrNone);

    SetActive();
    }

void CMsvGenericFileParser::ParseMessageL()
    {
	// Header of message has already been stripped away by Socket Observer
	// The message body is therefore our binary data converted to unicode.
	// We need to call a function to convert descriptor and create an 
	// eight bit attachment file. Also want to put a message into the body

	// Store (Ringing Tones or OL/VCD/VCL...) to file 

	TUid uid;
	TInt count =0;
	HBufC* fileExt=NULL;

	CBIODatabase* bioDB = CBIODatabase::NewL(iFs);
	CleanupStack::PushL(bioDB);
	
	count = bioDB->BIOCount();
	while (count--)
		{
		bioDB->GetBioMsgID(count, uid);
		if(uid == TUid::Uid(iBioType))
			{
			fileExt=bioDB->GetFileExtL(uid).AllocLC();//get file extension
			break;
			}
		}

	if(fileExt == NULL || fileExt->Length()<=0)
		User::Leave(KErrNotFound);
	
	TBuf<64> tempBuf;
	tempBuf.Format(_L("%x"), iEntry.Entry().Id());
	tempBuf.Append(fileExt->Des());
	StoreL(TFileName(tempBuf));   // pass the filename to base class method - creates a new folder 
						// linked to message entry and stores body as an 8 bit  attachment file
		
	CleanupStack::PopAndDestroy(2);// fileExt, bioDB
    }

void CMsvGenericFileParser::CompleteMessageL()
    {
	// Binary data has been saved in attachment file. 

	iEntry.SetEntryL(iEntryId);

    //  Change the current message details
	TMsvEntry entry= iEntry.Entry();

    entry.SetMtmData3(1);    //  Indicates that we've parsed it..

	iEntry.ChangeL(entry);
    }

void CMsvGenericFileParser::RequestComplete(TRequestStatus& aStatus, TInt aError)
    {
    TRequestStatus* p = &aStatus;
    User::RequestComplete(p, aError);
    }
