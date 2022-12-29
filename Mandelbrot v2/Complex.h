#pragma once

struct Complex
{

	double real;
	double imaginary;

	Complex()
		:real(0), imaginary(0)
	{};

	Complex(double r, double i)
		:real(r), imaginary(i)
	{};

	void operator*=(const Complex& other)
	{
		real *= other.real;
		imaginary *= other.imaginary;
	}
	void operator+=(const Complex& other)
	{
		real += other.real;
		imaginary += other.imaginary;
	}
	Complex operator+(const Complex& other) const
	{
		return Complex(real + other.real, imaginary + other.imaginary);
	}
	void operator-=(const Complex& other)
	{
		real -= other.real;
		imaginary -= other.imaginary;
	}
	Complex operator*(Complex& other)
	{
		return Complex(real * other.real, imaginary * other.imaginary);
	}
	Complex operator*(double val)
	{
		return Complex(real * val, imaginary * val);
	}
	Complex operator-(Complex& other)
	{
		return Complex(real - other.real, imaginary - other.imaginary);
	}
	Complex operator/(double val)
	{
		return Complex(real / val, imaginary / val);
	}
	void square()
	{
		double r, i;
		r = real * real - imaginary * imaginary;
		i = 2 * real * imaginary;
		real = r;
		imaginary = i;
	}
	double modulusSquared()
	{
		return real * real + imaginary * imaginary;
	}
};

