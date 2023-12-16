#include <iostream>
#include <vector>
#include <string>

class big_integer {
	std::string number;
public:
	//конструктор
	big_integer(std::string number) {
		this->number = number;
	}
	//конструктор перемещения
	big_integer(big_integer&& other) {
		number = std::move(other.number);
	}
	//перемещающее присваивание
	big_integer& operator= (big_integer&& other){
		if (this == &other) {
			return *this;
		}
		number = std::move(other.number);
		return *this;
	}

	//оператор сложения
	big_integer operator+(big_integer& other) {
		//записываем длину большей строки и меньшей строки
		int shorter_end = number.length();
		int longer_end = other.number.length();
		//и запоминаем указатель на строку подлиннее
		std::string* str_ptr = &other.number;

		//если не угадали в начале, меняем
		if (number.length() > other.number.length()) {
			std::swap(shorter_end,longer_end);
			str_ptr = &number;
		} 
		std::string buf1, buf2;//буфер для тройки от больших чисел
		int tik = 0;//счетчик
		bool perenos = false;//флаг для обозначения наличия переноса
		std::vector<std::string> final_string;//вектор для записи промежуточного результата
		int number_end = number.length()-1;//конец левого
		int other_num_end = other.number.length()-1;//конец правого большого числа
		

		//крутим, пока не дойдем до конца меньшей строки
		while (tik < shorter_end) {
			buf1 = number.substr(number_end - tik, 1);//берем с конца 1число от каждого большого числа
			buf2 = other.number.substr(other_num_end - tik, 1);
			//потом складываем
			int summ = std::stoi(buf1) + std::stoi(buf2);
			//если сумма получилась больше 9, тогда запоминаем 1 для переноса
			std::string summ_str;//буфер суммы в виде строки
			//если есть число для перенса, то добавляем к сумме 1 и обнуляем статус переноса
			if (perenos == true) {
				summ++;
				perenos == false;
			}

			if (summ > 9) {
				perenos = true;
				//если равно 10, то пишем
				if (summ == 10) {
					summ_str = "0";
				}
				//в противном случае пишем разницу суммы от 10
				else {
					summ_str = std::to_string(summ - 10);
				}
			}
			//если меньше или равно 9, то просто превращаем в строку
			else {
				summ_str = std::to_string(summ);
			}
			//и вставляем в начало вектора
			final_string.insert(final_string.begin(), summ_str);
			tik++;//увеличиваем счетчик

			//std::cout << buf2;
		}
		//когда дошли до конца меньшей строки, проверяем, есть ли еще что-то в более длвинной
		while (tik < longer_end) {
			buf1 = (*str_ptr).substr(longer_end - tik, 1);
			final_string.insert(final_string.begin(), buf1);
			tik++;
		}
		//после этого вытаскиваем набор строк из вектора в одну строку и возвращаем конструктор класса с ней
		std::string buf;
		for (auto& x: final_string) {
			buf += std::move(x);
		}
		return big_integer(buf);
		
	}

	//оператор вывода
	friend std::ostream& operator<<(std::ostream &os, big_integer& x) {
		return os << x.number;
	}
};

int main() {
	auto number1 = big_integer("114575");
	auto number2 = big_integer("78524");
	auto result = number1 + number2;
	std::cout << result; // 193099
	return 0;
}