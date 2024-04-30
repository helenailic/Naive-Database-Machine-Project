#include "db.hpp"

void Database::CreateTable(const std::string& table_name) {
  // creates new database table
  tables_[table_name] = new DbTable();
}

void Database::DropTable(const std::string& table_name) {
  // drops existing table from the database
  if (tables_.find(table_name) == tables_.end()) {
    throw std::invalid_argument("no key found in map");
  }

  delete tables_[table_name];
  tables_.erase(table_name);
}

DbTable& Database::GetTable(const std::string& table_name) {
  return *(tables_[table_name]);
}

Database::Database(const Database& rhs) {
  for (auto const& [key, value] : rhs.tables_) {
    tables_[key] = new DbTable(*value);
  }
}

Database& Database::operator=(const Database& rhs) {
  if (this == &rhs) {
    return *this;
  }

  // deallocate current
  for (auto const& [key, value] : tables_) {
    delete tables_[key];
  }
  tables_.clear();

  // then reassign
  for (auto const& [key, value] : rhs.tables_) {
    tables_[key] = new DbTable(*value);
  }

  return *this;
}

Database::~Database() {
  for (auto const& [key, value] : tables_) {
    delete tables_[key];
  }
  tables_.clear();
}

std::ostream& operator<<(std::ostream& os, const Database& db) {
  for (auto const& [key, value] : db.tables_) {
    os << key << '\n';
    os << *value << '\n';
  }
  return os;
}