#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

float convert(int g, float m,  float s,  char w) {
	float deg;
	if ((w == 'N') or (w == 'E')) {
		deg = (s / 3600) + (m / 60) + g;
	}
	else {
		deg = -(s / 3600) - (m / 60) - g;
	}
	deg = round(deg * 100) / 100;
	return deg;
}

float linear(float deg, float deg1, float dl) {
	const int r = 6371;
	float l,df,dq;
	df = deg1 - deg;
	dq = 2 * asin(sqrt((pow(sin(df/2),2.0)) + (cos(deg) * cos(deg1) * pow(sin(dl/2),2.0))));
	deg = (deg * M_PI) / 180;
	l = dq * r;
	l = round(l * 100) / 100;
	return l;
}

float azimut1(float deg, float deg1, float dl) {
	float a1;
	a1 = atan(((cos(deg)*tan(deg1))/sin(dl))-(sin(deg)/tan(dl)));
	a1 = M_PI_2 - a1;
	a1 = (a1 * 180) / M_PI;
	a1 = round(a1 * 100) / 100;
	return a1;
}

float azimut2(float deg, float deg1, float dl) {
	float a2;
	a2 = atan((sin(deg1) / tan(dl))-((cos(deg1) * tan(deg)) / sin(dl)));
	a2 = M_PI_2 - a2;
	a2 = (a2 * 180) / M_PI;
	a2 = round(a2 * 100) / 100;
	return a2;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Введите координаты в формате “градусы, минуты, секунды, символы”, через пробел \n";
	int g,g1,g2;
	float m,m1,m2,s,s1,s2,deg,deg1,dl,l,a1,a2;
	char w,w1,w2;
	cin >> g >> m >> s >> w;
	cin >> g1 >> m1 >> s1 >> w1;
	cin >> g2 >> m2 >> s2;
	deg = convert(g, m, s, w);
	deg1 = convert(g1, m1, s1, w1);
	dl = convert(g2, m2, s2, w2 = 'E');
	deg = (deg * M_PI) / 180;
	deg1 = (deg1 * M_PI) / 180;
	dl = (dl * M_PI) / 180;
	l = linear(deg, deg1, dl);
	a1 = azimut1(deg, deg1, dl);
	a2 = azimut2(deg, deg1, dl);
	cout << l << " - расстояние; " << a1 << " - начальный азимут; " << a2 << " - конечный азимут;";
	return 0;
}

