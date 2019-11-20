#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <future>

#include "Core/Utility/ProgramConfigurations/LayeredConfiguration.h"
#include "Core/Utility/ProgramConfigurations/MapConfiguration.h"
#include "Core/Utility/ProgramConfigurations/FileImporter.h"
#include "Core/Utility/ProgramConfigurations/CLIImporter.h"
#include "Core/Utility/ProgramConfigurations/LogExporter.h"
#include "Core/Utility/LoggerSink.hpp"
#include "Core/Utility/Utils.h"
#include "Core/Utility/Timer.h"
#include "Core/Client.h"
#include "Core/Workloads/CoreWorkload.h"
#include "Core/DBFactory.h"

using namespace std;
using namespace dbbenchmark;
using namespace dbbenchmark::utility::programconfigurations;

static const std::string IDR_CONF_PATH = "GeneralSettings.configurationpath";
static const std::string IDR_CONF_DEFAULT_FILENAME = "dbsettings.xml";

int DelegateClient(std::shared_ptr<DB> db, std::shared_ptr<CoreWorkload> wl, const int num_ops,
    bool is_loading) {
  if (!db) {
    throw Exception("Database is not initilized!");
  }
  Client client(db, wl);
  int oks = 0;
  for (int i = 0; i < num_ops; ++i) {
    if (is_loading) {
      oks += client.DoInsert();
    } else {
      oks += client.DoTransaction();
    }
  }
  try{
    db->cleanup();
  }
  catch(const std::exception& e){
    LOG(WARNING) << e.what() << '\n';
  }
  return oks;
}

int main(const int argc, const char *argv[]) {
  //----------------------------- READING CONFIGURATIONS ---------------------------------------------

  // Layered Configuration singleton
  LayeredConfiguration& localConf = LayeredConfiguration::Instance();
  // Add CLI and XML Map Configuration objects to Layered singleton object.
  std::shared_ptr<MapConfiguration> mapConfCli( new MapConfiguration());
  std::shared_ptr<MapConfiguration> mapConfFile( new MapConfiguration());
  localConf.add(mapConfCli, "CLI Configuration", 1); //>! Configurations with lower priority values have precedence
  localConf.add(mapConfFile, "File Configuration", 2);
  FileImporter fileReader(mapConfFile);

  try {
      CliImporter cliReader(mapConfCli);
      cliReader.setCliParameters(argc, argv);
      cliReader.readProperties();
      bool IsFileSet = fileReader.setFileDirectory(localConf.getString(IDR_CONF_PATH, IDR_CONF_DEFAULT_FILENAME));
      if(IsFileSet) {
          fileReader.readProperties();
      }
  }
  catch(boost::program_options::error& rcError) {
      std::cerr << "Error while parsing command line options: " << rcError.what() << std::endl;
  }
  catch( std::exception& rcError ) {
      std::cerr << rcError.what() << std::endl;
  }
  //-------------------------------------------------------------------------------------------------
  //----------------------------- LOGGER INITIALIZATION ---------------------------------------------
  std::string logFileDirectory = localConf.getString("GeneralSettings.LogDirectory",
                                              "./logs/");
  utility::logger::Initializer::instance(argv[0], logFileDirectory, "", true);
  //-------------------------------------------------------------------------------------------------
  LogExporter logExporter;
  logExporter.flushProperties();

  DBFactory dbFactory;
  std::shared_ptr<DB> db = dbFactory.CreateDB();
  if (!db) {
    LOG(FATAL) << "Unknown database name " << localConf.getString("DBSettings.dbname") << endl;
  }

  auto wl = make_shared<CoreWorkload>();
  wl->Init();

  const int num_threads = localConf.getInt("GeneralSettings.numberofthreads", 1);

  // Loads data
  vector<future<int>> actual_ops;
  unsigned int total_ops = localConf.getUInt(CoreWorkload::RECORD_COUNT_PROPERTY);
  for (int i = 0; i < num_threads; ++i) {
    actual_ops.emplace_back(std::async(std::launch::async,
        DelegateClient, db, wl, total_ops / num_threads, true));
  }
  assert((int)actual_ops.size() == num_threads);

  int sum = 0;
  for (auto &n : actual_ops) {
    assert(n.valid());
    sum += n.get();
  }
  LOG(INFO) << "# Loading records:\t" << sum << endl;

  // Peforms transactions
  actual_ops.clear();
  total_ops = localConf.getUInt(CoreWorkload::RECORD_COUNT_PROPERTY);
  utility::Timer<double> timer;
  timer.Start();
  for (int i = 0; i < num_threads; ++i) {
    actual_ops.emplace_back(std::async(std::launch::async,
        DelegateClient, db, wl, total_ops / num_threads, false));
  }
  assert((int)actual_ops.size() == num_threads);

  sum = 0;
  for (auto &n : actual_ops) {
    assert(n.valid());
    sum += n.get();
  }
  double duration = timer.End();
  LOG(INFO) << "# Transaction throughput (KTPS)" << endl;
  LOG(INFO) << localConf.getString("DBSettings.dbname") << '\t'; //<< file_name << '\t' << num_threads << '\t';
  LOG(INFO) << total_ops / duration / 1000 << endl;
}

