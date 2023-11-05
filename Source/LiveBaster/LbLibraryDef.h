// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbLibraryDef.h                                                //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#if !defined( __LBLIBRARYDEF_H )
#define __LBLIBRARYDEF_H

typedef __int32 index_t;
typedef __int32 id_t;
typedef __int32 count_t;
typedef __int32 active_t;
typedef __int32 context_t; // тип данных для контекста

typedef __int64 result_t; // тип данных для результата
typedef result_t target_t; // тип данных для цели
typedef result_t action_t; // тип данных для действия
typedef __int32 priority_t; // тип данных для приоритета целей

enum control_t { UndefinedControl, SimpleControl, TextControl, EyeControl, EarControl, TouchControl, SpeakerControl };

#define MAXSTR 1024
const id_t EmptyId              = -1;
const index_t EmptyIndex        = -1;
const context_t EmptyContext    = 0;
const index_t AddCount          = 50;

#include "livebaster_global.h"

enum param_t
// типы параметров компонента
{
	DemandParam = 0,	// параметр потребности
	ContextParam = 1,	// параметр контекста
	SensorParam = 2,	// параметр датчика
	MotorParam  = 3,	// параметр мотора
    ParentIdParam = 4   // параметр ID родительского ОТР
};

#endif
