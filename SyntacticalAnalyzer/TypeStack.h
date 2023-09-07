#pragma once
#include <stack>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
/*!
* \file
* \brief �������� �������� ������ ����� �����
*/

/*!
* \brief ����� ����� �����
* 
* ����� ������������ ��� �������� ����� � ���������� � ����������
*/
class TypeStack {
 public:
  /*!
   * @brief ��������� ������� � ����
   * @param val ��� ��� ��������, ������� ���� �������� � ����
  */
  void Push(std::string val) { stack_.push(val); }
  /*!
   * @brief ��������� ������������ ���� ����� � �������� ��������
  */
  void CheckBin() {
    auto rhs = stack_.top();
    stack_.pop();
    auto oper = stack_.top();
    stack_.pop();
    auto lhs = stack_.top();
    stack_.pop();

    auto err = std::invalid_argument(
        "Incompatible operands or operation: " + lhs + oper + rhs);

    if (oper == "[]") {
      if (lhs[0] != 'l' || rhs == "string" || rhs == "astring") {
        throw err;
      }
      lhs.erase(lhs.begin());
      Push("a" + lhs);
      return;
    }

    if (lhs[0] == 'l' || rhs[0] == 'l') throw err;

    if (oper.find("=") != oper.npos && oper != "==" && oper != "!=" &&
        oper != ">=" && oper != "<=") {
      if (rhs[0] == 'a') rhs.erase(rhs.begin());
      if (lhs[0] == 'a') {
        lhs.erase(lhs.begin());
        if (lhs == "string" && rhs == "float" ||
            lhs != "string" && rhs == "string" ||
            (lhs == "string" && oper != "=" && oper != "+=")) {
          throw err;
        }
        if (!IsStandardType(lhs) && lhs != rhs && oper != "=") {
          throw err;
        }
      } else {
        throw err;
      }
      Push("a" + lhs);
      return;
    }

    if (oper == ",") {
      Push(rhs);
      return;
    }

    if (!IsStandardType(lhs) || !IsStandardType(rhs)) throw err;

    if (rhs[0] == 'a') rhs.erase(rhs.begin());
    if (lhs[0] == 'a') lhs.erase(lhs.begin());

    if (oper == "&&" || oper == "||") {
      if (lhs == "string" || rhs == "string" || lhs == "float" ||
          rhs == "float") {
        throw err;
      }
      Push("bool");
      return;
    }

    if (oper == "==" || oper == "!=" || oper == ">" || oper == "<" ||
        oper == ">=" || oper == "<=") {
      if (rhs == "string" && lhs == "float" ||
          rhs == "float" && lhs == "string") {
        throw err;
      }
      Push("bool");
      return;
    }

    if (oper == "+" || oper == "-") {
      if (rhs == "string" && lhs == "float" ||
          rhs == "float" && lhs == "string") {
        throw err;
      }
      if (rhs == "string" || lhs == "string") {
        Push("string");
      } else if (lhs == "float" || rhs == "float") {
        Push("float");
      } else {
        Push("int");
      }
      return;
    }

    if (oper == "*" || oper == "/" || oper == "%" || oper == "**") {
      if (rhs == "string" || lhs == "string") {
        throw err;
      }

      if (lhs == "float" || rhs == "float") {
        Push("float");
      } else {
        Push("int");
      }
      return;
    }

    throw err;
  }
  /*!
   * @brief ��������� ������������ ����������� ������� �������� � ����
  */
  void CheckUnoR() {
    auto oper = stack_.top();
    stack_.pop();
    auto lhs = stack_.top();
    stack_.pop();

    auto err = std::invalid_argument("Incompatible operands or operation: " + lhs + oper);

    if (lhs[0] == 'l' ||!IsStandardType(lhs)) throw err;

    if ((lhs[0] != 'a' && (oper == "++" || oper == "--")) || lhs == "string" ||
        lhs == "astring" || (oper=="!" && (lhs == "float" || lhs == "afloat"))) {
      throw err;
    }

    if (oper == "++" || oper == "--") {
      Push(lhs);
    } else {
      if (lhs[0] == 'a') lhs.erase(lhs.begin());
      Push(lhs);
    }
  }
  /*!
   * @brief ��������� ������������ ���������� ������� �������� � ����
  */
  void CheckUnoL() {
    auto rhs = stack_.top();
    stack_.pop();
    auto oper = stack_.top();
    stack_.pop();

    auto err = std::invalid_argument("Incompatible operands or operation: " + oper + rhs);

    if (rhs[0] == 'l' || !IsStandardType(rhs)) throw err;

    if ((rhs[0] != 'a' && (oper == "++" || oper == "--")) || rhs == "string" ||
        rhs == "astring") {
      throw err;
    }

    if (oper == "!") {
      Push("bool");
    } else {
      Push(rhs);
    }
  }
  /*!
   * @brief ���������, ��� ����� ������� ��� � ����� ����� �������� � ����������� ����
  */
  void CheckBool() {
    auto val = stack_.top();
    stack_.pop();
    if (val == "string" || val == "astring" || val[0] == 'l') {
      throw std::invalid_argument("Value is expected to be bool convertable");
    }
  }
  /*!
   * @brief ���������� ������� ������� � �����
   * @return ������� ������� � �����
  */
  std::string Top() const { return stack_.top(); }
  /*!
   * @brief ������� ������� ������� � �����
  */
  void Pop() { stack_.pop(); }

 private:
  std::stack<std::string> stack_; ///< ����
  /*!
   * @brief ���������, �������� �� ��� ����������� ��� �����
   * @param type ���, ������� ���� ���������
   * @return true � false ��������������
  */
  bool IsStandardType(std::string type) {
    if (type[0] == 'a' || type[0] == 'l') type.erase(type.begin());
    if (type != "int" && type != "bool" && type != "string" &&
        type != "float" && type != "char") {
      return false;
    }
    return true;
  }
};
