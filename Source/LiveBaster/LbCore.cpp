// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbCore.cpp                                                    //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#include "LbCore.h"

LbCore::LbCore()
{
}

LbCore::~LbCore()
{
}

id_t LbCore::AddControl( control_t controlType )
{
	return m_controls.Insert( new LbBkr( m_controls.GetCount(), controlType ) );
}

bool LbCore::DeleteControl( id_t controlId )
{
	LbBkr item( controlId, UndefinedControl );
	index_t index = 0;
	if( m_controls.Search( &item, index ) ) {
		delete m_controls.AtRemove( index );
		return true;
	}
	return false;
}

bool LbCore::Load( const wchar_t* pathName )
{
	bool rt = false;
	FILE* pFile = 0;
	errno_t err = _wfopen_s( &pFile, pathName, L"rb" );
	if( err == 0 ) {
		rt = Load( pFile );
		fclose( pFile );
	}
	return rt;
}

bool LbCore::Save( const wchar_t* pathName )
{
	bool rt = false;
	FILE* pFile = 0;
	errno_t err = _wfopen_s( &pFile, pathName, L"wb" );
	if( err == 0 ) {
		rt = Save( pFile );
		fclose( pFile );
	}
	return rt;
}

bool LbCore::Load( FILE* in )
{
	bool rt = true;
	if( !m_controls.Load( in ) )
		rt = false;
	return rt;
}

bool LbCore::Save( FILE* out )
{
	bool rt = true;
	if( !m_controls.Save( out ) )
		rt = false;
	return rt;
}

id_t LbCore::AddParameter( id_t controlId, param_t parameterType, result_t minResult, result_t maxResult, result_t result )
{
	LbBkr item( controlId, UndefinedControl );
	index_t index = 0;
	if( m_controls.Search( &item, index ) )
		return m_controls[index].AddParameter( parameterType, minResult, maxResult, result );
	return EmptyId;
}

bool LbCore::ChangeParameter( id_t controlId, param_t parameterType, id_t paramId, result_t result )
{
	LbBkr item( controlId, UndefinedControl );
	index_t index = 0;
	if( m_controls.Search( &item, index ) )
		return m_controls[index].ChangeParameter( parameterType, paramId, result );
	return false;
}

bool LbCore::DeleteParameter( id_t controlId, param_t parameterType, id_t paramId )
{
	LbBkr item( controlId, UndefinedControl );
	index_t index = 0;
	if( m_controls.Search( &item, index ) )
		return m_controls[index].DeleteParameter( parameterType, paramId );
	return false;
}

result_t LbCore::GetParameter( id_t controlId, param_t parameterType, id_t paramId )
{
	LbBkr item( controlId, UndefinedControl );
	index_t index = 0;
	if( m_controls.Search( &item, index ) )
		return m_controls[index].GetParameter( parameterType, paramId );
	return 0;
}

bool LbCore::IsArd( id_t controlId )
{
	LbBkr item( controlId, UndefinedControl );
	index_t index = 0;
	if( m_controls.Search( &item, index ) )
		return m_controls[index].IsArd();
	return 0;
}

int LbCore::UpdateMotorAction( id_t controlId )
{
	LbBkr item( controlId, UndefinedControl );
	index_t index = 0;
	if( m_controls.Search( &item, index ) )
		return m_controls[index].UpdateMotorAction( this );
	return -1;
}
