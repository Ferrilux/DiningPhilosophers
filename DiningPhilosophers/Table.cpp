#include "Table.h"


Table::Table(int philosophersNo, int thinkTime, int eatTime) : forks(philosophersNo) {
	this->philosophersNo = philosophersNo;
	this->eatTime = eatTime;
	this->thinkTime = thinkTime;
	this->isOver = false;
	this->timeEaten = new int[philosophersNo];
	this->timeThinked = new int[philosophersNo];

	for (int i = 0; i < philosophersNo; i++) {
		timeEaten[i] = 0;
		timeThinked[i] = 0;
	}
	for (auto& fork : forks) {
		fork = std::make_unique<std::mutex>();
	}
}

Table::~Table() {
	/*for (auto& fork : forks) {
		delete &fork;
	}*/
	delete[] timeEaten;
	delete[] timeThinked;
}

int Table::randomTime(int time) {
	time = time * 1000;
	std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> num(0.8 * time, 1.2 * time);

	return num(mt);
}

void Table::sleepFor(int time) {
	std::chrono::milliseconds duration(time);
	std::this_thread::sleep_for(duration);
}

void Table::dine() {
	char sign = ' ';
	std::cout << "Dinner started." << std::endl << std::endl;
	for (int i = 0; i < philosophersNo; ++i) {
		threads.emplace_back(std::thread(&Table::philosopher, this, i));
	}
	std::thread observe(&Table::check, this);
	observe.join();
	std::cout << "Dinner ended." << std::endl << std::endl;
	printEating();
	printThinking();
}

void Table::check() {
	char sign = ' ';
	while (!this->isOver) {
		sign = getchar();
		if (sign == 'q') {
			this->isOver = true;
			for (auto& t : threads) {
				t.join();
			}
		}
	}
}

void Table::philosopher(int number) {
	while (!this->isOver) {
		think(number);
		eat(number);
	}
}

void Table::think(int number) {
	std::cout << "Philosopher " << number << " is thinking." << std::endl;
	auto timeOnThinking = randomTime(thinkTime);
	sleepFor(timeOnThinking);
	timeThinked[number] += timeOnThinking/1000;
	std::cout << "Philosopher " << number << " finished thinking. It took " << timeOnThinking / 1000 << " seconds." << std::endl;
}

void Table::eat(int number) {
	std::unique_lock<std::mutex> lockLeft(*forks[number], std::defer_lock);
	std::unique_lock<std::mutex> lockRight(*forks[(number + 1) % philosophersNo], std::defer_lock);

	std::lock(lockLeft, lockRight);

	std::cout << "Philosopher " << number << " is eating." << std::endl;
	auto timeOnEating = randomTime(eatTime);
	sleepFor(timeOnEating);
	timeEaten[number] += timeOnEating / 1000;
	std::cout << "Philosopher " << number << " finished eating. It took " << timeOnEating / 1000 << " seconds." << std::endl;
}

void Table::printEating() {
	std::cout << std::endl << "Results for eat time." << std::endl;
	for (int i = 0; i < philosophersNo; i++) {
		std::cout << "Philosopher " << i << " = " << this->timeEaten[i] << "s" << std::endl;
	}
	std::cout << std::endl;
}

void Table::printThinking() {
	std::cout << std::endl << "Results for think time." << std::endl;
	for (int i = 0; i < philosophersNo; i++) {
		std::cout << "Philosopher " << i << " = " << this->timeThinked[i] << "s" << std::endl;;
	}
	std::cout << std::endl;
}