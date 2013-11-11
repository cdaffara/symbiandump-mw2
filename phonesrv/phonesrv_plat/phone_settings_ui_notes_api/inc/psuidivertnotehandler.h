/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef PSUIDIVERTNOTETHANDLER_H_
#define PSUIDIVERTNOTEHANDLER_H_

#include <QObject>
#include <QList>

class PSetCallDivertingWrapper;
class PSCallDivertingCommand;
class PSCallDivertingStatus;

#ifdef BUILD_PSUINOTES
#define PSUINOTES_EXPORT Q_DECL_EXPORT
#else
#define PSUINOTES_EXPORT Q_DECL_IMPORT
#endif

class PSUINOTES_EXPORT PsUiDivertNoteHandler : public QObject
{
    Q_OBJECT
    
public:
    PsUiDivertNoteHandler(PSetCallDivertingWrapper& callDivertingWrapper);
    ~PsUiDivertNoteHandler();

public slots: // Slots: 

   /**
    * Set info request results
    * @param setting 
    * @param plural
    */
   void handleDivertingChanged(
        const PSCallDivertingCommand& setting, 
        bool plural);
   
   /**
    * Get info request results
    * @param divertList
    * @param plural
    */
   void handleDivertingStatus(
        QList<PSCallDivertingStatus*>& divertList, 
        bool plural);
   
   /**
    * Request failed.
    * @param reason Reason code for fail 
    */
   void handleDivertingError(int reason);
 
   /**
    * Handles requesting notes.
    *
    * @param aOngoing Is there a request going on.
    * @param aInterrupted Request needs to immediately cancelled.
    */
   void handleCFRequesting(bool ongoing, bool interrupted);
   
private:   // data
    
   PSetCallDivertingWrapper& m_callDivertingWrapper;
   
   int m_activeNoteId;
   int m_activeProgressNoteId;
   
};

#endif /* PSUIDIVERTNOTEHANDLER_H_ */
