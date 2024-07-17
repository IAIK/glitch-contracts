#include "Options.h"
#include "cxxopts.hpp"

void parse_value(const std::string& text, VerifMode& value)
{
    if(text == "stable") value = VerifMode::MODE_STABLE;
    else if (text == "transient") value = VerifMode::MODE_TRANS;
    else throw cxxopts::OptionException("Unsupported verification mode");
}

Options::Options(int argc, const char* argv[]):
    program_path(""), order(1),
    mode(VerifMode::MODE_TRANS), num_leaks(1)
{
    cxxopts::Options parser(argv[0], "simulator");
    parser.add_options()
        ("h,help", "Show this help menu")
        ("l,label", "Path to one or more label files", cxxopts::value<std::vector<std::string>>())
        ("p,program", "Path to a program file", cxxopts::value<std::string>())
        ("q,order", "Number of probes",
         cxxopts::value<int>()->default_value("1"))
        ("m,mode", "The verification mode (stable or transient)",
         cxxopts::value<VerifMode>()->default_value("transient"))
        ("n,num-leaks", "Number or reported leaks",
         cxxopts::value<int>()->default_value("1"));
    try
    {
        const int orig_argc = argc;
        cxxopts::ParseResult opts = parser.parse(argc, argv);
        if(orig_argc == 1 || opts.count("help"))
        {
            std::cout << parser.help() << std::endl;
            throw OptionsException("");
        }

        if(opts.count("label") == 0)
            throw cxxopts::OptionException("Missing label file");
        if(opts.count("program") == 0)
            throw cxxopts::OptionException("Missing program file");

        auto labels = std::move(opts["label"].as<std::vector<std::string>>());
        std::copy(labels.begin(), labels.end(), std::back_inserter(label_paths));        
        program_path = opts["program"].as<std::string>();

        if(opts.count("order"))
            order = opts["order"].as<int>();
        if(order < 1)
            throw cxxopts::OptionException("Order must be positive");
        if(opts.count("mode"))
            mode = opts["mode"].as<VerifMode>();
        if(opts.count("num-leaks"))
            num_leaks = opts["num-leaks"].as<int>();
        if(num_leaks < 0)
            throw cxxopts::OptionException("Number of leaks cannot be negative");
    }
    catch (const cxxopts::OptionException& e)
    {
        throw OptionsException(e.what());
    }
}

std::ostream& operator<<(std::ostream& out, Options& opts)
{
    for (const auto& path : opts.label_paths)
        out << "label:   " << path << std::endl;
    out << "program: " << opts.program_path << std::endl;
    out << "order:   " << opts.order << " ";
    out << "mode:    " << (opts.mode == VerifMode::MODE_TRANS ? "t" : "s") << std::endl;
    return out;
}
