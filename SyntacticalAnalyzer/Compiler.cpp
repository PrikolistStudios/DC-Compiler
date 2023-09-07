#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <map>
#include <list>
#include <string>
#include "Separator.h"
#include "TypeStack.h"
#include "TID.h"
#include "Lexeme.h"
#include "ComponentsInfo.h"
#include "Poliz.h"
#include "Interpreter.h"
/*!
* \file
* \brief Основной файл, содержащий описание класса транслятора и запускает компиляцию кода
*/

/*!
* Функция проверяет, можно ли привести один тип к другому
*/
bool CompareTypes(std::string lhs, std::string rhs) {
  if (lhs[0] == 'a') lhs.erase(lhs.begin());
  if (rhs[0] == 'a') rhs.erase(rhs.begin());
  if (lhs == rhs) return true;
  if (lhs != "int" && lhs != "bool" && lhs != "string" && lhs != "float" &&
      lhs != "char")
    return false;
  if (rhs != "int" && rhs != "bool" && rhs != "string" && rhs != "float" &&
      rhs != "char")
    return false;

  if (lhs == "string" && rhs != "string" ||
      lhs != "string" && rhs == "string" || lhs[0] == 'l' || rhs[0] == 'l') {
    return false;
  }
  return true;
}

/*!
* \brief Класс транслятора
* 
* Класс, отвечающий за синтаксический и семантический анализ, а также генерацию кода (ПОЛИЗ) для интерпретатора. Принимает на вход два файла - последовательность лексем ("separated.txt") и список операций с указанием приоритета ("operations.txt")
* Все функции без описания используются для анализа соответствующих элементов языка из грамматики
*/
class Compiler {
 public:
  /*!
   * @brief Конструктор класса компилятора по файлу лексем и списка операций с
   * приоритетами
   * @param lexemes_filename Имя файла с последовательностью лексем
   * @param opers_filename Имя файла с операциями и их приоритетами
   */
  Compiler(const std::string& lexemes_filename,
           const std::string& opers_filename)
      : cur_ind_(-1) {
    std::ifstream in(lexemes_filename);
    while (!in.eof()) {
      int type, line;
      std::string text;
      in >> text >> type >> line;
      if (text == "") break;
      if (text == "true" || text == "false") {
        type = 23;
      } else if (type == 4 || type == 9) {
        std::ifstream in_op(opers_filename);
        int cur_type;
        while (!in_op.eof()) {
          std::string tmp;
          in_op >> tmp;
          if (std::isdigit(tmp[0])) {
            cur_type = std::stoi(tmp);
          } else if (tmp == text) {
            type = cur_type + 12;
            break;
          }
        }
      }
      lexemes_.emplace_back(static_cast<LexemeTypes>(type), text, line);
    }
    PushLexeme();
  }

  /*!
   * Запускает компиляцию исходного кода
   */
  void Program() {
    /* if (cur_ == "struct") {
      Struct();
      Program();
    } else*/
    if (lexemes_[cur_ind_ + 2] == "," || lexemes_[cur_ind_ + 2] == ";" ||
        lexemes_[cur_ind_ + 2] == "=") {
      VarDefinition();
      Program();
    } else if (cur_ == "int" && lexemes_[cur_ind_ + 1] == "main" &&
               lexemes_[cur_ind_ + 2] == "(") {
      PushLexeme(), PushLexeme(), PushLexeme();
      cur_func.set_type("int");
      if (cur_ != ")") {
        throw std::invalid_argument("error in lexeme " + cur_.text() +
                                    " in line " + std::to_string(cur_.line()));
      }
      PushLexeme();
      poliz_start_ind_ = poliz_.size();
      Block();
    } else if (lexemes_[cur_ind_ + 2] == "(") {
      funcs_.push_back(Func());
      cur_func = FunctionInfo();
      Program();
    } else {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
    if (cur_ind_ != lexemes_.size() - 1) {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
  }

  /*!
   * @brief Выводит ПОЛИЗ в необходимы файл
   * @param filename Файл, в который нужно вывести полиз
  */
  void WritePoliz(const std::string& filename) {
    std::ofstream stream(filename);
    for (auto val : poliz_) {
      if (val->type() == Variable) {
        stream << "Var: " << dynamic_cast<PolizVar*>(val)->var();
      }
      else if (val->type()==Oper) {
        stream << "Oper: " << dynamic_cast<PolizOper*>(val)->oper();
      }
      else if (val->type() == Address) {
        stream << "Address: " << dynamic_cast<PolizAddress*>(val)->address();
      } else if (val->type() == Lit) {
        auto lit = dynamic_cast<PolizLit*>(val);
        stream << ToString(lit->data_.type_) << " lit: ";
        if (lit->data_.type_ == String) {
          stream << lit->data_.string_;
        } else if (lit->data_.type_ == Int) {
          stream << lit->data_.int_;
        } else if (lit->data_.type_ == Float) {
          stream << lit->data_.float_;
        } else if (lit->data_.type_ == Bool) {
          stream << lit->data_.bool_;
        } else if (lit->data_.type_ == Char) {
          stream << lit->data_.char_;
        }
      }
      else if (val->type() == PolizElemTypes::Func) {
        stream << "Func call: "
                  << dynamic_cast<PolizFuncCall*>(val)->func_name() << ' '
                  << dynamic_cast<PolizFuncCall*>(val)->arguments();
      }
      stream << std::endl;
    }
    stream.close();
  }

  /*!
   * @brief Запускает исполнение кода
  */
  void Launch() { 
    Interpreter interpreter(funcs_, poliz_);
    interpreter.Launch(poliz_start_ind_);
  }
 private:
  std::vector<Lexeme>
      lexemes_;  ///< Массив из лексем - по сути исходный код программы
  Lexeme cur_;   ///< Текущая анализируемая лексема
  int cur_ind_;  ///< Индекс текущей анализируемой лексемы
  std::vector<FunctionInfo> funcs_;  ///< Успешно считанные функции в программе
  TIDTree tid_;  ///< Дерево таблиц идентификаторов для контроля имен
  TypeStack stack_;  ///< Стек типов для контроля типов в выражениях
  std::vector<StructInfo>
      structs_;  ///< Массив успешно считанных структур программы
  FunctionInfo cur_func;  ///< Текущая анализируемая функция
  std::vector<PolizElem*> poliz_;  ///< ПОЛИЗ
  int poliz_start_ind_; ///< Индекс элемента ПОЛИЗа, с которого начинается исполнение программы
  std::stack<int> loops_starts_; ///< Индексы начал циклов в ПОЛИЗЕ. Верхний элемент стека - индекс начала текущего цикла
  std::stack<std::vector<int>> loops_breaks_to_fill_; ///< Индексы элементов полиза, куда нужно будет подставить индекс элемента, следующего после кода текущего цикла.

  /*!
   * @brief Добавить в конец полиза операцию
   * @param oper Операция, которую надо добавить в полиз
  */
  void PolizPush(std::string oper) { poliz_.push_back(new PolizOper(oper)); }
  /*!
   * @brief Добавить адрес переменной в полиз
   * @param var Адрес переменной который надо добавить
  */
  void PolizPush(Var* var) { poliz_.push_back(new PolizVar(var)); }
  /*!
   * @brief Добавить индекс элемента ПОЛИЗа в ПОЛИЗ
   * @param address индес, который надо добавить
  */
  void PolizPush(int address) { poliz_.push_back(new PolizAddress(address)); }
  /*!
   * @brief Добавить литерал в ПОЛИЗ
   * @param lit Литерал, который надо добавить в полиз
  */
  void PolizPush(PolizLit lit) { poliz_.push_back(new PolizLit(lit)); }
  /*!
   * @brief Добавить в ПОЛИЗ операцию вызова функции
   * @param args Кол-во аргументов функции
   * @param func Имя вызываемой функции
  */
  void PolizPush(int args, std::string func) {
    poliz_.push_back(new PolizFuncCall(args, func));
  }
  /*!
   * @brief Добавить в полиз пустой элемент
   * @return Индек пропущенного элемента
  */
  int PolizSkip() { 
    poliz_.push_back(nullptr);
    return poliz_.size() - 1;
  }

  /*!
   * @brief Переход к следующей лексеме
   */
  void PushLexeme() { 
    if (cur_ind_ != lexemes_.size() - 1) cur_ = lexemes_[++cur_ind_];
  }
  /*!
   * @brief Проверяет, равна ли текущая лексема необходимой строке
   * @param val Строка, совпадение с которой надо проверить
   * @throw std::invalid_argument В случае несовпадения строки с лексемой
   */
  void CheckLexeme(std::string val) {
    if (cur_ != val.data()) {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
  }
  /*!
   * @brief Проверяет, есть ли функция или структура с именем name
   * @param name Имя, которое нужно найти среди функций и структур
   * @return true или false если найдено и нет соответственно
   */
  bool FindFuncStruct(std::string name) {
    bool func = false, struc = false;
    for (auto val : structs_) {
      if (val.name() == name) {
        struc = true;
      }
    }

    for (auto val : funcs_) {
      if (val.name() == name) {
        func = true;
      }
    }
    return func || struc;
  }
  /*!
   * @brief Проверяет, есть ли функция с именем name
   * @param name Имя, которое нужно найти среди функций
   * @return true или false если найдена и нет соответственно
   */
  bool FindFunc(std::string name) {
    for (auto val : funcs_) {
      if (val.name() == name) {
        return true;
      }
    }
    return false;
  }
  /*!
   * @brief Проверяет, есть ли структура с именем name
   * @param name Имя, которое нужно найти среди структур
   * @return true или false если найдена и нет соответственно
   */
  bool FindStruc(std::string name) {
    for (auto val : structs_) {
      if (val.name() == name) {
        return true;
      }
    }
    return false;
  }

  void Struct() {
    CheckLexeme("struct");
    PushLexeme();
    Identifier();
    std::string type = lexemes_[cur_ind_ - 1].text();
    StructInfo new_struct(type);
    if (tid_.CheckID(type) || FindFuncStruct(type)) {
      throw std::invalid_argument("Typename " + type + " is busy");
    }
    CheckLexeme("{");
    PushLexeme();
    tid_.NewScope();

    while (cur_ != "}" && lexemes_[cur_ind_ + 2] != "(" &&
           cur_.type() == LexemeTypes::Keyword &&
           lexemes_[cur_ind_ + 1].type() == LexemeTypes::Id) {
      VarDefinition();
    }

    for (auto val : tid_.cur()->rows()) {
      new_struct.AddVariable(val.type(), val.name());
    }

    while (cur_ != "}") {
      new_struct.AddMethod(Func());
    }

    structs_.push_back(new_struct);
    CheckLexeme("}");
    PushLexeme();
    tid_.Back();
    if (cur_ != ";") {
      ObjectNames(type);
    }
    CheckLexeme(";");
    PushLexeme();
  }
  /*!
   * @brief Анализ функции
   * @return Возвращает информацию о проанализированной функции
   */
  FunctionInfo Func() {
    Type();
    std::string type = lexemes_[cur_ind_ - 1].text();
    cur_func.set_type(type);
    Identifier();
    std::string id = lexemes_[cur_ind_ - 1].text();
    if (tid_.CheckID(type) || FindFuncStruct(type)) {
      throw std::invalid_argument("Object with the name of the function " +
                                  type + " already exists");
    }
    CheckLexeme("(");
    PushLexeme();
    tid_.NewScope();

    std::vector<std::pair<std::string, std::string>> args;
    if (cur_ != ")") {
      do {
        if (cur_ == ",") {
          PushLexeme();
        }
        Parameter();
        args.emplace_back(lexemes_[cur_ind_ - 2].text(),
                          lexemes_[cur_ind_ - 1].text());
      } while (cur_ == ",");
      CheckLexeme(")");
      PushLexeme();
    } else {
      PushLexeme();
    }

    cur_func = FunctionInfo(type, id, args);
    cur_func.set_poliz_start(poliz_.size());
    for (auto val : args) {
      cur_func.AddArgument(tid_.GetID(val.second).var());
    }


    Block(true);
    return cur_func;
  }
  void VarDefinition() {
    Type();
    if (lexemes_[cur_ind_ - 1].text() == "void") {
      throw std::invalid_argument(
          "Variable type can't be void in line " +
          std::to_string(lexemes_[cur_ind_ - 1].line()));
    }
    ObjectNames(lexemes_[cur_ind_ - 1].text());
    CheckLexeme(";");
    PushLexeme();
  }
  /*!
   * @brief Анализ оператора блок
   * @param func Является блок -- блоком функции или нет
   * Если блок принадлежит описанию функции, то создавать новый узел в TID не
   * нужно
   */
  void Block(bool func = false) {
    CheckLexeme("{");
    PushLexeme();
    if (!func) {
      tid_.NewScope();
    }
    if (cur_ != "}") {
      Operators();
    }
    CheckLexeme("}");
    PushLexeme();
    tid_.Back();
  }
  void Identifier() {
    if (cur_.type() != LexemeTypes::Id) {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
    PushLexeme();
  }
  void Definitions() {
    if (lexemes_[cur_ind_ + 2] == "(") {
      Func();
    } else {
      VarDefinition();
    }
    if (cur_ != "}") {
      Definitions();
    }
  }
  /*!
   * @brief Анализ имен объектов
   * @param type Какому типа принадлежат имена объектов
   */
  void ObjectNames(std::string type) {
    int size = 0;
    // без объявления функции
    Identifier();
    std::string id = lexemes_[cur_ind_ - 1].text();
    if (tid_.cur()->FindVar(id) || FindFuncStruct(id)) {
      throw std::invalid_argument("Variable name " + id + " is busy in line " +
                                  std::to_string(cur_.line()));
    }

    std::string list;
    if (cur_ == "[") {
      PushLexeme();
      IntLit();
      if (std::stoi(lexemes_[cur_ind_ - 1].text()) < 1) {
        throw std::invalid_argument("Array size must be pozitive in line " +
                                    std::to_string(cur_.line()));
      }
      size = std::stoi(lexemes_[cur_ind_ - 1].text());
      CheckLexeme("]");
      list = "l";
      PushLexeme();
      tid_.Push(id, list + type, size);
      cur_func.AddLocalVar(tid_.GetID(id).var());
    } else if (cur_ == "=") {
      tid_.Push(id, list + type, size);
      cur_func.AddLocalVar(tid_.GetID(id).var());

      PolizPush(tid_.GetID(id).var());
      PushLexeme();
      Expression();
      PolizPush("=");
    } else {
      tid_.Push(id, list + type, size);
      cur_func.AddLocalVar(tid_.GetID(id).var());
    }
    if (cur_ == ",") {
      PushLexeme();
      ObjectNames(type);
    }
  }
  void Type() {
    bool found_struct = false;
    for (auto val : structs_) {
      if (val.name() == cur_.text()) {
        found_struct = true;
      }
    }
    if (cur_ != "int" && cur_ != "float" && cur_ != "void" && cur_ != "bool" &&
        cur_ != "string" && !found_struct) {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
    PushLexeme();
  }
  void Parameter() {
    Type();
    Identifier();
    if (tid_.CheckID(lexemes_[cur_ind_ - 1].text()) ||
        FindFuncStruct(lexemes_[cur_ind_ - 1].text())) {
      throw std::invalid_argument("Function argument name " +
                                  lexemes_[cur_ind_ - 1].text() + " is busy");
    }
    tid_.Push(lexemes_[cur_ind_ - 1].text(), lexemes_[cur_ind_ - 2].text());
  }
  void Operators() {
    Operator();
    if (cur_ != "}") {
      Operators();
    }
  }
  void IntLit() {
    if (cur_.type() != LexemeTypes::NumLit ||
        cur_.text().find('.') != std::string::npos) {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
    PushLexeme();
  }
  void Expression() {
    ExpressionNoComma();
    while (cur_.type() == LexemeTypes::Priority10) {
      stack_.Push(cur_.text());
      auto tmp = cur_.text();
      PushLexeme();
      Expression();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void ExpressionNoComma() {
    Chain8();
    while (cur_.type() == LexemeTypes::Priority9) {
      stack_.Push(cur_.text());
      auto tmp = cur_.text();

      PushLexeme();
      ExpressionNoComma();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain8() {
    Chain7();
    while (cur_.type() == LexemeTypes::Priority8) {
      stack_.Push(cur_.text());
      auto tmp = cur_.text();

      PushLexeme();
      Chain8();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain7() {
    Chain6();
    while (cur_.type() == LexemeTypes::Priority7) {
      stack_.Push(cur_.text());

      auto tmp = cur_.text();

      PushLexeme();
      Chain7();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain6() {
    Chain5();
    while (cur_.type() == LexemeTypes::Priority6) {
      stack_.Push(cur_.text());

      auto tmp = cur_.text();

      PushLexeme();
      Chain6();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain5() {
    Chain4();
    while (cur_.type() == LexemeTypes::Priority5) {
      stack_.Push(cur_.text());

      auto tmp = cur_.text();

      PushLexeme();
      Chain5();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain4() {
    Chain3();
    while (cur_.type() == LexemeTypes::Priority4) {
      stack_.Push(cur_.text());

      auto tmp = cur_.text();

      PushLexeme();
      Chain4();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain3() {
    Chain2();
    while (cur_.type() == LexemeTypes::Priority3) {
      stack_.Push(cur_.text());

      auto tmp = cur_.text();

      PushLexeme();
      Chain3();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain2() {
    Chain1();
    while (cur_.type() == LexemeTypes::Priority2) {
      stack_.Push(cur_.text());

      auto tmp = cur_.text();

      PushLexeme();
      Chain2();

      PolizPush(tmp);

      stack_.CheckBin();
    }
  }
  void Chain1() {
    std::stack<std::string> opers;
    std::string tmp;
    if (cur_ == "--" || cur_ == "++" || cur_ == "!") {
      stack_.Push(cur_.text());
      tmp = cur_.text();
      PushLexeme();
      Chain1();
      stack_.CheckUnoL();

      PolizPush("pre" + tmp);
    }
    else {
      Chain0();
    }
  }
  void Chain0() {
    std::string tmp;
    if (cur_ == "+" || cur_ == "-") {
      PushLexeme();
      tmp = "un" + cur_.text();
    }
    ExpressionStop();
    while (cur_ == "[" || cur_ == "." ||
           cur_.type() == LexemeTypes::Priority0) {
      Priority0();
    }

    if (!tmp.empty()) {
      PolizPush(tmp);
    }
  }
  void Priority0() {
    if (cur_ == "[") {
      stack_.Push("[]");
      PushLexeme();
      Expression();
      CheckLexeme("]");
      stack_.CheckBin();
      PolizPush("[]");
      PushLexeme();
    } else if (cur_ == ".") {
      // to do
      PushLexeme();
      Identifier();
      std::string name = lexemes_[cur_ind_ - 1].text();
      StructInfo struc;
      for (auto val : structs_) {
        auto tmp = stack_.Top();
        if (tmp[0] == 'a') tmp.erase(tmp.begin());
        if (tmp == val.name()) {
          struc = val;
        }
      }
      if (cur_.text() == "(") {
        if (struc.FindMethod(name).name().empty()) {
          throw std::invalid_argument("No member " + name + " in struct " +
                                      struc.name());
        }
        cur_ = lexemes_[--cur_ind_];
        FuncCall(struc.FindMethod(name));
        stack_.Pop();
        stack_.Push(struc.FindMethod(name).type());
      } else {
        if (struc.FindVariable(name).empty()) {
          throw std::invalid_argument("No member " + name + " in struct " +
                                      struc.name());
        }
        stack_.Pop();
        stack_.Push("a" + struc.FindVariable(name));
      }
    } else if (cur_.type() == LexemeTypes::Priority0) {
      PushLexeme();
      PolizPush("post" + cur_.text());
    } else {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
  }
  void ExpressionStop() {
    if (cur_ == "(") {
      PushLexeme();
      Expression();
      CheckLexeme(")");
      PushLexeme();
    } else if (cur_.type() == LexemeTypes::NumLit ||
               LexemeTypes::StringLit == cur_.type() ||
               cur_.type() == LexemeTypes::BoolLit) {
      if (cur_.type() == LexemeTypes::BoolLit) {
        stack_.Push("bool");
        PolizLit lit(Bool);
        lit.data_.bool_ = cur_.text() == "true";
        PolizPush(lit);
      } else if (cur_.type() == LexemeTypes::NumLit) {
        if (cur_.text().find('.') != std::string::npos) {
          stack_.Push("float");
          PolizLit lit(Float);
          lit.data_.float_ = std::stof(cur_.text());
          PolizPush(lit);
        } else {
          stack_.Push("int");
          PolizLit lit(Int);
          lit.data_.int_ = std::stoi(cur_.text());
          PolizPush(lit);
        }
      } else if (LexemeTypes::StringLit == cur_.type()) {
        stack_.Push("string");
        PolizLit lit(String);
        lit.data_.string_ = cur_.text();
        PolizPush(lit);
      }
      PushLexeme();
    } else if (cur_.type() == LexemeTypes::Id) {
      if (tid_.CheckID(cur_.text())) {
        auto tmp = tid_.GetID(cur_.text()).type();
        if (tmp[0] != 'l')
          stack_.Push("a" + tid_.GetID(cur_.text()).type());
        else
          stack_.Push(tid_.GetID(cur_.text()).type());

        PolizPush(tid_.GetID(cur_.text()).var());

        PushLexeme();
      } else if (FindFunc(cur_.text()) || cur_func.name()==cur_.text()) {
        FunctionInfo func;
        for (auto val : funcs_) {
          if (val.name() == cur_.text()) {
            func = val;
          }
        }

        if (cur_func.name() == cur_.text()) {
          func = cur_func;
        }

        FuncCall(func);
        stack_.Push(func.type());
      } else {
        throw std::invalid_argument("No member named: " + cur_.text() +
                                    " in line " + std::to_string(cur_.line()));
      }
    } else {
      throw std::invalid_argument("error in lexeme " + cur_.text() +
                                  " in line " + std::to_string(cur_.line()));
    }
  }
  /*!
   * @brief Анализ вызова функции
   * @param func Информация о функции, которая вызывается
  */
  void FuncCall(FunctionInfo func) {
    PushLexeme();
    CheckLexeme("(");
    PushLexeme();

    int i = 0;
    while (cur_.text() != ")") {
      ExpressionNoComma();
      if (cur_ == ")") {
        i++;
        break;
      }
      CheckLexeme(",");
      PushLexeme();
      if (i >= func.args().size() ||
          !CompareTypes(stack_.Top(), func.args()[i++].first)) {
        throw std::invalid_argument(
            stack_.Top() + " can't be converted to " + func.args()[i++].first +
            " in function call or there are too much arguments in line " +
            std::to_string(cur_.line()));
      }
      stack_.Pop();
    }

    if (i < func.args().size()) {
      throw std::invalid_argument(
          "In function call too few arguments in line " +
          std::to_string(cur_.line()));
    }

    if (i > func.args().size()) {
      throw std::invalid_argument("There are too much arguments in line " +
          std::to_string(cur_.line()));
    }

    CheckLexeme(")");
    PolizPush(func.args().size(), func.name());
    PushLexeme();
  }
  void Operator() {
    if (cur_ == "if") {
      If();
    } else if (cur_ == "for") {
      For();
    } else if (cur_ == "while") {
      While();
    } /* else if (cur_ == "do") {
      DoWhile();
    }*/
    else if (cur_ == "return") {
      PushLexeme();
      if (cur_ != ";") {
        Expression();

        PolizPush("return");

        if (!CompareTypes(stack_.Top(), cur_func.type())) {
          throw std::invalid_argument(
              "Function return type " + cur_func.type() +
              " doesn't match with the return value type " + stack_.Top() +
              " in line " + std::to_string(cur_.line()));
        }
        CheckLexeme(";");
        PushLexeme();
        stack_.Pop();
      } else {
        if (cur_func.type() != "void") {
          throw std::invalid_argument("Function must return value in line " +
                                      std::to_string(cur_.line()));
        }
        PushLexeme();
      }
    } else if (cur_ == "in") {
      In();
    } else if (cur_ == "out") {
      Out();
    } else if (cur_ == "break") {
      PushLexeme();
      CheckLexeme(";");
      PushLexeme();

      if (loops_breaks_to_fill_.empty()) {
        throw std::invalid_argument("Break operator without loop in line " +
                                    std::to_string(cur_.line()));
      }

      loops_breaks_to_fill_.top().push_back(PolizSkip());
      PolizPush("B!");

    } else if (cur_ == "continue") {
      PushLexeme();
      CheckLexeme(";");
      PushLexeme();

      if (loops_starts_.empty()) {
        throw std::invalid_argument("Continue operator without loop in line " +
                                    std::to_string(cur_.line()));
      }

      PolizPush(loops_starts_.top());
      PolizPush("B!");

    } else if (cur_ == "{") {
      Block();
    } else if ((cur_.type() == LexemeTypes::Keyword ||
                FindStruc(cur_.text())) &&
               lexemes_[cur_ind_ + 1].type() == LexemeTypes::Id) {
      VarDefinition();
    } else if (cur_ == ";") {
      PushLexeme();
    } else {
      Expression();
      CheckLexeme(";");

      PolizPush(";");

      PushLexeme();
      stack_.Pop();
    }
  }
  void If() {
    CheckLexeme("if");
    PushLexeme();
    CheckLexeme("(");
    PushLexeme();
    Expression();

    int tmp1 = PolizSkip();
    PolizPush("F!");

    stack_.CheckBool();
    CheckLexeme(")");
    PushLexeme();
    Operator();

    int tmp2 = PolizSkip();
    PolizPush("B!");

    poliz_[tmp1] = new PolizAddress(poliz_.size());
    if (cur_ == "else") {
      PushLexeme();
      Operator();
    }


    poliz_[tmp2] = new PolizAddress(poliz_.size());
  }
  void While() {
    CheckLexeme("while");
    PushLexeme();
    CheckLexeme("(");
    PushLexeme();

    int tmp2 = poliz_.size();
    loops_starts_.push(tmp2);

    Expression();

    int tmp1 = PolizSkip();
    PolizPush("F!");

    stack_.CheckBool();
    CheckLexeme(")");
    PushLexeme();

    loops_breaks_to_fill_.push(std::vector<int>());

    Operator();


    PolizPush(tmp2);
    PolizPush("B!");
    
    poliz_[tmp1] = new PolizAddress(poliz_.size());

    loops_starts_.pop();

    for (auto val : loops_breaks_to_fill_.top()) {
      poliz_[val] = new PolizAddress(poliz_.size());
    }
    loops_breaks_to_fill_.pop();
  }
  void For() {
    CheckLexeme("for");
    PushLexeme();
    CheckLexeme("(");
    PushLexeme();
    if (cur_ != ";") {
      Expression();
      stack_.Pop();
    }
    CheckLexeme(";");
    PushLexeme();

    int tmp1 = poliz_.size();

    if (cur_ != ";") {
      Expression();
      stack_.CheckBool();
    }
    CheckLexeme(";");
    PushLexeme();

    int tmp2 = PolizSkip();
    PolizPush("F!");

    int tmp4 = PolizSkip();
    PolizPush("B!");

    int tmp3 = poliz_.size();
    loops_starts_.push(tmp3);
    if (cur_ != ")") {
      Expression();
      stack_.Pop();
    }

    PolizPush(tmp1);
    PolizPush("B!");

    CheckLexeme(")");
    PushLexeme();

    poliz_[tmp4] = new PolizAddress(poliz_.size());

    loops_breaks_to_fill_.push(std::vector<int>());

    Operator();

    PolizPush(tmp3);
    PolizPush("B!");

    poliz_[tmp2] = new PolizAddress(poliz_.size());

    loops_starts_.pop();
    for (auto val : loops_breaks_to_fill_.top()) {
      poliz_[val] = new PolizAddress(poliz_.size());
    }
    loops_breaks_to_fill_.pop();
  }
  void DoWhile() {
    CheckLexeme("do");
    PushLexeme();

    int tmp1 = poliz_.size();

    Operator();
    CheckLexeme("while");
    PushLexeme();
    CheckLexeme("(");
    PushLexeme();

    loops_starts_.push(poliz_.size());

    Expression();

    int tmp2 = PolizSkip();
    PolizPush("F!");

    PolizPush(tmp1);
    PolizPush("B!");

    stack_.CheckBool();
    CheckLexeme(")");
    PushLexeme();
    CheckLexeme(";");
    PushLexeme();

    poliz_[tmp2] = new PolizAddress(poliz_.size());
  }
  void In() {
    CheckLexeme("in");
    PushLexeme();
    CheckLexeme("(");
    PushLexeme();

    int n = 0;
    while (cur_ != ")") {
      ExpressionNoComma();
      stack_.Pop();
      n++;
      if (cur_ != ")") {
        CheckLexeme(",");
        PushLexeme();
        if (cur_ == ")") {
          throw std::invalid_argument("error in lexeme " + cur_.text() +
                                      " in line " +
                                      std::to_string(cur_.line()));
        }
      }
    }

    CheckLexeme(")");
    PushLexeme();
    CheckLexeme(";");
    PushLexeme();

    PolizLit lit(Int);
    lit.data_.int_ = n;
    PolizPush(lit);
    PolizPush("in");

  }
  void Out() {
    CheckLexeme("out");
    PushLexeme();
    CheckLexeme("(");
    PushLexeme();

    int n = 0;
    while (cur_ != ")") {
      ExpressionNoComma();
      stack_.Pop();
      n++;
      if (cur_ != ")") {
        CheckLexeme(",");
        PushLexeme();
        if (cur_ == ")") {
          throw std::invalid_argument("error in lexeme " + cur_.text() +
                                      " in line " +
                                      std::to_string(cur_.line()));
        }
      }
    }


    CheckLexeme(")");
    PushLexeme();
    CheckLexeme(";");
    PushLexeme();

    PolizLit lit(Int);
    lit.data_.int_ = n;
    PolizPush(lit);
    PolizPush("out");
  }
  void Expressions() {
    ExpressionNoComma();
    if (cur_ == ",") {
      stack_.Push(cur_.text());
      PushLexeme();
      Expressions();

      stack_.CheckBool();

      PolizPush(",");
    }
  }
};

int main() {
  Separator sep;
  sep.unload("text.txt");
  Compiler analyzer("separated.txt", "operations.txt");
  try {
    analyzer.Program();
    analyzer.WritePoliz("poliz.txt");
    analyzer.Launch();
  }
  catch (std::invalid_argument& err) {
    std::cout << err.what();
    return 0;
  }
  std::cout << "this is fine";
}
