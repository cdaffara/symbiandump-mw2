/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
#include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QTextBlock>
#include <QTextDocument>

#include <smcmockclassincludes.h>
#include "hblineedit.h"
#include "hbabstractedit.h"
#include "hbwidget.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbLineEdit::HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::HbLineEdit(QGraphicsItem *parent) 
{
    Q_UNUSED(parent)
}

// -----------------------------------------------------------------------------
// HbLineEdit::HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::HbLineEdit( 
        const QString & text,
        QGraphicsItem * parent )
    {
    Q_UNUSED(text)
    Q_UNUSED(parent)
    }


// -----------------------------------------------------------------------------
// HbLineEdit::~HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::~HbLineEdit(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbLineEdit::type
// -----------------------------------------------------------------------------
//
int HbLineEdit::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::maxLength
// -----------------------------------------------------------------------------
//
int HbLineEdit::maxLength(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setMaxLength
// -----------------------------------------------------------------------------
//
void HbLineEdit::setMaxLength( 
        int length )
    {
    SMC_MOCK_METHOD1( void, int, length )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setMinRows
// -----------------------------------------------------------------------------
//
void HbLineEdit::setMinRows( 
        int rows )
    {
    SMC_MOCK_METHOD1( void, int, rows )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::minRows
// -----------------------------------------------------------------------------
//
int HbLineEdit::minRows(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setMaxRows
// -----------------------------------------------------------------------------
//
void HbLineEdit::setMaxRows( 
        int rows )
    {
    SMC_MOCK_METHOD1( void, int, rows )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::maxRows
// -----------------------------------------------------------------------------
//
int HbLineEdit::maxRows(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::isExpandable
// -----------------------------------------------------------------------------
//
bool HbLineEdit::isExpandable(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::text
// -----------------------------------------------------------------------------
//
QString HbLineEdit::text(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::displayText
// -----------------------------------------------------------------------------
//
QString HbLineEdit::displayText(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::hasSelectedText
// -----------------------------------------------------------------------------
//
bool HbLineEdit::hasSelectedText(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::selectedText
// -----------------------------------------------------------------------------
//
QString HbLineEdit::selectedText(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::selectionStart
// -----------------------------------------------------------------------------
//
int HbLineEdit::selectionStart(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setSelection
// -----------------------------------------------------------------------------
//
void HbLineEdit::setSelection( 
        int start,
        int length )
    {
    SMC_MOCK_METHOD2( void, int, start, 
        int, length )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setCursorPosition
// -----------------------------------------------------------------------------
//
void HbLineEdit::setCursorPosition( 
        int pos )
    {
    SMC_MOCK_METHOD1( void, int, pos )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::echoMode
// -----------------------------------------------------------------------------
//
HbLineEdit::EchoMode HbLineEdit::echoMode(  ) const
    {
    SMC_MOCK_METHOD0( HbLineEdit::EchoMode )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setEchoMode
// -----------------------------------------------------------------------------
//
void HbLineEdit::setEchoMode( HbLineEdit::EchoMode echoMode )
    {
    SMC_MOCK_METHOD1( void, HbLineEdit::EchoMode, echoMode )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setCapitalization
// -----------------------------------------------------------------------------
//
void HbLineEdit::setCapitalization( 
        QFont::Capitalization caps )
    {
    SMC_MOCK_METHOD1( void, QFont::Capitalization, caps )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::capitalization
// -----------------------------------------------------------------------------
//
QFont::Capitalization HbLineEdit::capitalization(  ) const
    {
    SMC_MOCK_METHOD0( QFont::Capitalization )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::adjustFontSizeToFitHeight
// -----------------------------------------------------------------------------
//
bool HbLineEdit::adjustFontSizeToFitHeight(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setAdjustFontSizeToFitHeight
// -----------------------------------------------------------------------------
//
void HbLineEdit::setAdjustFontSizeToFitHeight( 
        bool active )
    {
    SMC_MOCK_METHOD1( void, bool, active )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::event
// -----------------------------------------------------------------------------
//
bool HbLineEdit::event( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setAutoCompleteContent
// -----------------------------------------------------------------------------
//
void HbLineEdit::setAutoCompleteContent( 
        QGraphicsLayoutItem * content )
    {
    SMC_MOCK_METHOD1( void, QGraphicsLayoutItem *, content )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::setText
// -----------------------------------------------------------------------------
//
void HbLineEdit::setText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::editingFinished
// -----------------------------------------------------------------------------
//
void HbLineEdit::editingFinished(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::textChanged
// -----------------------------------------------------------------------------
//
void HbLineEdit::textChanged( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::selectionChanged
// -----------------------------------------------------------------------------
//
void HbLineEdit::selectionChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::HbLineEdit( 
        HbLineEditPrivate & dd,
        QGraphicsItem * parent )
    {
    Q_UNUSED(dd)
    Q_UNUSED(parent)
    }


// -----------------------------------------------------------------------------
// HbLineEdit::inputMethodEvent
// -----------------------------------------------------------------------------
//
void HbLineEdit::inputMethodEvent( 
        QInputMethodEvent * event )
    {
    SMC_MOCK_METHOD1( void, QInputMethodEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::inputMethodQuery
// -----------------------------------------------------------------------------
//
QVariant HbLineEdit::inputMethodQuery( 
        Qt::InputMethodQuery input) const
    {
    SMC_MOCK_METHOD1( QVariant, Qt::InputMethodQuery, input )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::keyPressEvent
// -----------------------------------------------------------------------------
//
void HbLineEdit::keyPressEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::keyReleaseEvent
// -----------------------------------------------------------------------------
//
void HbLineEdit::keyReleaseEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::canInsertFromMimeData
// -----------------------------------------------------------------------------
//
bool HbLineEdit::canInsertFromMimeData( 
        const QMimeData * source ) const
    {
    SMC_MOCK_METHOD1( bool, const QMimeData *, source )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::insertFromMimeData
// -----------------------------------------------------------------------------
//
void HbLineEdit::insertFromMimeData( 
        const QMimeData * source )
    {
    SMC_MOCK_METHOD1( void, const QMimeData *, source )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::focusOutEvent
// -----------------------------------------------------------------------------
//
void HbLineEdit::focusOutEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::focusInEvent
// -----------------------------------------------------------------------------
//
void HbLineEdit::focusInEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLineEdit::eventFilter
// -----------------------------------------------------------------------------
//
bool HbLineEdit::eventFilter( 
        QObject *obj,
        QEvent *ev )
    {
    SMC_MOCK_METHOD2( bool, QObject *, obj, 
        QEvent *, ev )
    }


