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

#include <xqsysinfo.h>
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <BTSapDomainPSKeys.h>
#include <settingsinternalcrkeys.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <NumberGroupingCRKeys.h>

#include <etelmm.h>
#include <exterror.h>               
#include <gsmerror.h>

#include <hbglobal.h>
#include "psuiutils.h"
#include "psuilogging.h"

/*!
  PsUiTools::errorCodeTextMapping.
 */
bool PsUiUtils::errorCodeTextMapping(const int errorcode, QString &errorText)
{
    DPRINT;
    errorText.clear();
    bool errorTextFound(true);

    switch(errorcode){
        case KErrGsmSSSubscriptionViolation:
            errorText = hbTrId("txt_phone_info_barring_operation_not_successful");
            break;
        case KErrGsmSSUnknownSubscriber:
        case KErrGsmSSAbsentSubscriber:
        case KErrGsmSSIllegalOperation:
        case KErrGsmSSIllegalSubscriber:
        case KErrGsmSSIllegalEquipment:
        case KErrGsmSSCallBarred:
        case KErrGsmSSDataMissing:
        case KErrAccessDenied:
            errorText = hbTrId("txt_phone_info_not_allowed");
            break;
        case KErrGsmSSIncompatibility:
            errorText = hbTrId("txt_phone_info_conflict_error");
            break;
        case KErrGsmSSSystemFailure:
            errorText = hbTrId("txt_phone_info_result_unknown");
            break;
        case KErrGsmSSUnexpectedDataValue:
        case KErrGsmSSResourcesUnavailable:
            errorText = hbTrId("txt_phone_info_request_rejected");
            break;
        case KErrGsmSSNegativePasswordCheck:
        case KErrGsmSSPasswordRegistrationFailure:
            errorText = hbTrId("txt_phone_info_password_error");
            break;
        case KErrGsmSSPasswordAttemptsViolation:
            errorText = hbTrId("txt_phone_info_barring_password_blocked");
            break;
        case KErrGsmSMSNoNetworkService:
        case KErrGsmNoService:
            errorText = hbTrId("txt_phone_info_no_network_coverage");
            break;
        case KErrSsActivationDataLost:
            errorText = hbTrId("txt_phone_info_check_network_services");
            break;
        case KErrGsmOfflineOpNotAllowed:
            if(XQSysInfo::isSupported(KFeatureIdOfflineMode)){
               errorText = hbTrId("txt_phone_info_offline_not_allowed");
            } else {
                errorText = hbTrId("txt_phone_info_request_not_completed");
            }
            break;
        case KErrGsmSSUnknownAlphabet:
            errorText = hbTrId("txt_phone_info_invalid_phone_number");
            break;
        default:
            errorText = hbTrId("txt_phone_info_request_not_completed");
            break;
    }

    return errorTextFound;
}

PsUiSettingsWrapper::PsUiSettingsWrapper(QObject *parent): 
    QObject(parent)
{
    m_Settings = new XQSettingsManager();
}

PsUiSettingsWrapper::~PsUiSettingsWrapper()
{
    delete m_Settings;
}

bool PsUiSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled()
{
    bool enabled = readCenrepValue(KCRUidPhoneSettings.iUid, KPSetCallWaiting).toBool();
    DPRINT << "enabled: " << enabled;
    return enabled;
}

bool PsUiSettingsWrapper::isPhoneOffline() const
{
    bool offLinesupport(false);
    if (XQSysInfo::isSupported(KFeatureIdOfflineMode)) {
        offLinesupport = !readCenrepValue(
                KCRUidCoreApplicationUIs.iUid, 
                KCoreAppUIsNetworkConnectionAllowed).toBool();
    }
    return offLinesupport;
}

QVariant PsUiSettingsWrapper::readCenrepValue(
    const long int uid, const unsigned long int key) const
{
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, uid, key);
    QVariant ret = m_Settings->readItemValue(settingsKey);
    DPRINT << "ret: " << ret;
    return ret;
}

bool PsUiSettingsWrapper::numberGroupingSupported() const
 {
     return readCenrepValue(KCRUidNumberGrouping.iUid, KNumberGrouping).toBool();
 }

