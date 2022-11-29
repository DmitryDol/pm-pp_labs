// mp5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
/*
Реализовать консольное приложение, 
предназначенное для работы с массивом экземпляров класса. 

Входные данные: количество элементов массива, экземпляры класса. 

Выходные данные: массив экземпляров класса 
и массив результатов работы метода по обработке данных.

Особенности:
- у класса должны быть перегружены операторы взятия из потока и помещения в поток;
- должна быть создана функция-член класса, реализующая обработку данных экземпляра класса согласно варианту;
- должна быть реализована возможность выбора чтения из файла или с консоли;
- должна быть реализована возможность выбора записи в файла или вывода в консоль.

*/

/*
Класс: Данные о прибыли
Формат: Номер филиала Число дней Прибыль
Пример: 1 5 100 78.5 1000 0 1000
Обработка: Вывод числа дней, когда 0 прибыли
*/
#include <iostream>
#include <fstream>
#include <string>

class Income_data {
private:
	int number_of_branch;
	int amount_of_days;
	double* income;
public:
	Income_data() {
		number_of_branch = 0;
		amount_of_days = 0;
		income = nullptr;
	}
	Income_data(int number_of_branch, int amount_of_days) {
		this->number_of_branch = number_of_branch;
		this->amount_of_days = amount_of_days;
		this->income = new double[amount_of_days];
	}
	Income_data(int number_of_branch, int amount_of_days, int* income) {
		this->number_of_branch = number_of_branch;
		this->amount_of_days = amount_of_days;
		this->income = new double[amount_of_days];
		for (int i = 0; i < amount_of_days; ++i) {
			this->income[i] = income[i];
		}
	}
	Income_data(const Income_data& ref) {
		number_of_branch = ref.number_of_branch;
		amount_of_days = ref.amount_of_days;
		income = new double[amount_of_days];
		for (int i = 0; i < amount_of_days; ++i)
			income[i] = ref.income[i];
	}
	~Income_data() {
		delete[] income;
		income = nullptr;
		amount_of_days = 0;
		number_of_branch = 0;
	}
	
	int amount_of_zero_income_days() const {
		int count = 0;
		for (int i = 0; i < amount_of_days; ++i)
			if (income[i] == 0)
				++count;
		return count;
	}
	
	
	friend std::ifstream& operator >> (std::ifstream& in, Income_data& ref);
	friend std::istream& operator >> (std::istream& in, Income_data& ref);
	
	friend std::ostream& operator << (std::ostream& out, const Income_data& ref);
};

std::ifstream& operator >> (std::ifstream& in, Income_data& ref) {
	in >> ref.number_of_branch >> ref.amount_of_days;
	if (ref.income != nullptr)
		delete[] ref.income;
	ref.income = new double[ref.amount_of_days];
	for (int i = 0; i < ref.amount_of_days; ++i)
		in >> ref.income[i];
	return in;
}
std::istream& operator >> (std::istream& in, Income_data& ref) {
	std::cout << "Enter number of branch: ";
	in >> ref.number_of_branch;
	std::cout << "Enter amount of days: ";
	in >> ref.amount_of_days;
	if (ref.income != nullptr)
		delete[] ref.income;
	std::cout << "Enter income for each of " << ref.amount_of_days << " days: ";
	
	ref.income = new double[ref.amount_of_days];
	for (int i = 0; i < ref.amount_of_days; ++i) {
		in >> ref.income[i];
	}
	if (in.peek() == '\n')
		in.get();
	return in;
}

std::ostream& operator << (std::ostream& out, const Income_data& ref) {
	for (int i = 0; i < ref.amount_of_days; ++i)
		out << ref.income[i] << ' ';
	out << std::endl << "Amount of days when income of branch number "
		<< ref.number_of_branch << " was equal to 0: "
		<< ref.amount_of_zero_income_days();
	return out;
}

void file_input(Income_data& data) {
	std::cout << "Input path.\n";
	std::string path;
	std::getline(std::cin, path);
	std::ifstream file(path);
	file >> data;
	file.close();
}
void cmd_input(Income_data& data) {
	std::cin >> data;
}
void input(Income_data& data) {
	std::cout << "Input data from file or from cmd?\n[file/cmd]: ";
	std::string input_choice;
	std::getline(std::cin, input_choice);
	if (input_choice == "file" || input_choice == "FILE")
		file_input(data);
	else if (input_choice == "cmd" || input_choice == "CMD")
		cmd_input(data);
	else {
		std::cout << "Incorrect input type." << std::endl;
		input(data);
	}
}

void file_output(Income_data& data) {
	std::cout << "Input path.\n";
	std::string path;
	std::getline(std::cin, path);
	std::ofstream file(path);
	file << data;
	file.close();
}
void cmd_output(Income_data& data) {
	std::cout << data << std::endl;
}
void output(Income_data& data) {
	std::cout << "Output data from file or from cmd?\n[file/cmd]: ";
	std::string output_choice;
	std::getline(std::cin, output_choice);
	if (output_choice == "file" || output_choice == "FILE")
		file_output(data);
	else if (output_choice == "cmd" || output_choice == "CMD")
		cmd_output(data);
	else {
		std::cout << "Incorrect output type." << std::endl;
		output(data);
	}
}

int main()
{
	Income_data data;
	input(data);
	output(data);

}
