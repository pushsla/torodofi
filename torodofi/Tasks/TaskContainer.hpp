#pragma once

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Task.hpp"

namespace toro {
namespace tasks {

const std::string priority_start_point = "## ";
const std::string string_repr_delimiter = "\n";

class TaskContainer {
protected:
  std::vector<Task> _tasks_active, _tasks_done;
  std::vector<std::string> _categories, _tags;
  std::string _filename;

  // Sort containered Tasks by priority;
  void _sort_priority(std::vector<Task> &tasks);
  // Reset IDs (f.ex after sorting)
  void _reset_id(std::vector<Task> &tasks);

public:
  TaskContainer();
  // Reads collection of tasks from .md file
  // .md have to be declared as way:
  // ## 1
  // * .... task
  // * .... task
  // ## 2
  // ......
  // ## (x) means (x) priority of listed tasks
  TaskContainer(const std::string& afilename);
  // Read tasks file
  void readFile(const std::string& afilename);
  // Dump task to a file back
  void Dump(std::string afilename = "");
  // Add new task
  void addTask(std::string atext, types::date expire,
               std::vector<std::string> atags,
               std::vector<std::string> acategories, unsigned apriority);
  void addTask(const Task& atask);
  // Delete task by id
  void delTask(size_t aid, bool is_active);
  // Add new category
  void addCategory(const std::string& acat);
  void addCategory(const std::vector<std::string>& acats);
  // Add new tag
  void addTag(const std::string& atag);
  void addTag(const std::vector<std::string>& atags);
  // Repr task container as formatted string with delimiter
  // Return choosen active/done list.
  // Return vector {fields caption, fields}
  std::vector<std::string>
  toString(bool is_active, bool reversed=false, const std::string& delimiter = string_repr_delimiter);

  // sort tasks by priority
  void sortByPriority(bool is_active=true);
  // Reset tasks id
  void resetIds();
  // Refresh _tasks_active and _tasks_done vectors
  void refreshActiveDone();

  // getters
  std::vector<Task> getTasks(bool is_active, bool reversed=false);
  std::vector<std::string> getTags();
  std::vector<std::string> getCategories();
  Task *getTask(size_t index, bool is_active);
};

} // namespace tasks
} // namespace toro
