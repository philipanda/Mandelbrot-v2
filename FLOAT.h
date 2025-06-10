#include <gmp.h>
struct FLOAT
{
	mpf_t val;
	FLOAT() :FLOAT(0) {};
	FLOAT(double i) { mpf_init_set_d(val, i); }
	FLOAT(int i) { mpf_init_set_ui(val, i); }
	void operator=(int i) { mpf_set_si(val, i); }
	void operator=(double i) { mpf_set_d(val, i); }
	void operator=(FLOAT& i) { mpf_set(val, i.val); }
	FLOAT& operator*(FLOAT& i) { mpf_mul(val, val, i.val); return *this; }
	FLOAT& operator+(FLOAT& i) { mpf_add(val, val, i.val); return *this; }
	FLOAT& operator-(FLOAT& i) { mpf_sub(val, val, i.val); return *this; }
	FLOAT& operator/(FLOAT& i) { mpf_div(val, val, i.val); return *this; }
	FLOAT& operator++(int) { mpf_add(val, val, FLOAT(1).val); }
	void operator *=(FLOAT& i) { (*this) * i; }
	void operator +=(FLOAT& i) { (*this) + i; }
	void operator -=(FLOAT& i) { (*this) - i; }
	operator double()
	{
		return mpf_get_d(val);
	}
	~FLOAT()
	{
		mpf_clear(val);
	}


};