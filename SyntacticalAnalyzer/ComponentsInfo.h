#pragma once
#include <vector>
#include <string>
#include "Poliz.h"
/*!
 * \brief Класс, содержащий в себе информацию о функции
 *
 * Класс создан, чтобы упростить сохранение информации о функциях, а именно тип,
 * имя и аргументы
 */
class FunctionInfo {
 public:
  /*!
   * @brief Конструктор по типу возвращаемого значения функции, имени и списку
   * аргументов
   * @param type Тип возвращаемого значения функции
   * @param name Имя функции
   * @param args Аргументы функции
   */
  FunctionInfo(std::string type, std::string name,
               std::vector<std::pair<std::string,std::string>> args)
      : type_(type), name_(name), args_(args) {}
  /*!
   * @brief Конструктор по умолчанию
   */
  FunctionInfo() {}
  /*!
   * @brief Геттер type_
   * @return Значение type_
   */
  std::string type() const { return type_; }
  /*!
   * @brief Сеттер type_
   * @param type Новый тип функции
   */
  void set_type(std::string type) { type_ = type; }
  /*!
   * @brief Геттер name_
   * @return Значение name_
   */
  std::string name() const { return name_; }
  /*!
   * @brief Геттер для аргументов функции
   * @return Аргументы функции args_
   */
  std::vector<std::pair<std::string, std::string>> args() const {
    return args_;
  }
  /*!
   * @brief Геттер poliz_start_
   * @return Значение poliz_start_
   */
  int poliz_start() const { return poliz_start_; }
  void set_poliz_start(int poliz_start) { poliz_start_ = poliz_start; }
  void AddLocalVar(Var* var) { local_vars_.push_back(var); }
  void AddArgument(Var* var) { arguments_.push_back(var); }

  std::vector<Var*> arguments() const { return arguments_; }
  std::vector<Var*> local_vars() const { return local_vars_; }

 private:
  std::string type_;  ///< Тип возвр. значение функции
  std::string name_;  ///< Имя функции
  std::vector<std::pair<std::string, std::string>>
      args_;         ///< Аргументы функции
  int poliz_start_;  ///< Точка нчала кода функции в ПОЛИЗ
  std::vector<Var*> arguments_;
  std::vector<Var*> local_vars_;
};

/*!
 * \brief Класс, содержащий в себе информацию о структуре
 *
 * Класс создан, чтобы упростить сохранение информации о страуктурах, а именно
 * имя структуры и ее поля данных - методы и переменные
 *
 */
class StructInfo {
 public:
  /*!
   * @brief Конструктор по названию структуры, также коструктор по умолчанию
   * @param type Имя структуры
   */
  StructInfo(std::string type = "") : typename_(type) {}
  /*!
   * @brief Геттер name_
   * @return Значение name_
   */
  std::string name() const { return typename_; }
  /*!
   * @brief Добавляет метод в структуру
   * @param func ИТнформация о методе
   */
  void AddMethod(FunctionInfo func) { methods_.push_back(func); }
  /*!
   * @brief Добавляет переменную в структуру
   * @param type Тип новой переменной
   * @param name Имя новой переменной
   */
  void AddVariable(std::string type, std::string name) {
    variables_.emplace_back(type, name);
  }
  /*!
   * @brief Возвращает тип найденной переменной в структуре
   * @param name Имя искомой переменной
   * @return Тип найденной переменной, иначе пустая строка
   */
  std::string FindVariable(std::string name) const {
    for (auto val : variables_) {
      if (val.second == name) return val.first;
    }
    return "";
  }
  /*!
   * @brief Ищет метод в структуре
   * @param name Имя искомого метода
   * @return Информацию о методе. Если не нашел -- информация, сконструированная
   * по умолчанию
   */
  FunctionInfo FindMethod(std::string name) const {
    for (auto val : methods_) {
      if (val.name() == name) return val;
    }
    return FunctionInfo();
  }

 private:
  std::string typename_;               ///< Имя структуры
  std::vector<FunctionInfo> methods_;  ///< Массив методов структуры
  std::vector<std::pair<std::string, std::string>>
      variables_;  ///< Переменные в структуре
};
