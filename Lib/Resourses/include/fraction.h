#pragma once

#include <iostream>
inline int Abs(int a)
{
	return (a >= 0) ? a : (-a);
}

class Fraction
{
	int numerator;
	int denominator;
	
	//求两数的最大公约数
	unsigned int gcd(unsigned int a, unsigned int b)
	{
		if (a != 0 && b != 0) {
			return gcd(b, (a % b));
		}
		else {
			return (a + b);
		}
	}

	void simplify()
	{
		if (denominator == 0)
			return;

		if (denominator < 0) {
			numerator = -numerator;
			denominator = -denominator;
		}

		if (numerator == 0) {
			denominator = 1;
		}

		int temp = gcd(Abs(numerator), denominator);

		numerator = numerator / temp;
		denominator = denominator / temp;
	}

public:
	Fraction(int n = 0, int d = 1) : numerator(n), denominator(d)
	{
		simplify();
	}

	friend std::ostream& operator<<(std::ostream& os, const Fraction& f)
	{
		if (f.denominator == 0) {
			if (f.numerator > 0) {
				return os << "+INFTY";
			} else if (f.numerator < 0){
				return os << "-INFTY";
			} else {
				return os << "INFTY";
			}
		} else if (f.numerator == 0) {
			return os << 0;
		} else if (f.denominator == 1) {
			return os << f.numerator;
		} else if (f.numerator > 0) {
			return os << '('
				<< f.numerator
				<< '/'
				<< f.denominator
				<< ')';
		} else {//if (f.numerator < 0) {
			return os << '-('
				<< -f.numerator
				<< '/'
				<< f.denominator
				<< ')';
		}
	}

	friend Fraction operator+(const Fraction& lf, const Fraction& rf)
	{
		int den = lf.denominator * rf.denominator;
		int num = lf.numerator * rf.denominator + rf.numerator * lf.denominator;
		return Fraction(num, den);
	}

	friend Fraction operator-(const Fraction& lf, const Fraction& rf)
	{
		int den = lf.denominator * rf.denominator;
		int num = lf.numerator * rf.denominator - rf.numerator * lf.denominator;
		return Fraction(num, den);
	}

	friend Fraction operator*(const Fraction& lf, const Fraction& rf)
	{
		int num = lf.numerator * rf.numerator;
		int den = lf.denominator * rf.denominator;
		return Fraction(num, den);
	}

	friend Fraction operator/(const Fraction& lf, const Fraction& rf)
	{
		int num = lf.numerator * rf.denominator;
		int den = lf.denominator * rf.numerator;
		return Fraction(num, den);
	}

	friend bool operator==(const Fraction& lf, const Fraction& rf)
	{
		if (lf.denominator == 0 || rf.denominator == 0 ||lf.numerator * rf.denominator != rf.numerator * lf.denominator){
			return false;
		} else {
			return true;
		}
	}

	friend bool operator!=(const Fraction& lf, const Fraction& rf)
	{
		return !(lf == rf);
	}

	friend bool operator>(const Fraction& lf, const Fraction& rf)
	{
		if (lf.numerator * rf.denominator > rf.numerator * lf.denominator) {
			return true;
		} else {
			return false;
		}
	}

	friend bool operator<(const Fraction& lf, const Fraction& rf)
	{
		if (lf.numerator * rf.denominator < rf.numerator * lf.denominator) {
			return true;
		}
		else {
			return false;
		}
	}

	friend bool operator>=(const Fraction& lf, const Fraction& rf)
	{
		return (lf > rf || lf == rf);
	}

	friend bool operator<=(const Fraction& lf, const Fraction& rf)
	{
		return (lf < rf || lf == rf);
	}
};
