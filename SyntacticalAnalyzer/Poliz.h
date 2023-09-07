#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <math.h>
#include <istream>
#include <ostream>
/*!
*  @file
 * @brief Файл содержит описание классов элементов ПОЛИЗа и всех сопутствующих структур
*/

/*!
 * @brief Типы элементов ПОЛИЗа
*/
enum PolizElemTypes {
  Variable = 0, ///< Адрес переменной
  Oper = 1, ///< Операция
  Address = 2, ///< Адрес (индекс) элемента полиза
  Lit = 3, ///< Литерал
  Func = 4 ///< Операция вызова функции
};

/*!
 * @brief Типы переменных
*/
enum VarTypes {
  Int = 0, ///< Целочисленное число
  Char = 1, ///< Символ
  Float = 2, ///< Вещественное число
  Bool = 3, ///< Булевая переменная
  String = 4 ///< Строковая переменная
};

/*!
 * @brief Переводит тип переменной VarTypes в std::string
 * @param type Тип переменной, который надо преобразовать в строку
 * @return Строка, соответствующая типу переменной
*/
std::string ToString(VarTypes type) {
  std::vector<std::string> mas = {"int", "char", "float", "bool", "string"};
  return mas[static_cast<int>(type)];
}

/*!
 * @brief Переводит строку в тип переменной VarTypes
 * @param type Строка, которую надо преобразовать в VarTypes
 * @return Тип переменной VarTypes, соответствующий строке
*/
VarTypes ToVarType(std::string type) {
  if (type == "string") {
    return String;
  } else if (type == "int") {
    return Int;
  } else if (type == "float") {
    return Float;
  } else if (type == "char") {
    return Char;
  } else {
    return Bool;
  }
}

/*!
 * @brief Базовый класс элемента полиза
 * ПОЛИЗ является массивом указателей на переменные этого класса, от которого наследуются другие классы элементов ПОЛИЗа
*/
class PolizElem {
 public:
  /*!
   * @brief Конструктор по типу элемента ПОЛИЗа
   * @param type Тип элемента ПОЛИЗа
  */
  PolizElem(PolizElemTypes type) : type_(type) {}
  /*!
   * @brief Геттер переменной type_
   * @return Значение переменной type_
  */
  PolizElemTypes type() const { return type_; }

  /*!
   * @brief Виртуальный деструктор, чтобы класс был полиморфным
  */
  virtual ~PolizElem() {}
 private:
  PolizElemTypes type_; ///< Тип элемента полиза
};

/*!
 * @brief Класс элемента ПОЛИЗа - операции
*/
class PolizOper :  public PolizElem {
 public:
  /*!
   * @brief Конструктор операции в ПОЛИЗе по операции
   * @param oper Операция, которую надо добавить в ПОЛИЗ
  */
  PolizOper(std::string oper)
      : PolizElem(Oper), oper_(oper) {}
  /*!
   * @brief Геттер переменной oper_
   * @return Значение переменной oper_
  */
  std::string oper() const { return oper_; }
 private:
  std::string oper_; ///< Операция в ПОЛИЗе
};

/*!
 * @brief Класс элемента ПОЛИЗа - адреса
 * Чтобы выполнять операции перехода по ложности, безусловного перехода и т. д. нужно хранить в ПОЛИЗе адрес ячейки, на которую надо осуществить переход. Для этого и используется этот класс
*/
class PolizAddress :  public PolizElem {
 public:
  /*!
   * @brief Конструктор адреса (индекса) элемента ПОЛИЗа по адресу
   * @param address Индекс элемента ПОЛИЗа, который нужно добавить в ПОЛИЗ
  */
  PolizAddress(int address)
      : PolizElem(Address), address_(address) {}
  /*!
   * @brief Геттер переменной address_
   * @return Значение переменной address_
  */
  int address() const { return address_; }

 private:
  int address_; ///< Адрес (индекс) элемента ПОЛИЗа
};

struct Var;

/*!
 * @brief Класс элемента ПОЛИЗа - адреса переменной
 * Для доступа к переменной в ПОЛИЗе необходимо хранить ее адрес, для чего и нужен этот класс
*/
class PolizVar :  public PolizElem {
 public:
  /*!
   * @brief Конструктор элемента ПОЛИЗа по хранимому адресу переменной
   * @param var Адрес переменной, который надо добавить в ПОЛИЗ
  */
  PolizVar(Var* var) : PolizElem(Variable),var_(var){}
  /*!
   * @brief Геттер переменной var_
   * @return Значение переменной var_
  */
  Var* var() const { return var_; }
 private:
  Var* var_; ///< Адрес переменной, который хранится в ПОЛИЗе
};

/*!
 * @brief Структура данных, которые физически хранятся в переменной, в также в литералах
 * Содержит множество перегруженных операций, которые используются в интерпретаторе. Перегруженные операции нужны для использования операций с операндами разных типов, то есть для приведения одних типов к другим.
*/
struct VarData {
  /*!
   * @brief Конструктор по типу данных, которые хранятся в структуре
   * @param type Тип данных
  */
  VarData(VarTypes type) : type_(type) {}
  /*!
   * @brief Конструктор по целочисленному значению данных
   * @param val Целочисленное значение, которое хранится в структуре
  */
  VarData(int val) : type_(Int), int_(val) {}
  /*!
   * @brief Конструктор по символьному значению данных
   * @param val Символьное значение, которое хранится в структуре
  */
  VarData(char val) : type_(Char), char_(val) {}
  /*!
   * @brief Конструктор по вещественному значению данных
   * @param val Вещественное значение, которое хранится в структуре
   */
  VarData(float val) : type_(Float), float_(val) {}
  /*!
   * @brief Конструктор по булевому значению данных
   * @param val Булевое значение, которое хранится в структуре
   */
  VarData(bool val) : type_(Bool), bool_(val) {}
  /*!
   * @brief Конструктор по строковому значению данных
   * @param val Строковое значение, которое хранится в структуре
   */
  VarData(std::string val) : type_(String), string_(val) {}

  int int_ = 0; ///< Целочисленное значение, которое будет храниться в структуре, если тип данных соответствующий
  bool bool_ = false; ///< Булевое значение, которое будет храниться в структуре, если тип данных соответствующий
  std::string string_ = ""; ///< Строковое значение, которое будет храниться в структуре, если тип данных соответствующий
  char char_ = '0'; ///< Символьное значение, которое будет храниться в структуре, если тип данных соответствующий
  float float_ = 0.f; ///< Вещественное значение, которое будет храниться в структуре, если тип данных соответствующий
  VarTypes type_; ///< Тип данных, которые хранятся в структуре. В соответствии с типом данных в операциях будет использоваться соответствующее поле - вещественное число, целочисленное и т. д.

  VarData operator+(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ + rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ + rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ + rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(int_ + rhs.float_);
      } else if (rhs.type_ == String) {
        return VarData((char)int_ + rhs.string_);
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ + rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ + rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ + rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(bool_ + rhs.float_);
      } else if (rhs.type_ == String) {
        return VarData((char)bool_ + rhs.string_);
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ + rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ + rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ + rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(char_ + rhs.float_);
      } else if (rhs.type_ == String) {
        return VarData(char_ + rhs.string_);
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(float_ + rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(float_ + rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(float_ + rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(float_ + rhs.float_);
      } else if (rhs.type_ == String) {
        return VarData((char)(int)float_ + rhs.string_);
      }
    } else if (type_ == String) {
      if (rhs.type_ == Int) {
        return VarData(string_ + (char)rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(string_ + (char)rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(string_ + rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(string_ + (char)rhs.float_);
      } else if (rhs.type_ == String) {
        return VarData(string_ + rhs.string_);
      }
    }
  }
  VarData operator-(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ - rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ - rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ - rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(int_ - rhs.float_);
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ - rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ - rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ - rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(bool_ - rhs.float_);
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ - rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ - rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ - rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(char_ - rhs.float_);
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(float_ - rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(float_ - rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(float_ - rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(float_ - rhs.float_);
      }
    }
  }
  VarData operator*(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ * rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ * rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ * rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(int_ * rhs.float_);
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ * rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ * rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ * rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(bool_ * rhs.float_);
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ * rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ * rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ * rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(char_ * rhs.float_);
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(float_ * rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(float_ * rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(float_ * rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(float_ * rhs.float_);
      }
    }
  }
  VarData operator/(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ / rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ / rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ / rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(int_ / rhs.float_);
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ / rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ / rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ / rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(bool_ / rhs.float_);
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ / rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ / rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ / rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(char_ / rhs.float_);
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(float_ / rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(float_ / rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(float_ / rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(float_ / rhs.float_);
      }
    }
  }
  VarData operator%(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ % rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ % rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ % rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(std::fmodf(int_, rhs.float_));
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ % rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ % rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ % rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(std::fmodf(bool_, rhs.float_));
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ % rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ % rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ % rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(std::fmodf(char_, rhs.float_));
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(std::fmodf(float_, rhs.int_));
      } else if (rhs.type_ == Bool) {
        return VarData(std::fmodf(float_, rhs.bool_));
      } else if (rhs.type_ == Char) {
        return VarData(std::fmodf(float_, rhs.char_));
      } else if (rhs.type_ == Float) {
        return VarData(std::fmodf(float_, rhs.float_));
      }
    }
  }

  /*!
   * @brief Применяет операцию факториал к данным, хранящимся в структуре
   * @return Факториал от данных структуры
  */
  VarData Factorial() {
    int res = 1, tmp;
    if (type_ == Int) {
      tmp = int_;
    } else if (type_ == Bool) {
      tmp = bool_;
    } else if (type_ == Char) {
      tmp = char_;
    } else if (type_ == Float) {
      tmp = float_;
    }

    if (tmp < 0) {
      throw std::invalid_argument("Factorial operation on negative number");
    }

    while (tmp) res *= tmp--;
    return VarData(res);
  }
  /*!
   * @brief Применяет операцию унарный плюс к данным
   * @return Значение данных полученных в результате применения к ним унарного плюса
  */
  VarData UnPlus() { return *this; }
  /*!
   * @brief Применяет операцию унарный минус к данным
   * @return Значение данных полученных в результате применения к ним унарного минуса
   */
  VarData UnMinus() {
    if (type_ == Int) {
      return VarData(-int_);
    } else if (type_ == Bool) {
      return VarData(-bool_);
    } else if (type_ == Char) {
      return VarData(-char_);
    } else if (type_ == Float) {
      return VarData(-float_);
    }
  }
  /*!
   * @brief Применяет операцию отрицание к данным
   * @return Значение данных полученных в результате применения к ним отрицания
   */
  VarData Negation() {
    if (type_ == Int) {
      return VarData(!int_);
    } else if (type_ == Bool) {
      return VarData(!bool_);
    } else if (type_ == Char) {
      return VarData(!char_);
    } else if (type_ == Float) {
      return VarData(!float_);
    }
  }

  /*!
   * @brief Находит степень целочисленного числа
   * @param num Основание степени 
   * @param deg Показатель степени
   * @return Необходимая степень числа
  */
  int Power(int num, int deg) {
    if (deg == 0) return 1;
    if (deg % 2 == 0) return Power(num, deg / 2) * Power(num, deg / 2);
    return num * Power(num, deg - 1);
  }
  /*!
   * @brief Возводит одни данные в степень других данных
   * @param rhs Правый операнд операции возведения в степень
   * @return Данные, полученные в результате возведения одних данных в степень других
  */
  VarData Power(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(Power(int_, rhs.int_));
      } else if (rhs.type_ == Bool) {
        return VarData(Power(int_, rhs.bool_));
      } else if (rhs.type_ == Char) {
        return VarData(Power(int_, rhs.char_));
      } else if (rhs.type_ == Float) {
        return VarData(std::powf(int_, rhs.float_));
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(Power(bool_, rhs.int_));
      } else if (rhs.type_ == Bool) {
        return VarData(Power(bool_, rhs.bool_));
      } else if (rhs.type_ == Char) {
        return VarData(Power(bool_, rhs.char_));
      } else if (rhs.type_ == Float) {
        return VarData(std::powf(bool_, rhs.float_));
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(Power(char_, rhs.int_));
      } else if (rhs.type_ == Bool) {
        return VarData(Power(char_, rhs.bool_));
      } else if (rhs.type_ == Char) {
        return VarData(Power(char_, rhs.char_));
      } else if (rhs.type_ == Float) {
        return VarData(std::powf(char_, rhs.float_));
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(std::powf(float_, rhs.int_));
      } else if (rhs.type_ == Bool) {
        return VarData(std::powf(float_, rhs.bool_));
      } else if (rhs.type_ == Char) {
        return VarData(std::powf(float_, rhs.char_));
      } else if (rhs.type_ == Float) {
        return VarData(std::powf(float_, rhs.float_));
      }
    }
  }

  VarData operator<(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ < rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ < rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ < rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(int_ < rhs.float_);
      } else if (rhs.type_ == String) {
        std::string tmp;
        tmp += (char)int_;
        return VarData(tmp < rhs.string_);
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ < rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ < rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ < rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(bool_ < rhs.float_);
      } else if (rhs.type_ == String) {
        std::string tmp;
        tmp += (char)bool_;
        return VarData(tmp < rhs.string_);
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ < rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ < rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ < rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(char_ < rhs.float_);
      } else if (rhs.type_ == String) {
        std::string tmp;
        tmp += char_;
        return VarData(tmp < rhs.string_);
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(float_ < rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(float_ < rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(float_ < rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(float_ < rhs.float_);
      } else if (rhs.type_ == String) {
        std::string tmp;
        tmp += (char)float_;
        return VarData(tmp < rhs.string_);
      }
    } else if (type_ == String) {
      if (rhs.type_ == Int) {
        std::string tmp;
        tmp += (char)rhs.int_;
        return VarData(string_ < tmp);
      } else if (rhs.type_ == Bool) {
        std::string tmp;
        tmp += (char)rhs.bool_;
        return VarData(string_ < tmp);
      } else if (rhs.type_ == Char) {
        std::string tmp;
        tmp += (char)rhs.char_;
        return VarData(string_ < tmp);
      } else if (rhs.type_ == Float) {
        std::string tmp;
        tmp += (char)(int)rhs.float_;
        return VarData(string_ < tmp);
      } else if (rhs.type_ == String) {
        return VarData(string_ < rhs.string_);
      }
    }
  }
  VarData operator>=(VarData rhs) { return VarData(!((*this) < rhs).bool_); }
  VarData operator>(VarData rhs) { return rhs < *this; }
  VarData operator<=(VarData rhs) { return VarData(!((*this) > rhs).bool_); }
  VarData operator==(VarData rhs) {
    return VarData(!(*this < rhs).bool_ && !(*this > rhs).bool_);
  }
  VarData operator!=(VarData rhs) { return VarData(!(*this == rhs).bool_); }
  VarData operator&&(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ && rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ && rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ && rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(int_ && rhs.float_);
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ && rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ && rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ && rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(bool_ && rhs.float_);
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ && rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ && rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ && rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(char_ && rhs.float_);
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(float_ && rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(float_ && rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(float_ && rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(float_ && rhs.float_);
      }
    }
  }
  VarData operator||(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        return VarData(int_ || rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(int_ || rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(int_ || rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(int_ || rhs.float_);
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        return VarData(bool_ || rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(bool_ || rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(bool_ || rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(bool_ || rhs.float_);
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        return VarData(char_ || rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(char_ || rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(char_ || rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(char_ || rhs.float_);
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        return VarData(float_ || rhs.int_);
      } else if (rhs.type_ == Bool) {
        return VarData(float_ || rhs.bool_);
      } else if (rhs.type_ == Char) {
        return VarData(float_ || rhs.char_);
      } else if (rhs.type_ == Float) {
        return VarData(float_ || rhs.float_);
      }
    }
  }
  VarData& operator=(VarData rhs) {
    if (type_ == Int) {
      if (rhs.type_ == Int) {
        int_ = rhs.int_;
      } else if (rhs.type_ == Bool) {
        int_ = rhs.bool_;
      } else if (rhs.type_ == Char) {
        int_ = rhs.char_;
      } else if (rhs.type_ == Float) {
        int_ = rhs.float_;
      }
    } else if (type_ == Bool) {
      if (rhs.type_ == Int) {
        bool_ = rhs.int_;
      } else if (rhs.type_ == Bool) {
        bool_ = rhs.bool_;
      } else if (rhs.type_ == Char) {
        bool_ = rhs.char_;
      } else if (rhs.type_ == Float) {
        bool_ = rhs.float_;
      }
    } else if (type_ == Char) {
      if (rhs.type_ == Int) {
        char_ = rhs.int_;
      } else if (rhs.type_ == Bool) {
        char_ = rhs.bool_;
      } else if (rhs.type_ == Char) {
        char_ = rhs.char_;
      } else if (rhs.type_ == Float) {
        char_ = rhs.float_;
      }
    } else if (type_ == Float) {
      if (rhs.type_ == Int) {
        float_ = rhs.int_;
      } else if (rhs.type_ == Bool) {
        float_ = rhs.bool_;
      } else if (rhs.type_ == Char) {
        float_ = rhs.char_;
      } else if (rhs.type_ == Float) {
        float_ = rhs.float_;
      }
    } else if (type_ == String) {
      if (rhs.type_ == Int) {
        string_ = rhs.int_;
      } else if (rhs.type_ == Bool) {
        string_ = rhs.bool_;
      } else if (rhs.type_ == Char) {
        string_ = rhs.char_;
      } else if (rhs.type_ == Float) {
        string_ = rhs.float_;
      }
    }
    return *this;
  }
  VarData& operator+=(VarData rhs) { return *this = *this + rhs; }
  VarData& operator-=(VarData rhs) { return *this = *this - rhs; }
  VarData& operator*=(VarData rhs) { return *this = *this * rhs; }
  VarData& operator/=(VarData rhs) { return *this = *this / rhs; }
  VarData& operator%=(VarData rhs) { return *this = *this % rhs; }
};

/*!
 * @brief Перегружает оператор <<, чтобы можно было удобно выводить данные VarData на экран
 * @param os Выходной поток
 * @param data Данные, которые нужно вывести в поток
 * @return Выходной поток для каскадных операций
*/
std::ostream& operator<<(std::ostream& os, VarData const& data) {
  if (data.type_ == Int) {
    return os << data.int_;
  } else if (data.type_ == Float) {
    return os << data.float_;
  } else if (data.type_ == Bool) {
    return os << data.bool_;
  } else if (data.type_ == Char) {
    return os << data.char_;
  } else {
    return os << data.string_;
  }
}

/*!
 * @brief Перегружает оператор <<, чтобы можно было удобно вводить данные
 * VarData
 * @param is Входной поток
 * @param data Данные, которые должен ввести пользователь
 * @return Входной поток для каскадных операций
 */
std::istream& operator>>(std::istream& is, VarData& data) {
  if (data.type_ == Int) {
    return is >> data.int_;
  } else if (data.type_ == Float) {
    return is >> data.float_;
  } else if (data.type_ == Bool) {
    return is >> data.bool_;
  } else if (data.type_ == Char) {
    return is >> data.char_;
  } else {
    return is >> data.string_;
  }
}

/*!
 * @brief Класс элемента ПОЛИЗа - литерала
 * То же самое что и rvalue в других ЯП
*/
class PolizLit : public PolizElem {
 public:
  /*!
   * @brief Конструктор литерала по типу хранящихся данных
   * @param type Тип данных литерала
  */
  PolizLit(VarTypes type) : PolizElem(Lit), data_(type) {}
  /*!
   * @brief Конструктор литерала по данным, которые в нем хранятся
   * @param data Данные, которые должны храниться в литерале
  */
  PolizLit(VarData data) : PolizElem(Lit), data_(data) {}
  VarData data_; ///< Данные литерала

 private:
};

/*!
 * @brief Класс элемента ПОЛИЗа - операции вызова функции
*/
class PolizFuncCall : public PolizElem {
 public:
  /*!
   * @brief Конструирует операцию вызова функции по кол-ву аргументов и имени вызываемой функции
   * @param args Кол-во аргументов функции
   * @param func Имя вызываемой функции
  */
  PolizFuncCall(int args, std::string func)
      : PolizElem(Func), arguments_(args), func_name_(func) {}
  /*!
   * @brief Геттер переменной arguments_
   * @return Значение переменной arguments_
  */
  int arguments() const { return arguments_; }
  /*!
   * @brief Геттер переменной func_name_
   * @return Значение переменной func_name_
  */
  std::string func_name() const { return func_name_; }

 private:
  int arguments_; ///< Кол-во аргументов у вызываемой функции
  std::string func_name_; ///< Имя вызываемой функции
};

/*!
 * @brief Структура переменной, адрес который хранится в ПОЛИЗе
*/
struct Var {
  /*!
   * @brief Конструктор переменной
   * @param type Тип данных переменной
   * @param size Размер массива, если переменная является массивом
  */
  Var(VarTypes type, int size = 0) : data_(type) {
    if (size > 0) {
      for (int i = 0; i < size; ++i) {
        array_.push_back(new Var(type));
      }
    }
  }
  /*!
   * @brief Конструктор по умолчанию
  */
  Var() : data_(Int) {}

  
  VarData data_; ///< Данные, хранящиеся в переменной

  std::vector<Var*> array_; ///< Массив из адресов переменных, если корневая переменная является одномерным массивом
};
