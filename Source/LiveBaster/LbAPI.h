// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbAPI.h                                                       //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#if !defined( __LBAPI_H )
#define __LBAPI_H

#include "LbLibraryDef.h"

// загружает ядро
bool LIVEBASTERSHARED_EXPORT LbLoadCore();

// выгружает ядро
bool LIVEBASTERSHARED_EXPORT LbFreeCore();

// добавляет новый управляющий компонент в систему
id_t LIVEBASTERSHARED_EXPORT LbAddControl( control_t controlType );

// удаляет управляющий компонент из системы
bool LIVEBASTERSHARED_EXPORT LbDeleteControl( id_t controlId );

// сохраняет знания в файл pathName/LiveBaster.lbd
bool LIVEBASTERSHARED_EXPORT LbSave( const wchar_t* pathName );

// загружает знания из файла pathName/LiveBaster.lbd
bool LIVEBASTERSHARED_EXPORT LbLoad( const wchar_t* pathName );

// Функции Базового Компонента (БК)

// добавляет новый параметр в список параметров компонента
id_t LIVEBASTERSHARED_EXPORT LbAddParameter( id_t controlId, param_t parameterType, result_t minResult, result_t maxResult, result_t result );

// изменяет параметр в списке параметров компонента
bool LIVEBASTERSHARED_EXPORT LbChangeParameter( id_t controlId, param_t parameterType, id_t paramId, result_t result );

// удаляет параметр в списке параметров компонента
bool LIVEBASTERSHARED_EXPORT LbDeleteParameter( id_t controlId, param_t parameterType, id_t paramId );

// получает текущее значение параметра компонента
result_t LIVEBASTERSHARED_EXPORT LbGetParameter( id_t controlId, param_t parameterType, id_t paramId );

// обновляем параметры управления мотором - фактически принимаем решение Как Делать?
// перед этим должны быть актуализированы параметры датчика при помощи ChangeParameter
int LIVEBASTERSHARED_EXPORT LbUpdateMotorAction();

// управление последовательными целями

// управление одновременными целями

// управление зрением

#endif
