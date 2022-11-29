// pp5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

/*
Реализовать консольное приложение, предназначенное для работы с массивом экземпляров класса, который имеет в качестве переменной класса объект другого класса. 
Входные данные: количество элементов массива, экземпляры класса. 
Выходные данные: массив экземпляров класса и массив результатов работы метода по обработке данных.
Особенности:
- у классов должны быть перегружены операторы взятия из потока и помещения в поток;
- должна быть создана функция-член класса, реализующая обработку данных экземпляра класса согласно варианту;
- должна быть реализована возможность выбора чтения из файла или с консоли;
- должна быть реализована возможность выбора записи в файла или вывода в консоль.
*/

/*
Классы: Данные о прибыли; Данные о компании
Формат: Номер филиала, Число дней, Прибыль
Пример: 1 5 100 78.5 1000 0 1000; StroyDom 2 1 2 200 400 2 200 0 
Обработка: Определение номера филиала с максимальной выручкой
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
	}
	
	int branch_number() {
		return number_of_branch;
	}
	/*
	int days() {
		return amount_of_days;
	}
	
	double* income()  {
		return income;
	}
	*/
	double income_sum() {
		double sum = 0;
		for (int i = 0; i < amount_of_days; ++i) 
			sum += income[i];
		return sum;
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
	if (ref.income != nullptr) {
		delete[] ref.income;
		ref.income = nullptr;
	}
	ref.income = new double[ref.amount_of_days];
	for (int i = 0; i < ref.amount_of_days; ++i)
		in >> ref.income[i];
	if (in.peek() == '\n')
		in.get();
	return in;
}
std::istream& operator >> (std::istream& in, Income_data& ref) {
	std::cout << "Enter number of branch: ";
	in >> ref.number_of_branch;
	std::cout << "Enter amount of days: ";
	in >> ref.amount_of_days;
	if (ref.income != nullptr) {
		delete[] ref.income;
		ref.income = nullptr;
	}
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

class Company_data {
private:
	std::string company_name;
	int amount_of_branches;
	Income_data* company_income;
public:
	Company_data(std::string company_name, int amount_of_branches, Income_data* company_income) {
		this->company_name = company_name;
		this->amount_of_branches = amount_of_branches;
		//this->company_income = company_income;
		this->company_income = new Income_data[amount_of_branches];
		for (int i = 0; i < amount_of_branches; ++i)
			this->company_income[i] = company_income[i];
	}
	double* income_sum() {
		double* income_sum = new double[amount_of_branches];
		for (int i = 0; i < amount_of_branches; ++i)
			income_sum[i] = company_income[i].income_sum();
		
		return income_sum;
	}
	// return array, that contains [0] - max income of all branches [1] - amount of branches where income is max
	double* max_income() {
		double max_and_amount[2] = { company_income[0].income_sum(),1};
		for (int i = 0; i < amount_of_branches; ++i) {
			double income = company_income[i].income_sum();
			if (max_and_amount[0] == income)
				max_and_amount[1]++;
			if (max_and_amount[0] < income) {
				max_and_amount[0] = income;
				max_and_amount[1] = 1;
			}
		}
		return max_and_amount;
	}
	//return array of branch numbers with max income. [0] - amount of this branches 
	int* best_branch() {
		double* best_branch_info = max_income();
		double max_sum = best_branch_info[0];
		int amount_of_best_branches = max_income()[1];
		int* best_branch_numbers = new int[amount_of_best_branches];
		best_branch_numbers[0] = amount_of_best_branches;
		int counter = 0;
		for (int i = 1; i <= amount_of_best_branches; ++i) {
			double income = company_income[i].income_sum();
			if (max_sum == company_income[i].income_sum()) {
				best_branch_numbers[counter] = company_income[i].branch_number();
				++counter;
			}
		}
		return best_branch_numbers;
			// пробегаться по массиву массивов в котором хранится сумма и номер филиала или использовать 2 массива (1 сумма, 2й номера филиалов)
	}
	
	friend std::ifstream& operator>>(std::ifstream& in, Company_data& ref);
	friend std::istream& operator>>(std::istream& in, Company_data& ref);
	friend std::ostream& operator<<(std::ostream& in, const Company_data& ref);
};
//Входные данные: количество элементов массива, экземпляры класса. 
/*
Классы: Данные о прибыли; Данные о компании
Формат: Номер филиала, Число дней, Прибыль
Пример: 1 5 100 78.5 1000 0 1000; StroyDom 2 1 2 200 400 2 200 0
Обработка: Определение номера филиала с максимальной выручкой
*/

std::ifstream& operator>>(std::ifstream& in, Company_data& ref) {
	in >> ref.company_name >> ref.amount_of_branches;
	if (ref.company_income != nullptr) {
		delete[] ref.company_income;
		ref.company_income = nullptr;
	}
	ref.company_income = new Income_data[ref.amount_of_branches];
	for (int i = 0; i < ref.amount_of_branches; ++i) 
		in >> ref.company_income[i];
	if (in.peek() == '\n')
		in.get();
	return in;
}
std::istream& operator>>(std::istream& in, Company_data& ref) {
	std::cout << "Enter company name: ";
	in >> ref.company_name;
	std::cout << "Enter amount of " << ref.company_name << " branches: ";
	in >> ref.amount_of_branches;
	if (ref.company_income != nullptr) {
		delete[] ref.company_income;
		ref.company_income = nullptr;
	}
	ref.company_income = new Income_data[ref.amount_of_branches];
	for (int i = 1; i <= ref.amount_of_branches; ++i)
		in >> ref.company_income[i];
	if (in.peek() == '\n')
		in.get();
	return in;
}

std::ostream& operator<<(std::ostream& out, Company_data& ref) {
	int* best_branches = ref.best_branch();
	out << "Best branch is\n";
	for (int i = 1; i <= best_branches[0]; ++i) 
		out << best_branches[i] << ' ';
	return out;
}
void file_input(Income_data& data) {
	std::cout << "Input path.\n";
	std::string path;
	std::cin >> path;
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
	std::cin >> path;
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

int main() {

}