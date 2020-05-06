#include "Logger.h"

Logger::Logger(RunManager *runManager)
    : runManager(runManager) {}

Logger::Logger(RunManager *runManager, const QString &fileName)
    : runManager(runManager), fileName(fileName) {}

Logger::~Logger() {
    delete logTimer;
}

void Logger::start_logging() {
    assert(fileName.isEmpty() == false);

    runManager->register_component(this, fileName);
    runManager->set_file_header(this, generate_header());
    logging = true;
}

void Logger::stop_logging() {
    logging = false;
    runManager->deregister_component(this);
}
