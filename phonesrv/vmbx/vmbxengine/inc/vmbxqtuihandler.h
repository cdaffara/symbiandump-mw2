/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Hanlder Ui which implement of qt.
*  Interface   : Private, VmbxQtUiHandler
*
*/


#ifndef VMBXQTUIHANDLER_H
#define VMBXQTUIHANDLER_H

// INCLUDES
#include <QObject>
#include <QTranslator>
#include <voicemailboxdefs.h>

#include "voicemailboxdefsinternal.h"

// FORWARD DECLARATION
class CVoiceMailboxEntry;
class TVoiceMailboxParams;
class HbInputDialog;

// CLASS DECLARATION

/**
*  Dialog manager
*
*  @lib vmbxengine.lib
*/
class VmbxQtUiHandler : public QObject
    {
    Q_OBJECT
    
public: // Constructors and destructor

    /**
    * C++ default constructor.
    *
    */
    VmbxQtUiHandler(QObject* parent = 0 );

    /**
    * Destructor.
    */
    virtual ~VmbxQtUiHandler();

     /**
     * Initialize, load qt translator.
     */
     void  init();

    /**
    * Show mailbox query dialog
    *
    * @param aType in TVmbxType type
    * @param aNumber the voice mailbox number
    * @param aResult out the result of dialog
    */
    void showVmbxQueryDialog(const TVmbxType& aType,
                             QString& aNumber, int& aResult);

    /**
     * Show define number in selection dialog
     * Leaves if user cancel selected.
     *
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    void showDefineSelectionDialog(TVmbxType& aType, int& aResult);

    /**
     * Show call number in selection dialog
     *
     * @param in entryList array of the defined voice mailbox entry
     * @param out params the type TVoiceMailboxParams which should include
     *          the service id and the type of seclected TVmbxType
     * @param out result the result user seclected
     */
    void showCallSelectionDialog(
                const QList<CVoiceMailboxEntry *> entryList,
                TVoiceMailboxParams &params, int &result );

    /**
    * Show voice mailbox information number
    *
    * @param aNoteType in TVmbxType type
    */
    void showInformationNote(const TVmbxNoteType aType);

    /**
    * Check qt environment supported or not.
    *
    * @return qt environment supported or not.
    */
    bool isQtEnvSupported();
    
private slots:
    /**
    * update Ok button status according to user input string
    *
    * @param aInput User input string
    */
    void updatePrimaryAction(const QString &aInput);

private:

    /**
    * Translator vmbx localization
    */
    QTranslator iTranslator;

    /**
    * Translator common localization
    */
    QTranslator iCommonTranslator;
    
    /**
    * Query and Define mailbox dialog. Own
    */    
    HbInputDialog *iQueryDialog;
    };

#endif  // VMBXQTUIHANDLER_H
