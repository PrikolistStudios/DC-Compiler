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
 * @brief ���� �������� �������� ������� ��������� ������ � ���� ������������� ��������
*/

/*!
 * @brief ���� ��������� ������
*/
enum PolizElemTypes {
  Variable = 0, ///< ����� ����������
  Oper = 1, ///< ��������
  Address = 2, ///< ����� (������) �������� ������
  Lit = 3, ///< �������
  Func = 4 ///< �������� ������ �������
};

/*!
 * @brief ���� ����������
*/
enum VarTypes {
  Int = 0, ///< ������������� �����
  Char = 1, ///< ������
  Float = 2, ///< ������������ �����
  Bool = 3, ///< ������� ����������
  String = 4 ///< ��������� ����������
};

/*!
 * @brief ��������� ��� ���������� VarTypes � std::string
 * @param type ��� ����������, ������� ���� ������������� � ������
 * @return ������, ��������������� ���� ����������
*/
std::string ToString(VarTypes type) {
  std::vector<std::string> mas = {"int", "char", "float", "bool", "string"};
  return mas[static_cast<int>(type)];
}

/*!
 * @brief ��������� ������ � ��� ���������� VarTypes
 * @param type ������, ������� ���� ������������� � VarTypes
 * @return ��� ���������� VarTypes, ��������������� ������
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
 * @brief ������� ����� �������� ������
 * ����� �������� �������� ���������� �� ���������� ����� ������, �� �������� ����������� ������ ������ ��������� ������
*/
class PolizElem {
 public:
  /*!
   * @brief ����������� �� ���� �������� ������
   * @param type ��� �������� ������
  */
  PolizElem(PolizElemTypes type) : type_(type) {}
  /*!
   * @brief ������ ���������� type_
   * @return �������� ���������� type_
  */
  PolizElemTypes type() const { return type_; }

  /*!
   * @brief ����������� ����������, ����� ����� ��� �����������
  */
  virtual ~PolizElem() {}
 private:
  PolizElemTypes type_; ///< ��� �������� ������
};

/*!
 * @brief ����� �������� ������ - ��������
*/
class PolizOper :  public PolizElem {
 public:
  /*!
   * @brief ����������� �������� � ������ �� ��������
   * @param oper ��������, ������� ���� �������� � �����
  */
  PolizOper(std::string oper)
      : PolizElem(Oper), oper_(oper) {}
  /*!
   * @brief ������ ���������� oper_
   * @return �������� ���������� oper_
  */
  std::string oper() const { return oper_; }
 private:
  std::string oper_; ///< �������� � ������
};

/*!
 * @brief ����� �������� ������ - ������
 * ����� ��������� �������� �������� �� ��������, ������������ �������� � �. �. ����� ������� � ������ ����� ������, �� ������� ���� ����������� �������. ��� ����� � ������������ ���� �����
*/
class PolizAddress :  public PolizElem {
 public:
  /*!
   * @brief ����������� ������ (�������) �������� ������ �� ������
   * @param address ������ �������� ������, ������� ����� �������� � �����
  */
  PolizAddress(int address)
      : PolizElem(Address), address_(address) {}
  /*!
   * @brief ������ ���������� address_
   * @return �������� ���������� address_
  */
  int address() const { return address_; }

 private:
  int address_; ///< ����� (������) �������� ������
};

struct Var;

/*!
 * @brief ����� �������� ������ - ������ ����������
 * ��� ������� � ���������� � ������ ���������� ������� �� �����, ��� ���� � ����� ���� �����
*/
class PolizVar :  public PolizElem {
 public:
  /*!
   * @brief ����������� �������� ������ �� ��������� ������ ����������
   * @param var ����� ����������, ������� ���� �������� � �����
  */
  PolizVar(Var* var) : PolizElem(Variable),var_(var){}
  /*!
   * @brief ������ ���������� var_
   * @return �������� ���������� var_
  */
  Var* var() const { return var_; }
 private:
  Var* var_; ///< ����� ����������, ������� �������� � ������
};

/*!
 * @brief ��������� ������, ������� ��������� �������� � ����������, � ����� � ���������
 * �������� ��������� ������������� ��������, ������� ������������ � ��������������. ������������� �������� ����� ��� ������������� �������� � ���������� ������ �����, �� ���� ��� ���������� ����� ����� � ������.
*/
struct VarData {
  /*!
   * @brief ����������� �� ���� ������, ������� �������� � ���������
   * @param type ��� ������
  */
  VarData(VarTypes type) : type_(type) {}
  /*!
   * @brief ����������� �� �������������� �������� ������
   * @param val ������������� ��������, ������� �������� � ���������
  */
  VarData(int val) : type_(Int), int_(val) {}
  /*!
   * @brief ����������� �� ����������� �������� ������
   * @param val ���������� ��������, ������� �������� � ���������
  */
  VarData(char val) : type_(Char), char_(val) {}
  /*!
   * @brief ����������� �� ������������� �������� ������
   * @param val ������������ ��������, ������� �������� � ���������
   */
  VarData(float val) : type_(Float), float_(val) {}
  /*!
   * @brief ����������� �� �������� �������� ������
   * @param val ������� ��������, ������� �������� � ���������
   */
  VarData(bool val) : type_(Bool), bool_(val) {}
  /*!
   * @brief ����������� �� ���������� �������� ������
   * @param val ��������� ��������, ������� �������� � ���������
   */
  VarData(std::string val) : type_(String), string_(val) {}

  int int_ = 0; ///< ������������� ��������, ������� ����� ��������� � ���������, ���� ��� ������ ���������������
  bool bool_ = false; ///< ������� ��������, ������� ����� ��������� � ���������, ���� ��� ������ ���������������
  std::string string_ = ""; ///< ��������� ��������, ������� ����� ��������� � ���������, ���� ��� ������ ���������������
  char char_ = '0'; ///< ���������� ��������, ������� ����� ��������� � ���������, ���� ��� ������ ���������������
  float float_ = 0.f; ///< ������������ ��������, ������� ����� ��������� � ���������, ���� ��� ������ ���������������
  VarTypes type_; ///< ��� ������, ������� �������� � ���������. � ������������ � ����� ������ � ��������� ����� �������������� ��������������� ���� - ������������ �����, ������������� � �. �.

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
   * @brief ��������� �������� ��������� � ������, ���������� � ���������
   * @return ��������� �� ������ ���������
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
   * @brief ��������� �������� ������� ���� � ������
   * @return �������� ������ ���������� � ���������� ���������� � ��� �������� �����
  */
  VarData UnPlus() { return *this; }
  /*!
   * @brief ��������� �������� ������� ����� � ������
   * @return �������� ������ ���������� � ���������� ���������� � ��� �������� ������
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
   * @brief ��������� �������� ��������� � ������
   * @return �������� ������ ���������� � ���������� ���������� � ��� ���������
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
   * @brief ������� ������� �������������� �����
   * @param num ��������� ������� 
   * @param deg ���������� �������
   * @return ����������� ������� �����
  */
  int Power(int num, int deg) {
    if (deg == 0) return 1;
    if (deg % 2 == 0) return Power(num, deg / 2) * Power(num, deg / 2);
    return num * Power(num, deg - 1);
  }
  /*!
   * @brief �������� ���� ������ � ������� ������ ������
   * @param rhs ������ ������� �������� ���������� � �������
   * @return ������, ���������� � ���������� ���������� ����� ������ � ������� ������
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
 * @brief ����������� �������� <<, ����� ����� ���� ������ �������� ������ VarData �� �����
 * @param os �������� �����
 * @param data ������, ������� ����� ������� � �����
 * @return �������� ����� ��� ��������� ��������
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
 * @brief ����������� �������� <<, ����� ����� ���� ������ ������� ������
 * VarData
 * @param is ������� �����
 * @param data ������, ������� ������ ������ ������������
 * @return ������� ����� ��� ��������� ��������
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
 * @brief ����� �������� ������ - ��������
 * �� �� ����� ��� � rvalue � ������ ��
*/
class PolizLit : public PolizElem {
 public:
  /*!
   * @brief ����������� �������� �� ���� ���������� ������
   * @param type ��� ������ ��������
  */
  PolizLit(VarTypes type) : PolizElem(Lit), data_(type) {}
  /*!
   * @brief ����������� �������� �� ������, ������� � ��� ��������
   * @param data ������, ������� ������ ��������� � ��������
  */
  PolizLit(VarData data) : PolizElem(Lit), data_(data) {}
  VarData data_; ///< ������ ��������

 private:
};

/*!
 * @brief ����� �������� ������ - �������� ������ �������
*/
class PolizFuncCall : public PolizElem {
 public:
  /*!
   * @brief ������������ �������� ������ ������� �� ���-�� ���������� � ����� ���������� �������
   * @param args ���-�� ���������� �������
   * @param func ��� ���������� �������
  */
  PolizFuncCall(int args, std::string func)
      : PolizElem(Func), arguments_(args), func_name_(func) {}
  /*!
   * @brief ������ ���������� arguments_
   * @return �������� ���������� arguments_
  */
  int arguments() const { return arguments_; }
  /*!
   * @brief ������ ���������� func_name_
   * @return �������� ���������� func_name_
  */
  std::string func_name() const { return func_name_; }

 private:
  int arguments_; ///< ���-�� ���������� � ���������� �������
  std::string func_name_; ///< ��� ���������� �������
};

/*!
 * @brief ��������� ����������, ����� ������� �������� � ������
*/
struct Var {
  /*!
   * @brief ����������� ����������
   * @param type ��� ������ ����������
   * @param size ������ �������, ���� ���������� �������� ��������
  */
  Var(VarTypes type, int size = 0) : data_(type) {
    if (size > 0) {
      for (int i = 0; i < size; ++i) {
        array_.push_back(new Var(type));
      }
    }
  }
  /*!
   * @brief ����������� �� ���������
  */
  Var() : data_(Int) {}

  
  VarData data_; ///< ������, ���������� � ����������

  std::vector<Var*> array_; ///< ������ �� ������� ����������, ���� �������� ���������� �������� ���������� ��������
};
