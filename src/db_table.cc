#include "db_table.hpp"

#include <iostream>
#include <stdexcept>

DbTable::DbTable(const DbTable& rhs) {
  // copy constructor
  // deep copy policy

  // populate rows_
  // do a deep copy of the array

  // going thorugh the rhs rows_ map
  for (const auto& [key, row] : rhs.rows_) {
    // iterate through the row
    rows_[key] = new void*[rhs.row_col_capacity_];
    for (unsigned int i = 0; i < rhs.col_descs_.size(); ++i) {
      if (rhs.col_descs_.at(i).second == DataType::kString) {
        // cast to right type, dereference, assign to string pointer
        std::string str = *static_cast<std::string*>(row[i]);
        // create new void pointer with that value
        rows_[key][i] = static_cast<void*>(new std::string(str));
      } else if (rhs.col_descs_.at(i).second == DataType::kDouble) {
        // cast to right type, dereference, assign to string pointer
        double dub = *static_cast<double*>(row[i]);
        // create new void pointer with that value
        rows_[key][i] = static_cast<void*>(new double(dub));
      } else if (rhs.col_descs_.at(i).second == DataType::kInt) {
        // cast to right type, dereference, assign to string pointer
        int num = *static_cast<int*>(row[i]);
        // create new void pointer with that value
        rows_[key][i] = static_cast<void*>(new int(num));
      }
    }
  }

  next_unique_id_ = rhs.next_unique_id_;
  row_col_capacity_ = rhs.row_col_capacity_;
  col_descs_ = rhs.col_descs_;
}

DbTable& DbTable::operator=(const DbTable& rhs) {
  if (this == &rhs) {
    return *this;
  }
  for (const auto& [key, row] : rows_) {
    for (unsigned int i = 0; i < col_descs_.size(); ++i) {
      if (col_descs_.at(i).second == DataType::kString) {
        delete static_cast<std::string*>(row[i]);
      } else if (col_descs_.at(i).second == DataType::kDouble) {
        delete static_cast<double*>(row[i]);
      } else if (col_descs_.at(i).second == DataType::kInt) {
        delete static_cast<int*>(row[i]);
      }
    }
    delete[] row;
  }
  rows_.clear();
  col_descs_ = rhs.col_descs_;
  next_unique_id_ = rhs.next_unique_id_;
  row_col_capacity_ = rhs.row_col_capacity_;
  for (const auto& [key, row] : rhs.rows_) {
    rows_[key] = new void*[rhs.row_col_capacity_];
    for (unsigned int i = 0; i < rhs.col_descs_.size(); ++i) {
      if (rhs.col_descs_.at(i).second == DataType::kString) {
        std::string str = *static_cast<std::string*>(row[i]);
        rows_[key][i] = static_cast<void*>(new std::string(str));
      } else if (rhs.col_descs_.at(i).second == DataType::kDouble) {
        double dub = *static_cast<double*>(row[i]);
        rows_[key][i] = static_cast<void*>(new double(dub));
      } else if (rhs.col_descs_.at(i).second == DataType::kInt) {
        int num = *static_cast<int*>(row[i]);
        rows_[key][i] = static_cast<void*>(new int(num));
      }
    }
  }
  return *this;
}

void DbTable::ResizeArray(void**& row_array) const {
  unsigned long num = 2;
  void** temp_row_array = new void*[row_col_capacity_ * num];
  for (unsigned int i = 0; i < row_col_capacity_; ++i) {
    temp_row_array[i] = row_array[i];
  }
  delete[] row_array;
  row_array = temp_row_array;
}

// const unsigned int kRowGrowthRate = 2;
void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
  // if col_descs_size() == row_col_capacity_
  if (col_descs_.size() == row_col_capacity_) {
    // resize every data row array mapped to by rows_ to have
    // row_col_capacity*2
    for (auto& [key, row] : rows_) {
      ResizeArray(row);
    }
    // update row_col_capacity to this new value
    row_col_capacity_ = row_col_capacity_ * 2;
  }

  // pushes back the col_desc std::pair to col_desks (increasing number of
  // table columns by 1)
  col_descs_.push_back(col_desc);

  // for every data row mapped by rows_
  for (const auto& [key, row] : rows_) {
    // void**& current_row = row;  // this is a void** object
    void*& new_col =
        row[col_descs_.size() - 1];  // going to the last col just added
    // set row's new column to new obj with 0 value of the column's type
    // (string - empty string, int 0, double 0.0) type in col_descs of new
    // column
    if (col_descs_.at(col_descs_.size() - 1).second == DataType::kString) {
      new_col = static_cast<void*>(new std::string(""));
    } else if (col_descs_.at(col_descs_.size() - 1).second == DataType::kInt) {
      new_col = static_cast<void*>(new int(0));
    } else if (col_descs_.at(col_descs_.size() - 1).second ==
               DataType::kDouble) {
      new_col = static_cast<void*>(new double(0.0));
    }
  }
}

void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
  // check whether col_idx is valid index into col_desks
  if (col_idx > col_descs_.size() - 1) {
    throw std::out_of_range("the index is out of range");
  }
  if (col_descs_.size() == 1 && !(rows_.empty())) {
    throw std::runtime_error(
        "any table with at least one row must have at least one column");
  }

  // for every mapped row in rows...
  for (const auto& [key, row] : rows_) {
    // deallocate object at row[col_idx]
    if (col_descs_.at(col_idx).second == DataType::kString) {
      delete static_cast<std::string*>(row[col_idx]);
    } else if (col_descs_.at(col_idx).second == DataType::kDouble) {
      delete static_cast<double*>(row[col_idx]);
    } else if (col_descs_.at(col_idx).second == DataType::kInt) {
      delete static_cast<int*>(row[col_idx]);
    }  // 5, 6, 8, 10, 10

    // shift all elements with indexes greater than col_idx down one index
    // position
    for (unsigned int i = col_idx; i < col_descs_.size() - 1; ++i) {
      row[i] = row[i + 1];
      // static_cast<void*>(new double(*static_cast<double*>(original[i])));
    }
    // delete that last index
    row[col_descs_.size() - 1] = nullptr;
  }  // 5, 6,  15 ,    15,    20

  // erase pair at col_idx from col_descs
  col_descs_.erase(col_descs_.begin() + col_idx);
}

void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
  // check number of items in col_data is equal to the number of column's in
  // the table's rows if not, throw std::invalid_argument exception
  if (col_data.size() != col_descs_.size()) {
    throw std::invalid_argument(
        "number of items in col_data is not equal to the number of column's in "
        "the table's rows");
  }

  // create new entry in rows_ with next_unique_id_ to row_col_capacity array
  // of void* objects
  rows_[next_unique_id_] = new void*[row_col_capacity_];
  // increment next_unique_id_ by one
  next_unique_id_ += 1;

  // insert col_data objects into the new row
  int index_counter = 0;
  for (const std::string& str : col_data) {
    // insert that str into the new row (convert if necessary)
    // then add it to the new row
    if (col_descs_.at(index_counter).second == DataType::kString) {
      rows_[next_unique_id_ - 1][index_counter] =
          static_cast<void*>(new std::string(str));
    } else if (col_descs_.at(index_counter).second == DataType::kDouble) {
      rows_[next_unique_id_ - 1][index_counter] =
          static_cast<void*>(new double(std::stod(str)));
    } else if (col_descs_.at(index_counter).second == DataType::kInt) {
      rows_[next_unique_id_ - 1][index_counter] =
          static_cast<void*>(new int(std::stoi(str)));
    }
    index_counter += 1;
  }
}

void DbTable::DeleteRowById(unsigned int id) {
  // delete a row from the table

  // if row with id doesn't exist, throw exception
  if (rows_.find(id) == rows_.end()) {
    throw std::invalid_argument("this key doesn't exist in the map");
  }

  // if every row's column points to object, release memory to those objects
  // iterate through the row
  for (unsigned int i = 0; i < col_descs_.size(); ++i) {
    // static_cast each void* pointer, invoke delete operator on the address
    // returned by that cast
    if (col_descs_.at(i).second == DataType::kString) {
      delete static_cast<std::string*>(rows_[id][i]);
    } else if (col_descs_.at(i).second == DataType::kDouble) {
      delete static_cast<double*>(rows_[id][i]);
    } else if (col_descs_.at(i).second == DataType::kInt) {
      delete static_cast<int*>(rows_[id][i]);
    }
  }

  // row is dynamically allocated to, invoke delete[] on the address in the
  // void** object
  delete[] rows_[id];

  // remove the row from rows_map
  rows_.erase(id);
}

DbTable::~DbTable() {
  // iterate through the row's map
  for (const auto& [key, value] : rows_) {
    // void** current_row = value;
    //  iterate through the row
    for (unsigned int i = 0; i < col_descs_.size(); ++i) {
      if (col_descs_.at(i).second == DataType::kString) {
        delete static_cast<std::string*>(value[i]);
      } else if (col_descs_.at(i).second == DataType::kDouble) {
        delete static_cast<double*>(value[i]);
      } else if (col_descs_.at(i).second == DataType::kInt) {
        delete static_cast<int*>(value[i]);
      }
    }
    delete[] value;
  }
  rows_.clear();
}

std::ostream& operator<<(std::ostream& os, const DbTable& table) {
  for (unsigned int i = 0; i < table.col_descs_.size(); ++i) {
    if (table.col_descs_.at(i).second == DataType::kString) {
      os << table.col_descs_.at(i).first << "(std::string)";
    } else if (table.col_descs_.at(i).second == DataType::kDouble) {
      os << table.col_descs_.at(i).first << "(double)";
    } else if (table.col_descs_.at(i).second == DataType::kInt) {
      os << table.col_descs_.at(i).first << "(int)";
    }
    if (i < table.col_descs_.size() - 1) {
      os << ", ";
    }
  }
  os << "\n";
  for (unsigned int index = 0; index < table.next_unique_id_; ++index) {
    if (table.rows_.contains(index)) {
      for (unsigned int i = 0; i < table.col_descs_.size(); ++i) {
        if (table.col_descs_.at(i).second == DataType::kString) {
          os << *(static_cast<std::string*>(table.rows_.at(index)[i]));
        } else if (table.col_descs_.at(i).second == DataType::kDouble) {
          os << *(static_cast<double*>(table.rows_.at(index)[i]));
        } else if (table.col_descs_.at(i).second == DataType::kInt) {
          os << *(static_cast<int*>(table.rows_.at(index)[i]));
        }

        if (i < table.col_descs_.size() - 1) {
          os << ", ";
        }
      }
      os << "\n";
    }
  }
  return os;
}

/**
 *   for (const auto& [key, row] : table.rows_) {
   // go through the cols in a specific row
   for (unsigned int i = 0; i < table.col_descs_.size(); ++i) {
     if (table.col_descs_.at(i).second == DataType::kString) {
       os << *(static_cast<std::string*>(row[i]));
     } else if (table.col_descs_.at(i).second == DataType::kDouble) {
       os << *(static_cast<double*>(row[i]));
     } else if (table.col_descs_.at(i).second == DataType::kInt) {
       os << *(static_cast<int*>(row[i]));
     }

     if (i < table.col_descs_.size() - 1) {
       os << ", ";
     }
   }

   os << "\n";
 }
*/