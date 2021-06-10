#include <iostream>
#include "Table.h"

using namespace std;


int main(){
	int philosophersNo;
	int thinkTime;
	int eatTime;

    cout << "Enter the number of philosophers: ";
	cin >> philosophersNo;
	cout << "Enter the time philosophers spend on thinking: ";
	cin >> thinkTime;
	cout << "Enter the time philosophers spend on eating: ";
	cin >> eatTime;


	Table table(philosophersNo, thinkTime, eatTime);
	table.dine();
}
