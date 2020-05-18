#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    Processor cpu;
    cpu_=cpu;
    return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    std::vector<int> pids = LinuxParser::Pids();
    for (auto x:pids)
    {
        Process p(x);
        processes_.push_back(p);
    }
    return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    string kernal_ver = LinuxParser::Kernel();
    return kernal_ver; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    float mem_util = LinuxParser::MemoryUtilization();
    return mem_util; }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    string os = LinuxParser::OperatingSystem();
    return os; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    int running = LinuxParser::RunningProcesses();
     return running; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    int tot = LinuxParser::TotalProcesses();
    return tot; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    long int up = LinuxParser::UpTime();
    return up; }