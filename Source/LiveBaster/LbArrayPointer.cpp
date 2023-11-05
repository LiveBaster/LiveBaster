// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbArrayPointer.cpp                                            //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#include "LbArrayPointer.h"

//--- Class LbLinkPointer -------------------------------------------------------

LbLinkPointer::~LbLinkPointer()
{
}

int LbLinkPointer::Compare( const LbLinkPointer* )
{
 return 0;
}

LbLinkPointer* LbLinkPointer::Duplicate() const
{
 return (new LbLinkPointer())->Copy( this );
}

LbLinkPointer* LbLinkPointer::Copy( const LbLinkPointer* )
{
 return this;
}

//--- Class LbArrayPointer -------------------------------------------------------

LbArrayPointer::~LbArrayPointer()
{
 Free();
}

int LbArrayPointer::Compare( const LbLinkPointer* poi )
{
	LbArrayPointer* p = (LbArrayPointer*)poi;
	if( GetCount() != p->GetCount() )
		return -1;
	for( index_t i=0; i<GetCount(); i++ ) {
		if( (*this)[i].Compare( &(*p)[i] ) )
			return -1;
	}
	return 0;
}

LbLinkPointer* LbArrayPointer::Duplicate() const
{
 return (new LbArrayPointer())->Copy( this );
}

LbLinkPointer* LbArrayPointer::Copy( const LbLinkPointer* poi )
{
 Free();
 LbArrayPointer* p = (LbArrayPointer*)poi;
 flags = p->flags;
 LbLinkPointer** lst = p->list;
 if( lst ) {
  index_t cnt = p->count;
  SetMaxCount( cnt );
  if( (flags & flpBrainFreeHead) == 0 ) {
   for( index_t i=0; i<cnt; i++ )
    AtInsert( i, (*lst++)->Duplicate() );
  }
  else {
   for( index_t i=0; i<cnt; i++ )
    AtInsert( i, *lst++ );
  }
 }
 return this;
}

void LbArrayPointer::SetMaxCount( index_t count )
{
 if( count <= m_maxCount )
  return;
 LbLinkPointer** p = list;
 if( p == 0 ) {
//  HeapCreate();
//  p = (LbLinkPointer**)HeapAlloc( m_heap, HEAP_ZERO_MEMORY, DWORD(count*sizeof( LbLinkPointer* )) );
  p = (LbLinkPointer**)::malloc( size_t(count*sizeof( LbLinkPointer* )) );
 }
 else
//  p = (LbLinkPointer**)HeapReAlloc( m_heap, HEAP_ZERO_MEMORY, p, DWORD(count*sizeof( LbLinkPointer* )) );
  p = (LbLinkPointer**)::realloc( p, size_t(count*sizeof( LbLinkPointer* )) );
// if( p == 0 )
//  AfxThrowMemoryException();
 list = p;
 m_maxCount = count;
}

void LbArrayPointer::AtInsert( index_t index, LbLinkPointer* aItem )
{
 #ifdef _DEBUG
// if( index > count )
//  AfxMessageBox( _T("AtInsert") );
 #endif
 if( count == m_maxCount )
  SetMaxCount( m_maxCount+count/2+BrainAddCountArrayPointer );
 LbLinkPointer** poi = list+index;
 if( index < count )
  memmove( poi+1, poi, size_t(count-index)*sizeof( LbLinkPointer* ) );
 count ++;
 *poi = aItem;
}

LbLinkPointer* LbArrayPointer::Remove( LbLinkPointer* p )
// удалить элемент списка (без освобождения памяти)
{
 index_t ind;
 if( Search( p, ind ) ) {
  if( &(*this)[ ind ] == p )
   return AtRemove( ind );
 }
 for( index_t i=ind+1; i<count; i++ ) {
  LbLinkPointer* pp = &(*this)[i];
  if( pp->Compare( p ) != 0 )
   break;
  if( pp == p )
   return AtRemove( i );
 }
 for( index_t i=ind-1; i>=0; i-- ) {
  LbLinkPointer* pp = &(*this)[i];
  if( pp->Compare( p ) != 0 )
   break;
  if( pp == p )
   return AtRemove( i );
 }
 return 0;
}

const LbLinkPointer* LbArrayPointer::Prev( const LbLinkPointer* p ) const
{
 LbLinkPointer** lst = list;
 for( index_t i=0; i<count; i++ )
  if( *(lst+i) == p )
   return i>0 ? *(lst+i-1) : 0;
 return 0;
}

LbLinkPointer* LbArrayPointer::Prev( LbLinkPointer* p )
{
 LbLinkPointer** lst = list;
 for( index_t i=0; i<count; i++ )
  if( *(lst+i) == p )
   return i>0 ? *(lst+i-1) : 0;
 return 0;
}

const LbLinkPointer* LbArrayPointer::Next( const LbLinkPointer* p ) const
{
 LbLinkPointer** lst = list;
 for( index_t i=0; i<count; i++ )
  if( *(lst+i) == p )
   return i<count-1 ? *(lst+i+1) : 0;
 return 0;
}

LbLinkPointer* LbArrayPointer::Next( LbLinkPointer* p )
{
 LbLinkPointer** lst = list;
 for( index_t i=0; i<count; i++ )
  if( *(lst+i) == p )
   return i<count-1 ? *(lst+i+1) : 0;
 return 0;
}

bool LbArrayPointer::Search( const LbLinkPointer* aItem, index_t& index, index_t beginIndex ) const
{
 register index_t i;
 LbLinkPointer** lst = list;
 index_t l = beginIndex;
 index_t h = count-1;
 while( l <= h ) {
  register int c;
  if( (c = (*(lst+(i = ((l+h)>>1))))->Compare( aItem )) < 0 )
   l = i+1;
  else {
   h = i-1;
   if( c == 0 ) {
    index = i;
    return true;
   }
  }
 }
 index = l;
 return false;
}

index_t LbArrayPointer::GetIndex( const LbLinkPointer* aItem ) const
{
 LbLinkPointer** lst = list;
 for( index_t i=0; i<count; i++ )
  if( *(lst+i) == aItem )
   return i;
 return index_t( -1 );
}
