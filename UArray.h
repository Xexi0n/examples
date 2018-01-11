/*********************************************
Description: simple array template implementation with hash

//TODO: Add Log and Assert
//TODO: ADD separated allocator and resize policy
//TODO: STL support functions
//TODO: Hash
//TODO: override operator= with std::intilize_list
*/

#ifndef _UARRAY_H_
#define _UARRAY_H_

#undef _STL_USES

//include common types from engine
#include <../Platform.h>

#include <iostream>

template <class Type>
class UArray
{
public:

//default constructor
	UArray(Uint32 capacity = 1, bool hash = false):
		mHash(hash),
		mCapacity(capacity),
		mSize(0),
		mData(nullptr)		
	{
#ifdef _DEBUG
		std::cout << "UArray constructor with pointer: " << this << std::endl;
#endif
		mData = static_cast<Type*>( malloc(mCapacity*sizeof(Type)) );
	};

//Copy constructor
	UArray(const UArray& other):
		mHash(other.mHash),
		mCapacity(other.mCapacity),
		mSize(other.mSize),
		mData(nullptr)	
	{	
#ifdef _DEBUG
		std::cout << "UArray copy constructor with pointer: " << this << " and old pointer: "<< &other << std::endl;
#endif
		mData = static_cast<Type*>(malloc(mCapacity * sizeof(Type)));
		copyFrom(other.mData, other.mSize);
	}

//Destructor
	~UArray()
	{
#ifdef _DEBUG
		std::cout << "UArray destructor with pointer: " << this << std::endl;
#endif
		if(mData)
			free(mData);
	};

//access function's to raw data 
	const Type* data() const			{ return mData; };
	Type* data()						{ return mData; };

	const Uint32 size() const			{ return mSize; }; //return current array size
	const Uint32 capacity() const		{ return mCapacity;	}; //return current array capacity

	bool empty() const					{ return mSize == 0; };

//"clear" function does not delete/clear current data (if exist), 
//it only set current size by 0
	void clear()						{ mSize = 0; };

//simple iterators
	Type* begin() const					{ return mData; };
	Type* end() const					{ return (mData+mSize); };

/*Public interface*/

//insert object in array if it possible, else try to allocate memory
	void push_back(const Type& insert)
	{
		if (mCapacity >= mSize + 1)
		{
			mData[mSize] = insert;
			mSize++;
		}
		else
		{
			if(ResizeToGrow(1.33f))	       // magic const, should be inside resize policy class
			{
				mData[mSize] = insert;
				mSize++;
			}
			else
			{
				//Log:: Cant insert object
			};
		};
	};

//reserve memory for "size" elements
	bool reserve(Uint32 size)
	{
		return ResizeTo(size);
	};

//release excess memory. New memory capacity is equal array size
	void Shrink()
	{
		if (mCapacity - mSize) //not safe?
		{
			if (!empty())
			{
				ResizeTo(mSize);
			}
			else
			{
				ResizeTo(0);
				mSize = 0;
			}
		}
	};

	UArray& operator=(const UArray& other)
	{
		mHash = other.mHash; //hehash if need
		mSize = other.mSize;
		ResizeTo(other.capacity());
		copyFrom(other.mData, other.mSize); // TODO: Fix mdata Pointer

		return *this;
	}
	Type& operator[](const int index)
	{
		//TODO:: change assert message
		//assert(index >= 0 && index < mSize);
		return *(mData+index);
	}
	const Type& operator[](const int index) const
	{
		//TODO:: change assert message
		//assert(index >= 0 && index < mSize);
		return *(mData + index);
	}

private:
	Uint32 RangeCheck() const			{ return mCapacity - mSize; };
	
	bool ResizeTo(const Uint32 size)
	{
		//assert(size >= 1);

		Type*  TempPtr = static_cast<Type*>(realloc(mData, sizeof(Type)*size));
		if (TempPtr)
		{
			mCapacity = size;
			mData = TempPtr;
		}
		else
		{
			std::cout << "ERRor" << std::endl;
		}

		if (mCapacity < mSize)
			mSize = mCapacity;

		return true;
	};
	bool ResizeToGrow(const float rate)
	{
		int size(rate*mCapacity + 1);
		//assert(size >= 1);

		Type* TempPtr = static_cast<Type*>(realloc(mData, sizeof(Type)*size));
		if (TempPtr)
		{
			mCapacity = size;
			mData = TempPtr;
		}
		else
		{
			std::cout << "ERRor" << std::endl;
		}
		return true;
	};

	void copyTo(const Type* dst, const Uint32 size)
	{
		for (int i = 0; i < size; ++i)
			*(dst + i) = mData[i];
	}
	const void copyTo(Type* const dst, const Uint32 size) const
	{
		for (int i = 0; i < size; ++i)
			*(dst + i) = mData[i];
	}
	const void copyFrom(const Type* src, const Uint32 size) const
	{
		for (Uint32 i = 0; i < size; ++i)
			*(mData + i) = *(src + i);
	}

#ifdef _STL_USES_
public:
	typedef Type value_type;
	typedef size_t size_type;
	typedef Type& reference;
	typedef const Type& const_reference;

	void pop_back()
	{
		mSize--;
	};
	Type& front()
	{
		if (mData)
			return (*mData);
	};
#endif

////////////////////////////////////////////////////////////////
//							MEMBER							  //
////////////////////////////////////////////////////////////////
private:
	bool mHash;
	Uint32 mCapacity;
	Uint32 mSize;

	Type* mData;
	//Type[64] mStackData; //for small and faster array
};

#endif

