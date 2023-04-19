/*
 * future-promise.cpp
 *
 *  Created on: 15-Mar-2022
 *      Author: Jotirling Swami
 */

// TOPIC: std::future and std::promise in threading.
// NOTES:
// 1. std::promise
//      a. Used to set values or exceptions.
// 2. std::future
//      a. Used to get values from promise.
//      b. Ask promise if the value is available.
//      c. Wait for the promise.

#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>
using namespace std;

#define LIMIT 1500000000

typedef long long int longInt;

void findOdd(std::promise<longInt> &&promisObject) {

	longInt value = 0;
	while (value != LIMIT)
		value += 1;
	promisObject.set_value(value);
}

int main() {

	promise<longInt> promisObject;
	future<longInt> futureObject = promisObject.get_future();

	cout << "Thread Created!!" << endl;
	thread t1(findOdd, std::move(promisObject));

	cout << "Waiting For Result!!" << endl;

	cout << "OddSum : " << futureObject.get() << endl;

	cout << "Completed!!" << endl;
	t1.join();

	return 0;
}

