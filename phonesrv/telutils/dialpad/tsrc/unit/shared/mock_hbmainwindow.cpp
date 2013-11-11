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

#include <QtGui>
#include <QDebug>
#include <smcmockclassincludes.h>
#include <hbmainwindow.h>
#include <hbview.h>
#include <hbnamespace.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbMainWindow::~HbMainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow::~HbMainWindow(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbMainWindow::addView
// -----------------------------------------------------------------------------
//
HbView * HbMainWindow::addView( 
        QGraphicsWidget * widget )
    {
    SMC_MOCK_METHOD1( HbView *, QGraphicsWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::insertView
// -----------------------------------------------------------------------------
//
HbView * HbMainWindow::insertView( 
        int index,
        QGraphicsWidget * widget )
    {
    SMC_MOCK_METHOD2( HbView *, int, index, 
        QGraphicsWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::removeView
// -----------------------------------------------------------------------------
//
void HbMainWindow::removeView( 
        QGraphicsWidget * widget )
    {
    SMC_MOCK_METHOD1( void, QGraphicsWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::views
// -----------------------------------------------------------------------------
//
QList <HbView * > HbMainWindow::views(  ) const
    {
    SMC_MOCK_METHOD0( QList <HbView * > )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::currentView
// -----------------------------------------------------------------------------
//
HbView * HbMainWindow::currentView(  ) const
    {
    SMC_MOCK_METHOD0( HbView * )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setCurrentView
// -----------------------------------------------------------------------------
//
void HbMainWindow::setCurrentView( 
        HbView * view,
        bool animate,
        Hb::ViewSwitchFlags flags )
    {
    }


// -----------------------------------------------------------------------------
// HbMainWindow::orientation
// -----------------------------------------------------------------------------
//
Qt::Orientation HbMainWindow::orientation(  ) const
    {
    SMC_MOCK_METHOD0( Qt::Orientation )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::setOrientation( 
        Qt::Orientation orientation,
        bool animate )
    {
    SMC_MOCK_METHOD2( void, Qt::Orientation, orientation, 
        bool, animate )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::unsetOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::unsetOrientation( 
        bool animate )
    {
    SMC_MOCK_METHOD1( void, bool, animate )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::nativeBackgroundWindow
// -----------------------------------------------------------------------------
//
WId HbMainWindow::nativeBackgroundWindow(  )
    {
    SMC_MOCK_METHOD0( WId )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::resetNativeBackgroundWindow
// -----------------------------------------------------------------------------
//
void HbMainWindow::resetNativeBackgroundWindow(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::layoutRect
// -----------------------------------------------------------------------------
//
QRectF HbMainWindow::layoutRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setBackgroundImageName
// -----------------------------------------------------------------------------
//
void HbMainWindow::setBackgroundImageName( 
        Qt::Orientation orientation,
        const QString & name )
    {
    SMC_MOCK_METHOD2( void, Qt::Orientation, orientation, 
        const QString &, name )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::backgroundImageName
// -----------------------------------------------------------------------------
//
QString HbMainWindow::backgroundImageName( 
        Qt::Orientation orientation ) const
    {
    SMC_MOCK_METHOD1( QString, Qt::Orientation, orientation )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setBackgroundImageMode
// -----------------------------------------------------------------------------
//
void HbMainWindow::setBackgroundImageMode( 
        Hb::BackgroundImageMode mode )
    {
    SMC_MOCK_METHOD1( void, Hb::BackgroundImageMode, mode )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::backgroundImageMode
// -----------------------------------------------------------------------------
//
Hb::BackgroundImageMode HbMainWindow::backgroundImageMode(  ) const
    {
    SMC_MOCK_METHOD0( Hb::BackgroundImageMode )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setAutomaticOrientationEffectEnabled
// -----------------------------------------------------------------------------
//
void HbMainWindow::setAutomaticOrientationEffectEnabled( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::automaticOrientationEffectEnabled
// -----------------------------------------------------------------------------
//
bool HbMainWindow::automaticOrientationEffectEnabled(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::broadcastEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::broadcastEvent( 
        int eventType )
    {
    SMC_MOCK_METHOD1( void, int, eventType )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::viewReady
// -----------------------------------------------------------------------------
//
void HbMainWindow::viewReady(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::aboutToChangeView
// -----------------------------------------------------------------------------
//
void HbMainWindow::aboutToChangeView( 
        HbView * oldView,
        HbView * newView )
    {
    SMC_MOCK_METHOD2( void, HbView *, oldView, 
        HbView *, newView )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::currentViewChanged
// -----------------------------------------------------------------------------
//
void HbMainWindow::currentViewChanged( 
        HbView * view )
    {
    SMC_MOCK_METHOD1( void, HbView *, view )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::aboutToChangeOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::aboutToChangeOrientation(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::aboutToChangeOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::aboutToChangeOrientation( 
        Qt::Orientation newOrientation,
        bool animated )
    {
    SMC_MOCK_METHOD2( void, Qt::Orientation, newOrientation, 
        bool, animated )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::orientationChanged
// -----------------------------------------------------------------------------
//
void HbMainWindow::orientationChanged( 
        Qt::Orientation orientation )
    {
    SMC_MOCK_METHOD1( void, Qt::Orientation, orientation )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::changeEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::closeEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::closeEvent( 
        QCloseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QCloseEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::keyPressEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::keyPressEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::keyReleaseEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::keyReleaseEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::resizeEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::resizeEvent( 
        QResizeEvent * event )
    {
    SMC_MOCK_METHOD1( void, QResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::customEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::customEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::scrollContentsBy
// -----------------------------------------------------------------------------
//
void HbMainWindow::scrollContentsBy( 
        int dx,
        int dy )
    {
    SMC_MOCK_METHOD2( void, int, dx, 
        int, dy )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::paintEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::paintEvent( 
        QPaintEvent * event )
    {
    SMC_MOCK_METHOD1( void, QPaintEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::showEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::showEvent( 
        QShowEvent * event )
    {
    SMC_MOCK_METHOD1( void, QShowEvent *, event )
    }


