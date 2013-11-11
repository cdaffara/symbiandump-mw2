/*!
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Provides information about ongoing calls.
*
*/

#include "dialpadsymbianwrapper.h"
#include "dialpadsymbianwrapper_p.h"
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <centralrepository.h>
#include <profileenginesdkcrkeys.h>
#include <profileengineinternalcrkeys.h>

DialpadSymbianWrapperPrivate::DialpadSymbianWrapperPrivate(DialpadSymbianWrapper *parent) : 
    q_ptr(parent)
{
    TRAPD(error, mVmbx = CVoiceMailbox::NewL());
    qt_symbian_throwIfError(error);
}

DialpadSymbianWrapperPrivate::~DialpadSymbianWrapperPrivate()
{
    delete mVmbx;
    mVmbx = NULL;
}

int DialpadSymbianWrapperPrivate::getMailboxNumber(QString &vmbxNumber)
{
    int errValue(KErrNone);
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;

    errValue = mVmbx->GetStoredEntry(vmbxParams, vmbxEntry);
    
    if (KErrNone == errValue) {
        // Number retrieved succesfully:
        vmbxNumber = getVmbxNumber(*vmbxEntry);
     }
    // Entry ownership was transferred.
    delete vmbxEntry;
    vmbxEntry = NULL;
    return errValue;
}

int DialpadSymbianWrapperPrivate::getVideoMailboxNumber(QString &vmbxNumber)
{
    int errValue(KErrNone);
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    
    errValue = mVmbx->GetStoredEntry(vmbxParams, vmbxEntry);
    
    if (KErrNone == errValue) {
        // Number retrieved succesfully:
        vmbxNumber = getVmbxNumber(*vmbxEntry);
     }
    // Entry ownership was transferred.
    delete vmbxEntry;
    vmbxEntry = NULL;
    return errValue;		
}

int DialpadSymbianWrapperPrivate::defineMailboxNumber(QString &vmbxNumber)
{
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;

    int errValue = mVmbx->QueryNewEntry(vmbxParams, vmbxEntry);
    if (KErrNone == errValue) {
        mVmbx->SaveEntry(*vmbxEntry);
        // Do appropriate tasks, e.g. save number.
        vmbxNumber = getVmbxNumber(*vmbxEntry);
    }

    return errValue;
}

int DialpadSymbianWrapperPrivate::defineVideoMailboxNumber(QString &vmbxNumber)
{
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;

    int errValue = mVmbx->QueryNewEntry(vmbxParams, vmbxEntry);
    if (KErrNone == errValue) {
        mVmbx->SaveEntry(*vmbxEntry);
        // Do appropriate tasks, e.g. save number.
        vmbxNumber = getVmbxNumber(*vmbxEntry);
    }

    return errValue;
}

QString DialpadSymbianWrapperPrivate::getVmbxNumber(CVoiceMailboxEntry &vmbxEntry)
{
    TPtrC ptrNumber(KNullDesC);
    QString dialpadText = NULL;
    if(KErrNone == vmbxEntry.GetVmbxNumber(ptrNumber)) {
        dialpadText = QString::fromUtf16(ptrNumber.Ptr(), ptrNumber.Length());
    }
    return dialpadText;
}

bool DialpadSymbianWrapperPrivate::changeSilentModeState()
{   
    // first get present value from cenrep
    int silenceMode( 0 );
    CRepository* cenRep = CRepository::NewL( KCRUidProfileEngine );
    int err = cenRep->Get( KProEngSilenceMode, silenceMode );
    
    // then set it to another one
    if (KErrNone == err) {
        if (silenceMode) {
            silenceMode = 0;
        }
        else {
            silenceMode = 1;
        }
    }

    err = cenRep->Set( KProEngSilenceMode, silenceMode );
    delete cenRep;
    return silenceMode;
}

