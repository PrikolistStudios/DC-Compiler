#pragma once
#include <vector>
#include <string>
#include "Poliz.h"
/*!
 * \brief �����, ���������� � ���� ���������� � �������
 *
 * ����� ������, ����� ��������� ���������� ���������� � ��������, � ������ ���,
 * ��� � ���������
 */
class FunctionInfo {
 public:
  /*!
   * @brief ����������� �� ���� ������������� �������� �������, ����� � ������
   * ����������
   * @param type ��� ������������� �������� �������
   * @param name ��� �������
   * @param args ��������� �������
   */
  FunctionInfo(std::string type, std::string name,
               std::vector<std::pair<std::string,std::string>> args)
      : type_(type), name_(name), args_(args) {}
  /*!
   * @brief ����������� �� ���������
   */
  FunctionInfo() {}
  /*!
   * @brief ������ type_
   * @return �������� type_
   */
  std::string type() const { return type_; }
  /*!
   * @brief ������ type_
   * @param type ����� ��� �������
   */
  void set_type(std::string type) { type_ = type; }
  /*!
   * @brief ������ name_
   * @return �������� name_
   */
  std::string name() const { return name_; }
  /*!
   * @brief ������ ��� ���������� �������
   * @return ��������� ������� args_
   */
  std::vector<std::pair<std::string, std::string>> args() const {
    return args_;
  }
  /*!
   * @brief ������ poliz_start_
   * @return �������� poliz_start_
   */
  int poliz_start() const { return poliz_start_; }
  void set_poliz_start(int poliz_start) { poliz_start_ = poliz_start; }
  void AddLocalVar(Var* var) { local_vars_.push_back(var); }
  void AddArgument(Var* var) { arguments_.push_back(var); }

  std::vector<Var*> arguments() const { return arguments_; }
  std::vector<Var*> local_vars() const { return local_vars_; }

 private:
  std::string type_;  ///< ��� �����. �������� �������
  std::string name_;  ///< ��� �������
  std::vector<std::pair<std::string, std::string>>
      args_;         ///< ��������� �������
  int poliz_start_;  ///< ����� ����� ���� ������� � �����
  std::vector<Var*> arguments_;
  std::vector<Var*> local_vars_;
};

/*!
 * \brief �����, ���������� � ���� ���������� � ���������
 *
 * ����� ������, ����� ��������� ���������� ���������� � �����������, � ������
 * ��� ��������� � �� ���� ������ - ������ � ����������
 *
 */
class StructInfo {
 public:
  /*!
   * @brief ����������� �� �������� ���������, ����� ���������� �� ���������
   * @param type ��� ���������
   */
  StructInfo(std::string type = "") : typename_(type) {}
  /*!
   * @brief ������ name_
   * @return �������� name_
   */
  std::string name() const { return typename_; }
  /*!
   * @brief ��������� ����� � ���������
   * @param func ����������� � ������
   */
  void AddMethod(FunctionInfo func) { methods_.push_back(func); }
  /*!
   * @brief ��������� ���������� � ���������
   * @param type ��� ����� ����������
   * @param name ��� ����� ����������
   */
  void AddVariable(std::string type, std::string name) {
    variables_.emplace_back(type, name);
  }
  /*!
   * @brief ���������� ��� ��������� ���������� � ���������
   * @param name ��� ������� ����������
   * @return ��� ��������� ����������, ����� ������ ������
   */
  std::string FindVariable(std::string name) const {
    for (auto val : variables_) {
      if (val.second == name) return val.first;
    }
    return "";
  }
  /*!
   * @brief ���� ����� � ���������
   * @param name ��� �������� ������
   * @return ���������� � ������. ���� �� ����� -- ����������, �����������������
   * �� ���������
   */
  FunctionInfo FindMethod(std::string name) const {
    for (auto val : methods_) {
      if (val.name() == name) return val;
    }
    return FunctionInfo();
  }

 private:
  std::string typename_;               ///< ��� ���������
  std::vector<FunctionInfo> methods_;  ///< ������ ������� ���������
  std::vector<std::pair<std::string, std::string>>
      variables_;  ///< ���������� � ���������
};
