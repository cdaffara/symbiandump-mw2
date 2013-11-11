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
* Description:  Implementation for positioning indicator plugin class
*
*/

#include "posindicator.h"
#include "posindicatorinfo.h"
#include "posgeoaddress.h"
#include "apilogger.h"

#include <QtPlugin>
#include <QVariant>
#include <QDebug>
#include <QTranslator>
#include <QTCore>


Q_EXPORT_PLUGIN(PosIndicatorPlugin)

// This plugin implements just one indicator type
const QString typeOfIndicator ="com.nokia.positioning.indicatorplugin/1.0";

// ---------------------------------------------------------------------------
// PosIndicatorPlugin::PosIndicatorPlugin
// ---------------------------------------------------------------------------
//
PosIndicatorPlugin::PosIndicatorPlugin() : mError(0), mTranslator(0)
{
    #ifndef QT_NO_DEBUG_OUTPUT
    ApiLogger::OpenLogFile();
        #ifndef POSINDICATOR_NFT
        qInstallMsgHandler(ApiLogger::MyOutputHandler);
        #endif
    #endif
        
    qDebug() << "+ PosIndicatorPlugin::PosIndicatorPlugin()";
    
    mTranslator = new QTranslator();
    
    QString locale = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    mTranslator->load(path + QString("lilocationmw_") + locale);
    qApp->installTranslator(mTranslator);
        
    mIndicatorTypes.append(typeOfIndicator);
    qDebug() << "- PosIndicatorPlugin::PosIndicatorPlugin()";

}

// ---------------------------------------------------------------------------
// PosIndicatorPlugin::~PosIndicatorPlugin
// ---------------------------------------------------------------------------
//
PosIndicatorPlugin::~PosIndicatorPlugin()
{   
    qDebug() << "+ PosIndicatorPlugin::~PosIndicatorPlugin()";
    if (mTranslator)
    {
    if (mTranslator->isEmpty() == false)
        qApp->removeTranslator(mTranslator);
    delete mTranslator;
    }
	qInstallMsgHandler(0);
    ApiLogger::CloseLogFile();
}

// ---------------------------------------------------------------------------
// PosIndicatorPlugin::indicatorTypes
// ---------------------------------------------------------------------------
//
QStringList PosIndicatorPlugin::indicatorTypes() const
{
	qDebug() << "+ PosIndicatorPlugin::indicatorTypes()";
	// Return indicator types this plugin implements
    return mIndicatorTypes;
}

// ---------------------------------------------------------------------------
// PosIndicatorPlugin::createIndicator
// ---------------------------------------------------------------------------
//
HbIndicatorInterface* PosIndicatorPlugin::createIndicator(
        const QString &indicatorType)
{
	qDebug() << "+ PosIndicatorPlugin::createIndicator()";
	ApiLogger::NftLogger("Before launching indicator");
    PosIndicator* ind = new PosIndicator( indicatorType );
    ApiLogger::NftLogger("After launching indicator");
    return ind;
}

// ---------------------------------------------------------------------------
// PosIndicatorPlugin::accessAllowed
// ---------------------------------------------------------------------------
//
bool PosIndicatorPlugin::accessAllowed(const QString &/*indicatorType*/, const QVariantMap &securityInfo) const
{
	qDebug() << "+ PosIndicatorPlugin::accessAllowed()";
	QVariant value = securityInfo.value("sym-caps");
	if ((value.toInt() && ECapabilityLocation) == true )
	    {
        qDebug() << "Location capability present, pass";
        return true;
	    }
	else
	    {
        qDebug() << "Location capability absent, fail";
        return false;
	    }
}

// ---------------------------------------------------------------------------
// PosIndicatorPlugin::error
// ---------------------------------------------------------------------------
//
int PosIndicatorPlugin::error() const
{
	qDebug() << "+ PosIndicatorPlugin::error()";
    return mError;
}

// ---------------------------------------------------------------------------
// PosIndicator::PosIndicator
// ---------------------------------------------------------------------------
//
PosIndicator::PosIndicator(const QString &indicatorType) :
    HbIndicatorInterface(indicatorType, SettingCategory, NoInteraction),
    mDisplayString(QString(hbTrId("txt_loe_dblist_positioning_val_acquiring_position"))),
    mPosIndicatorInfo(0),
    processedAddess(false)
{
	qDebug() << "+ PosIndicator::PosIndicator()";
	qDebug() << "- PosIndicator::PosIndicator()";
}

// ---------------------------------------------------------------------------
// PosIndicator::RefreshPosIndicatorPane
// ---------------------------------------------------------------------------
//
void PosIndicator::refreshPosIndicatorPane(PosGeoAddress& posGeoAddress, int error)
    {
	qDebug() << "+ PosIndicator::refreshPosIndicatorPane()";
    if(error!=KErrNone)
        {
        qDebug() << "- PosIndicator::refreshPosIndicatorPane(),error code : "<<error;
        return;
        }
    qDebug() << "latitude : "<< posGeoAddress.latitude() << ", longitude : "<<posGeoAddress.longitude();
    TBool addressResolved = false;
    qDebug() << " No Errors.";
    mDisplayString.clear();
    if(!posGeoAddress.number().isEmpty())
        {
        qDebug() << "Number - %s"<<posGeoAddress.number();

        addressResolved = true;
        mDisplayString = mDisplayString + QString(posGeoAddress.number()) + QString(' '); 
        }
    if(!posGeoAddress.street().isEmpty())
        {
        qDebug() << "street - %d"<<posGeoAddress.street();

        addressResolved = true;
        mDisplayString = mDisplayString + QString(posGeoAddress.street()) + QString(' ');
        }
    if(!posGeoAddress.district().isEmpty())
        {
        qDebug() << "district - %s"<<posGeoAddress.district();

        addressResolved = true;
        mDisplayString = mDisplayString + QString(posGeoAddress.district()) + QString(' ');
        }
    if(!posGeoAddress.city().isEmpty())
        {
        qDebug() << "city - %s"<<posGeoAddress.city();

        addressResolved = true;
        mDisplayString = mDisplayString + QString(posGeoAddress.city()) + QString(' ');
        }
    if(!posGeoAddress.state().isEmpty())
        {
        qDebug() << "state - %s"<<posGeoAddress.state();

        addressResolved = true;
        mDisplayString = mDisplayString + QString(posGeoAddress.state()) + QString(' ');
        }
    if(!posGeoAddress.country().isEmpty())
        {
        qDebug() << "country - %s"<<posGeoAddress.country();

        addressResolved = true;
        mDisplayString = mDisplayString + QString(posGeoAddress.country());
        ApiLogger::NftLogger("address information available");
        }
    
    if(addressResolved == false)
            {
                qDebug() << "Address Not available, displaying lat,long";
                QString latitude, longitude;
                latitude = QString("latitude");
                ConvertToDMSFormat(latitude, posGeoAddress.latitude());
                longitude = QString("longitude");
                ConvertToDMSFormat(longitude, posGeoAddress.longitude());

                QString comma(", ");
                mDisplayString.clear();
                mDisplayString.append(latitude);
                mDisplayString.append(comma);
                mDisplayString.append(longitude);
                ApiLogger::NftLogger("Position information available");
            }
    
    if( processedAddess == true)
        {
        processedAddess = false;
        }
    else
        {
        processedAddess = true;
        }
    
    emit dataChanged();
	qDebug() << "- PosIndicator::refreshPosIndicatorPane()";
    }

// ---------------------------------------------------------------------------
// PosIndicator::~PosIndicator
// ---------------------------------------------------------------------------
//
PosIndicator::~PosIndicator()
{
    qDebug() << "+ PosIndicator::~PosIndicator()";
    if(mPosIndicatorInfo)
    		{
			mPosIndicatorInfo->cancelPosInfo();
		    delete mPosIndicatorInfo;
		    mPosIndicatorInfo = 0;
				}    			
    qDebug() << "- PosIndicator::~PosIndicator()";
}

// ---------------------------------------------------------------------------
// PosIndicator::indicatorData
// ---------------------------------------------------------------------------
//
QVariant PosIndicator::indicatorData(int role) const
{
   qDebug() << "+ PosIndicator::indicatorData(), role = "<< role;
//    Q_UNUSED(role)
        
    if (role == PrimaryTextRole) {
        return QString(hbTrId("txt_loe_dblist_positioning"));
    } else if (role == SecondaryTextRole) {
        return mDisplayString;   
    } else if (role == DecorationNameRole) {
        return QString("qtg_mono_gps");
    } else if (role == MonoDecorationNameRole  ) {
        return QString("qtg_mono_gps");
    }
     else
        return QVariant();
}

// ---------------------------------------------------------------------------
// PosIndicator::refreshData
// ---------------------------------------------------------------------------
//
bool PosIndicator::refreshData()
    {
    qDebug() << "+ PosIndicator::refreshData()";
    if(!mPosIndicatorInfo)
    {
    qDebug() << "First Call";
    	mPosIndicatorInfo = new PosIndicatorInfo();
			if(!mPosIndicatorInfo)
	    		{
                qDebug() << "Error in constructing PosIndicatorInfo class";
                throw;
	    		}
	    		
     	QObject::connect(mPosIndicatorInfo, SIGNAL(posInfoUpdated(PosGeoAddress&, int)), this, SLOT(refreshPosIndicatorPane(PosGeoAddress&, int))); 
	  }
		ApiLogger::NftLogger("Before requesting position information");
	  qDebug() << "Call requestPosInfo()";
    mPosIndicatorInfo->requestPosInfo();
    return true;
    }

// ---------------------------------------------------------------------------
// PosIndicator::ConvertToDMSFormat
// ---------------------------------------------------------------------------
//
void PosIndicator::ConvertToDMSFormat(QString &displayString, double decimal)
{
    qDebug() << "+ PosIndicator::ConvertToDMSFormat()";

    int tempNum;
    QString temp;
    float tempDec;
    
    int degrees = (int)decimal;
    
    tempDec = decimal - degrees;
    tempDec = tempDec * 60;
    int minutes = (int)tempDec;

    tempDec = tempDec - minutes;
    tempDec = tempDec * 60;
    int seconds = (int)tempDec;
    
    tempDec = tempDec - seconds;
    tempNum = tempDec * 100;
 
    QVariant varDegrees(degrees);
    QVariant varMinutes(minutes);
    QVariant varSeconds(seconds);
    QVariant varTemp(tempNum);
       
    if (QString::compare(displayString, "latitude") == KErrNone)
    {
    displayString.clear();

    if (decimal>=0)
        {
        displayString = QString(hbTrId("txt_loe_list_l1l2l3l4n"))
                                .arg(varDegrees.toString())
                                .arg(varMinutes.toString())
                                .arg(varSeconds.toString())
                                .arg(varTemp.toString());
        }
    else
        {
        displayString = QString(hbTrId("txt_loe_list_l1l2l3l4s"))
                                .arg(varDegrees.toString())
                                .arg(varMinutes.toString())
                                .arg(varSeconds.toString())
                                .arg(varTemp.toString());
        }
    }
    else
        {
        displayString.clear();
        {
        if (decimal>=0)
            {
            displayString = QString(hbTrId("txt_loe_list_l1l2l3l4e"))
                                .arg(varDegrees.toString())
                                .arg(varMinutes.toString())
                                .arg(varSeconds.toString())
                                .arg(varTemp.toString());
            }
        else
            {
            displayString = QString(hbTrId("txt_loe_list_l1l2l3l4w"))
                                .arg(varDegrees.toString())
                                .arg(varMinutes.toString())
                                .arg(varSeconds.toString())
                                .arg(varTemp.toString());
            }
        }
        }
    qDebug() << "- PosIndicator::ConvertToDMSFormat()";
}
