// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbArrayTemp.h                                                 //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#if !defined( __LBARRAYTEMP_H )
#define __LBARRAYTEMP_H

#if !defined( __LBLIBRARYDEF_H )
 #include "LbLibraryDef.h"
#endif

const index_t AddMaxCountSimple	= 5;

//--- Class LbArrayTemp -------------------------------------------------------

template <class T>
class LbArrayTemp
{
protected:
	T* m_list;
	index_t m_count;
	index_t m_maxCount;
public:
	LbArrayTemp();
	LbArrayTemp( index_t maxCount );
	LbArrayTemp( const LbArrayTemp<T>& );
	~LbArrayTemp();

	void Free();
	void FullFree();
	void FastFree();
	void FreeHead();											// отсоединение списка от головы

	T& operator []( index_t i );
	T operator []( index_t i ) const;
	LbArrayTemp<T>& operator = ( const LbArrayTemp<T>& );
	template <class T> friend bool operator == ( const LbArrayTemp<T>& aItem1, const LbArrayTemp<T>& aItem2 );
	template <class T> friend bool operator != ( const LbArrayTemp<T>& aItem1, const LbArrayTemp<T>& aItem2 );

	bool IsNull() const;
	index_t GetCount() const;
	void SetMaxCount( index_t count );

	void AtInsert( index_t index, T aItem );
	void InsertStart( T aItem );							// добавить в начало списка
	void InsertEnd( T aItem );								// добавить в конец списка
	index_t Insert( T aItem );								// добавить, используя сортировку

	void AtRemove( index_t index );							// удалить элемент списка (без освобождения памяти)
	bool Remove( T aItem );									// удалить элемент списка (без освобождения памяти)

	bool Search( T aItem, index_t& index ) const;
	index_t GetIndex( const T aItem ) const;

	bool Load( FILE* in );
	bool Save( FILE* out );
};

template <class T>
inline
LbArrayTemp<T>::LbArrayTemp() :
	m_list( 0 )
{
	m_count = m_maxCount = 0;
}

template <class T>
inline
LbArrayTemp<T>::LbArrayTemp( index_t maxCount ) :
	m_list( 0 )
{
	m_count = m_maxCount = 0;
	SetMaxCount( maxCount );
}

template <class T>
inline
LbArrayTemp<T>::LbArrayTemp( const LbArrayTemp<T>& aItem ) :
	m_list( 0 ),
	m_count( 0 ),
	m_maxCount( 0 )
{
	T* lst = aItem.m_list;
	if( lst ) {
		index_t cnt = aItem.m_count;
		for( index_t i=0; i<cnt; i++ )
			AtInsert( i, *lst++ );
	}
}

template <class T>
inline
LbArrayTemp<T>::~LbArrayTemp()
{
	Free();
}

template <class T>
inline
void LbArrayTemp<T>::Free()
{
	if( m_list ) {
		::free( m_list );
		m_list = 0;
	}
	m_count = m_maxCount = 0;
}

template <class T>
inline
void LbArrayTemp<T>::FullFree()
{
	if( m_list ) {
		for( index_t i=m_count-1; i>=0; i-- )
			delete *(m_list+i);
		::free( m_list );
		m_list = 0;
	}
	m_count = m_maxCount = 0;
}

template <class T>
inline
void LbArrayTemp<T>::FastFree()
{
	m_count = 0;
}

template <class T>
inline
void LbArrayTemp<T>::FreeHead()
{
	m_list = 0;
	m_count = m_maxCount = 0;
}

template <class T>
inline
T LbArrayTemp<T>::operator []( index_t i ) const
{
	if( i < 0 || i >= m_count ) {
#ifdef _DEBUG
//		MessageBox( 0, _T("Range"), _T("LbArrayTemp"), MB_OK );
#endif
		return *(m_list+0);
	}
	else
		return *(m_list+i);
}

template <class T>
inline
T& LbArrayTemp<T>::operator []( index_t i )
{
	if( i < 0 || i >= m_count ) {
#ifdef _DEBUG
//		MessageBox( 0, _T("Range"), _T("LbArrayTemp"), MB_OK );
#endif
		return *(m_list+0);
	}
	else
		return *(m_list+i);
}

template <class T>
LbArrayTemp<T>& LbArrayTemp<T>::operator = ( const LbArrayTemp<T>& aItem )
{
	Free();
	T* lst = aItem.m_list;
	if( lst ) {
		index_t cnt = aItem.m_count;
		for( index_t i=0; i<cnt; i++ )
			AtInsert( i, *lst++ );
	}
	return *this;
}

template <class T>
bool operator == ( const LbArrayTemp<T>& aItem1, const LbArrayTemp<T>& aItem2 )
{
	if( aItem1.m_count != aItem2.m_count )
		return false;
	for( index_t i=0; i<aItem1.m_count; i++ ) {
		if( aItem1[i] != aItem2[i] )
			return false;
	}
	return true;
}

template <class T>
bool operator != ( const LbArrayTemp<T>& aItem1, const LbArrayTemp<T>& aItem2 )
{
	if( aItem1.m_count != aItem2.m_count )
		return true;
	for( index_t i=0; i<aItem1.m_count; i++ ) {
		if( aItem1[i] != aItem2[i] )
			return true;
	}
	return false;
}

template <class T>
inline
bool LbArrayTemp<T>::IsNull() const
{
	return m_count ? false : true;
}

template <class T>
inline
index_t LbArrayTemp<T>::GetCount() const
{
	return m_count;
}

template <class T>
void LbArrayTemp<T>::SetMaxCount( index_t count )
{
	if( count <= m_maxCount )
		return;
	T* p;
	if( m_list == 0 ) {
		p = (T*)::malloc( size_t(count*sizeof( T )) );
	}
	else
		p = (T*)::realloc( m_list, size_t(count*sizeof( T )) );
//	if( p == 0 )
//		AfxThrowMemoryException();
	m_list = p;
	memset( p+m_maxCount, 0, size_t(count-m_maxCount)*sizeof( T ) );
	m_maxCount = count;
}

template <class T>
void LbArrayTemp<T>::AtInsert( index_t index, T aItem )
{
	if( m_count == m_maxCount )
		SetMaxCount( m_maxCount+m_count/2+AddMaxCountSimple );
	T* poi = m_list+index;
	if( index < m_count )
		memmove( poi+1, poi, size_t(m_count-index)*sizeof( T ) );
	m_count ++;
	*poi = aItem;
}

template <class T>
inline
void LbArrayTemp<T>::InsertStart( T aItem )
{
	AtInsert( 0, aItem );
}

template <class T>
inline
void LbArrayTemp<T>::InsertEnd( T aItem )
{
	AtInsert( m_count, aItem );
}

template <class T>
inline
index_t LbArrayTemp<T>::Insert( T aItem )
{
	index_t i;
	if( Search( aItem, i ) == false )
		AtInsert( i, aItem );
	return i;
}

template <class T>
inline
void LbArrayTemp<T>::AtRemove( index_t ind )
{
	if( m_count > 0 && ind >= 0 && ind < m_count ) {
		m_count-- ;
		memmove( m_list+ind, m_list+ind+1, size_t((m_count-ind)*sizeof( T )) );
	}
}

template <class T>
bool LbArrayTemp<T>::Remove( T aItem )
{
	T* lst = m_list;
	for( index_t i=0; i<m_count; i++ )
		if( *(lst++) == aItem ) {
			AtRemove( i );
			return true;
		}
	return false;
}

template <class T>
bool LbArrayTemp<T>::Search( T aItem, index_t& index/*, index_t beginIndex*/ ) const
{
	register index_t i;
	register T item;
	T* lst = m_list;
	index_t l = 0;
	index_t h = m_count-1;
	while( l <= h ) {
		if( (item = *(lst+(i = (l+h)>>1))) < aItem )
			l = i+1;
		else {
			h = i-1;
			if( item == aItem ) {
				index = i;
				return true;
			}
		}
	}
	index = l;
	return false;
}

template <class T>
index_t LbArrayTemp<T>::GetIndex( const T aItem ) const
{
	T* lst = m_list;
	for( index_t i=0; i<m_count; i++ )
		if( *(lst+i) == aItem )
			return i;
	return index_t( -1 );
}

template <class T>
bool LbArrayTemp<T>::Load( FILE* in )
{
	FastFree();
	bool rt = true;
	index_t cnt = 0;
	if( fread( &cnt, sizeof(cnt), 1, in ) != 1 )
		rt = false;
	SetMaxCount( cnt );
	for( index_t i=0; i<cnt; i++ ) {
		T p;
		if( fread( &p, sizeof(T), 1, in ) != 1 )
			rt = false;
		InsertEnd( p );
	}
	return rt;
}

template <class T>
bool LbArrayTemp<T>::Save( FILE* out )
{
	bool rt = true;
	if( fwrite( &m_count, sizeof(m_count), 1, out ) != 1 )
		rt = false;
	for( index_t i=0; i<m_count; i++ ) {
		if( fwrite( &(*this)[i], sizeof(T), 1, out ) != 1 )
			rt = false;
	}
	return rt;
}

#endif
