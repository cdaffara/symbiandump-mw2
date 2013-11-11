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
#include <smcmockclassincludes.h>
#include "hbnotificationdialog.h"
#include <qgraphicsscene.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbNotificationDialog::Q_PROPERTY
// -----------------------------------------------------------------------------
//
/*Q_OBJECT HbNotificationDialog::Q_PROPERTY( 
        QString text READ text WRITE setText ) Q_PROPERTY ( QString title READ title WRITE setTitle ) Q_PROPERTY ( HbIcon icon READ icon WRITE setIcon ) Q_PROPERTY ( Hb::TextWrapping titleTextWrapping READ titleTextWrapping WRITE setTitleTextWrapping ) Q_PROPERTY ( bool sequentialShow READ isSequentialShow WRITE setSequentialShow ):HbNotificationDialog ( )
    {
    SMC_MOCK_METHOD1( Q_OBJECT, QString text READ text WRITE setText ) Q_PROPERTY ( QString title READ title WRITE setTitle ) Q_PROPERTY ( HbIcon icon READ icon WRITE setIcon ) Q_PROPERTY ( Hb::TextWrapping titleTextWrapping READ titleTextWrapping WRITE setTitleTextWrapping ) Q_PROPERTY ( bool sequentialShow READ isSequentialShow WRITE setSequentialShow ):HbNotificationDialog, ( )
    }*/

HbNotificationDialog::HbNotificationDialog() //: HbDialog(*new HbNotificationDialogPrivate, 0)
{    
}


// -----------------------------------------------------------------------------
// HbNotificationDialog::~HbNotificationDialog
// -----------------------------------------------------------------------------
//
HbNotificationDialog::~HbNotificationDialog(  )
    {
    //SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::launchDialog
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::launchDialog( 
        const QString & title,
        QGraphicsScene * scene )
    {
    SMC_MOCK_METHOD2( void, const QString &, title, 
        QGraphicsScene *, scene )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::launchDialog
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::launchDialog( 
        const QString & title,
        const QString & text,
        QGraphicsScene * scene )
    {
    SMC_MOCK_METHOD3( void, const QString &, title, 
        const QString &, text, 
        QGraphicsScene *, scene )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::launchDialog
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::launchDialog( 
        const HbIcon & icon,
        const QString & title,
        const QString & text,
        QGraphicsScene * scene )
    {
    SMC_MOCK_METHOD4( void, const HbIcon &, icon, 
        const QString &, title, 
        const QString &, text, 
        QGraphicsScene *, scene )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::enableTouchActivation
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::enableTouchActivation( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::isTouchActivating
// -----------------------------------------------------------------------------
//
bool HbNotificationDialog::isTouchActivating(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::setTitle
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::setTitle( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::setText
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::setText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::setIcon
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::setIcon( 
        const HbIcon & icon )
    {
    SMC_MOCK_METHOD1( void, const HbIcon &, icon )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::title
// -----------------------------------------------------------------------------
//
QString HbNotificationDialog::title(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::text
// -----------------------------------------------------------------------------
//
QString HbNotificationDialog::text(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::icon
// -----------------------------------------------------------------------------
//
HbIcon HbNotificationDialog::icon(  ) const
    {
    SMC_MOCK_METHOD0( HbIcon )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::setTitleTextWrapping
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::setTitleTextWrapping( 
        Hb::TextWrapping wrapping )
    {
    SMC_MOCK_METHOD1( void, Hb::TextWrapping, wrapping )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::titleTextWrapping
// -----------------------------------------------------------------------------
//
Hb::TextWrapping HbNotificationDialog::titleTextWrapping(  ) const
    {
    SMC_MOCK_METHOD0( Hb::TextWrapping )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::setSequentialShow
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::setSequentialShow( 
        bool sequential )
    {
    SMC_MOCK_METHOD1( void, bool, sequential )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::isSequentialShow
// -----------------------------------------------------------------------------
//
bool HbNotificationDialog::isSequentialShow(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }

// -----------------------------------------------------------------------------
// HbNotificationDialog::primitive
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbNotificationDialog::primitive( 
        const QString & itemName ) const
    {
    SMC_MOCK_METHOD1( QGraphicsItem *, const QString &, itemName )
    }

// -----------------------------------------------------------------------------
// HbNotificationDialog::HbNotificationDialog
// -----------------------------------------------------------------------------
//
/*void*/ HbNotificationDialog::HbNotificationDialog( 
        HbNotificationDialogPrivate & dd,
        QGraphicsItem * parent )
    {
    Q_UNUSED(dd)
    Q_UNUSED(parent)    
    //SMC_MOCK_METHOD2( void, HbNotificationDialogPrivate &, dd, 
     //   QGraphicsItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::gestureEvent
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::gestureEvent( 
        QGestureEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGestureEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::hideEvent
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::hideEvent( 
        QHideEvent * event )
    {
    SMC_MOCK_METHOD1( void, QHideEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::showEvent
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::showEvent( 
        QShowEvent * event )
    {
    SMC_MOCK_METHOD1( void, QShowEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbNotificationDialog::itemChange( 
        GraphicsItemChange change,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( QVariant, GraphicsItemChange, change, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// HbNotificationDialog::orientationChanged
// -----------------------------------------------------------------------------
//
void HbNotificationDialog::orientationChanged(Qt::Orientation orientation)
{
    SMC_MOCK_METHOD1( void, Qt::Orientation, orientation )
}
