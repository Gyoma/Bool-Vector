#pragma once
#include <string>
#include <iostream>

#define byte unsigned char

class BoolVector
{
public:

	BoolVector(int length = 0);
	BoolVector(const char* vector, int requiredSize = -1);
	BoolVector(const BoolVector& object, int requiredSize = -1);

	BoolVector& operator = (const BoolVector& object);
	BoolVector operator&(const BoolVector& obj2);
	BoolVector& operator&=(const BoolVector& object);
	BoolVector operator|(const BoolVector& obj2);
	BoolVector& operator|=(const BoolVector& object);
	BoolVector operator^(const BoolVector& obj2);
	BoolVector& operator^=(const BoolVector& object);
	BoolVector operator~();
	BoolVector& operator<<=(unsigned int k);
	BoolVector& operator>>=(unsigned int k);
	BoolVector operator<<(unsigned int k);
	BoolVector operator>>(unsigned int k);
	int operator[](int ind) const;

	void setOne(int ind);

	void setZero(int ind);

	void setOneFrom(int ind, int count);

	void setZeroFrom(int ind, int count);

	void invert(int ind);

	void invertFrom(int ind, int count);

	void extend(int size);

	int weight() const;

	int size() const;

	//void setVector(const char* vector);

	void printVector();

	~BoolVector();

	friend std::istream& operator>>(std::istream& os, BoolVector vector);
	friend std::ostream& operator<<(std::ostream& os, BoolVector vector);

protected:
	byte * bv;
	int len_bv, len_mas;
};