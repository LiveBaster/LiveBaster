// Project LiveBaster                                                         //
// Copyright � 1993-2023 LiveBaster. All Rights Reserved.                     //
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
	// ������ ���� ����������� �����������
	LbPointerTemp<LbBkr> m_controls;
public:
    LbCore();
	~LbCore();

	// ��������� ����� ����������� ��������� � �������
	id_t AddControl( control_t controlType );
	// ������� ����������� ��������� �� �������
	bool DeleteControl( id_t controlId );
	bool Load( const wchar_t* pathName );
	bool Save( const wchar_t* pathName );
	bool Load( FILE* in );
	bool Save( FILE* out );

	//--- ������� �������� ���������� (��)

	// ��������� ����� �������� � ������ ���������� ����������
	id_t AddParameter( id_t controlId, param_t parameterType, result_t minResult, result_t maxResult, result_t result );
	// �������� �������� � ������ ���������� ����������
	bool ChangeParameter( id_t controlId, param_t parameterType, id_t paramId, result_t result );
	// ������� �������� � ������ ���������� ����������
	bool DeleteParameter( id_t controlId, param_t parameterType, id_t paramId );
	// �������� ������� �������� ��������� ����������
	result_t GetParameter( id_t controlId, param_t parameterType, id_t paramId );
	// ������ � ������������ ���
	bool IsArd( id_t controlId );

	// ��������� ��������� ���������� ������� - ���������� ��������� ������� ��� ������?
	// ����� ���� ������ ���� ��������������� ��������� ������� ��� ������ ChangeParameter
	int UpdateMotorAction( id_t controlId );

};

#endif // LBCORE_H
