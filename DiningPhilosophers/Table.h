#pragma once
#include<vector>
#include<mutex>
#include<thread>
#include<random>
#include<condition_variable>
#include<iostream>
#include<string>

class Table{
public:
	Table(int philosophersNo, int thinkTime, int eatTime);
	~Table();
	void dine();

private:
	std::vector<std::unique_ptr<std::mutex>> forks;
	std::vector<std::thread> threads;

	int philosophersNo;
	atomic bool isOver;

	int thinkTime;
	int eatTime;

	int *timeEaten = 0;
	int *timeThinked = 0;

	void philosopher(int number);
	void eat(int number);
	void think(int number);
	int randomTime(int time);
	void sleepFor(int time);
	void check();
	void printEating();
	void printThinking();
};

