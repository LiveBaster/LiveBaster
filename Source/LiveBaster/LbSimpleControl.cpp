// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbSimpleControl.cpp                                           //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#include "LbSimpleControl.h"
#include "LbLogFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "LbCore.h"

#define ID_DEBUG 24

//--- Class LbOtr --------------------------------------------------------------

LbOtr::LbOtr() :
    m_idControl( EmptyId ),
    m_context( EmptyContext ),
    m_result( 0 ),
    m_demand( 0 ),
    m_demandDone( false )
{
}

LbOtr::LbOtr( id_t idControl, context_t context, result_t result, result_t demand ) :
    m_idControl( idControl ),
    m_context( context ),
    m_result( result ),
    m_demand( demand ),
    m_demandDone( false )
{
}

LbOtr::~LbOtr()
{
}

//--- Class LbOtrRoot ----------------------------------------------------------

LbOtrRoot::LbOtrRoot() :
    m_minDemand( 0 ),
    m_maxDemand( 0 ),
    m_sign( 1 )
{
}

LbOtrRoot::LbOtrRoot( result_t minDemand, result_t maxDemand ) :
    m_minDemand( minDemand ),
    m_maxDemand( maxDemand ),
    m_sign( 1 )
{
    Init( m_minDemand, m_maxDemand );
}

LbOtrRoot::~LbOtrRoot()
{
}

void LbOtrRoot::Init( result_t minDemand, result_t maxDemand )
{
    m_minDemand = minDemand;
    m_maxDemand = maxDemand;
    for( index_t i=m_minDemand; i<=m_maxDemand; i++ ) {
        LbOtrPtr pNewOtr = std::make_shared<LbOtr>( 0 /*???*/, 0, 0, i );
        m_axis.InsertEnd( pNewOtr );
        if( i == 0 )
            // индекс начала координат
            m_curIndex = m_axis.GetCount()-1;
    }
}

bool LbOtrRoot::SynthesisBranch( id_t id, result_t demand, context_t context, result_t result, LbArrayTemp<index_t>& decision )
{
    result_t minR = INT_MAX;
    LbOtrPtr pNearOtr = nullptr;
    index_t minIndex = 0;
    for( index_t i=0; i<m_axis.GetCount(); i++ ) {
        m_curOtr = m_axis[i];
        for( std::list<LbOtrPtr>::iterator itOtr=m_curOtr->GetNextOtrList().begin(); itOtr != m_curOtr->GetNextOtrList().end(); ++itOtr ) {
	        result_t rResult = (demand-(*itOtr)->GetResult())*(demand-(*itOtr)->GetResult())+(context-(*itOtr)->GetContext())*(context-(*itOtr)->GetContext());
            if( minR > rResult ) {
                minR = rResult;
                pNearOtr = *itOtr;
                minIndex = i;
            }
        }
    }
    if( pNearOtr ) {
        m_axis[m_curIndex]->GetNextOtrList().remove( pNearOtr );
        if( minIndex >= m_curIndex ) {
            for( index_t j=m_curIndex; j<=minIndex; j++ ) {
                decision.InsertEnd( j );
                m_curIndex = j;
            }
        }
        else {
            for( index_t j=m_curIndex; j>=minIndex; j-- ) {
                decision.InsertEnd( j );
                m_curIndex = j;
            }
        }
    }
    // ориентировочно-исследовательская реакция
    // m_sign обеспечивает симметрию
    const index_t maxOffset = (m_maxDemand-m_minDemand)*2/3;
    if( m_sign ) {
        for( index_t i=m_curIndex, offset=0; i>=0&&offset<maxOffset; i--, offset++ ) {
            decision.InsertEnd( i );
            m_curIndex = i;
        }
        for( index_t i=m_curIndex, offset=0; i<m_axis.GetCount()&&offset<maxOffset; i++, offset++ ) {
            decision.InsertEnd( i );
            m_curIndex = i;
        }
    }
    else {
        for( index_t i=m_curIndex, offset=0; i<m_axis.GetCount()&&offset<maxOffset; i++, offset++ ) {
            decision.InsertEnd( i );
            m_curIndex = i;
        }
        for( index_t i=m_curIndex, offset=0; i>=0&&offset<maxOffset; i--, offset++ ) {
            decision.InsertEnd( i );
            m_curIndex = i;
        }
    }
    m_sign = -m_sign;
    return true;
}

result_t LbOtrRoot::GetDemand( index_t ind )
{
    m_curIndex = ind;
    LbOtrPtr pOtr = m_axis[ind];
    return pOtr->GetDemand();
}

void LbOtrRoot::AddResult( context_t context, result_t result, result_t demand )
{
    LbOtrPtr pOtr = m_axis[m_curIndex];
    // проверяем существование такого же ОТР
    LbOtrPtr pNearOtr = nullptr;
    for( std::list<LbOtrPtr>::iterator itOtr=pOtr->GetNextOtrList().begin(); itOtr != pOtr->GetNextOtrList().end(); ++itOtr ) {
        if( (*itOtr)->GetResult() == result && (*itOtr)->GetContext() == context && (*itOtr)->GetDemand() == demand ) {
            pNearOtr = *itOtr;
            break;
        }
    }
    if( pNearOtr == nullptr ) {
        LbOtrPtr pNewOtr = std::make_shared<LbOtr>( 0, context, result, demand );
        pOtr->GetNextOtrList().push_back( pNewOtr );
    }
}

//--- Class LbBkr --------------------------------------------------------------

LbBkr::LbBkr() :
	m_controlType( SimpleControl ),
	m_id( EmptyId ),
    m_parentId( EmptyId ),
	m_actualDemand( 0 ),
	m_actualContext( 0 ),
	m_actualResult( 0 ),
    m_minDemand( 0 ),
    m_maxDemand( 0 ),
	m_actualAction( 0 )
{
}

LbBkr::LbBkr( id_t id, control_t controlType ) :
	m_controlType( controlType ),
	m_id( id ),
    m_parentId( EmptyId ),
	m_actualDemand( 0 ),
	m_actualContext( 0 ),
	m_actualResult( 0 ),
    m_minDemand( 0 ),
    m_maxDemand( 0 ),
	m_actualAction( 0 )
{
}

LbBkr::~LbBkr()
{
}

int LbBkr::Compare( const LbLinkPointer* poi )
{
	LbBkr* p = (LbBkr*)poi;
	if( m_id == p->m_id )
		return 0;
	if( m_id < p->m_id )
		return -1;
	return 1;
}

LbLinkPointer* LbBkr::Duplicate() const
{
	return (new LbBkr())->Copy( this );
}

LbLinkPointer* LbBkr::Copy( const LbLinkPointer* )
{
	return 0;
}

id_t LbBkr::AddParameter( param_t parameterType, result_t minResult, result_t maxResult, result_t result )
{
	id_t newId = EmptyId;
	if( parameterType == DemandParam ) {
		newId = 0;
		m_actualDemand = 0; // начальное значение
        m_minDemand = minResult;
        m_maxDemand = maxResult;
	}
	else
	if( parameterType == ContextParam ) {
		newId = 0;
		m_actualContext = 0; // начальное значение
        m_minContext = minResult;
        m_maxContext = maxResult;
	}
	else
	if( parameterType == SensorParam ) {
		newId = 0;
		m_actualResult = 0; // начальное значение
        m_minResult = minResult;
        m_maxResult = maxResult;
	}
	else
	if( parameterType == MotorParam ) {
		newId = 0;
		m_actualAction = 0; // начальное значение
        m_minAction = minResult;
        m_maxAction = maxResult;
        m_dr.Init( minResult, maxResult );
	}
	else
	if( parameterType == ParentIdParam ) {
		newId = 0;
		m_parentId = 0; // начальное значение
	}
	return newId;
}

bool LbBkr::ChangeParameter( param_t parameterType, id_t paramId, result_t result )
{
	if( parameterType == DemandParam ) {
		m_actualDemand = result;
		return true;
	}
	else
	if( parameterType == ContextParam ) {
		m_actualContext = result;
		return true;
	}
	else
	if( parameterType == SensorParam ) {
		m_actualResult = result;
		return true;
	}
	else
	if( parameterType == MotorParam ) {
		m_actualAction = result;
		return true;
	}
	else
	if( parameterType == ParentIdParam ) {
		m_parentId = result;
		return true;
	}
	return false;
}

bool LbBkr::DeleteParameter( param_t parameterType, id_t paramId )
{
	if( parameterType == DemandParam ) {
		return true;
	}
	else
	if( parameterType == ContextParam ) {
		return true;
	}
	else
	if( parameterType == SensorParam ) {
		return true;
	}
	else
	if( parameterType == MotorParam ) {
		return true;
	}
	else
	if( parameterType == ParentIdParam ) {
		return true;
	}
	return false;
}

result_t LbBkr::GetParameter( param_t parameterType, id_t paramId )
{
	if( parameterType == DemandParam ) {
		return m_actualDemand;
	}
	else
	if( parameterType == ContextParam ) {
		return m_actualContext;
	}
	else
	if( parameterType == SensorParam ) {
		return m_actualResult;
	}
	else
	if( parameterType == MotorParam ) {
		return m_actualAction;
	}
	else
	if( parameterType == ParentIdParam ) {
		return m_parentId;
	}
	return 0;
}

int LbBkr::UpdateMotorAction( LbCore* pCore )
{
	// все входные данные, на текущий момент времени, уже загружены в m_result
	int rt = ResultSearch;
	result_t rResult = abs( m_actualDemand-m_actualResult );
    m_dr.AddResult( m_actualContext, m_actualResult, m_actualDemand );
	if( rResult <= 50 ) {
        pCore->IsArd( m_parentId );
        rt = ResultSuccess;
        return rt;
    }
    if( m_decision.GetCount() == 0 )
        m_dr.SynthesisBranch( m_id, m_actualDemand, m_actualContext, m_actualResult, m_decision );
    else {
        index_t otrInd = m_decision[0];
        m_decision.AtRemove( 0 );
        m_actualAction = m_dr.GetDemand( otrInd );
        rt = ResultSearch;
    }
	return rt;
}

int LbBkr::IsArd()
{
    return 0;
}

bool LbBkr::Load( FILE* in )
{
	bool rt = true;
	if( fread( &m_controlType, sizeof(m_controlType), 1, in ) != 1 )
		rt = false;
	if( fread( &m_id, sizeof(m_id), 1, in ) != 1 )
		rt = false;
	return rt;
}

bool LbBkr::Save( FILE* out )
{
	bool rt = true;
	if( fwrite( &m_controlType, sizeof(m_controlType), 1, out ) != 1 )
		rt = false;
	if( fwrite( &m_id, sizeof(m_id), 1, out ) != 1 )
		rt = false;
	return rt;
}
