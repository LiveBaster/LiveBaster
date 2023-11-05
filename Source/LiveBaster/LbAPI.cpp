// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbAPI.cpp                                                     //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#include "LbAPI.h"
#include "LbCore.h"

LbCore* pLbCore = 0;

bool LIVEBASTERSHARED_EXPORT LbLoadCore()
{
	if( pLbCore != 0 )
		return true;
	if( pLbCore == 0 ) {
		pLbCore = new LbCore();
		return true;
	}
	return false;
}

bool LIVEBASTERSHARED_EXPORT LbFreeCore()
{
	if( pLbCore != 0 ) {
		delete pLbCore;
		pLbCore = 0;
		return true;
	}
    return false;
}

id_t LIVEBASTERSHARED_EXPORT LbAddControl( control_t controlType )
{
	if( !pLbCore )
		LbLoadCore();
	if( pLbCore )
		return pLbCore->AddControl( controlType );
	return EmptyId;
}

bool LIVEBASTERSHARED_EXPORT LbDeleteControl( id_t controlId )
{
	if( pLbCore )
		return pLbCore->DeleteControl( controlId );
	return false;
}

bool LIVEBASTERSHARED_EXPORT LbSave( const wchar_t* pathName )
{
	if( pLbCore )
		return pLbCore->Save( pathName );
	return false;
}

bool LIVEBASTERSHARED_EXPORT LbLoad( const wchar_t* pathName )
{
	if( pLbCore )
		return pLbCore->Load( pathName );
	return false;
}

id_t LIVEBASTERSHARED_EXPORT LbAddParameter( id_t controlId, param_t parameterType, result_t minResult, result_t maxResult, result_t result )
{
	id_t outId = EmptyId;
	if( pLbCore )
		outId = pLbCore->AddParameter( controlId, parameterType, minResult, maxResult, result );
	return outId;
}

bool LIVEBASTERSHARED_EXPORT LbChangeParameter( id_t controlId, param_t parameterType, id_t paramId, result_t result )
{
	if( pLbCore )
		return pLbCore->ChangeParameter( controlId, parameterType, paramId, result );
	return false;
}

bool LIVEBASTERSHARED_EXPORT LbDeleteParameter( id_t controlId, param_t parameterType, id_t paramId )
{
	if( pLbCore )
		return pLbCore->DeleteParameter( controlId, parameterType, paramId );
	return false;
}

result_t LIVEBASTERSHARED_EXPORT LbGetParameter( id_t controlId, param_t parameterType, id_t paramId )
{
	result_t outResult = 0;
	if( pLbCore )
		outResult = pLbCore->GetParameter( controlId, parameterType, paramId );
	return outResult;
}

int LIVEBASTERSHARED_EXPORT LbUpdateMotorAction( id_t controlId )
{
	if( pLbCore )
		return pLbCore->UpdateMotorAction( controlId );
	return -1;
}

bool LIVEBASTERSHARED_EXPORT LbEyeAddFacet( id_t controlId, int x, int y )
{
	return false;
}

id_t LIVEBASTERSHARED_EXPORT LbEyeAddAction( id_t controlId )
{
	return false;
}

action_t LIVEBASTERSHARED_EXPORT LbEyeFacetMakeAction( id_t controlId, int x, int y, target_t inDesiredResult, result_t inResult, context_t inContext )
{
	return 0;
}

action_t LIVEBASTERSHARED_EXPORT LbEyeMakeAction( id_t controlId, id_t actionId, target_t inDesiredResult, result_t inResult, context_t inContext )
{
	return 0;
}
