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

class XQServiceRequest : public QObject
{
public:
    XQServiceRequest(const QString& service, const QString& message, const bool& synchronous);
    ~XQServiceRequest();
    bool send(QVariant& retValue);
    void addArg(const QVariant& v);
    template<typename T>
    inline XQServiceRequest &operator<< (const T &var)
    {
        QVariant v = qVariantFromValue(var);
        addArg(v);
        return *this;
    }
};
