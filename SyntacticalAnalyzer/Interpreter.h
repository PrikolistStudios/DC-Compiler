#pragma once
#include <map>
#include <stack>
#include "Poliz.h"
#include "ComponentsInfo.h"
#include <iostream>
/*!
* @file
* @brief ���� �������� �������� ������ ��������������
*/


/*!
 * @brief ����� �������� ����� �������
 * ��� ������ ����� ������� ����� ��������� ������ �������� ������, ���� ����� ���������, ����� ����� ������� �������� ����������. ����� ����� ��������� �������� ��������� ���������� ������� �������, ����� ������������ �� ��� ���������� ���������� ����� �������.
*/
class CallStackElem {
 public:
  /*!
   * @brief ����������� �� ������ ����������
   * @param return_point ����� ��������
   * @param func_name ��� �������
  */
  CallStackElem(int return_point, std::string func_name) : return_point_(return_point), func_name_(func_name) {}
  /*!
   * @brief ������ ���������� return_point_
   * @return �������� ���������� return_point_
  */
  int return_point() const { return return_point_; }
  /*!
   * @brief �������� ������ � ����������� ��������� ��������� ���������� ��������� �������
   * @return ������ �� ��������� ����������� ��������, ����� ����� ���� ��������� � ���� ��������
  */
  std::map<Var*, Var>& local_vars() { return local_vars_; }

  /*!
   * @brief ������ ���������� func_name_
   * @return �������� ���������� func_name_
   */
  std::string func_name() const { return func_name_; }

 private:
  std::map<Var*, Var> local_vars_; ///< ��������� �������� ����������. ��� ������� ������ - ���� ��������
  int return_point_; ///< ����� �������� - ������ �������� ������, ���� ����� ����� ��������� ����� ���������� ���������� �������
  std::string func_name_; ///< ��� �������, ������� ������ �����������
};

class Interpreter {
 public:
  /*!
   * @brief ����������� ��������������
   * @param funcs ������� ���������
   * @param poliz �����, ������� ����� �����������
  */
  Interpreter(std::vector<FunctionInfo> funcs, std::vector<PolizElem*> poliz)
      : funcs_(funcs),
        poliz_(poliz),
        cur_func_("int", "main", std::vector<std::pair<std::string,std::string>>()) {}

  /*!
   * @brief ��������� ����������
   * @param start_ind ������ �������� ������, ������ ���������� ����������
  */
  void Launch(int start_ind) {
    cur_ind_ = start_ind;
    while (true) {
      if (poliz_[cur_ind_]->type() != Oper &&
          poliz_[cur_ind_]->type() != Func) {
        operands_.push(poliz_[cur_ind_]);
        cur_ind_++;
      } else if (poliz_[cur_ind_]->type() == Oper) {
        ProcessOper(dynamic_cast<PolizOper*>(poliz_[cur_ind_])->oper());
      } else {
        ProcessFunc(dynamic_cast<PolizFuncCall*>(poliz_[cur_ind_]),
                    cur_ind_ + 1);
      }
    }
  }

 private:
  std::stack<CallStackElem> call_stack_; ///< ���� �������
  std::vector<FunctionInfo> funcs_; ///< ������� ���������
  std::vector<PolizElem*> poliz_; ///< �����
  std::stack<PolizElem*> operands_; ///< ���� ���������
  FunctionInfo cur_func_; ///< ������� ����������� �������
  int cur_ind_; ///< ������ �������� �������� ������

  /*!
   * @brief ����������� ������� ������� �� ����� ���������, ���� ��� �������� PolizLit ��� PolizVar
   * @return ������ �������� �������� ����� ���������
  */
  VarData& PopData() {
    if (operands_.top()->type()==Variable) {
      VarData& tmp = dynamic_cast<PolizVar*>(operands_.top())->var()->data_;
      operands_.pop();
      return tmp;
    } else {
      VarData& tmp = dynamic_cast<PolizLit*>(operands_.top())->data_;
      operands_.pop();
      return tmp;
    }
  }

  /*!
   * @brief ����������� ������� ������� ����� ���������, ���� ��� �������� ������� ������ ������
   * @return �������� ������ ������ ������
  */
  int PopAddress() {
    int res = dynamic_cast<PolizAddress*>(operands_.top())->address();
    operands_.pop();
    return res;
  }

  /*!
   * @brief ������������ �������� ������
   * @param oper ��������, ������� ���������� ����������
  */
  void ProcessOper(std::string oper) {
    if (oper == "B!") {
      cur_ind_ = PopAddress();
      return;
    }
    if (oper == "F!") {
      int address = PopAddress();
      auto data = PopData();
      if ((data == VarData(false)).bool_)
        cur_ind_ = address;
      else
        cur_ind_++;
      return;
    }
    if (oper == "return") {
      if (call_stack_.empty()) {
        std::exit(0);
      }

      auto data = PopData();
      operands_.push(new PolizLit(data));
      cur_ind_ = call_stack_.top().return_point();

      call_stack_.pop();

      if (!call_stack_.empty()) {
        for (auto& val : call_stack_.top().local_vars()) {
          *val.first = val.second;
        }

        for (auto& val : funcs_) {
          if (val.name() == call_stack_.top().func_name()) {
            cur_func_ = val;
            break;
          }
        }
      }
      return;
    }
    cur_ind_++;
    if (oper == "in") {
      int n = PopData().int_;
      std::stack<Var*> elems;
      for (int i = 0; i < n; ++i) {
        elems.push(dynamic_cast<PolizVar*>(operands_.top())->var());
        operands_.pop();
      }
      while (!elems.empty()) {
        std::cin >> elems.top()->data_;
        elems.pop();
      }
    }
    if (oper == "out") {
      int n = PopData().int_;
      std::stack<VarData> elems;
      for (int i = 0; i < n; ++i) {
        elems.push(PopData());
      }
      while (!elems.empty()) {
        std::cout << elems.top();
        elems.pop();
      }
    }
    if (oper == "post++") {
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      operands_.push(new PolizLit(var->data_));
      var->data_ += VarData(1);
    }
    if (oper == "post--") {
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      operands_.push(new PolizLit(var->data_));
      var->data_ -= VarData(1);
    }
    if (oper == "[]") {
      VarData ind = PopData();
      Var* arr = dynamic_cast<PolizVar*>(operands_.top())->var();
      int ind_int;
      if (ind.type_ == Int) ind_int = ind.int_;
      if (ind.type_ == Bool) ind_int = ind.bool_;
      if (ind.type_ == Char) ind_int = ind.char_;
      if (ind.type_ == Float) ind_int = ind.float_;
      if (ind_int >= arr->array_.size() || ind_int < 0) {
        throw std::invalid_argument("Array index out of bounds");
      } else {
        operands_.push(new PolizVar(arr->array_[ind_int]));
      }
    }
    if (oper == "post!") {
      operands_.push(new PolizLit(PopData().Factorial()));
    }
    if (oper == "pre++") {
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ += VarData(1);
      operands_.push(new PolizVar(var));
    }
    if (oper == "pre--") {
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ -= VarData(1);
      operands_.push(new PolizVar(var));
    }
    if (oper == "un+") {
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      operands_.push(new PolizLit(var->data_.UnPlus()));
    }
    if (oper == "un-") {
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      operands_.push(new PolizLit(var->data_.UnMinus()));
    }
    if (oper == "pre!") {
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      operands_.push(new PolizLit(var->data_.Negation()));
    }
    if (oper == "**") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData().Power(rhs)));
    }
    if (oper == "*") {
      operands_.push(new PolizLit(PopData() * PopData()));
    }
    if (oper == "/") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() / rhs));
    }
    if (oper == "%") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() % rhs));
    }
    if (oper == "+") {
      operands_.push(new PolizLit(PopData() + PopData()));
    }
    if (oper == "-") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() - rhs));
    }
    if (oper == "<") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() < rhs));
    }
    if (oper == "<=") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() <= rhs));
    }
    if (oper == ">") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() > rhs));
    }
    if (oper == ">=") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() >= rhs));
    }
    if (oper == "==") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() == rhs));
    }
    if (oper == "!=") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() != rhs));
    }
    if (oper == "&&") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() && rhs));
    }
    if (oper == "||") {
      auto rhs = PopData();
      operands_.push(new PolizLit(PopData() || rhs));
    }
    if (oper == "=") {
      auto rhs = PopData();
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ = rhs;
      operands_.push(new PolizVar(var));
    }
    if (oper == "+=") {
      auto rhs = PopData();
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ += rhs;
      operands_.push(new PolizVar(var));
    }
    if (oper == "-=") {
      auto rhs = PopData();
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ -= rhs;
      operands_.push(new PolizVar(var));
    }
    if (oper == "*=") {
      auto rhs = PopData();
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ *= rhs;
      operands_.push(new PolizVar(var));
    }
    if (oper == "/=") {
      auto rhs = PopData();
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ /= rhs;
      operands_.push(new PolizVar(var));
    }
    if (oper == "%=") {
      auto rhs = PopData();
      auto var = dynamic_cast<PolizVar*>(operands_.top())->var();
      var->data_ %= rhs;
      operands_.push(new PolizVar(var));
    }
    if (oper == ",") {
      auto tmp = operands_.top();
      operands_.pop();
      operands_.push(tmp);
    }
    if (oper == ";") {
      PopData();
    }
  }

  /*!
   * @brief ������������ �������� ������ ������� � ������
   * @param func_call �������� ������ �������
   * @param return_point ����� �������� ���������� �������
  */
  void ProcessFunc(PolizFuncCall* func_call, int return_point) {
    if (cur_func_.name() != "main") {
      for (auto& val : cur_func_.local_vars()) {
        if (!val->array_.empty()) {
          for (auto& var : val->array_) {
            call_stack_.top().local_vars()[var] = *var;
          }
        } else {
          call_stack_.top().local_vars()[val] = *val;
        }
      }
      for (auto& val : cur_func_.arguments()) {
        if (!val->array_.empty()) {
          for (auto& var : val->array_) {
            call_stack_.top().local_vars()[var] = *var;
          }
        } else {
          call_stack_.top().local_vars()[val] = *val;
        }
      }
    }

    call_stack_.emplace(return_point, func_call->func_name());

    for (auto& val : funcs_) {
      if (val.name() == func_call->func_name()) {
        std::stack<VarData> data_stack;
        for (int i = 0; i < val.arguments().size(); ++i) {
          data_stack.push(PopData());
        }

        for (auto&var:val.arguments()) {
          var->data_ = data_stack.top();
          data_stack.pop();
        }

        for (auto& var : val.local_vars()) {
          var->data_ = VarData(var->data_.type_);
        }

        cur_ind_ = val.poliz_start();

        cur_func_ = val;

        break;
      }
    }
  }
};
