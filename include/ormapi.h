#ifndef ORMAPI
#define ORMAPI

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

#endif
