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
#include <QDebug>
#include <QtGui>
#include <hbframedrawer.h>
#include <hbinstance.h>
#include <hbeffect.h>
#include <hbinstance.h>
#include <hbmainwindow.h>
#include <hbstyleloader.h>
#include <hblineedit.h>
#include <hbapplication.h>
#include <smcmockclassincludes.h>
#include "dialpad.h"

static const int DialpadCloseAnimDuration = 200; // ms
static const int DialpadOpenAnimDuration = 200; // ms

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Dialpad::Dialpad
// -----------------------------------------------------------------------------
//
Dialpad::Dialpad(  )
    :
    mMainWindow(*hbInstance->allMainWindows().at(0)),
    mBackgroundDrawer(0),
    mIconDrawer(0),
    mBackgroundItem(0),
    mInputField(0),
    mKeypad(0),
    mMultitap(0),
    mOpenTimeLine(DialpadOpenAnimDuration),
    mCloseTimeLine(DialpadCloseAnimDuration),
    mAnimationOngoing(false),
    mOrientation(Qt::Vertical),
    mIsOpen(false)
    {
    
    }


// -----------------------------------------------------------------------------
// Dialpad::Dialpad
// -----------------------------------------------------------------------------
//
Dialpad::Dialpad( const HbMainWindow & mainWindow )
    :
    mMainWindow(mainWindow)
    {
    
    }


// -----------------------------------------------------------------------------
// Dialpad::~Dialpad
// -----------------------------------------------------------------------------
//
Dialpad::~Dialpad(  )
    {
    }


// -----------------------------------------------------------------------------
// Dialpad::editor
// -----------------------------------------------------------------------------
//
HbLineEdit & Dialpad::editor(  ) const
    {
    SMC_MOCK_METHOD0( HbLineEdit & )
    }


// -----------------------------------------------------------------------------
// Dialpad::changeEvent
// -----------------------------------------------------------------------------
//
void Dialpad::changeEvent(QEvent *event)
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::isOpen
// -----------------------------------------------------------------------------
//
bool Dialpad::isOpen(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// Dialpad::openDialpad
// -----------------------------------------------------------------------------
//
void Dialpad::openDialpad(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeDialpad
// -----------------------------------------------------------------------------
//
void Dialpad::closeDialpad(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::setCallButtonEnabled
// -----------------------------------------------------------------------------
//
void Dialpad::setCallButtonEnabled( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// Dialpad::setTapOutsideDismiss
// -----------------------------------------------------------------------------
//
void Dialpad::setTapOutsideDismiss( 
        bool dismiss )
    {
    SMC_MOCK_METHOD1( void, bool, dismiss )
    }


// -----------------------------------------------------------------------------
// Dialpad::paint
// -----------------------------------------------------------------------------
//
void Dialpad::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }

// -----------------------------------------------------------------------------
// Dialpad::showEvent
// -----------------------------------------------------------------------------
//
void Dialpad::showEvent( 
        QShowEvent * event )
    {
    SMC_MOCK_METHOD1( void, QShowEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::hideEvent
// -----------------------------------------------------------------------------
//
void Dialpad::hideEvent( 
        QHideEvent * event )
    {
    SMC_MOCK_METHOD1( void, QHideEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeEvent
// -----------------------------------------------------------------------------
//
void Dialpad::closeEvent( 
        QCloseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QCloseEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeAnimValueChanged
// -----------------------------------------------------------------------------
//
void Dialpad::closeAnimValueChanged( 
        qreal value )
    {
    SMC_MOCK_METHOD1( void, qreal, value )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeAnimFinished
// -----------------------------------------------------------------------------
//
void Dialpad::closeAnimFinished(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::openAnimValueChanged
// -----------------------------------------------------------------------------
//
void Dialpad::openAnimValueChanged( 
        qreal value )
    {
    SMC_MOCK_METHOD1( void, qreal, value )
    }


// -----------------------------------------------------------------------------
// Dialpad::openAnimFinished
// -----------------------------------------------------------------------------
//
void Dialpad::openAnimFinished(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::orientationChangeStarted
// -----------------------------------------------------------------------------
//
void Dialpad::orientationChangeStarted(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::orientationChangeFinished
// -----------------------------------------------------------------------------
//
void Dialpad::orientationChangeFinished( 
        Qt::Orientation current )
    {
    SMC_MOCK_METHOD1( void, Qt::Orientation, current )
    }

// -----------------------------------------------------------------------------
// Dialpad::gestureEvent
// -----------------------------------------------------------------------------
//
void Dialpad::gestureEvent(QGestureEvent *event)
    {
	 	SMC_MOCK_METHOD1( void, QGestureEvent *, event )
    }


