#include "BoolVector.h"

BoolVector::BoolVector(int length)
{
	len_bv = length;
	len_mas = 0;

	if (length)
	{
		len_mas = (len_bv - 1) / 8 + 1;
		bv = new byte[len_mas]();
	}
	else
		bv = nullptr;
}

BoolVector::BoolVector(const char* vector, int requiredSize)
{
	len_mas = 0;
	int index;
	if (requiredSize == -1)
	{
		len_bv = strlen(vector);
		index = len_bv - 1;
	}
	else
	{
		len_bv = requiredSize;
		index = strlen(vector) - 1;
	}

	if (len_bv)
	{
		len_mas = (len_bv - 1) / 8 + 1;
		bv = new byte[len_mas]();

		int indmas = len_mas - 1;
		byte mask = 1;
		for (int count = len_bv; count > 0; count--, index--, mask <<= 1)
		{
			if (!mask)
			{
				indmas--;
				mask = 1;
			}

			if (vector[index] == '1')
				bv[indmas] |= mask;
		}
	}
	else
		bv = nullptr;
}

BoolVector::BoolVector(const BoolVector& object, int requiredSize)
{
	/*if (this != &object)
	{
		len_mas = object.len_mas;
		len_bv = object.len_bv;

		if (len_mas)
		{
			bv = new byte[len_mas]();

			for (int i = 0; i < len_mas; i++)
				bv[i] = object.bv[i];
		}
		else
			bv = nullptr;
	}*/

	len_bv = requiredSize == -1 ? object.len_bv : requiredSize;

	if (len_bv)
	{
		len_mas = (len_bv - 1) / 8 + 1;
		bv = new byte[len_mas]();

		for (int i = (len_mas < object.len_mas ? len_mas : object.len_mas) - 1; i >= 0; i--)
			bv[i] = object.bv[i];
	}
	else
		bv = nullptr;

}

BoolVector& BoolVector::operator = (const BoolVector& object)
{
	if (this != &object)
	{
		delete[] bv;
		len_bv = object.len_bv;
		len_mas = object.len_mas;
		bv = new byte[len_mas]();

		for (int i = 0; i < len_mas; i++)
			bv[i] = object.bv[i];
	}
	return *this;
}

int BoolVector::operator[](int ind) const
{
	if (ind >= 0 && ind < len_bv)
	{
		int indmas, shift = ind % 8;
		if (shift)
			indmas = len_mas - (ind - 1) / 8 - 1;
		else
			indmas = len_mas - ind / 8 - 1;

		if (bv[indmas] & (1 << shift))
			return 1;
	}
	return 0;
}

BoolVector BoolVector::operator&(const BoolVector& obj2)
{
	BoolVector temp(len_bv < obj2.len_bv ? obj2.len_bv : len_bv);
	for (int i = len_mas - 1, j = obj2.len_mas - 1, ind = temp.len_mas - 1; i >= 0 && j >= 0; ind--, i--, j--)
		temp.bv[ind] = bv[i] & obj2.bv[j];

	return temp;
}

BoolVector& BoolVector::operator&=(const BoolVector& object)
{
	for (int i = len_mas - 1, j = object.len_mas - 1; i >= 0 && j >= 0; i--, j--)
		bv[i] &= object.bv[j];
	for (int i = len_mas - object.len_mas - 1; i >= 0; i--)
		bv[i] = 0;

	return *this;
}

BoolVector BoolVector::operator|(const BoolVector& obj2)
{
	BoolVector temp(len_bv < obj2.len_bv ? obj2 : *this);
	for (int i = len_mas - 1, j = obj2.len_mas - 1, ind = temp.len_mas - 1; i >= 0 && j >= 0; ind--, i--, j--)
		temp.bv[ind] = bv[i] | obj2.bv[j];

	return temp;
}

BoolVector& BoolVector::operator|=(const BoolVector& object)
{
	for (int i = len_mas - 1, j = object.len_mas - 1; i >= 0 && j >= 0; i--, j--)
		bv[i] |= object.bv[j];

	return *this;
}

BoolVector BoolVector::operator^(const BoolVector& obj2)
{
	BoolVector temp(len_bv < obj2.len_bv ? obj2 : *this);
	for (int i = len_mas - 1, j = obj2.len_mas - 1, ind = temp.len_mas - 1; i >= 0 && j >= 0; ind--, i--, j--)
		temp.bv[ind] = bv[i] ^ obj2.bv[j];

	return temp;
}

BoolVector& BoolVector::operator^=(const BoolVector& object)
{
	for (int i = len_mas - 1, j = object.len_mas - 1; i >= 0 && j >= 0; i--, j--)
		bv[i] ^= object.bv[j];

	return *this;
}

BoolVector BoolVector::operator~()
{
	BoolVector temp(*this);
	for (int i = 0; i < len_mas; i++)
		temp.bv[i] = ~bv[i];
	return temp;
}

BoolVector& BoolVector::operator<<=(unsigned int k)
{
	int j = len_bv - 1;
	for (int i = j - k; i >= 0; i--, j--)
		if (this->operator[](i))
			setOne(j);
		else
			setZero(j);
	setZeroFrom(0, j + 1);

	return *this;
}

BoolVector& BoolVector::operator>>=(unsigned int k)
{
	int j = 0;
	for (; k < len_bv; j++, k++)
		if (this->operator[](k))
			setOne(j);
		else
			setZero(j);
	setZeroFrom(j, len_bv - j - 1);

	return *this;
}

BoolVector BoolVector::operator<<(unsigned int k)
{
	return (BoolVector(*this) <<= k);
}

BoolVector BoolVector::operator>>(unsigned int k)
{
	return (BoolVector(*this) >>= k);
}

void BoolVector::setOne(int ind)
{
	if (ind >= 0 && ind < len_bv)
	{
		int indmas, shift = ind % 8;
		if (shift)
			indmas = len_mas - (ind - 1) / 8 - 1;
		else
			indmas = len_mas - ind / 8 - 1;
		bv[indmas] |= (1 << shift);
	}
}

void BoolVector::setZero(int ind)
{
	if (ind >= 0 && ind < len_bv)
	{
		int indmas, shift = ind % 8;
		if (shift)
			indmas = len_mas - (ind - 1) / 8 - 1;
		else
			indmas = len_mas - ind / 8 - 1;
		bv[indmas] &= ~(1 << shift);
	}
}

void BoolVector::setOneFrom(int ind, int count)
{
	if (ind >= 0 && ind < len_bv)
	{
		int indmas, shift = ind % 8;
		if (shift)
			indmas = len_mas - (ind - 1) / 8 - 1;
		else
			indmas = len_mas - ind / 8 - 1;

		for (int i = 0; i < count && ind < len_bv; i++, ind++, shift = ind % 8, shift == 0 ? indmas-- : indmas)
			bv[indmas] |= (1 << shift);
	}
}

void BoolVector::setZeroFrom(int ind, int count)
{
	if (ind >= 0 && ind < len_bv)
	{
		int indmas, shift = ind % 8;
		if (shift)
			indmas = len_mas - (ind - 1) / 8 - 1;
		else
			indmas = len_mas - ind / 8 - 1;

		for (int i = 0; i < count && ind < len_bv; i++, ind++, shift = ind % 8, shift == 0 ? indmas-- : indmas)
			bv[indmas] &= ~(1 << shift);
	}
}

void BoolVector::invert(int ind)
{
	if (ind >= 0 && ind < len_bv)
	{
		int indmas, shift = ind % 8;
		if (shift)
			indmas = len_mas - (ind - 1) / 8 - 1;
		else
			indmas = len_mas - ind / 8 - 1;
		bv[indmas] ^= (1 << shift);
	}
}

void BoolVector::invertFrom(int ind, int count)
{
	if (ind >= 0 && ind < len_bv)
	{
		int indmas, shift = ind % 8;
		if (shift)
			indmas = len_mas - (ind - 1) / 8 - 1;
		else
			indmas = len_mas - ind / 8 - 1;

		for (int i = 0; i < count && ind < len_bv; i++, ind++, shift = ind % 8, shift == 0 ? indmas-- : indmas)
			bv[indmas] ^= (1 << shift);
	}
}

void BoolVector::extend(int size)
{
	byte* tmp = new byte[len_mas + size]();
	for (int i = len_mas + size - 1, j = len_mas - 1; j >= 0; i--, j--)
		tmp[i] = bv[j];
	delete bv;
	bv = tmp;
	len_mas += size;
}

int BoolVector::weight() const
{
	int indmas = len_mas - 1, weight = 0;
	byte mask = 1;
	for (int i = len_bv - 1; i >= 0; i--, mask <<= 1)
	{
		if (!mask)
		{
			indmas--;
			mask = 1;
		}

		if (bv[indmas] & mask)
			weight++;
	}
	return weight;
}

int BoolVector::size() const
{
	return len_bv;
}

void BoolVector::printVector()
{
	byte mask = 1 << len_bv - len_mas * 8 + 7;
	for (int i = 0; i < len_mas; i++)
	{
		while (mask)
		{
			std::cout << (bv[i] & mask ? 1 : 0);
			mask >>= 1;
		}
		mask = 1 << 7;
	}
}

BoolVector::~BoolVector()
{
	delete[] bv;
}

std::istream& operator>>(std::istream& is, BoolVector vector)
{
	std::cout << "Enter size of the vector : ";
	int size = 0;
	is >> size;
	char* vec = new char[size + 1]();
	for (int i = 0; i < size; i++)
		is >> vec[i];
	vector = vec;
	return is;
}

std::ostream& operator<<(std::ostream& os, BoolVector vector)
{
	byte mask = 1 << vector.len_bv - vector.len_mas - (vector.len_mas - 1) * 7;
	for (int i = 0; i < vector.len_mas; i++)
	{
		while (mask)
		{
			os << (vector.bv[i] & mask ? 1 : 0);
			mask >>= 1;
		}
		mask = 1 << 7;
	}
	return os;
}