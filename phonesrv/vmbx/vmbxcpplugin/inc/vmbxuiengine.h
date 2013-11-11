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
#ifndef VMBXUIENGINE_H
#define VMBXUIENGINE_H

// System includes
#include <QObject>
#include <mvoicemailboxobserver.h>
#include <voicemailboxdefs.h>

// Forward declarations
class CVoiceMailbox;
class TVoiceMailboxParams;

// Class declaration
class VmbxUiEngine : public QObject, public MVoiceMailboxObserver
{
    Q_OBJECT
            
public:
    
    /*!
        Constructor.
    */
    VmbxUiEngine();

    /*!
        Destructor
    */
    ~VmbxUiEngine();

public:

    /*!
        Getter method for primary voice number of Voice Mailbox.
        @param aValue Retrieved number.
    */
    void getCsVoiceNumber(QString &aValue);
    
    /*!
        Getter method for primary video number of Voice Mailbox.
        @param aValue Retrieved number.
    */
    void getCsVideoNumber(QString &aValue);
    
    /*!
        Set number when cs voice number has been edited on UI.
        @param aValue New value.
    */
    void setCsVoiceNumber( const QString &aValue );
    
    /*!
        Set number when cs voice number has been edited on UI.
        @param aValue New value.
    */
    void setCsVideoNumber( const QString &aValue );
    
    /*!
        If video mailbox supported.
    */    
    bool isVideoSupport();

    /*!
        If voice mailbox is writable.
    */
    bool isVoiceWritable();

    /*!
        If video mailbox is writable.
    */
    bool isVideoWritable();

      
    // Add new Getter methods for other voice mailbox types

    /*!
        From MVoiceMailboxObserver
        Observer callback for number/address change notification
        from voice mailbox engine.
        The UI should be notified to update the number.
        @param aVmbxEntry New voicemailbox entry.
    */
    void HandleNotifyL(const CVoiceMailboxEntry &aVmbxEntry);

    /*!
        Query number when cs voice number has been edited on UI.
        @param aValue New value.
    */
    int queryVoiceNumber(QString &aValue);

    /*!
        Query number when cs video number has been edited on UI.
        @param aValue New value.
    */
    int queryVideoNumber(QString &aValue);
    
    /*!
        Request notify when VMBX number changed
        @param  aNotifyOnActiveLineOnly  Only active line or NOT
     */
    void notifyVmbxNumberChange(bool aNotifyOnActiveLineOnly);

signals:

    /*!
        Signal emitted when voice mailbox data has been updated.
    */
    void voiceMailboxEngineEntriesUpdated(const TVmbxType vmbxType); 


private: // New Method

    /*!
        Initialize vmbx ui engine.
    */
    void init();
 
    /*!
        Getter method for primary voice number of Voice Mailbox.
        @param aValue Retrieved number.
    */
    void getCsVoicePrimaryNumber(QString &aValue);
    
    /*!
        Getter method for primary video number of Voice Mailbox.
        @param aValue Retrieved number.
    */
    void getCsVideoPrimaryNumber(QString &aValue);
 
    /*!
        Getter method for voice mailbox number.
        @param aParam Identify voice mailbox type
        @param aValue Retrieved number.
    */    
    void getNumber(const TVoiceMailboxParams &aParam, 
                   QString &aValue);
    
    /*!
        Setter method for voice mailbox number.
        @param aParam Identify voice mailbox type
        @param aValue Retrieved number.
    */
    void setNumber(const TVoiceMailboxParams &aParam, 
                   const QString &aValue);

    /*!
        Query method for voice mailbox number.
        @param aParam Identify voice mailbox type
        @param aValue Retrieved number.
    */
    int queryNumber(const TVoiceMailboxParams &aParam, QString &aValue);
    

private:

    /*!
        Symbian voice mailbox engine which offers basic entry retrieval,
        saving, querying etc. services.
        Own.
    */
    CVoiceMailbox *mVmbxEngine;
    /*!
        Cs voice number
     */
    QString mCsVoiceNumber;

    /*!
        Cs video number
     */
    QString mCsVideoNumber;
    
};

#endif // VMBXUIENGINE_H
