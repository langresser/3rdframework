# This file is configured by CMake automatically as DartConfiguration.tcl
# If you choose not to use CMake, this file may be hand configured, by
# filling in the required variables.


# Configuration directories and files
SourceDirectory: E:/llvm/libarchive-3.0.4/libarchive-3.0.4
BuildDirectory: E:/llvm/libarchive-3.0.4/libarchive-3.0.4

# Where to place the cost data store
CostDataFile: 

# Site is something like machine.domain, i.e. pragmatic.crd
Site: LANGRESSER

# Build name is osname-revision-compiler, i.e. Linux-2.4.2-2smp-c++
BuildName: Win32-vs10

# Submission information
IsCDash: 
CDashVersion: 
QueryCDashVersion: 
DropSite: 
DropLocation: 
DropSiteUser: 
DropSitePassword: 
DropSiteMode: 
DropMethod: http
TriggerSite: 
ScpCommand: SCPCOMMAND-NOTFOUND

# Dashboard start time
NightlyStartTime: 00:00:00 EDT

# Commands for the build/test/submit cycle
ConfigureCommand: "C:/Program Files/CMake 2.8/bin/cmake.exe" "E:/llvm/libarchive-3.0.4/libarchive-3.0.4"
MakeCommand: C:\PROGRA~1\MICROS~2.0\Common7\IDE\devenv.com libarchive.sln /build ${CTEST_CONFIGURATION_TYPE} /project ALL_BUILD
DefaultCTestConfigurationType: Release

# CVS options
# Default is "-d -P -A"
CVSCommand: CVSCOMMAND-NOTFOUND
CVSUpdateOptions: -d -A -P

# Subversion options
SVNCommand: SVNCOMMAND-NOTFOUND
SVNUpdateOptions: 

# Git options
GITCommand: GITCOMMAND-NOTFOUND
GITUpdateOptions: 
GITUpdateCustom: 

# Generic update command
UpdateCommand: 
UpdateOptions: 
UpdateType: 

# Compiler info
Compiler: 

# Dynamic analysis (MemCheck)
PurifyCommand: 
ValgrindCommand: 
ValgrindCommandOptions: 
MemoryCheckCommand: MEMORYCHECK_COMMAND-NOTFOUND
MemoryCheckCommandOptions: 
MemoryCheckSuppressionFile: 

# Coverage
CoverageCommand: COVERAGE_COMMAND-NOTFOUND
CoverageExtraFlags: -l

# Cluster commands
SlurmBatchCommand: SLURM_SBATCH_COMMAND-NOTFOUND
SlurmRunCommand: SLURM_SRUN_COMMAND-NOTFOUND

# Testing options
# TimeOut is the amount of time in seconds to wait for processes
# to complete during testing.  After TimeOut seconds, the
# process will be summarily terminated.
# Currently set to 25 minutes
TimeOut: 1500

UseLaunchers: 0
CurlOptions: 
# warning, if you add new options here that have to do with submit,
# you have to update cmCTestSubmitCommand.cxx

# For CTest submissions that timeout, these options
# specify behavior for retrying the submission
CTestSubmitRetryDelay: 5
CTestSubmitRetryCount: 3
