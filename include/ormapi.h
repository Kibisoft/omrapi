#ifndef ORMAPI
#define ORMAPI

#define _ORMAPI_MEMORY_MAP

const size_t undefined_id = -1;

class oObject;
template <class T> class orm_ptr;

class oBase
{
public:
	virtual size_t open() = 0;
	virtual size_t close() = 0;
	virtual size_t store(const oObject& object) const = 0;
};

class oObject
{
private:
	size_t _orm_id;
	oBase* _base;
public:
	oObject(oBase* b) : _base(b), _orm_id(undefined_id) {}
	inline oBase* base() const { return _base; }
};

template <class T> class orm_ptr
{
private:
	template <class T> class orm_ptr_counter
	{
	public:
		size_t counter;
		T* object;
	public:
		inline orm_ptr_counter(T* o) : counter(1), object(o) {}
		inline ~orm_ptr_counter() { delete object; }
	};

	orm_ptr_counter<T>* ptr;
public:
	inline orm_ptr(T* o) {
		static_assert(std::is_base_of<oObject, T>::value, "orm_ptr constructor needs oObject as parameter");
		ptr = new orm_ptr_counter<T>(o);
	}
	inline orm_ptr(const orm_ptr<T>& o) {
		ptr = o.ptr;
		ptr->counter++;
	}
	inline orm_ptr(orm_ptr<T>&& o) {
		ptr = o.ptr;
		o.ptr = nullptr;
	}
	inline ~orm_ptr() {
		delete_orm_ptr_counter();
	}
	inline const orm_ptr<T>& operator=(T* o) {
		delete_orm_ptr_counter();
		ptr = new orm_ptr_counter<T>(o);
		return *this;
	}
	inline const orm_ptr<T>& operator=(const orm_ptr<T>& o) {
		delete_orm_ptr_counter();
		ptr = o.ptr;
		ptr->counter++;
		return *this;
	}
	inline const orm_ptr<T>& operator=(orm_ptr<T>&& o) {
		delete_orm_ptr_counter();
		ptr = o.ptr;
		o.ptr = nullptr;
		return *this;
	}

	inline T* operator->() const { return ptr->object; }

	inline size_t store() const { return ptr->object->base()->store(*ptr->object); }

private:
	inline void delete_orm_ptr_counter() {
		if (ptr)
		{
			--ptr->counter;
			if (ptr->counter == 0)
				delete ptr;
		}
	}
};

#ifdef _ORMAPI_MEMORY_MAP

#include <map>

class oBaseMemoryMap : public oBase
{
private:
	std::map<size_t, orm_ptr<oObject>> map;
public:
	virtual size_t open() { return 1; }
	virtual size_t close() { map.clear(); return 1; }
	virtual size_t store(const oObject& object) const {
		return 1;
	}
};

#endif

#endif
