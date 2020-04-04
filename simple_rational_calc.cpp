#include <iostream>
#include <sstream>
#include <exception>
#include <iomanip>

using namespace std;

int GreatestCommonFactor(int num, int denom)
{
	if(num < 0)
		num *= -1;
	if(denom < 0)
		denom *= -1;
	while(num > 0 && denom > 0) {
		if(num > denom)
			num %= denom;
		else
			denom %= num;
	}
	return num + denom;
}

class Rational {
public:
	Rational() {
		p = 0;
		q = 1;
	}

	Rational(int numerator, int denominator) {
		int gcf = GreatestCommonFactor(numerator, denominator);
		p = numerator/gcf;
		q = denominator/gcf;
		if(q == 0) 
			throw invalid_argument("Invalid argument\n");
		if (p == 0)
			q = 1;
		else if (p < 0) {
			if (q < 0){
				p *= -1;
				q *= -1;
			} 
		} else {
			if (q < 0) {
				p *= -1;
				q *= -1;
			} 
		}
	}

	int Numerator() const {
		return p;
	}

	int Denominator() const {
		return q;
	}

private:
	int p;
	int q;
};

int CommonDenominator(const Rational &lhs, const Rational &rhs)
{
	int lp = lhs.Numerator(), rp = rhs.Numerator();
	int lq = lhs.Denominator(), rq = rhs.Denominator();
	int gcf = GreatestCommonFactor(lq, rq);
	return lq * rq / gcf;
}

Rational operator+(const Rational &lhs, const Rational &rhs) 
{
	int common_denom = CommonDenominator(lhs, rhs);
	int common_num = lhs.Numerator() * common_denom / lhs.Denominator() 
		        + rhs.Numerator() * common_denom / rhs.Denominator();
	return Rational(common_num, common_denom);
}

Rational operator-(const Rational &lhs, const Rational &rhs) 
{
	int common_denom = CommonDenominator(lhs, rhs);
	int common_num = lhs.Numerator() * common_denom / lhs.Denominator() 
		        - rhs.Numerator() * common_denom / rhs.Denominator();
	return Rational(common_num, common_denom);
}

Rational operator*(const Rational &lhs, const Rational &rhs) 
{
	return Rational(lhs.Numerator()*rhs.Numerator(), 
			lhs.Denominator()*rhs.Denominator());
}

Rational operator/(const Rational &lhs, const Rational &rhs) 
{
	if(rhs.Numerator() == 0)
		throw domain_error("Division by zero\n");
	return Rational(lhs.Numerator()*rhs.Denominator(), 
			lhs.Denominator()*rhs.Numerator());
}

bool operator==(const Rational &lhs, const Rational & rhs)
{
	if(lhs.Numerator() == rhs.Numerator() && lhs.Denominator() == rhs.Denominator())
		return true;
	else
		return false;
}

bool operator<(const Rational &lhs, const Rational &rhs)
{
	int common_denom = CommonDenominator(lhs, rhs);
	int left_num = lhs.Numerator() * common_denom / lhs.Denominator();
	int right_num = rhs.Numerator() * common_denom / rhs.Denominator();
	return left_num < right_num;
}

ostream& operator<<(ostream& stream, const Rational rational) 
{
	stream << rational.Numerator() << "/" << rational.Denominator();
	return stream;
}

void ErrorCase()
{
	stringstream ss;
	ss << "Invalid argument" << endl;
	throw runtime_error(ss.str());
}

istream& operator>>(istream &stream, Rational &rational) 
{
	int p, q;
	char c;
	if(stream) {
		stream >> p >> c >> q;
		if(stream && c == '/')
			rational = Rational(p, q);
		else 
			ErrorCase();
	}
	return stream;
}

void EnsureNextSymbolAndSkip(stringstream &stream, char symb) 
{
	if(stream.peek() != symb) 
		ErrorCase();
	stream.ignore(1);
}

Rational HandleOperation(const Rational &lhs, const Rational &rhs, char operation)
{
	switch(operation) {
	case '+':
		return lhs + rhs;
	case '-':
		return lhs - rhs;
	case '*':
		return lhs * rhs;
	case '/':
		return lhs / rhs;
	}
}

void ParseExpression(stringstream &stream, Rational &lhs, Rational &rhs, char &operation) 
{
	stream >> lhs;
	EnsureNextSymbolAndSkip(stream, ' ');
	stream >> operation;
	EnsureNextSymbolAndSkip(stream, ' ');
	stream >> rhs;
}

Rational Calculate(const string str) 
{
	stringstream stream(str);
	Rational lhs, rhs;
	char operation;
	ParseExpression(stream, lhs, rhs, operation);
	return HandleOperation(lhs, rhs, operation);
}

int main() {
	cout << setw(10) <<"Enter the expression in the format x/y operation p/q " << endl;
	Rational res;
	string str;
	getline(cin, str);
	try {
		res = Calculate(str);
		cout << "Result: " << res << endl;
	} catch(exception& ex) {
		cout << ex.what();
	}
	return 0;
}
