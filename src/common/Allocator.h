/*
 * File:   Allocator.h
 * Author: Zozzz
 *
 * Created on 2011. február 21., 22:37
 */

#include "../settings.h"
#include "Exception.h"

#include <stdlib.h>

#ifndef ALLOCATOR_H
#define	ALLOCATOR_H

#define __ALLOC_ARRAY(var, type, _nsize) {var = (type*) malloc(_nsize); if( var == NULL ) { ex_throw(common::MemoryError::NotEnough, _nsize); }}
#define __RESIZE_ARRAY(var, type, _nsize) {var = (type*) realloc(var, _nsize); if( var == NULL ) { ex_throw(common::MemoryError::NotEnough, _nsize); }}

#define ALLOC_ARRAY(var, type, size) __ALLOC_ARRAY(var, type, sizeof(type) * (size))
#define RESIZE_ARRAY(var, type, size) __RESIZE_ARRAY(var, type, sizeof(type) * (size))
#define FREE_ARRAY(var) free(var)

namespace common
{
	template<typename _Node>
	class Allocator
	{
	public:
		typedef _Node  Node;
		typedef _Node* NodePtr;
		typedef _Node& NodeRef;
		typedef Allocator<Node> _self;

		Allocator(size_t poolSize);
		Allocator(const _self& orig);
		~Allocator();

		inline size_t nodeSize(){ return _nodeSize; };
		inline size_t freeSpace(){ return _freeSpace; };
		inline size_t currentSize(){ return _currentSize; };
		inline NodePtr first(){ return &_storage[0]; };
		inline NodePtr last(){ return _last; };

		NodePtr allocate(size_t size);
		NodePtr resize(size_t newSize);
		NodePtr slice(size_t start, size_t end);
		NodePtr append(NodePtr start, size_t size);
		void release();

	private:
		NodePtr _storage;
		NodePtr _last;
		size_t _nodeSize;
		size_t _currentSize;
		size_t _freeSpace;
	};


	//------------------------------------------------------------------------------
	// Implement Allocator
	//------------------------------------------------------------------------------

	template<typename _Node>
	Allocator<_Node>::Allocator(size_t poolSize):
		_nodeSize(sizeof(Node)),
		_freeSpace(poolSize),
		_currentSize(poolSize)
	{
		__ALLOC_ARRAY(_storage, Node, _nodeSize * poolSize);
		_last = &_storage[0];
	}

	template<typename _Node>
	Allocator<_Node>::Allocator(const _self& orig)
	{
		ex_throw(Exception, "Copy not allowed!");
	}

	template<typename _Node>
	Allocator<_Node>::~Allocator()
	{
		release();
	}

	template<typename _Node>
	_Node* Allocator<_Node>::allocate(size_t size)
	{
		if( _freeSpace < size )
			resize((_currentSize - _freeSpace) + size);

		_freeSpace -= size;

		NodePtr ret = _last;
		_last += size;
		return ret;
	}

	template<typename _Node>
	_Node* Allocator<_Node>::resize(size_t newSize)
	{
		if( newSize > _currentSize )
		{
			__RESIZE_ARRAY(_storage, Node, _nodeSize * newSize);
			_last = &_storage[_currentSize - _freeSpace];
			_freeSpace = newSize - _currentSize;
			_currentSize = newSize;
		}

		return _last;
	}

	template<typename _Node>
	_Node* Allocator<_Node>::append(NodePtr start, size_t size)
	{
		if( _freeSpace < size )
			resize((_currentSize - _freeSpace) + size);

		NodePtr ret = _last;
		memcpy(_last, start, size);
		_last += size;
		return ret;
	}

	template<typename _Node>
	_Node* Allocator<_Node>::slice(size_t start, size_t end)
	{
		if( start > 0 && end < _currentSize && start < end )
		{
			for(size_t i = start; start < end ; i++ )
			{
				if( _storage[i] != NULL )
					delete _storage[i];
				else
					ex_throwm(ValueError::OutOfRange, "Try to delete unallocated memory at: %d", i);
			}

			size_t cpys = _freeSpace - end;
			memmove(&_storage[start], &_storage[end], cpys);
			_last = _storage[start + cpys];
		}
		else
		{
			if( start > end && end > 0)
				ex_throw(ValueError::OutOfRange, 0, end, start);
			else
				ex_throw(ValueError::OutOfRange, 0, _currentSize, end);
		}

		return _last;
	}

	template<typename _Node>
	void Allocator<_Node>::release()
	{
		FREE_ARRAY(_storage);
	}

}

#endif	/* ALLOCATOR_H */

