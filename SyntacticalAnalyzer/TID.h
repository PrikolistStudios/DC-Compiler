#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "Poliz.h"

/*!
* \file
* \brief Этот файл является описанием класса дерева TID и всех сопутствующих структур
* 
* Файл содержит в себе описание класса дерева TID, используемого в трансляторе 
* для сохранения и отслеживания инициализированных созданных переменных, а также классов, которые класс TID задействует
*/

/*!
* \brief Простой класс, облегчающий хранение информации в TID
* 
* По сути это просто пара двух элементов - имя и тип созданной переменной переменной, представляет собой строку в таблице идентификаторов
*/
class TIDRow {
 public:
  /*!
  * \brief Конструирует элемент TID по имени переменной и ее типу
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
  * \brief Геттер для переменной name_
  * \return Геттер для переменной type_
  */
  std::string name() const { return name_; }
  /*!
   * \brief Возвращает тип переменной в TID
   * \return Тип переменной в TID
   */
  std::string type() const { return type_; }

  Var* var() const { return var_; }
 private:
  std::string name_; ///< Имя переменной в TID
  std::string type_; ///< Тип переменной в TID
  Var* var_; ///< Адрес переменной
};

/*!
* \brief Класс представляет собой узел в дереве TID
* 
* Хранит в себе указатели на детей узла дерева, указатель на родителя узла а также саму таблицу идентификаторов TID
*/
class TIDNode {
 public:
  /*!
  * \brief Конструктор для нового узла
  * \param parent Родитель нового узла, nullptr если вершина -- корень
  */
  TIDNode(TIDNode* parent = nullptr) : parent_(parent) {}
  /*!
  * \brief Добавляет ребенка к узлу
  * \param child Новый ребенок
  */
  void AddChild(TIDNode* child) { children_.push_back(child); }
  /*!
   * \brief Добавляет элемент в TID узла
   * \param row Новый элемент
   */
  void AddRow(TIDRow row) {
    tid_.push_back(row);
  }
  /*!
   * \brief Геттер для переменной tid_
   * \return Копию TID узла
   */
  std::vector<TIDRow> rows() const {
    return tid_;
  }
  /*!
   * \brief Геттер для переменной parent_
   * \return Указатель на родителя узла
   */
  TIDNode* parent() const {
    return parent_;
  }
  /*!
  * \brief Ищет переменную в TID по имени
  * \param name Имя переменной которую требуется найти
  * \return true если переменная нашлась, false если нет
  */
  bool FindVar(std::string name) {
    for (auto val : tid_) {
      if (val.name() == name) return true;
    }
    return false;
  }

 private:
  std::vector<TIDRow> tid_; ///< TID узла
  std::vector<TIDNode*> children_; ///< Дети узла
  TIDNode* parent_ = nullptr; ///< Родитель узла
};

/*!
* \brief Класс дерева таблиц идентификаторов
* 
* Облегчает взаимодействие с деревом таблиц идентификаторов. Самая главная его функция - поиск уже объявленного идентификатора. 
*/
class TIDTree {
 public:
  /*!
  * \brief Конструктор по умолчанию не принимает никаких параметров. Для корневой вершины создает новый указатель на узел, текущая вершина становится равной корню
  */
  TIDTree() : root_(new TIDNode), cur_(root_) {}
  /*!
  * \brief Ищет переменную по имени
  * 
  * Вызывает вспомогательную функцию GetID_ для поиска переменной по имени
  * 
  * \param name Имя переменной которую требуется найти в дереве
  * \return Элемент TID соответствующей переменной
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
