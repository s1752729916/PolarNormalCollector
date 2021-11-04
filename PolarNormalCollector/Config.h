//Config.h  
#pragma once  

#include <string>  
#include <map>  
#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <vector>

/*
* \brief Generic configuration Class
*
*/
class Config {
    // Data  
protected:
    std::string m_Delimiter;  //!< separator between key and value  
    std::string m_Comment;    //!< separator between value and comments  
    std::map<std::string, std::string> m_Contents;  //!< extracted keys and values  
    std::string filename;
    typedef std::map<std::string, std::string>::iterator mapi;
    typedef std::map<std::string, std::string>::const_iterator mapci;

    // Methods  
public:

    Config(std::string filename, std::string delimiter = "=", std::string comment = "#");
    Config();
    ~Config();
    template<class T> T Read(const std::string& in_key) const;  //!<Search for key and read value or optional default value, call as read<T>  
    template<class T> T Read(const std::string& in_key, const T& in_value) const;
    template<class T> bool ReadInto(T& out_var, const std::string& in_key) const;
    template<class T> bool ReadInto(std::vector<T>& out_var, const std::string& in_key);
    template<class T> std::vector<T> string_as_vector(const std::string& s);//字符串转vector，默认用逗号分隔没细写
    template<class T>
    bool ReadInto(T& out_var, const std::string& in_key, const T& in_value) const;
    bool FileExist(std::string filename);
    void ReadFile(std::string filename, std::string delimiter = "=", std::string comment = "#");

    // Check whether key exists in configuration  
    bool KeyExists(const std::string& in_key) const;

    // Modify keys and values  
    template<class T> void Add(const std::string& in_key, const T& in_value);
    template<class T> void Add(const std::string& in_key, const std::vector<T>& value);
    void Remove(const std::string& in_key);

    // Check or change configuration syntax  
    std::string GetDelimiter() const { return m_Delimiter; }
    std::string GetComment() const { return m_Comment; }
    std::string SetDelimiter(const std::string& in_s)
    {
        std::string old = m_Delimiter;  m_Delimiter = in_s;  return old;
    }
    std::string SetComment(const std::string& in_s)
    {
        std::string old = m_Comment;  m_Comment = in_s;  return old;
    }

    // Write or read configuration  
    friend std::ostream& operator<<(std::ostream& os, const Config& cf);
    friend std::istream& operator>>(std::istream& is, Config& cf);

protected:
    template<class T> static std::string T_as_string(const T& t);
    template<class T> static std::string vector_as_string(const std::vector<T>& t);
    template<class T> static T string_as_T(const std::string& s);
    static void Trim(std::string& inout_s);



    // Exception types  
public:
    struct File_not_found {
        std::string filename;
        File_not_found(const std::string& filename_ = std::string())
            : filename(filename_) {}
    };
    struct Key_not_found {  // thrown only by T read(key) variant of read()  
        std::string key;
        Key_not_found(const std::string& key_ = std::string())
            : key(key_) {}
    };
};


/* static */
template<class T>
std::string Config::T_as_string(const T& t)
{
    // Convert from a T to a string  
    // Type T must support << operator  
    std::ostringstream ost;
    ost << t;
    return ost.str();
}


/* static */
template<class T>
T Config::string_as_T(const std::string& s)
{
    // Convert from a string to a T  
    // Type T must support >> operator  
    T t;
    std::istringstream ist(s);
    ist >> t;
    return t;
}



/* static */
template<>
inline std::string Config::string_as_T<std::string>(const std::string& s)
{
    // Convert from a string to a string  
    // In other words, do nothing  
    return s;
}


/* static */
template<>
inline bool Config::string_as_T<bool>(const std::string& s)
{
    // Convert from a string to a bool  
    // Interpret "false", "F", "no", "n", "0" as false  
    // Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true  
    bool b = true;
    std::string sup = s;
    for (std::string::iterator p = sup.begin(); p != sup.end(); ++p)
        *p = toupper(*p);  // make string all caps  
    if (sup == std::string("FALSE") || sup == std::string("F") ||
        sup == std::string("NO") || sup == std::string("N") ||
        sup == std::string("0") || sup == std::string("NONE"))
        b = false;
    return b;
}


template<class T>
T Config::Read(const std::string& key) const
{
    // Read the value corresponding to key  
    mapci p = m_Contents.find(key);
    if (p == m_Contents.end()) throw Key_not_found(key);
    return string_as_T<T>(p->second);
}


template<class T>
T Config::Read(const std::string& key, const T& value) const
{
    // Return the value corresponding to key or given default value  
    // if key is not found  
    mapci p = m_Contents.find(key);
    if (p == m_Contents.end()) return value;
    return string_as_T<T>(p->second);
}


template<class T>
bool Config::ReadInto(T& var, const std::string& key) const
{
    // Get the value corresponding to key and store in var  
    // Return true if key is found  
    // Otherwise leave var untouched  
    mapci p = m_Contents.find(key);
    bool found = (p != m_Contents.end());
    if (found) var = string_as_T<T>(p->second);
    return found;
}


template<class T>
bool Config::ReadInto(T& var, const std::string& key, const T& value) const
{
    // Get the value corresponding to key and store in var  
    // Return true if key is found  
    // Otherwise set var to given default  
    mapci p = m_Contents.find(key);
    bool found = (p != m_Contents.end());
    if (found)
        var = string_as_T<T>(p->second);
    else
        var = value;
    return found;
}


template<class T>
void Config::Add(const std::string& in_key, const T& value)
{
    // Add a key with given value  
    std::string v = T_as_string(value);
    std::string key = in_key;
    Trim(key);
    Trim(v);
    m_Contents[key] = v;
    std::ofstream out(filename.c_str());
    out << (*this);//Write Config to file
    return;
}

//简单尝试写了下读取数组到vector中，可以用，有兴趣可以自己再改改
template<class T>
std::vector<T> Config::string_as_vector(const std::string& s)//字符串转vector，默认用逗号分隔没细写
{
    std::vector<T> v;
    
    std::stringstream ss(s);
    std::string tmp;
    while (getline(ss, tmp, ','))
    {
        v.push_back(string_as_T<T>(tmp));
    }
    return v;
}

template<class T>
bool Config::ReadInto(std::vector<T>& out_var, const std::string& in_key) //读入数组，没有重写Read，有兴趣的可以自己写一下，感觉Read会难写一些
{
    mapci p = m_Contents.find(in_key);
    bool found = (p != m_Contents.end());
    if (found)
    {
        auto s = string_as_vector<T>(p->second);
        out_var.swap(s);
    }
    return found;
}

template<class T>
std::string Config::vector_as_string(const std::vector<T>& t) //将vector转化为字符串
{
    // Convert from a vector to a string  
    // Type T must support << operator  
    std::ostringstream ost;
    for (int i = 0; i < t.size()-1; i++)
    {
        ost << t[i];
        ost << ',';
    }
    ost << t[t.size() - 1];
    return ost.str();
}


template<class T> //添加vector型变量
void Config::Add(const std::string& in_key, const std::vector<T>& value)
{
    // Add a key with given value  
    std::string v = vector_as_string(value);
    std::string key = in_key;
    Trim(key);
    Trim(v);
    m_Contents[key] = v;
    std::ofstream out(filename.c_str());
    out << (*this);//Write Config to file
    return;
}
//vector<int> test;
//configSettings.ReadInto(test, "test");
//copy(test.begin(), test.end(), ostream_iterator<int>(cout, " "));//没有重写&lt;&lt;，随便打印看了下，是可以的
////test=1,2,3,4,5