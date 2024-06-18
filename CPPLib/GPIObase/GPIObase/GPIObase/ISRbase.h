/*
 * ISRbase.h
 *
 *  Author: Adam Czaga czagaadam@gmail.com
 *
 *  A simple implementation of a generic storage class instead of std::vector<T*>
 *
 */ 

//#todo: move out ISRbase from GPIObase folder

#ifndef ISRBASE_H_
#define ISRBASE_H_

//Due to it is template file everything need to be defined here
template<class T>
class ISRbase
{
	public:	
	ISRbase(void)  	{Count = 0;}
	~ISRbase(void)	{
		//#todo:
	}
	//T * needed because passing "this" special pointer to function
	void add(T * const & obj)
	{
		if (Count < MAX_DIN_COUNT) Storage[Count++] = obj;
		//#todo: else
	}
	//void remove(T& obj)
	void remove(T* obj)
	{
		//#todo: 
	}
	T* get(uint8_t index)
	{
		if (index >= 0 && index < Count)	return Storage[index];
		//#todo: else
	}
	uint8_t get_size(void)
	{
		return Count;
	}
	private:
	uint8_t Count;
	static const uint8_t MAX_DIN_COUNT = 24;
	T* Storage[MAX_DIN_COUNT];
};

#endif /* ISRBASE_H_ */