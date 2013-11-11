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
 * Description: Header File for Framework Manager
 *
 */

#ifndef XAFRAMEWORKMANAGER_H
#define XAFRAMEWORKMANAGER_H

#include <stdlib.h>

typedef enum
    {
    FWMgrMOUnknown,
    FWMgrMOPlayer,
    FWMgrMORecorder
    } FWMgrMOType;

typedef enum
    {
    FWMgrFWUknown,
    FWMgrFWMMF,
    FWMgrFWGST
    } FWMgrFwType;

typedef enum
    {
    FWMgrFalse,
    FWMgrTrue
    } FWMgrBool;

typedef struct FrameworkMap_ FrameworkMap;

struct FrameworkMap_
    {
    FWMgrMOType moType;
    int uriSchemeCount;
    char **uriSchemes;
    int fileExtCount;
    char **fileExts;
    FWMgrFwType fwType;
    FrameworkMap *next;
    };

FrameworkMap* XAFrameworkMgr_CreateFrameworkMap(void);
#ifdef _DEBUG
void XAFrameworkMgr_DumpFrameworkMap(FrameworkMap *map);
#endif
void XAFrameworkMgr_DeleteFrameworkMap(FrameworkMap **map);

FWMgrFwType XAFrameworkMgr_GetFramework(FrameworkMap *map, const char *uri,
        FWMgrMOType mediaObject);

#endif //XAFRAMEWORKMANAGER_H
