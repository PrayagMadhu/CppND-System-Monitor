#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  ifstream file (kProcDirectory + kMeminfoFilename);
  string line, key;
  long int memtot, memfree;
  if (file.is_open())
  {
    getline(file, line);
    istringstream stream(line);
    stream>>key>>memtot;
    getline(file, line);
    istringstream stream1(line);
    stream1>>key>>memfree;
    float util= (float)(memtot-memfree)/memtot;
    return util;
  }
   return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    ifstream file (kProcDirectory + kUptimeFilename);
    string line;
    long int upactive, upidle;
    if (file.is_open())
    {
      getline(file, line);
      istringstream stream (line);
      stream>>upactive>>upidle;
      return upactive;
    }

  return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::ActiveJiffies(int pid) { //actually calculates cpu utilzation by the process
  ifstream file (kProcDirectory + to_string(pid) + kStatFilename);
  string line, temp;
  int utime, stime, cutime, cstime, start_time, hz, seconds;
  float sys_up, util;
  long int total;
  hz=sysconf(_SC_CLK_TCK);
  sys_up = (float)(LinuxParser::UpTime())/hz;
  
  if (file.is_open())
  {
    getline(file, line);
    istringstream stream (line);
    for (int i=0; i<=12; i++) stream>>temp;
    stream>>utime>>stime>>cutime>>cstime>>temp>>temp>>temp>>temp>>start_time;
    total=utime+stime+cutime+cstime;
    seconds=sys_up-(start_time/hz);
    util=(float)((float)(total/hz)/seconds);
    return util;
  }
  
  return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  ifstream file (kProcDirectory + kStatFilename);
  long int user, nice, system, irq, soft_irq, steal, temp;
  string line;
  if (file.is_open())
  {
    getline(file, line);
    istringstream stream (line);
    stream>>temp;
    stream>>user>>nice>>system>>temp>>temp>>irq>>soft_irq>>steal;
    return user+nice+system+irq+soft_irq+steal+temp; 
  }

  return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  ifstream file (kProcDirectory + kStatFilename);
  long int idle, iowait;
  string line, temp;
  if (file.is_open())
  {
    getline(file, line);
    istringstream stream (line);
    stream>>temp>>temp>>temp>>temp>>idle>>iowait;
    return idle+iowait;
  }
  return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() { 

long int PrevAct = LinuxParser::ActiveJiffies();
long int Act = LinuxParser::ActiveJiffies();
long int PrevIdle = LinuxParser::IdleJiffies();
long int Idle = LinuxParser::IdleJiffies();
long int PrevTotal = PrevAct + PrevIdle;
long int Total = Act + Idle;
long int Totald = Total - PrevTotal;
long int Idled = (Idle - PrevIdle);
return (float)(Totald - Idled)/Idled ;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  ifstream file (kProcDirectory + kStatFilename);
  string line, key;
  int value;
  if (file.is_open())
  {
    while(getline(file, line))
    {
      istringstream stream (line);
      stream>>key>>value;
      if (key=="processes") return value;
    }

  }return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  ifstream file (kProcDirectory + kStatFilename);
  string line, key;
  int value;
  if (file.is_open())
  {
    while(getline(file, line))
    {
      istringstream stream (line);
      stream>>key>>value;
      if (key=="procs_running") return value;
    }

  }

  return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  ifstream file (kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line, cmd;
  if (file.is_open())
  {
    getline(file, line);
    replace(line.begin(), line.end(), ' ', '_');
    istringstream stream (line);
    stream>>cmd;
    replace(cmd.begin(), cmd.end(), '_', ' ');
    return cmd;
  }

  return ""; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  ifstream file (kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, value;
  if (file.is_open())
  {
    while(getline(file, line))
    {
      istringstream stream (line);
      stream>>key>>value;
      if (key=="VmSize:") return value;
    }
  }
  
  return ""; }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  ifstream file (kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, value;
  if (file.is_open())
  {
    while(getline(file, line))
    {
      istringstream stream (line);
      stream>>key>>value;
      if (key=="Uid:") return value;
    }
  }
  return ""; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  ifstream file (kPasswordPath);
  string key, temp, line;
  int uid;
  if(file.is_open())
  {
    while(getline(file, line))
    {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream stream(line);
      stream>>key>>temp>>uid;
      if (uid==stol(LinuxParser::Uid(pid))) return key;
    }
  }
  
  return ""; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  ifstream file (kProcDirectory + to_string(pid) + kUptimeFilename);
  string line, temp;
  long int value;
  int count=0;
  
  if (file.is_open())
  {
    getline(file, line);
    istringstream stream (line);
    for (int i=0; i<=20; i++) stream>>temp;
    stream>>value;
    return value;
  }
  
  return 0; }