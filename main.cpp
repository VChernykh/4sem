#include <iostream>
#include "Vector2.h"
#include <cmath>

using namespace std;

int main()
{

	float len, squareLen;
	double pi = acos(-1.0);

	cout << "Enter your vector" << endl;
	Vector2 vector;
	cin >> vector;
	Vector2 other_vec(5, 12);

	cout << "Vector = " << vector << endl;		//1
	cout << "Other vector = " << other_vec << endl; 

	Vector2 sum_vec = vector + other_vec;		//2
	cout << "The sum of Vector and Other vector = " << sum_vec << endl;

	vector += other_vec;		//3
	cout << "Result: Vector += Other vector = " << vector << endl;
	vector -= other_vec;
	
	vector -= other_vec;		//4
	cout << "Result: Vector -= Other vector = " << vector << endl;
	vector += other_vec;

	Vector2 diff_vec = vector - other_vec;		//5
	cout << "The difference of Vector and Other vector = " << diff_vec << endl;

	int scal = vector * other_vec;		//6
	cout << "The scalar product of Vector and Other vector = " << scal << endl;

	vector = vector * 2;		//7
	cout << "Vector * 2 = " << vector << endl;

	vector = 3 * vector;		//8
	cout << "Vector * 3 = " << vector << endl;

	vector = vector / 6;		//9
	std::cout << "Vector / 6 = " << vector << endl;
	

	len = vector.len();		//10
	cout << "Vector length = " << len << endl;

	squareLen = vector.squareLen();		//11
	cout << "Length squared = " << squareLen << endl;

	Vector2 norma = vector.norm();		//12
	cout << "The norm of the vector = " << norma << endl;

	Vector2 perped = vector.perpendicular();		//13
	cout << "Perpendicular to the vector = " << perped << endl;

	Vector2 opposite_vector = -vector;		//14
	cout << "The opposite vector = " << opposite_vector << endl;

	Vector2 start_vector(0,1);		//15
	cout << "Rotatable vector = " << start_vector << endl;
	Vector2 rotated_vector = start_vector.getRotated(pi/2);
	cout << "90 degree rotated vector = " << rotated_vector << endl;

	start_vector.rotate(pi);		//16
	cout << "180 degree rotated vector = " << start_vector << endl;

	system("pause");

	return 0;
}