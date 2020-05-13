#include "TaskContainer.hpp"

using namespace std;

namespace toro {
namespace tasks {

// class TaskContainer
TaskContainer::TaskContainer() {
  _tags = {no_tag};
  _categories = {no_category};
}
TaskContainer::TaskContainer(string afilename) {
  _tags = {no_tag};
  _categories = {no_category};

  readFile(afilename);
}

void TaskContainer::Dump(std::string afilename) {
  if (afilename == "") {
    afilename = _filename;
  }
  unsigned priority;
  Task cmptask;
  ofstream file(afilename);

  if (file.is_open()) {
    for (size_t t = 0; t < _tasks.size(); t++) {
      cmptask = _tasks[t];
      if (cmptask.getTask().priority != priority) {
        priority = cmptask.getTask().priority;
        file << priority_start_point + to_string(priority) << endl;
      }
      file << cmptask.toFileString() << endl;
    }
  } else {
    printf("tasks::TaskContainer: string afilename: %s\n", afilename.c_str());
    throw std::runtime_error("Bad IO: could not create file");
  }
}

void TaskContainer::readFile(string afilename) {
  size_t pri_del_len = priority_start_point.length();
  string line;
  unsigned priority;
  vector<string> tmp;
  ifstream file(afilename);

  if (file.is_open()) {
    // Read file: priorities and tasks
    while (getline(file, line)) {
      if (line.find(priority_start_point) == 0) {
        priority = atoi(line.substr(pri_del_len, pri_del_len + 2).c_str());
      } else {
        if (line.find(task_start_point_active) >= 0 ||
            line.find(task_start_point_done) >= 0) {
          _tasks.push_back(Task(line, priority));
        }
      }
    }
    // If read was successfull, then we can store afilename
    _filename = afilename;
    // Sort read tasks by priority
    _sort_priority();
    // Aggregate all tags and categories
    for (size_t t = 0; t < _tasks.size(); t++) {
      addTag(_tasks[t].getTags());
      addCategory(_tasks[t].getCategories());
    }
  } else {
    printf("tasks::TaskContainer: string afilename: %s\n", afilename.c_str());
    throw std::invalid_argument("File could not be opened");
  }
}

void TaskContainer::addTask(string atext, types::date expire,
                            vector<string> atags, vector<string> acategories,
                            unsigned apriority) {
  Task new_task(atext, expire, atags, acategories, apriority);
  _tasks.push_back(new_task);
  _sort_priority();
}

void TaskContainer::delTask(size_t aid) {
  for (size_t i = 0; i < _tasks.size(); i++) {
    if (_tasks[i].getId() == aid) {
      _tasks.erase(_tasks.begin() + i);
    }
  }
}

void TaskContainer::addCategory(string acat) {
  if (find(_categories.begin(), _categories.end(), acat) == _categories.end()) {
    _categories.push_back(acat);
  }
}

void TaskContainer::addCategory(vector<string> acats) {
  if (acats.size() > 0) {
    for (size_t i = 0; i < acats.size(); i++) {
      if (acats[i] != empty_category) {
        addCategory(acats[i]);
      }
    }
  }
}

void TaskContainer::addTag(string atag) {
  if (find(_tags.begin(), _tags.end(), atag) == _tags.end()) {
    _tags.push_back(atag);
  }
}

void TaskContainer::addTag(vector<string> atags) {
  if (atags.size() > 0) {
    for (size_t i = 0; i < atags.size(); i++) {
      if (atags[i] != empty_tag) {
        addTag(atags[i]);
      }
    }
  }
}

string TaskContainer::toString(bool is_active, string delimiter) {
  string result;

  for (size_t t = 0; t < _tasks.size() - 1; t++) {
    if (_tasks[t].getActive() == is_active) {
      result += _tasks[t].toString() + delimiter;
    }
  }
  if (_tasks[_tasks.size() - 1].getActive() == is_active) {
    result += _tasks[_tasks.size() - 1].toString();
  }

  return logic::linuxColumns(result);
}

void TaskContainer::sortByPriority() { _sort_priority(); }

// protected
void TaskContainer::_sort_priority() {
  sort(_tasks.begin(), _tasks.end(), cmp_prioroty);
  for (size_t i = 0; i < _tasks.size(); i++) {
    _tasks[i]._setId(i);
  }
}

// getters
vector<string> TaskContainer::getTags() {
  for (size_t s = 0; s < _tags.size(); s++) {
    printf("%s\n", _tags[s].c_str());
  }
  return _tags;
}

vector<string> TaskContainer::getCategories() { return _categories; }

vector<Task> TaskContainer::getTasks() { return _tasks; }
Task *TaskContainer::getTask(size_t index) { return &_tasks[index]; }

} // namespace tasks
} // namespace toro
