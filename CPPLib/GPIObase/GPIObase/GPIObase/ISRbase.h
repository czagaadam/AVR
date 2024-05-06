/*
 * ISRbase.h
 *
 * Created: 2024. 04. 30. 10:51:19
 *  Author: acer1
 */ 

#ifndef ISRBASE_H_
#define ISRBASE_H_


//due to it is template file everything need to be defined here
template<class T>
class ISRbase
{
	public:
	static const uint8_t MAX_DIN_COUNT = 24;
	//uint8_t MAX_DIN_COUNT; // Maximum Din elemek száma
	T Storage[MAX_DIN_COUNT]; // A hagyományos tömb, amiben tároljuk a Din elemeket
	uint8_t Count;	
	ISRbase(void)  	{Count = 0;}
	~ISRbase(void)	{}
	//void add(T& obj)
	void add(T* obj)
	{
		if (Count < MAX_DIN_COUNT) {
			Storage[Count++] = *obj;
			} else {
		}
	}
	//void remove(T& obj)
	void remove(T* obj)
	{
		
	}
	T* get(uint8_t index)
	{
		//if (index >= 0 && index < Count) {
			return &Storage[index];
			/*} else {
			return Din();
		}*/
	}
	uint8_t size(void)
	{
		return MAX_DIN_COUNT;
	}

	private:
	//std::vector<T*> ISR_LIST;
};



#endif /* ISRBASE_H_ */