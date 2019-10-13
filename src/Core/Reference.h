#include <NullPointerException.h>
#include <typeinfo>
#include <iostream>
#ifndef MEOW_GARBAGE_COLLETION
//	#define MEOW_GARBAGE_COLLECTION 1
#endif
#ifdef MEOW_GARBAGE_COLLECTION
	#include <Kernel/Debug.h>
#endif
#include <assert.h>				

#ifndef NULL
  #define NULL 0x00000000
#endif


template <class T> class Reference
{
//	Constructors and destructor

public:
  Reference() : _ref(NULL),_noDelete(false) {}
  Reference(T* ptr_) : _ref(NULL) { _ref=ptr_; if ( _ref!=NULL) IncrRefCount();}
  Reference(void * ptr_) : _ref(NULL) 
  { 
     _ref=(T*)ptr_; 
     if ( _ref!=NULL) IncrRefCount();
  }
  Reference( const Reference &other )
  {
    _ref=NULL;
    if ( other._ref!=NULL )
    {
      _ref=other._ref;
    //  _noDelete=other._noDelete;
      IncrRefCount();
    }
  }

  virtual ~Reference()
  {
    if ( _ref!=NULL )
      DecrRefCount();
  }
  Reference & operator = ( Reference ptr_ )
  {
    if ( _ref  != NULL )
      DecrRefCount( );

    _ref=ptr_._ref;
    if ( _ref!=NULL )
      IncrRefCount();
    return *this;
  }
  Reference & operator = (T* ptr_)
  {
    if ( _ref  != NULL )
      DecrRefCount( );
    _ref=ptr_;
    if ( _ref!=NULL )
      IncrRefCount();
    return *this;
  }
  T* operator -> () 
  {
//    if ( _ref==NULL )
//    	throw new NullPointerException();    	
    return _ref; 
  }
  T& operator * () { 
    if ( _ref==NULL )
    	throw new NullPointerException();
    return *_ref; 
  }
  operator T* () { 
//    if ( _ref==NULL )
//    	throw new NullPointerException();
    return _ref;
  }

  bool operator == (Reference<T> ptr) { return _ref == ptr._ref;}
  bool operator != (Reference<T> ptr) { return _ref != ptr._ref;}
  bool operator == (T* ptr_) { return _ref == ptr_;}
  bool operator != (T* ptr_) { return _ref != ptr_;}

// Mark this ref as a in constructor ref
  void InConstructor()
  {
      _noDelete=true;
  }

// Shallow copy cloning, to deep copy clone override
// the clone method on Object
  static T* clone( Reference<T> ptr )
  {
    T* retval = new T();
    retval->clone(ptr._ref);
    return retval;
  }
  bool const IsNull() { return _ref == NULL; }
  long const getRefCount() { return _ref->getrefcount(); }
  T* getPointer() { return _ref; }
  T* Cast() {  return _ref; }
  T* getPointerForceIncref( ) { IncrRefCount(); return _ref; }

private:
  void IncrRefCount()
  {
    if ( _ref!=NULL )
    {
      // _ref->__lock();

      _ref->Incref( );
      // _ref->__unlock();

    }
  }
  void DecrRefCount()
  {
    if ( _ref!=NULL )
    {
//      _ref->__lock();
      _ref->Decref( );
      
      if ( _ref->GetRefCount() == 0 )
      {
        if ( !_noDelete )
		    delete _ref;
      }
//      else
//         _ref->__unlock();
      _ref = NULL;
    }
  }
private:


  bool _noDelete;
  T* _ref;
};
