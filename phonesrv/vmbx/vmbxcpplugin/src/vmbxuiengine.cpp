/*
 * Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 *
 */

// System includes
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>

// User includes
#include "vmbxuiengine.h"
#include "loggerutil.h"

/*!
    Constructor
*/
VmbxUiEngine::VmbxUiEngine() : mVmbxEngine(NULL),
    mCsVoiceNumber(""),mCsVideoNumber("")
{ 
    _DBGLOG("VmbxUiEngine::VmbxUiEngine >")
    QT_TRAP_THROWING(mVmbxEngine = CVoiceMailbox::NewL());
    init();
    _DBGLOG("VmbxUiEngine::VmbxUiEngine <")
}

/*!
    Destructor
*/
VmbxUiEngine::~VmbxUiEngine()
{
    _DBGLOG( "VmbxUiEngine::~VmbxUiEngine >")
    mVmbxEngine->NotifyVmbxNumberChangeCancel();
    delete mVmbxEngine;
    mVmbxEngine = NULL;
    _DBGLOG( "VmbxUiEngine::~VmbxUiEngine <")
}

/*!
    Initialize vmbx ui engine.
*/
void VmbxUiEngine::init()
{
    _DBGLOG("VmbxUiEngine::init >")
    // Init voice primary line number
    getCsVoicePrimaryNumber(mCsVoiceNumber);
    // Init video primary line nubmer
    if (isVideoSupport()) {
        getCsVideoPrimaryNumber(mCsVideoNumber);
    }
    _DBGLOG("VmbxUiEngine::init <")
}

/*!
    Getter method for primary voice number of Voice Mailbox.
*/
void VmbxUiEngine::getCsVoiceNumber(QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getCsVoiceNumber >")
    aValue = mCsVoiceNumber;
    _DBGLOG2("VmbxUiEngine::getCsVoiceNumber < , string=", aValue)
}

/*!
    Getter method for primary video number of Voice Mailbox.
*/
void VmbxUiEngine::getCsVideoNumber(QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getCsVideoNumber >")
    aValue = mCsVideoNumber;
    _DBGLOG2("VmbxUiEngine::getCsVideoNumber < , string=", aValue)
}

/*!
    Set number when cs voice number has been edited on UI.
    @param aValue New value.
*/
void VmbxUiEngine::setCsVoiceNumber(const QString &aValue)
{
    _DBGLOG2("VmbxUiEngine::setCsVoiceNumber >, value=",aValue)
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    setNumber(vmbxParams, aValue);
    mCsVoiceNumber = aValue;
    _DBGLOG("VmbxUiEngine::setCsVoiceNumber <");
}

/*!
    Set number when cs video number has been edited on UI.
    @param aValue New value.
*/
void VmbxUiEngine::setCsVideoNumber(const QString &aValue)
{
    _DBGLOG2("VmbxUiEngine::setCsVideoNumber >, value=",aValue)
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVideo;
    setNumber(vmbxParams, aValue);
    mCsVideoNumber = aValue;
    _DBGLOG("VmbxUiEngine::setCsVideoNumber <");
}

/*!
    If video mailbox supported.
*/
bool VmbxUiEngine::isVideoSupport()
{
    _DBGLOG("VmbxUiEngine::isVideoSupport >")
    // video support
    bool isVideoSupport(false);
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVideo;
    isVideoSupport = mVmbxEngine->CheckConfiguration(vmbxParams,
        EVmbxVideoMailboxSupported);
    _DBGLOG2("VmbxUiEngine::isVideoSupport ", isVideoSupport);
    return isVideoSupport;
}

/*!
    If voice mailbox allow to write.
*/
bool VmbxUiEngine::isVoiceWritable()
{
    _DBGLOG("VmbxUiEngine::isVoiceWritable >")
    bool isVoiceWritable(false);
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    isVoiceWritable = mVmbxEngine->CheckConfiguration(vmbxParams,
        EVmbxChangeNbrAllowedOnUi);
    _DBGLOG2("VmbxUiEngine::isVoiceWritable <", isVoiceWritable);
    return isVoiceWritable;
}

/*!
    If video mailbox allow to write.
*/
bool VmbxUiEngine::isVideoWritable()
{
    _DBGLOG( "VmbxUiEngine::isVideoWritable >")
    bool isVideoWritable(false);
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVideo;
    isVideoWritable = mVmbxEngine->CheckConfiguration(vmbxParams,
        EVmbxChangeNbrAllowedOnUi);
    _DBGLOG2("VmbxUiEngine::isVoiceWritable >", isVideoWritable);
    return isVideoWritable;
}

/*!
    queryVoiceNumber
*/
int VmbxUiEngine::queryVoiceNumber(QString &aValue)
{
    _DBGLOG2("VmbxUiEngine::queryVoiceNumber >, value=",aValue)
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    int result = queryNumber(vmbxParams, aValue);
    _DBGLOG2("VmbxUiEngine::queryVoiceNumber <, value=", aValue)
    return result;
}

/*!
    queryVideoNumber
*/
int VmbxUiEngine::queryVideoNumber(QString &aValue)
{
    _DBGLOG2("VmbxUiEngine::queryVideoNumber >, value=", aValue)
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    vmbxParams.iServiceId = KVmbxServiceVideo;
    int result = queryNumber(vmbxParams, aValue);
    _DBGLOG2("VmbxUiEngine::queryVideoNumber <, value=", aValue)
    return result;
}

/*!
    Request notify when VMBX number changed
 */
void VmbxUiEngine::notifyVmbxNumberChange(bool aNotifyOnActiveLineOnly)
{
    _DBGLOG2("VmbxUiEngine::notifyVmbxNumberChange >, OnActiveLineOnly=", 
             aNotifyOnActiveLineOnly)
    QT_TRAP_THROWING(mVmbxEngine->NotifyVmbxNumberChangeL
                                 (*this, aNotifyOnActiveLineOnly));
    _DBGLOG2("VmbxUiEngine::notifyVmbxNumberChange <, OnActiveLineOnly=", 
             aNotifyOnActiveLineOnly)
}

/*!
    Callback from voice mailbox engine when number has been updated
    This method notifies the UI to refresh its data.
*/
void VmbxUiEngine::HandleNotifyL(const CVoiceMailboxEntry &aVmbxEntry)
{
    _DBGLOG( "VmbxUiEngine::HandleNotifyL >")
    TVmbxType type = aVmbxEntry.VoiceMailboxType();
    TPtrC number(KNullDesC);
    aVmbxEntry.GetVmbxNumber(number);
    if (EVmbxVoice == type) {
        mCsVoiceNumber = QString::fromUtf16(number.Ptr(), number.Length());
    } else if (EVmbxVideo == type) {
        mCsVideoNumber = QString::fromUtf16(number.Ptr(), number.Length());
    }
    emit voiceMailboxEngineEntriesUpdated(type);
    _DBGLOG( "VmbxUiEngine::HandleNotifyL <")
}

/*!
    Getter method for primary voice number of Voice Mailbox.
*/
void VmbxUiEngine::getCsVoicePrimaryNumber(QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getCsVoicePrimaryNumber >")
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    getNumber(vmbxParams, aValue);
    _DBGLOG2("VmbxUiEngine::getCsVoicePrimaryNumber < , string=", aValue)
}

/*!
    Getter method for primary video number of Voice Mailbox.
*/
void VmbxUiEngine::getCsVideoPrimaryNumber(QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getCsVideoPrimaryNumber >")
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVideo;
    getNumber(vmbxParams, aValue);
    _DBGLOG2("VmbxUiEngine::getCsVideoPrimaryNumber < , string=", aValue)
}

/*!
    Private getter method for voice mailbox number.
*/  
void VmbxUiEngine::getNumber(const TVoiceMailboxParams &aParam, 
                             QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getNumber >")
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TInt result = mVmbxEngine->GetStoredEntry( aParam, vmbxEntry );
    _DBGLOG2("VmbxUiEngine::getNumber, GetStoredEntry result=", result);
    if ( KErrNone == result && vmbxEntry ){ 
        TPtrC entryNumber( KNullDesC );
        result = vmbxEntry->GetVmbxNumber( entryNumber );
        _DBGLOG3("VmbxUiEngine::getNumber, GetVmbxNumber result=", 
            result,
            " but ignore code and allow returning of an empty string to UI");
        aValue = QString::fromUtf16(entryNumber.Ptr(), entryNumber.Length());
    }
    delete vmbxEntry;
    vmbxEntry = NULL;
    _DBGLOG2("VmbxUiEngine::getNumber < , string=", aValue)    
}

/*!
    Setter method for voice mailbox number.
*/
void VmbxUiEngine::setNumber(const TVoiceMailboxParams &aParam, 
                             const QString &aValue)
{
    _DBGLOG("VmbxUiEngine::setNumber >")
        
    CVoiceMailboxEntry* vmbxEntry = NULL;
    QT_TRAP_THROWING(vmbxEntry = CVoiceMailboxEntry::NewL());
    vmbxEntry->SetServiceId(aParam.iServiceId);
    vmbxEntry->SetVoiceMailboxType(aParam.iType);
    vmbxEntry->SetVmbxAlsLineType(aParam.iLineType);
    _DBGLOG2("VmbxUiEngine::setNumber, SetVmbxNumber aValue=", aValue);
    _DBGLOG( "VmbxUiEngine::setNumber: cast to TPtrC")
    TPtrC newNumber( 
        reinterpret_cast<const TUint16*>( aValue.utf16() ),
        aValue.length() );
    int result = vmbxEntry->SetVmbxNumber( newNumber );
    if ( KErrNone == result ) {
        result = mVmbxEngine->SaveEntry( *vmbxEntry );
        _DBGLOG2("VmbxUiEngine::setNumber, SaveEntry result=", result);
    } 
    delete vmbxEntry;
    vmbxEntry = 0;
    _DBGLOG("VmbxUiEngine::setNumber <");
}

/*!
    Query method for voice mailbox number.
*/
int VmbxUiEngine::queryNumber(const TVoiceMailboxParams &aParam, 
    QString &aValue)
{
    _DBGLOG2("VmbxUiEngine::queryNumber >, value=", aValue)
    getNumber(aParam, aValue);
    CVoiceMailboxEntry *vmbxEntry = NULL;
    int result(0);
    if (aValue.length() > 0) {
        result = mVmbxEngine->QueryChangeEntry(aParam, vmbxEntry);
    } else {
        result = mVmbxEngine->QueryNewEntry(aParam, vmbxEntry);
    }
    if (KErrNone == result && vmbxEntry) {
        TPtrC entryNumber(KNullDesC);
        result = vmbxEntry->GetVmbxNumber(entryNumber);
        _DBGLOG3("VmbxUiEngine::queryNumber, GetVmbxNumber result=", 
            result,
            " but ignore code and allow returing of an empty string to UI");
        aValue = QString::fromUtf16(entryNumber.Ptr(), entryNumber.Length());
    } 
    delete vmbxEntry;
    vmbxEntry = NULL;
    _DBGLOG2("VmbxUiEngine::queryNumber <, value=", aValue)
    return result;
}

//End of file
