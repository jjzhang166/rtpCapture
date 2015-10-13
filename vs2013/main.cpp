#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/FileChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

int main(int argc, char** argv)
{
    Poco::FormattingChannel* Fchannel = new Poco::FormattingChannel(new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%T]:[%q] %s: %t"));
    Fchannel->setChannel(new Poco::FileChannel("sample.log"));
    Fchannel->getChannel()->setProperty("rotation", "1 M");
    Poco::Logger::root().setChannel(Fchannel);
    Poco::Logger& logger = Poco::Logger::get("main"); // inherits root channel

    logger.information("init sdk ...");

    return 0;
}