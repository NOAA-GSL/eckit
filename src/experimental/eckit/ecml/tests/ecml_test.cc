/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <sstream>

#include "eckit/log/Log.h"
#include "eckit/runtime/Tool.h"

#include "experimental/eckit/ecml/core/ExecutionContext.h"
#include "eckit/runtime/Context.h"

// Disable ODB support for now
//#include "odb_api/tools/ODBModule.h"

using namespace std;
using namespace eckit;

class TestECML : public Tool {
public:
    TestECML (int argc, char **argv) : Tool(argc, argv) {}
    ~TestECML() {}
    
    virtual void run();
protected:
    virtual void runScript(const string& pathName);
};

void TestECML::run()
{
    int argc (Context::instance().argc());
    if (argc < 2)
        throw UserError("Command line required (name(s) of file(s) with ECML script");

    for (size_t i(1); i < Context::instance().argc(); ++i)
        runScript(Context::instance().argv(i));
}

void TestECML::runScript(const string& pathName)
{
    Log::info() << "ecml_test: running " << pathName << endl;
    ExecutionContext context;
    context.executeScriptFile(pathName);
}

int main(int argc,char **argv)
{
    //TODO: set a behaviour?
    //Context::instance().behavior( new odb::ODBBehavior() );
    // TODO: enable $DEBUG (Log::debug)


    // TODO: No support for ODB verbs right now.
    //odb::ODBModule odbModule;
    //app.executionContext().import(odbModule);

    TestECML runner(argc, argv);
    runner.start();

    return 0;
}