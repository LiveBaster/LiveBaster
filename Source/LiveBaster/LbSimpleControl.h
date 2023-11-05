// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbSimpleControl.h                                             //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#if !defined( __LBSIMPLECONTROL_H )
#define __LBSIMPLECONTROL_H

#include "LbLibraryDef.h"
#include "LbArrayPointer.h"
#include "LbArrayTemp.h"
#include <time.h>
#include <memory>
#include <list>

//--- Class LbOtr --------------------------------------------------------------

class LbCore;
class LbOtr;
typedef std::shared_ptr<LbOtr> LbOtrPtr;

class LbOtr
// образ требуемого результата
{
protected:
	// идентификатор устройства, с которого пришло требование результата
	id_t m_idControl;
	// контекст, в котором получен результат m_result
	context_t m_context;
	// результат (состояние датчика), достигнутый посредством исполнения требования m_demand
	result_t m_result;
    // требование результата
    result_t m_demand;
    // подтверждение выполнения требования от дочернего ОТР
    bool m_demandDone;
    // следующие варианты требуемых результатов
	std::list<LbOtrPtr> m_nextOtrList;
public:
    LbOtr();
	LbOtr( id_t idControl, context_t context, result_t result, result_t demand );
    ~LbOtr();

	id_t GetIdControl() const { return m_idControl; }
	void SetIdControl( id_t id ) { m_idControl = id; }

	context_t GetContext() const { return m_context; }
	void SetContext( context_t context ) { m_context = context; }

	result_t GetResult() const { return m_result; }
	void SetResult( result_t result ) { m_result = result; }

    result_t GetDemand() const { return m_demand; }
	void SetDemand( result_t demand ) { m_demand = demand; }

    bool GetDemandDone() const { return m_demandDone; }
	void SetDemandDone( bool done ) { m_demandDone = done; }

    const std::list<LbOtrPtr>& GetNextOtrList() const { return m_nextOtrList; }
	std::list<LbOtrPtr>& GetNextOtrList() { return m_nextOtrList; }
};

//--- Class LbOtrRoot ----------------------------------------------------------

class LbOtrRoot
// корень дерева результатов
{
protected:
    // ограничения
    result_t m_minDemand;
    result_t m_maxDemand;
    // варианты ОТР в абсолютных координатах
    LbArrayTemp<LbOtrPtr> m_axis;
    // текущее направление поиска ОТР
    int m_sign;
    // текущий индекс в m_axis
    index_t m_curIndex;
    // текущий ОТР
    LbOtrPtr m_curOtr;
public:
    LbOtrRoot();
    LbOtrRoot( result_t minDemand, result_t maxDemand );
    ~LbOtrRoot();

    void Init( result_t minDemand, result_t maxDemand );
    // синтез ветки, стремящейся к ОТР
    bool SynthesisBranch( id_t id, result_t demand, context_t context, result_t result, LbArrayTemp<index_t>& decision );
    // запрос текущего требования
    result_t GetDemand( index_t ind );
    // добавим полученный результат
    void AddResult( context_t context, result_t result, result_t demand );
};

//--- Class LbBkr --------------------------------------------------------------

enum
{
	ResultWait = -1,
	ResultSuccess = 0,
	ResultSearch = 1,
	ResultTimeOut = 2
};

const int MaxHistory = 13;

class LbBkr : public LbLinkPointer
// Базовый Компонент Разума
{
protected:
	// тип управляющего компонента
	control_t m_controlType;
	// идентификатор БКР
	id_t m_id;
	// идентификатор родительского БКР
	id_t m_parentId;

	// текущий требуемый результат
    result_t m_actualDemand;
	// текущий контекст
	context_t m_actualContext;
	// текущий достигнутый результат
	result_t m_actualResult;
    // ограничения
    result_t m_minDemand;
    result_t m_maxDemand;
    // ограничения
    result_t m_minContext;
    result_t m_maxContext;
    // ограничения
    result_t m_minResult;
    result_t m_maxResult;
    // ограничения
    result_t m_minAction;
    result_t m_maxAction;
	// текущее выполненное действие
	action_t m_actualAction;
    // дерево результатов
    LbOtrRoot m_dr;
    // текущее решение
    LbArrayTemp<index_t> m_decision;
public:
	LbBkr();
	LbBkr( id_t id, control_t controlType );
	~LbBkr();

	virtual int Compare( const LbLinkPointer* poi );
	virtual LbLinkPointer* Duplicate() const;
	virtual LbLinkPointer* Copy( const LbLinkPointer* poi );

	id_t GetId() const { return m_id; }
	void SetId( id_t id ) { m_id = id; }

	id_t AddParameter( param_t parameterType, result_t minResult, result_t maxResult, result_t result );
	bool ChangeParameter( param_t parameterType, id_t paramId, result_t result );
	bool DeleteParameter( param_t parameterType, id_t paramId );
	result_t GetParameter( param_t parameterType, id_t paramId );

	int UpdateMotorAction( LbCore* pCore );
    int IsArd();

	bool Load( FILE* in );
	bool Save( FILE* out );
protected:
};

#endif
