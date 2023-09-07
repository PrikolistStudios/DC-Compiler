#pragma once
#include <string>
/*!
 * \file
 * \brief Содержит описание класса лексемы и типов лексем
 */

//! Различные типы лексем
enum LexemeTypes {
  Keyword = 1,         ///< Ключевое слово
  Id = 2,              ///< Идентификатор/имя
  NumLit = 3,          ///< Числовой литерал
  Punctuation = 5,     ///< Запятые, точка с запятой
  RoundBraces = 6,     ///< Круглые скобки
  SquareBraces = 7,    ///< Квадратные скобки
  CurlyBraces = 8,     ///< Фигурные скобки
  TriangleBraces = 9,  ///< Треугольнык скобки
  StringLit = 10,      ///< Строковый литерал
  Quotes = 11,         ///< Кавычки
  Priority0 = 12,  ///< Постфиксные унарные операторы ++, --, !
  Priority1 = 13,  ///< Префиксные унарные операторы ++, --, !
  Priority2 = 14,  ///< Операция ** - возведение в степень
  Priority3 = 15,  ///< Операции *, /, %
  Priority4 = 16,  ///< Операции +, -
  Priority5 = 17,  ///< Операции <, >, <=, >=
  Priority6 = 18,  ///< Операции ==, !=
  Priority7 = 19,  ///< Операция &&
  Priority8 = 20,  ///< Операция ||
  Priority9 = 21,  ///< Операции =, +=, -=, *=, /=, %=
  Priority10 = 22,  ///< Запятая как операция в выражении
  BoolLit = 23,  ///< Логический литерал
};

/*!
 * \brief Класс лексемы
 *
 * Класс лексемы содержит в себе тип лексемы, саму лексему и номер строчки кода,
 * в которой она написана. Класс также перегружает операторые сравнения для
 * удобсвта использования
 */
class Lexeme {
 public:
  /*!
   * @brief Конструирует лексему по типу, тексту и строке кода, на которой она
   * находится
   */
  Lexeme(LexemeTypes type, std::string text, int line)
      : type_(type), text_(text), line_(line) {}
  /*!
   * @brief Конструктор по умолчанию
   * Тип лексемы будет равен ключевому слову, текст - keyword
   */
  Lexeme() : type_(LexemeTypes::Keyword), text_("keyword") {}

  /*!
   * @brief Геттер для поля type_
   * @return type_
   */
  LexemeTypes type() const { return type_; }
  /*!
   * @brief Геттер text_
   * @return text_
   */
  std::string text() const { return text_; }
  /*!
   * @brief Геттер line_
   * @return line_
   */
  int line() const { return line_; }
  friend bool operator==(Lexeme& lhs, const char* str);
  friend bool operator==(const char* lhs, Lexeme& str);
  friend bool operator!=(Lexeme& lhs, const char* str);
  friend bool operator!=(const char* lhs, Lexeme& str);

 private:
  LexemeTypes type_;  ///< Тип лексемы
  std::string text_;  ///< Текст лексемы
  int line_;  ///< Строка на которой находится лексема
};

/*!
 * @brief Сравнивает лексемы со строковым литералом
 * @param lhs
 * @param str
 * @return true и false соответственно
 * Для сравнения берется текст лексемы
 */
bool operator==(Lexeme& lhs, const char* str) { return lhs.text_ == str; }
bool operator==(const char* lhs, Lexeme& str) { return lhs == str.text_; }
/*!
 * @brief То же, что и оператор ==, только наоборот
 * @param lhs
 * @param str
 * @return true и false соответственно
 */
bool operator!=(Lexeme& lhs, const char* str) { return lhs.text_ != str; }
bool operator!=(const char* lhs, Lexeme& str) { return lhs != str.text_; }
