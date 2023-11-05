// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbArrayPointer.h                                              //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#if !defined( __LBARRAYPOINTER_H )
#define __LBARRAYPOINTER_H

#if !defined( __LBLIBRARYDEF_H )
	#include "LbLibraryDef.h"
#endif
#include <stdio.h>

const index_t BrainAddCountArrayPointer	= 5;

//--- Class LbLinkPointer -------------------------------------------------------

class LIVEBASTERSHARED_EXPORT LbLinkPointer
{
public:
	LbLinkPointer() {}
	virtual ~LbLinkPointer();

	virtual int Compare( const LbLinkPointer* poi );
	virtual LbLinkPointer* Duplicate() const;
	virtual LbLinkPointer* Copy( const LbLinkPointer* poi );
};

//--- Class LbArrayPointer ------------------------------------------------------

const char
    flpBrainDuplicates	= 0x01,				// позволяет иметь одинаковые элементы
    flpBrainFreeHead	= 0x02;				// освобождает только голову списка

class LIVEBASTERSHARED_EXPORT LbArrayPointer : public LbLinkPointer
{
public:
	LbLinkPointer** list;
	index_t count;						// размер списка
	index_t m_maxCount;					// максимальный размер списка
	char flags;							// == 1, то список позволяет иметь одинаковые элементы
public:
#ifdef _DEBUG
	static int maxCountSearch;
#endif
public:
	LbArrayPointer();
	LbArrayPointer( char fl );
	virtual ~LbArrayPointer();

	void Free();
	void FastFree();
	void FreeHead();						// освободить список без удаления элементов

	virtual int Compare( const LbLinkPointer* poi );
	virtual LbLinkPointer* Duplicate() const;
	virtual LbLinkPointer* Copy( const LbLinkPointer* poi );

	const LbLinkPointer& operator []( index_t i ) const;
	LbLinkPointer& operator []( index_t i );

	void SetDuplicates( char fl = true );
	void SetFreeHead( char fl = true );
	bool IsNull() const;
	index_t GetCount() const;
	void SetMaxCount( index_t count );

	void AtInsert( index_t index, LbLinkPointer* p );
	index_t InsertStart( LbLinkPointer* p );					// добавить в начало списка
	index_t InsertEnd( LbLinkPointer* p );						// добавить в конец списка
	index_t Insert( LbLinkPointer* p );							// добавить, используя сортировку

	LbLinkPointer* AtRemove( index_t index );					// удалить элемент списка (без освобождения памяти)
	LbLinkPointer* Remove( LbLinkPointer* p );					// удалить элемент списка (без освобождения памяти)

	const LbLinkPointer*	GetLast() const;
	LbLinkPointer*			GetLast();
	const LbLinkPointer*	GetFirst() const;
	LbLinkPointer*			GetFirst();
	const LbLinkPointer*	Prev( const LbLinkPointer* p ) const;
	LbLinkPointer*			Prev( LbLinkPointer* p );
	const LbLinkPointer*	Next( const LbLinkPointer* p ) const;
	LbLinkPointer*			Next( LbLinkPointer* p );
	bool					Search( const LbLinkPointer* aItem, index_t& index, index_t beginIndex = 0 ) const;
	index_t				GetIndex( const LbLinkPointer* aItem ) const;
protected:
	LbArrayPointer( const LbArrayPointer& ) {}
	LbArrayPointer& operator = ( const LbArrayPointer& ) { return *this; }
};

inline
LbArrayPointer::LbArrayPointer() :
	list( 0 ),
	count( 0 ),
	m_maxCount( 0 ),
	flags( 0 )
{
}

inline
LbArrayPointer::LbArrayPointer( char fl ) :
	list( 0 ),
	count( 0 ),
	m_maxCount( 0 ),
	flags( (char)fl )
{
}

inline
void LbArrayPointer::Free()
{
	if( list != 0 && (flags & flpBrainFreeHead) == 0 )
		for( index_t i=count-1; i>=0; i-- )
			delete *(list+i);
	FreeHead();
}

inline
void LbArrayPointer::FastFree()
{
	if( list && (flags & flpBrainFreeHead) == 0 )
		for( index_t i=count-1; i>=0; i-- )
			delete *(list+i);
	count = 0;
}

inline
void LbArrayPointer::FreeHead()
{
	if( list != 0 )
		::free( list );
	list = 0;
	count = 0;
	m_maxCount = 0;
}

inline
const LbLinkPointer& LbArrayPointer::operator []( index_t i ) const
{
	if( i < 0 || i >= count ) {
#ifdef _DEBUG
//		MessageBox( 0, _T("Range"), _T("LbArrayPointer"), MB_OK );
#endif
		return **(list+0);
	}
	else
		return **(list+i);
}

inline
LbLinkPointer& LbArrayPointer::operator []( index_t i )
{
	if( i < 0 || i >= count ) {
#ifdef _DEBUG
//		MessageBox( 0, _T("Range"), _T("LbArrayPointer"), MB_OK );
#endif
		return **(list+0);
	}
	else
		return **(list+i);
}

inline
void LbArrayPointer::SetDuplicates( char fl )
{
	if( fl )
		flags |= flpBrainDuplicates;
	else
		flags &= ~flpBrainDuplicates;
}

inline
void LbArrayPointer::SetFreeHead( char fl )
{
	if( fl )
		flags |= flpBrainFreeHead;
	else
		flags &= ~flpBrainFreeHead;
}

inline
bool LbArrayPointer::IsNull() const
{
	return count ? false : true;
}

inline
index_t LbArrayPointer::GetCount() const
{
	return count;
}

inline
index_t LbArrayPointer::InsertStart( LbLinkPointer* p )
{
	AtInsert( 0, p );
	return 0;
}

inline
index_t LbArrayPointer::InsertEnd( LbLinkPointer* p )
{
	AtInsert( count, p );
	return count-1;
}

inline
index_t LbArrayPointer::Insert( LbLinkPointer* p )
// добавить, используя сортировку
{
	index_t i;
	if( Search( p, i ) == 0 || (flags & flpBrainDuplicates) )
		AtInsert( i, p );
	else
		if( (flags & flpBrainFreeHead) == 0 )
			delete p;
	return i;
}

inline
LbLinkPointer* LbArrayPointer::AtRemove( index_t index )
// удалить элемент списка (без освобождения памяти)
{
#ifdef _DEBUG
//	if( index >= count )
//		MessageBox( 0, _T("AtRemove"), _T("LbArrayPointer"), MB_OK );
#endif
	LbLinkPointer* p = *(list+index);
	count --;
	if( count > 0 )
		memmove( list+index, list+index+1, size_t(count-index)*sizeof( LbLinkPointer* ) );
	return p;
}

inline
const LbLinkPointer* LbArrayPointer::GetLast() const
{
	return count ? *(list+count-1): 0;
}

inline
LbLinkPointer* LbArrayPointer::GetLast()
{
	return count ? *(list+count-1): 0;
}

inline
const LbLinkPointer* LbArrayPointer::GetFirst() const
{
	return count ? *list : 0;
}

inline
LbLinkPointer* LbArrayPointer::GetFirst()
{
	return count ? *list : 0;
}

//--- Class mPointerTemp -------------------------------------------------------

template <class T>
class LbPointerTemp : public LbArrayPointer
{
public:
	LbPointerTemp() : LbArrayPointer() {}
	LbPointerTemp( char fl ) : LbArrayPointer( fl ) {}

	const T&		operator []( index_t i ) const { return (const T&)LbArrayPointer::operator []( i ); }
	T&			operator []( index_t i ) { return (T&)LbArrayPointer::operator []( i ); }

	T*			AtRemove( index_t index ) { return (T*)LbArrayPointer::AtRemove( index ); }// удалить элемент списка (без освобождения памяти)
	T*			Remove( T* p ) { return (T*)LbArrayPointer::Remove( (LbLinkPointer*)p ); }	// удалить элемент списка (без освобождения памяти)

	const T*		GetLast() const { return GetCount() ? &(*this)[ GetCount()-1 ]: 0; }
	T*			GetLast() { return (T*)(GetCount() ? &(*this)[ GetCount()-1 ]: 0); }
	const T*		GetFirst() const { return (const T*)(GetCount() ? &(*this)[ 0 ] : 0); }
	T*			GetFirst() { return (T*)(GetCount() ? &(*this)[ 0 ] : 0); }
	const T*		Prev( const T* p ) const { return (const T*)LbArrayPointer::Prev( (const LbLinkPointer*)p ); }
	T*			Prev( T* p ) { return (T*)LbArrayPointer::Prev( (LbLinkPointer*)p ); }
	const T*		Next( const T* p ) const { return (const T*)LbArrayPointer::Next( (const LbLinkPointer*)p ); }
	T*			Next( T* p ) { return (T*)LbArrayPointer::Next( (LbLinkPointer*)p ); }
	index_t GetIndex( const T* aItem ) const { return LbArrayPointer::GetIndex( (const LbLinkPointer*)aItem ); }

	bool Load( FILE* in );
	bool Save( FILE* out );

protected:
	LbPointerTemp( const LbPointerTemp& ) {}
	LbPointerTemp& operator = ( const LbPointerTemp& ) { return *this; }
};

template <class T>
bool LbPointerTemp<T>::Load( FILE* in )
{
	FastFree();
	bool rt = true;
	index_t cnt = 0;
	flags = 0;
	if( fread( &cnt, sizeof(cnt), 1, in ) != 1 )
		rt = false;
	if( fread( &flags, sizeof(flags), 1, in ) != 1 )
		rt = false;
	SetMaxCount( cnt );
	for( index_t i=0; i<cnt; i++ ) {
		T* p = new T();
		if( !p->Load( in ) )
			rt = false;
		InsertEnd( p );
	}
	return rt;
}

template <class T>
bool LbPointerTemp<T>::Save( FILE* out )
{
	bool rt = true;
	if( fwrite( &count, sizeof(count), 1, out ) != 1 )
		rt = false;
	if( fwrite( &flags, sizeof(flags), 1, out ) != 1 )
		rt = false;
	for( index_t i=0; i<count; i++ ) {
		if( !(*this)[i].Save( out ) )
			rt = false;
	}
	return rt;
}

#endif
