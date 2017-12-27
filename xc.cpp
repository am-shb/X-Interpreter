#include<iostream>
#include<cstdlib>
#include<sstream>
#include<string>
#include<stdexcept>
#include<fstream>

#define VERSION "0.2 beta"
#define DATE "Dec 2017"

#define MAX_VARS 1000
#define MAX_LABELS 100

using namespace std;

double var_values[MAX_VARS];
bool var_types[MAX_VARS]; // 0 for int, 1 for real
string var_names[MAX_VARS];
int var_index = 0;

ifstream inputFile;
string label_names[MAX_LABELS];
streampos label_pos[MAX_LABELS];
int label_index = 0;

void error(string msg)
{
    if(inputFile.is_open())
        cout << "at: " << inputFile.tellg() << " -- ";
    cout << msg << endl;
}

bool check_variable_name(string name)
{
    if(name.size() > 8 || name.size() == 0)
        return false;

    for(int i=0; i<name.size(); i++)
        if(!(name[i] <= 'Z' && name[i] >= 'A') && !(name[i] <= 'z' && name[i] >= 'a'))
            return false;

    return true;
}

int label_exists(string name)
{
    for(int i=0; i<label_index; i++)
        if(name == label_names[i])
            return true;
    return false;
}

void make_label(string name, streampos pos)
{
    if(label_index == MAX_LABELS)
    {
        error("Out of memory");
        return;
    }

    if(label_exists(name))
    {
        error("Duplicate label name");
        return;
    }

    if(!inputFile.is_open())
    {
        error("Not in file mode");
        return;
    }

    if(check_variable_name(name))
    {
        label_pos[label_index] = pos;
        label_names[label_index] = name;
        label_index++;
    }
    else
    {
        error("Bad variable name");
    }
}

void goto_label(string name)
{
    int index = -1;
    for(int i=0; i<label_index; i++)
        if(name == label_names[i])
            index = i;

    if(index == -1)
    {
        error("Undefined label " + name);
        return;
    }

    inputFile.seekg(label_pos[index]);
}

int get_var_index_by_name(string name)
{
    for(int i=0; i<var_index; i++)
        if(name == var_names[i])
            return i;
    return -1;
}

bool var_exists(string name)
{
    if(get_var_index_by_name(name) == -1)
        return false;
    return true;
}

void make_variable(string name, bool type)
{
    if(var_index == MAX_VARS)
    {
        error("Out of memory");
        return;
    }

    if(var_exists(name))
    {
        error("Duplicate variable name");
        return;
    }

    if(check_variable_name(name))
    {
        var_values[var_index] = 0;
        var_types[var_index] = type;
        var_names[var_index] = name;
        var_index++;
    }
    else
    {
        error("Bad variable name");
    }
}

void make_int_variable(string name)
{
    make_variable(name, 0);
}

void make_real_variable(string name)
{
    make_variable(name, 1);
}

void dump_variable(string name)
{
    int index = get_var_index_by_name(name);
    if(index == -1)
    {
        error("undefined variable " + name);
        return;
    }
    cout << "index: " << index << endl;
    cout << "name: " << var_names[index] << endl;
    cout << "type: " << (var_types[index] ? "real" : "int") << endl;
    cout << "value: " << var_values[index] << endl << endl;
}

double get_var(string name)
{
    int index = get_var_index_by_name(name);
    if(index == -1)
    {
        error("undefined variable " + name);
        throw invalid_argument("undefined variable " + name);
        //return 0;
    }

    return var_values[index];
}

void set_var(string name, double value)
{
    int index = get_var_index_by_name(name);
    if(index == -1)
    {
        error("undefined variable " + name);
        return;
    }

    if(var_types[index] == 0 && value != (int) value)
    {
        error("Warning: Invalid conversation from real to int");
        value = (int) value;
        //return;
    }

    var_values[index] = value;
}

void print(string arg)
{
    if(arg[0] == '"')
    {
        if(arg[arg.size()-1] == '"')
        {
            for(int i=1; i<arg.size()-1; i++)
                cout << arg[i];
            cout << endl;
        }
        else
            error("expected \"");
    }
    else
    {
        try {
        cout << get_var(arg) << endl;
        } catch (invalid_argument e) {}
    }
}

void read(string name)
{
    double x;
    cin >> x;
    cin.sync();
    set_var(name, x);
}

bool is_numeric(string str)
{
    bool dot = false;
    for(int i=0; i<str.size(); i++)
    {
        if(str[i] == '.' && dot)
            return false;
        if(str[i] == '.')
            dot = true;
        if((str[i] > '9' || str[i] < '0') && str[i] != '.')
            return false;
    }
    return true;
}

void assign(string l, string r)
{
    double val;

    if(is_numeric(r)){
        val = strtod(r.c_str(), NULL);
    }
    else
    {
        try {
            val = get_var(r);
        } catch (invalid_argument e) {
            return;
        }
    }


    set_var(l, val);
}

void add(string s, string a1, string a2)
{
    double arg1, arg2;

    if(is_numeric(a1)){
        arg1 = strtod(a1.c_str(), NULL);
    }
    else
    {
        try {
            arg1 = get_var(a1);
        } catch (invalid_argument e) {
            return;
        }
    }
    if(is_numeric(a2)){
        arg2 = strtod(a2.c_str(), NULL);
    }
    else
    {
        try {
            arg2 = get_var(a2);
        } catch (invalid_argument e) {
            return;
        }
    }

    set_var(s, arg1+arg2);
}
void sub(string s, string a1, string a2)
{
    double arg1, arg2;

    if(is_numeric(a1)){
        arg1 = strtod(a1.c_str(), NULL);
    }
    else
    {
        try {
            arg1 = get_var(a1);
        } catch (invalid_argument e) {
            return;
        }
    }
    if(is_numeric(a2)){
        arg2 = strtod(a2.c_str(), NULL);
    }
    else
    {
        try {
            arg2 = get_var(a2);
        } catch (invalid_argument e) {
            return;
        }
    }

    set_var(s, arg1-arg2);
}
void mult(string s, string a1, string a2)
{
    double arg1, arg2;

    if(is_numeric(a1)){
        arg1 = strtod(a1.c_str(), NULL);
    }
    else
    {
        try {
            arg1 = get_var(a1);
        } catch (invalid_argument e) {
            return;
        }
    }
    if(is_numeric(a2)){
        arg2 = strtod(a2.c_str(), NULL);
    }
    else
    {
        try {
            arg2 = get_var(a2);
        } catch (invalid_argument e) {
            return;
        }
    }

    set_var(s, arg1*arg2);
}
void divi(string s, string a1, string a2)
{
    double arg1, arg2;

    if(is_numeric(a1)){
        arg1 = strtod(a1.c_str(), NULL);
    }
    else
    {
        try {
            arg1 = get_var(a1);
        } catch (invalid_argument e) {
            return;
        }
    }
    if(is_numeric(a2)){
        arg2 = strtod(a2.c_str(), NULL);
    }
    else
    {
        try {
            arg2 = get_var(a2);
        } catch (invalid_argument e) {
            return;
        }
    }

    if(arg2 == 0)
    {
        error("Div by 0");
        return;
    }

    set_var(s, (int)arg1/arg2);
}
void divr(string s, string a1, string a2)
{
    double arg1, arg2;

    if(is_numeric(a1)){
        arg1 = strtod(a1.c_str(), NULL);
    }
    else
    {
        try {
            arg1 = get_var(a1);
        } catch (invalid_argument e) {
            return;
        }
    }
    if(is_numeric(a2)){
        arg2 = strtod(a2.c_str(), NULL);
    }
    else
    {
        try {
            arg2 = get_var(a2);
        } catch (invalid_argument e) {
            return;
        }
    }

    if(arg2 == 0)
    {
        error("Div by 0");
        return;
    }

    set_var(s, arg1/arg2);
}
void mod(string s, string a1, string a2)
{
    double arg1, arg2;

    if(is_numeric(a1)){
        arg1 = strtod(a1.c_str(), NULL);
    }
    else
    {
        try {
            arg1 = get_var(a1);
        } catch (invalid_argument e) {
            return;
        }
    }
    if(is_numeric(a2)){
        arg2 = strtod(a2.c_str(), NULL);
    }
    else
    {
        try {
            arg2 = get_var(a2);
        } catch (invalid_argument e) {
            return;
        }
    }

    if((int)arg1 != arg1 || (int)arg2 != arg2)
    {
        error("Warning: MOD operands should be integer");
    }

    set_var(s, (int)arg1%(int)arg2);
}

string strtoupper(string str)
{
    for(int i=0; i<str.size(); i++)
    {
        str[i] = toupper(str[i]);
    }
    return str;
}

string remove_extra_whitespaces(string str)
{
    string output;

    for(int i=0; i<str.size(); i++)
    {
        if(str[i] == ' ')
            while(str[i+1] == ' ')
                i++;

        output += str[i];
    }

    //trim first and last space
    if(output[0] == ' ') output.erase(0, 1);
    if(output[output.size()-1] == ' ') output.erase(output.size()-1);

    return output;
}

void end_program()
{
    cout << "Thanks for using our interpreter!\n";
    exit(0);
}

void print_help()
{
    cout << "X interpreter version " << VERSION << " -- " << DATE << endl;
    cout << "Usage: xc.exe [file] \t run input file\n" << endl;
    cout << "Available Commands: " << endl;
    cout << "INT [var_name],...\t\t\t\t- Define an integer variable" << endl;
    cout << "REAL [var_name],...\t\t\t\t- Define a real variable" << endl;
    cout << "ASSIGN [var], [value|var]\t\t\t- Assign a value to variable" << endl;
    cout << "READ [var],...\t\t\t\t\t- Read a value from stdin and put it in variable" << endl;
    cout << "PRINT [var|literal],...\t\t\t\t- Print a variable or an string literal to stdout" << endl;
    cout << "DUMP [var],...\t\t\t\t- Dump a variable info" << endl;
    cout << "ADD [var], [var|literal], [var|literal]\t\t- Adds second and third argument and put it on first argument" << endl;
    cout << "SUB [var], [var|literal], [var|literal]\t\t- Subtracts second and third argument and put it on first argument" << endl;
    cout << "MULT [var], [var|literal], [var|literal]\t- Multiplies second and third argument and put it on first argument" << endl;
    cout << "DIVI [var], [var|literal], [var|literal]\t- Divides(integer) second & third argument and put it on first argument" << endl;
    cout << "DIVR [var], [var|literal], [var|literal]\t- Divides(real) second and third argument and put it on first argument" << endl;
    cout << "MOD [var], [var|literal], [var|literal]\t\t- MOD second and third argument and put it on first argument" << endl;
    cout << "LABEL [label_name]\t\t\t\t- Set a label at current line" << endl;
    cout << "IFGO [var], [var], [label]\t\t\t- Goto label if var1 != var2" << endl;
    cout << "EXIT\t\t\t\t\t\t- Exit interpreter" << endl;
    cout << endl;
    cout << "   ===========================================================================" << endl;
    cout << "   ||   Credits: Amir Shahbandegan - shahbandegan.ir - hi@shahbandegan.ir   ||" << endl;
    cout << "   ===========================================================================" << endl;
}

void parse(string input)
{
    input = remove_extra_whitespaces(input);

    string cmd, args;
    int i=0;
    while(input[i] != ' ' && i < input.size()) cmd += input[i++];
    i++;
    while(i < input.size()) args += input[i++];

    istringstream argss(args);

    cmd = strtoupper(cmd);

    if(cmd == "EXIT")
        end_program();
    else if(cmd == "INT")
    {
        for (string arg; getline(argss, arg, ','); )
        {
            arg = remove_extra_whitespaces(arg);
            make_int_variable(arg);
        }
    }
    else if(cmd == "REAL")
    {
        for (string arg; getline(argss, arg, ','); )
        {
            arg = remove_extra_whitespaces(arg);
            make_real_variable(arg);
        }
    }
    else if(cmd == "DUMP")
    {
        for (string arg; getline(argss, arg, ','); )
        {
            arg = remove_extra_whitespaces(arg);
            dump_variable(arg);
        }
    }
    else if(cmd == "PRINT")
    {
        for (string arg; getline(argss, arg, ','); )
        {
            arg = remove_extra_whitespaces(arg);
            print(arg);
        }
    }
    else if(cmd == "READ")
    {
        for (string arg; getline(argss, arg, ','); )
        {
            arg = remove_extra_whitespaces(arg);
            read(arg);
        }
    }
    else if(cmd == "ASSIGN")
    {
        string l,r;
        getline(argss, l, ',');
        getline(argss, r, ',');
        assign(remove_extra_whitespaces(l), remove_extra_whitespaces(r));
    }
    else if(cmd == "ADD")
    {
        string s,a1,a2;
        getline(argss, s, ',');
        getline(argss, a1, ',');
        getline(argss, a2, ',');
        add(remove_extra_whitespaces(s), remove_extra_whitespaces(a1), remove_extra_whitespaces(a2));
    }
    else if(cmd == "SUB")
    {
        string s,a1,a2;
        getline(argss, s, ',');
        getline(argss, a1, ',');
        getline(argss, a2, ',');
        sub(remove_extra_whitespaces(s), remove_extra_whitespaces(a1), remove_extra_whitespaces(a2));
    }
    else if(cmd == "MULT")
    {
        string s,a1,a2;
        getline(argss, s, ',');
        getline(argss, a1, ',');
        getline(argss, a2, ',');
        mult(remove_extra_whitespaces(s), remove_extra_whitespaces(a1), remove_extra_whitespaces(a2));
    }
    else if(cmd == "DIVI")
    {
        string s,a1,a2;
        getline(argss, s, ',');
        getline(argss, a1, ',');
        getline(argss, a2, ',');
        divi(remove_extra_whitespaces(s), remove_extra_whitespaces(a1), remove_extra_whitespaces(a2));
    }
    else if(cmd == "DIVR")
    {
        string s,a1,a2;
        getline(argss, s, ',');
        getline(argss, a1, ',');
        getline(argss, a2, ',');
        divr(remove_extra_whitespaces(s), remove_extra_whitespaces(a1), remove_extra_whitespaces(a2));
    }
    else if(cmd == "MOD")
    {
        string s,a1,a2;
        getline(argss, s, ',');
        getline(argss, a1, ',');
        getline(argss, a2, ',');
        mod(remove_extra_whitespaces(s), remove_extra_whitespaces(a1), remove_extra_whitespaces(a2));
    }
    else if(cmd == "LABEL")
    {
        make_label(args, inputFile.tellg());
    }
    else if(cmd == "IFGO")
    {
        string v1,v2,label;
        getline(argss, v1, ',');
        getline(argss, v2, ',');
        getline(argss, label, ',');

        try {
            if(get_var(remove_extra_whitespaces(v1)) != get_var(remove_extra_whitespaces(v2)))
                goto_label(remove_extra_whitespaces(label));
        } catch(invalid_argument e) {}
    }
    else if(cmd == "HELP")
    {
        print_help();
    }
    else if(cmd == "\n" || cmd == "\r\n" || cmd == "");
    else
        error("Unknown Command!");
}

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        string line;
        inputFile.rdbuf()->pubsetbuf(NULL, 0);
        inputFile.open(argv[1]);
        if(inputFile.is_open())
        {
            while(getline(inputFile, line))
            {
                parse(line);
            }
            inputFile.close();
        }
        else
            cout << "Unable to open file\n";
    }
    else
    {
        string input;
        cout << "Welcome to the X interpreter!\nType help to see available commands\n";
        while(1)
        {
            cout << "->";
            getline(cin, input);
            parse(input);
        }
    }

}
