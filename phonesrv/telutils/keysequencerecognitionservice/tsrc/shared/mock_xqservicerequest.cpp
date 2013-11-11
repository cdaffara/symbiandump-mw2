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
#include <QVariant>
#include <smcmockclassincludes.h>
#include <xqservicerequest.h>
#include <xqrequestinfo.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest(  )
    {
    
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest( 
        const QString & service,
        const QString & message,
        const bool & synchronous )
    {
    Q_UNUSED(service)
    Q_UNUSED(message)
    Q_UNUSED(synchronous)
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest( 
        const XQServiceRequest & orig )
    {
    Q_UNUSED(orig)
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest( 
        const XQAiwInterfaceDescriptor & descriptor,
        const QString & message,
        const bool & synchronous )
    {
    Q_UNUSED(descriptor)
    Q_UNUSED(message)
    Q_UNUSED(synchronous)
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::~XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::~XQServiceRequest(  )
    {
    
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::send
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::send(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::send
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::send( 
        QVariant & retValue )
    {
    SMC_MOCK_METHOD1( bool, QVariant &, retValue )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::isNull
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::isNull(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setSynchronous
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setSynchronous( 
        const bool & synchronous )
    {
    SMC_MOCK_METHOD1( void, const bool &, synchronous )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::isSynchronous
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::isSynchronous(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setService
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setService( 
        const QString & service )
    {
    SMC_MOCK_METHOD1( void, const QString &, service )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::service
// -----------------------------------------------------------------------------
//
QString XQServiceRequest::service(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setMessage
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setMessage( 
        const QString & message )
    {
    SMC_MOCK_METHOD1( void, const QString &, message )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::message
// -----------------------------------------------------------------------------
//
QString XQServiceRequest::message(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::arguments
// -----------------------------------------------------------------------------
//
const QList <QVariant > & XQServiceRequest::arguments(  ) const
    {
    SMC_MOCK_METHOD0( const QList <QVariant > & )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setArguments
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setArguments( 
        const QList<QVariant> & arguments )
    {
    SMC_MOCK_METHOD1( void, const QList<QVariant> &, arguments )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::latestError
// -----------------------------------------------------------------------------
//
int XQServiceRequest::latestError(  )
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setInfo
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setInfo( 
        const XQRequestInfo & requestInfo )
    {
    SMC_MOCK_METHOD1( void, const XQRequestInfo &, requestInfo )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::info
// -----------------------------------------------------------------------------
//
XQRequestInfo XQServiceRequest::info(  ) const
    {
    SMC_MOCK_METHOD0( XQRequestInfo )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::=
// -----------------------------------------------------------------------------
//
XQServiceRequest & XQServiceRequest::operator=( 
        const XQServiceRequest & orig )
    {
    SMC_MOCK_METHOD1( XQServiceRequest &, const XQServiceRequest &, orig )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::serializeArguments
// -----------------------------------------------------------------------------
//
QByteArray XQServiceRequest::serializeArguments( 
        const XQServiceRequest & request )
    {
    SMC_MOCK_METHOD1( QByteArray, const XQServiceRequest &, request )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::deserializeArguments
// -----------------------------------------------------------------------------
//
void XQServiceRequest::deserializeArguments( 
        XQServiceRequest & request,
        const QByteArray & data )
    {
    SMC_MOCK_METHOD2( void, XQServiceRequest &, request, 
        const QByteArray &, data )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::addArg
// -----------------------------------------------------------------------------
//
void XQServiceRequest::addArg(const QVariant& v)
{
    Q_UNUSED(v)
}


// -----------------------------------------------------------------------------
// XQServiceRequest::handleSharableFileArgs
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::handleSharableFileArgs()
{
    SMC_MOCK_METHOD0( bool )
}
