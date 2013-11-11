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
* Description: Implements Life Timer key sequence handling.
*
*/

#include <QDebug>
#include <hbmessagebox.h>
#include <hbdevicemessagebox.h>
#include <hbextendedlocale.h>
#include <etelmm.h>
#include <rmmcustomapi.h>
#include <mmtsy_names.h>
#include <xqconversions.h>
#include "lifetimerkeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "telservicesinternalcrkeys.h"
#include "telservicesvariant.hrh"
#include "keysequencerecognitionservicelog.h"

/*!
  LifeTimerKeySequenceHandler::LifeTimerKeySequenceHandler.
 */
LifeTimerKeySequenceHandler::LifeTimerKeySequenceHandler(
    QObject* parent)
    : 
    KeySequenceHandler(parent),
    m_messageBox(0),
    m_repository(0)
{
    DPRINT_METHODENTRYEXIT;
    
    CRepository *repository = 0;
    QT_TRAP_THROWING(repository = CRepository::NewL(KCRUidTelSrvVariation))
    m_repository.reset(repository);
    
    if (lifeTimerFeatureEnabled()) {
        setKeySequenceValidator(QRegExp::escape(KCodeLifeTimer));
    }
}


/*!
  LifeTimerKeySequenceHandler::~LifeTimerKeySequenceHandler.
 */
LifeTimerKeySequenceHandler::~LifeTimerKeySequenceHandler()
{
    DPRINT_METHODENTRYEXIT;
    
    destroyMessageBox();
}


/*!
  LifeTimerKeySequenceHandler::executeKeySequence.
 */
bool LifeTimerKeySequenceHandler::executeKeySequence(
    const QString &keySequence)
{
    DPRINT_METHODENTRYEXIT;
    
    bool handled = true;
    
    if (KCodeLifeTimer == keySequence && lifeTimerFeatureEnabled()) {
        launchLifeTimerDialog();
    } else {
        handled = false;
    }
    
    return handled;
}


/*!
  LifeTimerKeySequenceHandler::launchLifeTimerDialog().
 */
void LifeTimerKeySequenceHandler::launchLifeTimerDialog()
{
    DPRINT_METHODENTRYEXIT;
    
    destroyMessageBox();
    
    QString lifeTimerData = constructLifeTimerData();
    QString lifeTimerNoteText = 
        hbTrId("txt_phone_info_life_timer").arg(lifeTimerData);
    m_messageBox = new HbDeviceMessageBox(
        lifeTimerNoteText, 
        HbMessageBox::MessageTypeInformation);
    m_messageBox->setTimeout(HbPopup::NoTimeout);
    
    QObject::connect(
        m_messageBox, SIGNAL(aboutToClose()), 
        this, SLOT(destroyMessageBox()));
    
    m_messageBox->show();
}


/*!
  LifeTimerKeySequenceHandler::constructLifeTimerData.
 */
QString LifeTimerKeySequenceHandler::constructLifeTimerData()
{
    DPRINT_METHODENTRYEXIT;
    
    QString lifeTimerText;
    
    RTelServer telephonyServer;
    RMobilePhone mobilePhone;
    RMmCustomAPI mmCustomApi;
    QT_TRAP_THROWING(
        openEtelConnectionL(telephonyServer, mobilePhone, mmCustomApi);
    )
    
    RMmCustomAPI::TLifeTimeData lifeTimeData;
    RMmCustomAPI::TLifeTimeDataPckg dataPckg(lifeTimeData);
    int lifeTimeDataQueryResult = mmCustomApi.GetLifeTime(dataPckg);
    closeEtelConnection(telephonyServer, mobilePhone, mmCustomApi);
    
    if (KErrNone == lifeTimeDataQueryResult) {
        lifeTimeData = dataPckg();
        
        HbExtendedLocale locale = HbExtendedLocale::system();
        lifeTimerText.append(QString::number(lifeTimeData.iHours));
        const int KMinuteSeparatorInd = 2;
        lifeTimerText.append(locale.timeSeparator(KMinuteSeparatorInd));
        lifeTimerText.append(
            QString::number(static_cast<int>(lifeTimeData.iMinutes)));
    }
    
    return lifeTimerText;
}


/*!
  LifeTimerKeySequenceHandler::openEtelConnectionL.
 */
void LifeTimerKeySequenceHandler::openEtelConnectionL(
    RTelServer &telephonyServer,
    RMobilePhone &mobilePhone,
    RMmCustomAPI &mmCustomApi)
{
    DPRINT_METHODENTRYEXIT;
    
    User::LeaveIfError(telephonyServer.Connect());
    User::LeaveIfError(mobilePhone.Open(telephonyServer, KMmTsyPhoneName()));
    User::LeaveIfError(mmCustomApi.Open(mobilePhone));
}


/*!
  LifeTimerKeySequenceHandler::closeEtelConnection.
 */
void LifeTimerKeySequenceHandler::closeEtelConnection(
    RTelServer &telephonyServer,
    RMobilePhone &mobilePhone,
    RMmCustomAPI &mmCustomApi)
{
    DPRINT_METHODENTRYEXIT;
    
    mmCustomApi.Close();
    mobilePhone.Close();
    telephonyServer.Close();
}


/*!
  LifeTimerKeySequenceHandler::lifeTimerFeatureEnabled.
 */
bool LifeTimerKeySequenceHandler::lifeTimerFeatureEnabled()
{
    DPRINT_METHODENTRYEXIT;
    
    TInt telephoneVariant = 0;
    TInt result = m_repository->Get(KTelSrvVariationFlags, telephoneVariant);
    return (KErrNone == result) 
        ? static_cast<bool>(telephoneVariant & KTelSrvLVFlagLifeTimer)
        : false;
}


/*!
  LifeTimerKeySequenceHandler::destroyMessageBox.
 */
void LifeTimerKeySequenceHandler::destroyMessageBox()
{
    DPRINT_METHODENTRYEXIT;
    
    if (m_messageBox) {
        m_messageBox->deleteLater();
        m_messageBox = 0;
    }
}
