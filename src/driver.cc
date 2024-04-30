#include <iostream>

#include "db.hpp"
#include "db_table.hpp"

int main() {}

/**
 *   // write informal tests here
  Database db = Database();
  db.CreateTable("table");
  DbTable& table = db.GetTable("table");

  std::pair<std::string, DataType> col1 = {"name", DataType::kString};
  table.AddColumn(col1);
  std::cout << table;
  std::cout << "this is the output for one column and no rows" << std::endl;

  std::pair<std::string, DataType> col2 = {"age", DataType::kDouble};
  std::pair<std::string, DataType> col3 = {"pet age", DataType::kInt};
  table.AddColumn(col2);
  table.AddColumn(col3);
  std::cout << table;
  std::cout << "this is the output for n columns and no rows" << std::endl;

  std::initializer_list<std::string> col1_data = {
      "helena ilic", "19.4 ", " 3 "};
  table.AddRow(col1_data);
  std::cout << table;
  std::cout << "this is the output for n columns and 1 row" << std::endl;

  std::initializer_list<std::string> col2_data = {"stefan", "16.3", "1"};
  std::initializer_list<std::string> col3_data = {"simi kama", "19.0", "2"};
  std::initializer_list<std::string> col4_data = {"olivia meyer", "20", "3"};
  table.AddRow(col2_data);
  table.AddRow(col3_data);
  table.AddRow(col4_data);
  std::cout << table;
  std::cout << "this is the output for n columns and m rows" << std::endl;

  table.DeleteRowById(0);
  table.DeleteRowById(1);
  std::cout << table;
  std::cout << "this is the output after deleting my first row and a row from "
               "the middle "
            << std::endl;

  table.DeleteColumnByIdx(0);
  std::cout << table;
  std::cout << "this is the output after deleting a column" << std::endl;
*/
