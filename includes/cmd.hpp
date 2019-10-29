#ifndef CMD__HPP
#define CMD__HPP

#include <string>
#include <unordered_map>

class cmd
{
    public:
    struct arg
    {
        struct option
        {
            std::string string;

            option(const std::string& _string) : string(_string) { }

            bool match(const std::string& _string) const;
        };

        std::string string;
        std::vector<option> options;

        bool check_options(const std::vector<std::string>& _options) const;
        bool have_option(const std::string& _string) const;
        
        void clear();
        bool empty() const;
    };

    typedef void (*function)(const std::vector<arg>&);

    struct data
    {
        std::string string;
        std::string help;
        std::vector<std::vector<std::string>> options;
        function callback = nullptr;
        int maxArgCount = -1;

        data() { }
        data(const std::string& _string, function _callback, int _maxArgCount = -1)
        : string(_string), callback(_callback), maxArgCount(_maxArgCount) { }

        void add_options(const std::vector<std::string>& _options);
        void add_options(std::vector<std::string>&& _options);
        void set_help(const std::string& _help);

        void check_args(const std::vector<arg>& _args);
    };

    private:
    std::unordered_map<std::string, data> cmds;

    public:
    void add(const data& _cmd);
    void execute(const std::string& _command, int _argc, char const* _argv[]);

    private:
    std::vector<arg> parse_args(int _argc, char const* _argv[]);
};

#endif // !CMD__HPP