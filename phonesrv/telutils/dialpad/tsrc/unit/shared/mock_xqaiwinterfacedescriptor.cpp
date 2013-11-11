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
#include <QDebug>
#include <smcmockclassincludes.h>
#include <xqaiwinterfacedescriptor.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::XQAiwInterfaceDescriptor
// -----------------------------------------------------------------------------
//
XQAiwInterfaceDescriptor::XQAiwInterfaceDescriptor(  )
    {
    
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::XQAiwInterfaceDescriptor
// -----------------------------------------------------------------------------
//
XQAiwInterfaceDescriptor::XQAiwInterfaceDescriptor( 
        const XQAiwInterfaceDescriptor & other )
    {
    
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::~XQAiwInterfaceDescriptor
// -----------------------------------------------------------------------------
//
XQAiwInterfaceDescriptor::~XQAiwInterfaceDescriptor(  )
    {
    
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::=
// -----------------------------------------------------------------------------
//
XQAiwInterfaceDescriptor & XQAiwInterfaceDescriptor::operator =( 
        const XQAiwInterfaceDescriptor & other )
    {
    return *this;
    //SMC_MOCK_METHOD1( XQAiwInterfaceDescriptor &, const XQAiwInterfaceDescriptor &, other )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::=
// -----------------------------------------------------------------------------
//
bool XQAiwInterfaceDescriptor::operator==( 
        const XQAiwInterfaceDescriptor & other ) const
    {
    SMC_MOCK_METHOD1( bool, const XQAiwInterfaceDescriptor &, other )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::serviceName
// -----------------------------------------------------------------------------
//
QString XQAiwInterfaceDescriptor::serviceName(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::interfaceName
// -----------------------------------------------------------------------------
//
QString XQAiwInterfaceDescriptor::interfaceName(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::majorVersion
// -----------------------------------------------------------------------------
//
int XQAiwInterfaceDescriptor::majorVersion(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::minorVersion
// -----------------------------------------------------------------------------
//
int XQAiwInterfaceDescriptor::minorVersion(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::isValid
// -----------------------------------------------------------------------------
//
bool XQAiwInterfaceDescriptor::isValid(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::inSystemScope
// -----------------------------------------------------------------------------
//
bool XQAiwInterfaceDescriptor::inSystemScope(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::property
// -----------------------------------------------------------------------------
//
QVariant XQAiwInterfaceDescriptor::property( 
        XQAiwInterfaceDescriptor::PropertyKey key ) const
    {
    SMC_MOCK_METHOD1( QVariant, XQAiwInterfaceDescriptor::PropertyKey, key )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::customProperty
// -----------------------------------------------------------------------------
//
QString XQAiwInterfaceDescriptor::customProperty( 
        const QString & key ) const
    {
    SMC_MOCK_METHOD1( QString, const QString &, key )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::customPropertyKeys
// -----------------------------------------------------------------------------
//
QStringList XQAiwInterfaceDescriptor::customPropertyKeys(  ) const
    {
    SMC_MOCK_METHOD0( QStringList )
    }


// -----------------------------------------------------------------------------
// XQAiwInterfaceDescriptor::setProperty
// -----------------------------------------------------------------------------
//
bool XQAiwInterfaceDescriptor::setProperty( 
        XQAiwInterfaceDescriptor::PropertyKey key,
        const QVariant value )
    {
    SMC_MOCK_METHOD2( bool, XQAiwInterfaceDescriptor::PropertyKey, key, 
        const QVariant, value )
    }


