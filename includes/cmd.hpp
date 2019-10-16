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

    public:
    typedef void (*function)(const std::vector<arg>&);

    private:
    std::unordered_map<std::string, function> functions; 

    public:
    void add(const std::string& _string, function _function);
    void execute(const std::string& _command, int _argc, char const* _argv[]);

    private:
    std::vector<arg> parse_args(int _argc, char const* _argv[]);
};

#endif // !CMD__HPP