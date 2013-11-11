/*
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
 * Description:
 *
 */

#include <psetcalldivertingwrapper.h>
#include "psuinotes.h"
#include "psuidivertnotehandler.h"
#include "psuilogging.h"

/*!
  PsUiDivertNoteHandler::PsUiDivertNoteHandler 
 */
PsUiDivertNoteHandler::PsUiDivertNoteHandler(PSetCallDivertingWrapper& callDivertingWrapper) :
    m_callDivertingWrapper(callDivertingWrapper)
{
    DPRINT << ": IN";
   
    QObject::connect(
        &m_callDivertingWrapper, 
        SIGNAL(handleDivertingChanged(const PSCallDivertingCommand&,bool)),
        this, 
        SLOT(handleDivertingChanged(const PSCallDivertingCommand&,bool)));
    QObject::connect(
        &m_callDivertingWrapper, 
        SIGNAL(handleDivertingStatus(QList<PSCallDivertingStatus*>&, bool)),
        this, 
        SLOT(handleDivertingStatus(QList<PSCallDivertingStatus*>&, bool)));
    QObject::connect(
        &m_callDivertingWrapper, 
        SIGNAL(handleDivertingError(int)),
        this, 
        SLOT(handleDivertingError(int))); 
    QObject::connect(
        &m_callDivertingWrapper, 
        SIGNAL(handleCFRequesting(bool,bool)),
        this,
        SLOT(handleCFRequesting(bool,bool)));
    DPRINT << ": OUT";
}

/*!
  PsUiDivertNoteHandler::~PsUiDivertNoteHandler
 */
PsUiDivertNoteHandler::~PsUiDivertNoteHandler()
{  
}

/*!
  PsUiDivertNoteHandler::handleDivertingChanged
 */
void PsUiDivertNoteHandler::handleDivertingChanged(
    const PSCallDivertingCommand& aSetting, 
    bool aPlural)
{  
    DPRINT << ": IN";
    switch ( aSetting.iStatus ) {
        case DivertingStatusActive:
            if (aPlural) {   
                PsUiNotes::instance()->showGlobalNotificationDialog(
                    hbTrId("txt_phone_info_diverts_activated"));
            } else {       
                PsUiNotes::instance()->showGlobalNotificationDialog(
                    hbTrId("txt_phone_info_divert_activated"));
            }
            break;
        case DivertingStatusNotRegistered:  
        case DivertingStatusInactive:
            if (aPlural) {
                PsUiNotes::instance()->showGlobalNotificationDialog(
                    hbTrId("txt_phone_info_diverts_deactivated"));
            } else {
                PsUiNotes::instance()->showGlobalNotificationDialog(
                    hbTrId("txt_phone_info_divert_deactivated"));
            }
            break;
        case DivertingStatusNotProvisioned:
        case DivertingStatusUnknown:
        default:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_info_request_not_completed"));
    }
    DPRINT << ": OUT";
}
   
/*!
  PsUiDivertNoteHandler::handleDivertingStatus
 */
void PsUiDivertNoteHandler::handleDivertingStatus(
    QList<PSCallDivertingStatus*>& divertList, 
    bool plural)
{
    DPRINT << ": IN";   
    Q_UNUSED(plural);

    PsUiNotes::instance()->showCallDivertDetails(divertList);
    DPRINT << ": OUT";
}
   
/*!
  PsUiDivertNoteHandler::handleDivertingError
 */
void PsUiDivertNoteHandler::handleDivertingError(int aReason)
{
    DPRINT << ": IN";
    // Cancel previous note
    PsUiNotes::instance()->cancelNote(m_activeNoteId);
    // Show error note
    PsUiNotes::instance()->showGlobalErrorNote(m_activeNoteId, aReason);    
    DPRINT << ": OUT";
}

/*!
  PsUiDivertNoteHandler::handleCFRequesting
 */
void PsUiDivertNoteHandler::handleCFRequesting(bool ongoing, bool interrupted)
{
    DPRINT << ": IN";
    if (interrupted) {
       PsUiNotes::instance()->cancelNote(m_activeProgressNoteId);
       PsUiNotes::instance()->cancelNote(m_activeNoteId);
    } else {
       if (ongoing) {
           PsUiNotes::instance()->showGlobalProgressNote(
               m_activeProgressNoteId, hbTrId("txt_common_info_requesting"));
       } else {
           PsUiNotes::instance()->cancelNote(m_activeProgressNoteId);
       }
    }       
    DPRINT << ": OUT";
}
