// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbCore.h                                                      //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#if !defined( __LBCORE_H )
#define __LBCORE_H

#include "LbLibraryDef.h"
#include "LbSimpleControl.h"

class LIVEBASTERSHARED_EXPORT LbCore
{
protected:
	// список всех управляющих компонентов
	LbPointerTemp<LbBkr> m_controls;
public:
    LbCore();
	~LbCore();

	// добавляет новый управляющий компонент в систему
	id_t AddControl( control_t controlType );
	// удаляет управляющий компонент из системы
	bool DeleteControl( id_t controlId );
	bool Load( const wchar_t* pathName );
	bool Save( const wchar_t* pathName );
	bool Load( FILE* in );
	bool Save( FILE* out );

	//--- Функции Базового Компонента (БК)

	// добавляет новый параметр в список параметров компонента
	id_t AddParameter( id_t controlId, param_t parameterType, result_t minResult, result_t maxResult, result_t result );
	// изменяет параметр в списке параметров компонента
	bool ChangeParameter( id_t controlId, param_t parameterType, id_t paramId, result_t result );
	// удаляет параметр в списке параметров компонента
	bool DeleteParameter( id_t controlId, param_t parameterType, id_t paramId );
	// получает текущее значение параметра компонента
	result_t GetParameter( id_t controlId, param_t parameterType, id_t paramId );
	// сигнал о срабатывании АРД
	bool IsArd( id_t controlId );

	// обновляем параметры управления мотором - фактически принимаем решение Как Делать?
	// перед этим должны быть актуализированы параметры датчика при помощи ChangeParameter
	int UpdateMotorAction( id_t controlId );

};

#endif // LBCORE_H
