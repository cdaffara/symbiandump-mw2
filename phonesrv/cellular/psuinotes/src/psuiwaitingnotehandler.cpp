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

#include <hbdevicenotificationdialog.h>
#include <psetcallwaitingwrapper.h>
#include "psuinotes.h"
#include "psuiwaitingnotehandler.h"
#include "psuiutils.h"
#include "psuilogging.h"

/*!
  PsUiWaitingNoteHandler::PsUiWaitingNoteHandler
 */
PsUiWaitingNoteHandler::PsUiWaitingNoteHandler(PSetCallWaitingWrapper& callWaitingWrapper) :
    m_callWaitingWrapper(callWaitingWrapper)
{
    DPRINT << ": IN";

    m_settingsWrapper = new PsUiSettingsWrapper();

    QObject::connect(
        &m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingGetStatus(
           PSetCallWaitingWrapper::PsCallWaitingStatus, const QList<unsigned char> &)),
        this, 
        SLOT(handleCallWaitingGetStatus(
           PSetCallWaitingWrapper::PsCallWaitingStatus, const QList<unsigned char> &)));
    QObject::connect(
        &m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingChanged(PSetCallWaitingWrapper::PsCallWaitingCommand, int)),
        this, 
        SLOT(handleCallWaitingChanged(PSetCallWaitingWrapper::PsCallWaitingCommand, int)));
    QObject::connect(
        &m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingRequesting( bool, bool )),
        this, 
        SLOT(handleCallWaitingRequesting( bool, bool )));
    QObject::connect(
        &m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingError( int )),
        this, 
        SLOT(handleCallWaitingError( int )));

    DPRINT << ": OUT";
}

/*!
  PsUiWaitingNoteHandler::~PsUiWaitingNoteHandler
 */
PsUiWaitingNoteHandler::~PsUiWaitingNoteHandler()
{
    DPRINT << ": IN";
    delete m_settingsWrapper;
    DPRINT << ": OUT";
}

/*!
  PsUiWaitingNoteHandler::handleCallWaitingGetStatus
 */
void PsUiWaitingNoteHandler::handleCallWaitingGetStatus( 
    PSetCallWaitingWrapper::PsCallWaitingStatus status,
    const QList<unsigned char> &basicServiceGroupIds)
{
    DPRINT << ": IN";
    DPRINT << ": IN status: " << status;
    Q_UNUSED(basicServiceGroupIds);

    PsUiNotes::instance()->cancelNote(m_activeNoteId);
    
    bool alsCaseOnly = false;
    if (1 == basicServiceGroupIds.count() &&
        PsUiUtils::AltTele == static_cast<PsUiUtils::BasicServiceGroups>(basicServiceGroupIds.at(0))) {
        DPRINT << "Status is only for als";
        alsCaseOnly = true;
    }

    if (PSetCallWaitingWrapper::StatusNotProvisioned == status && 
        m_settingsWrapper->isFeatureCallWaitingDistiquishNotProvisionedEnabled()) {
        DPRINT << ": not provisioned";
        PsUiNotes::instance()->
            showGlobalNotificationDialog(hbTrId("txt_phone_info_request_not_completed"));
    }  
    else if (PSetCallWaitingWrapper::StatusActive == status && !alsCaseOnly ) {
        DPRINT << ": status active";
        PsUiNotes::instance()->showGlobalNotificationDialog(
            hbTrId("txt_phone_dpopinfo_call_waiting_active")); 
    }
    else {
        DPRINT << ": status not active";
        PsUiNotes::instance()->showGlobalNotificationDialog(
            hbTrId("txt_phone_dpopinfo_call_waiting_not_active"));  
    }

    DPRINT << ": OUT";   
}

/*!
  PsUiWaitingNoteHandler::handleCallWaitingChanged
 */
void PsUiWaitingNoteHandler::handleCallWaitingChanged( 
    PSetCallWaitingWrapper::PsCallWaitingCommand setting,
    int result )
{
    DPRINT << ": IN";
    PsUiNotes::instance()->cancelNote(m_activeNoteId);
    if (result) {
        PsUiNotes::instance()->showGlobalNote(m_activeNoteId,
        hbTrId("txt_phone_info_request_not_confirmed"), 
            HbMessageBox::MessageTypeWarning);
    } else {
        switch (setting){
            case PSetCallWaitingWrapper::ActivateCallWaiting:
                 PsUiNotes::instance()->showGlobalNotificationDialog(
                 hbTrId("txt_phone_info_call_waiting_activated"));
             break;
            case PSetCallWaitingWrapper::DeactivateCallWaiting:
                 PsUiNotes::instance()->showGlobalNotificationDialog(
                 hbTrId("txt_phone_info_call_waiting_deactivated")); 
             break;
            default: 
                 PsUiNotes::instance()->showGlobalNotificationDialog(
                 hbTrId("txt_phone_info_result_unknown"));
             break;
        }
      }

    DPRINT << ": OUT";
}

/*!
  PsUiWaitingNoteHandler::handleCallWaitingRequesting
 */
void PsUiWaitingNoteHandler::handleCallWaitingRequesting( bool ongoing, bool interrupted )
{
    DPRINT << ": IN";
    DPRINT << "ongoing:" << ongoing << ", interrupted:" << interrupted;

    if (ongoing) {
        PsUiNotes::instance()->showGlobalProgressNote(m_activeNoteId, 
            hbTrId("txt_common_info_requesting"));
    }

    if (interrupted) {
        PsUiNotes::instance()->cancelNote(m_activeNoteId);
    }

    DPRINT << ": OUT";
}

/*!
  PsUiWaitingNoteHandler::handleCallWaitingError
 */
void PsUiWaitingNoteHandler::handleCallWaitingError( int error )
{
    DPRINT << ": IN";
    DPRINT << "errorCode:" << error;
    PsUiNotes::instance()->cancelNote(m_activeNoteId);
    PsUiNotes::instance()->showGlobalErrorNote(m_activeNoteId, error);

    DPRINT << ": OUT";
}
