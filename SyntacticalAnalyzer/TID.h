#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "Poliz.h"

/*!
* \file
* \brief ���� ���� �������� ��������� ������ ������ TID � ���� ������������� ��������
* 
* ���� �������� � ���� �������� ������ ������ TID, ������������� � ����������� 
* ��� ���������� � ������������ ������������������ ��������� ����������, � ����� �������, ������� ����� TID �����������
*/

/*!
* \brief ������� �����, ����������� �������� ���������� � TID
* 
* �� ���� ��� ������ ���� ���� ��������� - ��� � ��� ��������� ���������� ����������, ������������ ����� ������ � ������� ���������������
*/
class TIDRow {
 public:
  /*!
  * \brief ������������ ������� TID �� ����� ���������� � �� ����
  */
  TIDRow(std::string name, std::string type, int size = 0)
      : name_(name), type_(type) {
    if (type[0] == 'l') {
      type.erase(type.begin());
    }
    VarTypes var_type = ToVarType(type);
    var_ = new Var(var_type, size);
  }
  /*!
  * \brief ������ ��� ���������� name_
  * \return ������ ��� ���������� type_
  */
  std::string name() const { return name_; }
  /*!
   * \brief ���������� ��� ���������� � TID
   * \return ��� ���������� � TID
   */
  std::string type() const { return type_; }

  Var* var() const { return var_; }
 private:
  std::string name_; ///< ��� ���������� � TID
  std::string type_; ///< ��� ���������� � TID
  Var* var_; ///< ����� ����������
};

/*!
* \brief ����� ������������ ����� ���� � ������ TID
* 
* ������ � ���� ��������� �� ����� ���� ������, ��������� �� �������� ���� � ����� ���� ������� ��������������� TID
*/
class TIDNode {
 public:
  /*!
  * \brief ����������� ��� ������ ����
  * \param parent �������� ������ ����, nullptr ���� ������� -- ������
  */
  TIDNode(TIDNode* parent = nullptr) : parent_(parent) {}
  /*!
  * \brief ��������� ������� � ����
  * \param child ����� �������
  */
  void AddChild(TIDNode* child) { children_.push_back(child); }
  /*!
   * \brief ��������� ������� � TID ����
   * \param row ����� �������
   */
  void AddRow(TIDRow row) {
    tid_.push_back(row);
  }
  /*!
   * \brief ������ ��� ���������� tid_
   * \return ����� TID ����
   */
  std::vector<TIDRow> rows() const {
    return tid_;
  }
  /*!
   * \brief ������ ��� ���������� parent_
   * \return ��������� �� �������� ����
   */
  TIDNode* parent() const {
    return parent_;
  }
  /*!
  * \brief ���� ���������� � TID �� �����
  * \param name ��� ���������� ������� ��������� �����
  * \return true ���� ���������� �������, false ���� ���
  */
  bool FindVar(std::string name) {
    for (auto val : tid_) {
      if (val.name() == name) return true;
    }
    return false;
  }

 private:
  std::vector<TIDRow> tid_; ///< TID ����
  std::vector<TIDNode*> children_; ///< ���� ����
  TIDNode* parent_ = nullptr; ///< �������� ����
};

/*!
* \brief ����� ������ ������ ���������������
* 
* ��������� �������������� � ������� ������ ���������������. ����� ������� ��� ������� - ����� ��� ������������ ��������������. 
*/
class TIDTree {
 public:
  /*!
  * \brief ����������� �� ��������� �� ��������� ������� ����������. ��� �������� ������� ������� ����� ��������� �� ����, ������� ������� ���������� ������ �����
  */
  TIDTree() : root_(new TIDNode), cur_(root_) {}
  /*!
  * \brief ���� ���������� �� �����
  * 
  * �������� ��������������� ������� GetID_ ��� ������ ���������� �� �����
  * 
  * \param name ��� ���������� ������� ��������� ����� � ������
  * \return ������� TID ��������������� ����������
  */
  TIDRow GetID(std::string name) { return GetID_(name, cur_); }
  bool CheckID(std::string name) { return CheckID_(name, cur_); }
  void NewScope() { 
    auto new_child = new TIDNode(cur_);
    cur_->AddChild(new_child);
    cur_ = new_child;
  }
  void Back() {
    if (cur_ != root_) {
      cur_ = cur_->parent();
    }
  }
  void Push(std::string name, std::string type, int size=0) {
    cur_->AddRow(TIDRow(name, type, size));
  }
  TIDNode* cur() const { return cur_; }

 private:
  TIDNode *root_, *cur_;
  bool CheckID_(std::string name, TIDNode* node) {
    for (auto val : node->rows()) {
      if (val.name() == name) {
        return true;
      }
    }
    if (node == root_) {
      return false;
    } else {
      return CheckID_(name, node->parent());
    }
  }
  TIDRow GetID_(std::string name, TIDNode* node) {
    for (auto val : node->rows()) {
      if (val.name() == name) {
        return val;
      }
    }
    if (node == root_) {
      throw std::invalid_argument("Found no declareg variable with the name " +
                                  name);
    } else {
      return GetID_(name, node->parent());
    }
  }
};
