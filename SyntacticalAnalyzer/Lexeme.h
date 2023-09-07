#pragma once
#include <string>
/*!
 * \file
 * \brief �������� �������� ������ ������� � ����� ������
 */

//! ��������� ���� ������
enum LexemeTypes {
  Keyword = 1,         ///< �������� �����
  Id = 2,              ///< �������������/���
  NumLit = 3,          ///< �������� �������
  Punctuation = 5,     ///< �������, ����� � �������
  RoundBraces = 6,     ///< ������� ������
  SquareBraces = 7,    ///< ���������� ������
  CurlyBraces = 8,     ///< �������� ������
  TriangleBraces = 9,  ///< ����������� ������
  StringLit = 10,      ///< ��������� �������
  Quotes = 11,         ///< �������
  Priority0 = 12,  ///< ����������� ������� ��������� ++, --, !
  Priority1 = 13,  ///< ���������� ������� ��������� ++, --, !
  Priority2 = 14,  ///< �������� ** - ���������� � �������
  Priority3 = 15,  ///< �������� *, /, %
  Priority4 = 16,  ///< �������� +, -
  Priority5 = 17,  ///< �������� <, >, <=, >=
  Priority6 = 18,  ///< �������� ==, !=
  Priority7 = 19,  ///< �������� &&
  Priority8 = 20,  ///< �������� ||
  Priority9 = 21,  ///< �������� =, +=, -=, *=, /=, %=
  Priority10 = 22,  ///< ������� ��� �������� � ���������
  BoolLit = 23,  ///< ���������� �������
};

/*!
 * \brief ����� �������
 *
 * ����� ������� �������� � ���� ��� �������, ���� ������� � ����� ������� ����,
 * � ������� ��� ��������. ����� ����� ����������� ���������� ��������� ���
 * �������� �������������
 */
class Lexeme {
 public:
  /*!
   * @brief ������������ ������� �� ����, ������ � ������ ����, �� ������� ���
   * ���������
   */
  Lexeme(LexemeTypes type, std::string text, int line)
      : type_(type), text_(text), line_(line) {}
  /*!
   * @brief ����������� �� ���������
   * ��� ������� ����� ����� ��������� �����, ����� - keyword
   */
  Lexeme() : type_(LexemeTypes::Keyword), text_("keyword") {}

  /*!
   * @brief ������ ��� ���� type_
   * @return type_
   */
  LexemeTypes type() const { return type_; }
  /*!
   * @brief ������ text_
   * @return text_
   */
  std::string text() const { return text_; }
  /*!
   * @brief ������ line_
   * @return line_
   */
  int line() const { return line_; }
  friend bool operator==(Lexeme& lhs, const char* str);
  friend bool operator==(const char* lhs, Lexeme& str);
  friend bool operator!=(Lexeme& lhs, const char* str);
  friend bool operator!=(const char* lhs, Lexeme& str);

 private:
  LexemeTypes type_;  ///< ��� �������
  std::string text_;  ///< ����� �������
  int line_;  ///< ������ �� ������� ��������� �������
};

/*!
 * @brief ���������� ������� �� ��������� ���������
 * @param lhs
 * @param str
 * @return true � false ��������������
 * ��� ��������� ������� ����� �������
 */
bool operator==(Lexeme& lhs, const char* str) { return lhs.text_ == str; }
bool operator==(const char* lhs, Lexeme& str) { return lhs == str.text_; }
/*!
 * @brief �� ��, ��� � �������� ==, ������ ��������
 * @param lhs
 * @param str
 * @return true � false ��������������
 */
bool operator!=(Lexeme& lhs, const char* str) { return lhs.text_ != str; }
bool operator!=(const char* lhs, Lexeme& str) { return lhs != str.text_; }
