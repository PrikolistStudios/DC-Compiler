#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
/*!
* \file
* \brief ���� �������� �������� ������ Separator
*/

/*!
* \brief ����� ������������ �����������
* 
* ����� ������������ ��� ���������� ������ ��������� ���� �� ������� - ����������� ������. ��������� �� ���� ���� � ������� ��������� ���� ("text.txt"), ����������� �� ����� DC++, �� ������ ������ ������������������ ������ � �������, ����� � ������� ����, � ������� ��� �������� ("separated.txt")
*/
class Separator {
 public:
  /*!
   * @brief ����������� �� ���������
  */
  Separator() {
    std::ifstream is("reserved.txt");
    while (!is.eof()) {
      std::string now;
      is >> now;
      reserved.push_back(now);
    }
    os.open("separated.txt");
    last = ' ';
    now = "";
    type = 0;
  }

  /*!
   * @brief ���������� ��� ������� ��� ��������
   * @param c ������, ��� �������� ��������� ���������� ��� �������
   * @return ��� �������
  */
  int getType(char c) {
    if (c == '(' || c == ')') return 6;
    if (c == '[' || c == ']') return 7;
    if (c == '{' || c == '}') return 8;
    if (c == '<' || c == '>') return 9;
    if (c == ';') return 5;
    return 4;
  }

  /*!
   * @brief ���������, �������� �� ������ ������
   * @param c ������, ������� ��������� ���������
   * @return true � false ��������������
  */
  bool isLetter(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
  }

  /*!
   * @brief ���������, �������� �� ������ ������
   * @param c ������, ������� ��������� ���������
   * @return true � false ��������������
   */
  bool isDigit(char c) { return (c >= '0' && c <= '9'); }
  
  /*!
   * @brief ���������, ����� �� ������ ���� ������������ ����� ������� �� ������
   * @param c ������, ������� ��������� ���������
   * @return true � false ��������������
   */
  bool isDivider(char c) {
    return (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' ||
            c == ']' || c == '<' || c == '>' || c == '+' || c == '-' ||
            c == '^' || c == '|' || c == '&' || c == '%' || c == '/' ||
            c == '*' || c == ':' || c == '!' || c == '=' || c == ' ' ||
            c == '"' || c == '?' || c == '\n' || c == '\t' || c == ',' ||
            c == ';' || c == '.');
  }

  /*!
   * @brief 
   * @param c 
   * @return 
  */
  bool onceDivider(char c) {
    return (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' ||
            c == ']' || c == ';' || c == '.' || c == '*');
  }

  bool equalOperation(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '!' ||
            c == '|' || c == '&' || c == '%' || c == '^' || c == '=');
  }

  bool doubleOperation(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '|' || c == '&' ||
            c == '=' || c == '<' || c == '>' || c == ':');
  }

  /*!
   * @brief ���������, �������� �� ������ ����������������� ������ ������
   * @param s ������, ������� ���� ���������
   * @return true � false ��������������
  */
  bool isReserved(std::string& s) {
    for (int i = 0; i < reserved.size(); ++i) {
      if (s == reserved[i]) {
        return true;
      }
    }
    return false;
  }

  /*!
   * @brief ���������� ������� ������� � ����
   * @param x ��� �������, ������� ���� �������� � ����
  */
  void out(int x) {
    if (now == "") return;
    if (isReserved(now)) {
      os << now << " " << 1 << ' ' << line << '\n';
    } else {
      os << now << " " << x << ' ' << line << '\n';
    }
    now.clear();
    last = ' ';
    type = 0;
  }

  /*!
   * @brief ��������� ����������� ������
   * @param file_name ���� � �������� �����
  */
  void unload(std::string file_name) {
    std::ifstream is(file_name);
    // -1 - none of them, 0 - begin
    bool comment = false;
    bool string = false;
    while (!is.eof()) {
      char c;
      is.get(c);
      if (is.eof()) break;
      if (c == '\n') {
        line++;
      }
      if (last == '/' && c == '*') {
        now.clear();
        type = 0;
        comment = true;
      }
      if (last == '*' && c == '/') {
        now.clear();
        comment = false;
        type = 0;
        last = ' ';
        continue;
      }
      if (comment) {
        last = c;
        continue;
      }
      if (c == '"') {
        if (string) {
          out(10);
        } else {
          //out(type);
        }
        //now += '"';
        //out(11);
        string = !string;
        continue;
      }
      if (string) {
        now += c;
        continue;
      }
      if ((last == '<' || last == '>') && !isDivider(c)) {
        out(9);
      }
      if (last == '/' && !isDivider(c)) {
        out(4);
      }
      if ((doubleOperation(last) && c == last) ||
          (equalOperation(last) && c == '=')) {
        now += c;
        out(4);
        continue;
      }
      if (isDivider(c)) {
        out(type);
        if (c != ' ' && c != '\n' && c != '\t') {
          now += c;
          last = c;
          type = getType(c);
        }
        if (onceDivider(c)) {
          out(getType(c));
        }
        continue;
      } else if (isDivider(last)) {
        out(type);
        type = 0;
        last = ' ';
        now = "";
      }
      if (type == 0) {
        if (isLetter(c)) {
          type = 2;
        } else if (isDigit(c)) {
          type = 3;
        } else {
          type = -1;
        }
      } else if (type == 2) {
        if (!(isLetter(c) || isDigit(c))) {
          type = -1;
        }
      } else if (type == 3) {
        if (isLetter(c)) {
          type = -1;
        }
      }
      now += c;
      last = c;
    }
    out(type);
    os.close();
  }

 private:
  std::vector<std::string> reserved; ///< ������ ����������������� ����
  std::ofstream os; ///< �������� �����
  char last; ///< ���������� ������
  int type; ///< ��� ������� ������������� �������
  int line = 1; ///< ����� ������� �������
  std::string now; ///< ������� �������
};