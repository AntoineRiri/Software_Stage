#pragma once


#include <iostream>
#include <exception>
#include <stdexcept>
#include <excpt.h>
#include <string>



typedef std::string str;

class Excep : public std::exception
{
public:
    Excep(str const& label_ = "", int level_ = 0) throw()
        : label(label_), level(level_)
    {}

    virtual const char* what() const throw()
    {
        return label.c_str();
    }

    int getLevel() const throw()
    {
        return level;
    }

    virtual ~Excep() throw()
    {}

private:        
    str label;          //Description de l'erreur
    int level;          //Niveau de l'erreur
};