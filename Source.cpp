/*
Author: Jared Rigdon
CS 315 HW 6

Generate random binary numbers with n digits. Assume that the msb is 1 and the lsb is also 1.
Use a single Fermats test  3^(N-1)=1(mod N) for each number
Use mod exp

test algtm with 100 16 bit ints anc check how many are prime by using the brute force method of trying all divisors up to the square root of the number

this is a combination of algoritms from various homeworks
due note: there are still some final value errors that i havent fixed but fro the most part, it is correct and working
*/

#include <vector>
#include <iostream>
#include <time.h>

using namespace std;


void pause_215(bool have_newline);
vector<int> mod_pow(vector<int>& x, vector<int>& y, vector<int>& n);
int compare(vector<int>& r, vector<int>& d);
vector<int> addition(vector<int>& x, vector<int>& y);
vector<int> two_comp(vector<int>& x);
vector<int> multiply2(vector<int>& x, vector<int>& y);
vector<int> divide(vector<int>& x, vector<int>& y);

int main() {
	//generate n bit vectors with the lsb and msb = 1
	//use fermats test 3^(N-1)=1 mod N

	//test 100 random 16 bit integers by trying all divisors up until the square root of the number

	srand(time(NULL));
	//count the amount of primes from each method and compare if they are equal
	bool fermat_p, brute_p;
	int wrong = 0;
	//make the f_base == 3 (1,1)
	int cnt = 0;
	int first_p = 0;
	vector<int> f_base;
	f_base.push_back(1);
	f_base.push_back(1);
	//use one to increase the divisor by 1
	vector<int> one;
	one.push_back(1);
	vector<int> random;
	vector<int> y;
	//continue generating numbers until 100 is generated
	for (int i = 1; i <= 100; i++) {
		//generate the random 16 bit number
		//always make the lsb bit == 1
		random.push_back(1);

		for (int j = 1; j < 16; j++) {
			//generates a bit of either 0 or 1
			if (j < 15) {
				random.push_back(rand() % 2);
			}
			//the last bit should be 1
			else if (j == 15) {
				random.push_back(1);
			}
		}
		//make y equal to the random -1
		y = random;
		y[0] = 0;

		cout << "\nrandom: ";
		for (vector<int>::iterator it = random.begin(); it != random.end(); ++it)
			cout << *it;
		cout << endl;

		cout << "y: ";
		for (vector<int>::iterator it = y.begin(); it != y.end(); ++it)
			cout << *it;
		cout << endl;

		cout << "\nUsing fermats test: " << endl;
		//using fermats test
		vector<int> z = mod_pow(f_base, y, random);
		//if the vector z returns with a 1 or the same value as random, then random is prime
		if (compare(z, one) == 2 | compare(z, random) == 2) {
			cout << "The randomly generated 16 bit interger: ";
			for (vector<int>::iterator it = random.begin(); it != random.end(); ++it)
				cout << *it;
			cout << " is prime." << endl;
			fermat_p = true;
			cnt++;
			if (first_p == 0) {
				first_p = i;
			}
		}
		else {
			cout << "The randomly generated 16 bit interger: ";
			for (vector<int>::iterator it = random.begin(); it != random.end(); ++it)
				cout << *it;
			cout << " is not prime." << endl;
			fermat_p = false;
		}

		//do brute force division
		bool sqrt = true;
		vector<int> divisor; //start at 2
		divisor.push_back(0);
		divisor.push_back(1);
		vector<int> result; //remainder result of the random number divided by the divisor

		cout << "\nUsing the brute force method: " << endl;
		//while the sqrt 
		while (sqrt) {
			vector<int> test = multiply2(divisor, divisor);
			//if divisor^2 is greater than random, then the current divisor is slightly greater than the square root of random
			//as we have reached the limit
			if (compare(test, random) == 1) {
				//set the bool sqrt equal to false to exit the loop
				sqrt = false;

				cout << "The randomly generated 16 bit interger: ";
				for (vector<int>::iterator it = random.begin(); it != random.end(); ++it)
					cout << *it;
				cout << " is prime." << endl;
				brute_p = true;
			}
			else
			{
				result = divide(random, divisor);
				//if the size of the remainder result is 1 then check if the single element is not 0
				if (result.size() == 1) {
					//if the computation of the remainder result is 0 then the random is not prime
					if (result[0] == 0) {
						//state that random is not a prime and exit loop
						sqrt = false;

						cout << "The randomly generated 16 bit interger: ";
						for (vector<int>::iterator it = random.begin(); it != random.end(); ++it)
							cout << *it;
						cout << " is not prime." << endl;
						brute_p = false;
					}
				}
			}
			//clear the test and result vectors
			test.clear();
			result.clear();

			//try the next bit up
			divisor = addition(divisor, one);
		}
		//if fermats tests result is different than the brute force method, then fermats test was wrong
		if (fermat_p != brute_p) {
			wrong++;
		}
		random.clear();
		y.clear();
	}
	cout << " Amount of times that Fermats test was wrong: " << wrong << endl;
	cout << " Amount of times that a prime was found: " << cnt << endl;
	cout << "The first prime was found after " << first_p << " random numbers were computed." << endl;
	pause_215(true);
}

//just a small function that pauses the program, i find it very usefull
void pause_215(bool have_newline)
{
	if (have_newline) {
		// Ignore the newline after the user's previous input.
		cin.ignore(256, '\n');
	}

	// Prompt for the user to press ENTER, then wait for a newline.
	cout << endl << "Press ENTER to continue." << endl;
	cin.ignore(256, '\n');
}

//this uses a pretty straight forward approach, didnt have time to come up with a recursive algorithm
vector<int> mod_pow(vector<int>& x, vector<int>& y, vector<int>& n) {
	//initialize vectors z and w
	vector<int> z, w;
	//make z equal to 1 and w equal to x
	z.push_back(1);
	for (int i = 0; i < x.size(); i++) {
		w.push_back(x[i]);
	}

	//continue the loop until N-1 which is y
	for (int j = 0; j < y.size(); j++) {
		//if the binary value of y at i is = 1 then compute z=z*w mod N
		if (y[j] == 1) {
			if (z.size() < w.size()) {
				z = multiply2(w, z);
			}
			else {
				z = multiply2(z, w);
			}
			z = divide(z, n);
		}
		//compute w=w*w mod N
		w = multiply2(w, w);
		w = divide(w, n);
	}
	w.clear();
	//return z vector
	//if the z vector contains anything except a 1 or the x value, then Fermat's test failed(composite)
	return z;
}

//this is largely used in the divide function
int compare(vector<int>& n, vector<int>& d) {
	int state;
	//check if n size is less than d size
	if (n.size() < d.size()) {
		return state = 0;
	}
	//if the numerator size is greater than the denominator, then make the sizes equal
	if (n.size() > d.size()) {
		while (n.size() > d.size()) {
			d.push_back(0);
		}
	}
	//check in descending order for which binary is greater
	for (int i = d.size() - 1; i >= 0; i--) {
		//return a 0 if r < d
		if (n[i] < d[i]) {
			while (d.back() != 1) {
				d.pop_back();
			}
			return state = 0;
		}
		//return a 1 if r > d
		else if (n[i] > d[i]) {
			while (d.back() != 1) {
				d.pop_back();
			}
			return state = 1;
		}
	}
	//reutrn a 2 if r == d
	while (d.back() != 1) {
		d.pop_back();
	}
	return state = 2;

}

//addition of 2 binary numbers
//make sure that if you want to subtract using 2's compliment, take care of sign conversion
vector<int> addition(vector<int>& x, vector<int>& y) {
	int carry = 0;
	vector<int> c, temp;
	//temp vector to have x and y to equal both sizes assuming x > y

	for (int j = 0; j < (x.size()); j++) {
		if (j < (y.size())) {
			temp.push_back(y[j]);
		}
		else { temp.push_back(0); }
	}

	for (int i = 0; i < (x.size()); i++) {
		c.push_back(((x[i] ^ (temp)[i]) ^ carry));
		carry = ((x[i] & (temp)[i]) | (x[i] & carry)) | ((temp)[i] & carry);
		if ((i == (x.size() - 1)) & (carry == 1)) {
			c.push_back(carry);
		}
	}
	return c;
}

//conversion to twos compliment
vector<int> two_comp(vector<int>& x) {
	vector<int> x_comp;
	int n = x.size();
	//converts all 1s to 0s and 0s to 1s
	for (int i = 0; i < n; i++) {
		if (x[i] == 1) {
			x_comp.push_back(0);
		}
		else {
			x_comp.push_back(1);
		}
	}
	//add 1 to the end to compliment it
	x_comp.push_back(1);
	vector<int> one;
	one.push_back(1);
	//add 1 to x_comp to complete the 2's compliment equation
	x_comp = addition(x_comp, one);
	return x_comp;
}

//binary multiplication using unsigned binaries
vector<int> multiply2(vector<int>& x, vector<int>& y) {
	int m = x.size();
	int n = y.size();
	//initialize vectors
	vector<int> P, p_temp, xy, y_temp;
	if (y.size() == 1 & y[0] == 1) {
		return x;

	}
	else {
		//make the size if x and y equal to avoid issues
		for (int u = 0; u < m; u++) {
			if (u < n) {
				y_temp.push_back(y[u]);
			}
			else {
				y_temp.push_back(0);
			}
		}
		for (int i = 0; i < m; i++) {
			//for the first iteration, compare x elements with the first y element
			if (i == 0) {
				for (int j = 0; j < m; j++) {
					if ((x[j] == 1)&(y[0] == 1)) {
						p_temp.push_back(1);
					}
					else { p_temp.push_back(0); }
				}
				//add the lsb to the P(product)
				P.push_back(p_temp[0]);
				//erase the last element
				p_temp.erase(p_temp.begin());
			}
			else if ((i > 0) & (i < (n - 1))) {
				//compare the following iterations with the i element in y
				//clear from previous iteration
				xy.clear();
				for (int k = 0; k < m; k++) {
					if ((x[k] == 1)&(y[i] == 1)) {
						xy.push_back(1);
					}
					else { xy.push_back(0); }
				}
				//add xy now to p_temp
				p_temp = addition(xy, p_temp);
				P.push_back(p_temp[0]);
				p_temp.erase(p_temp.begin());

			}
			//if we arrive at the last iteration, then apply xy to the end of P
			else if (i == (n - 1)) {
				xy.clear();

				for (int k = 0; k < m; k++) {
					if ((x[k] == 1)&(y[i] == 1)) {
						xy.push_back(1);
					}
					else { xy.push_back(0); }
				}
				p_temp = addition(xy, p_temp);

				for (int p = 0; p < p_temp.size(); p++) {
					P.push_back(p_temp[p]);
				}
			}
		}
	}
	return P;
}

//the divide function will compare r(remainder) with y(dividor) from the msb to lsb 
//to find the quotient, just add 1 to the quotient vector where the current bit in the for loop is
vector<int> divide(vector<int>& x, vector<int>& y) {
	//Division
	int m = x.size();
	int n = y.size();
	vector<int> y_comp = two_comp(y);

	//asumming that x > y
	// create remainder r and quotient q vectors and set initial value to 0
	vector<int> r;
	r.push_back(0);
	vector<int> q;
	q.push_back(0);
	//since the most significant bit is in the beginning then start at end of vector
	//if the divisor is greater than the numerator then return the numerator as the remainder and quotient as 0
	int state = compare(x, y);

	if (state == 0) {
		r.clear();
		//copys the numerator(x) to the remainder
		for (int j = 0; j < (x.size()); j++) {
			r.push_back(x[j]);
		}
	}
	//if the numerator and denominator is equal, then return the quotient of 1 and remainder as 0
	else if (state == 2) {
		r.clear();
		//has the remainder equal to 1
		r.push_back(0);
	}
	else {
		for (int i = m - 1; i >= 0; i--) {
			//add zero to the begining of the vector and assign the numerator[i] to it
			r.insert(r.begin(), 0);
			r[0] = x[i];

			//see if the remainder is equal to or greater than the denominator
			if ((compare(r, y) == 1) | (compare(r, y) == 2)) {
				//find the difference between r and b_comp and assign it to r
				r = addition(r, y_comp);
				//take care of the twos complement leading 1
				if (r.back() == 1) {
					r.pop_back();
				}
				//take care of any leading 0s
				while ((r.back() != 1)&(r.size()>1)) {
					r.pop_back();
				}

			}
		}
	}
	//take care of any leading 0s 
	while ((r.back() != 1)&(r.size()>1)) {
		r.pop_back();
	}
	//return the remainder
	return r;
}