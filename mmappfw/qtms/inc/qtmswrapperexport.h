/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QTMS_WRAPPER_EXPORT_H_
#define QTMS_WRAPPER_EXPORT_H_

// System includes
#include <qglobal.h>

#ifdef BUILD_TMS_WRAPPER_DLL
#define QTMS_WRAPPER_DLL_EXPORT Q_DECL_EXPORT
#else
#define QTMS_WRAPPER_DLL_EXPORT Q_DECL_IMPORT
#endif

#endif // QTMS_WRAPPER_EXPORT_H_
