#include <iostream>
#include <sstream>
#include <fstream>
#include "return_codes.h"
#include "quicksort.h"

template <typename T>
std::string toString(const T &val)
{
    return std::to_string(val);
}

template<>
std::string toString<Book>(const Book &val)
{
    std::stringstream stream;
    stream << val.Surname << " " << val.Name << " " << val.Patronymic << " " << val.Number;
    return stream.str();

}

template <typename T>
bool fromString(const std::string &val, T *res, std::string &err)
{
    err = "";
    std::istringstream iss(val);
    if (!(iss >> *res))
    {
        err = "Error reading value from string '" + val + "'";
        return false;
    }
    return true;
}

template<>
bool fromString<Book>(const std::string &val, Book *res, std::string &err)
{
    err = "";
    std::istringstream iss(val);
    if (!(iss >> res->Surname >> res->Name >> res->Patronymic >> res->Number))
    {
        err = "Error reading value from string '" + val + "'";
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    int rc = ERROR_SUCCESS;

    if (argc != 3)
    {
        std::cerr << "Incorrect arguments count; must be 2 file names: <input_file_name> <output_file_name>" << std::endl;
        rc = ERROR_INVALID_PARAMETER;
    }

    if (rc == ERROR_SUCCESS)
    {
        if ((argv[1] == nullptr) || (argv[2] == nullptr)) {
            std::cerr << "Either <input_file_name> or <output_file_name> is nullptr" << std::endl;
            rc = ERROR_INVALID_PARAMETER;
        }
    }

    std::string inFileName = "";
    std::string outFileName = "";
    std::ifstream inStream;
    if (rc == ERROR_SUCCESS)
    {
        inFileName = argv[1];
        outFileName = argv[2];

        inStream.open(inFileName, std::ios::in);
        if (!inStream.is_open())
        {
            std::cerr << "Error opening file '" << inFileName << "'" << std::endl;
            rc = ERROR_FILE_NOT_FOUND;
        }
    }

    std::string dataType = "";
    std::string sortType = "";
    int nItems = 0;
    if (rc == ERROR_SUCCESS)
    {
        if (!(inStream >> dataType >> sortType >> nItems))
        {
            std::cerr << "Error reading the input file header" << std::endl;
            inStream.close();
            rc = ERROR_INVALID_DATA;
        }
    }

    if (rc == ERROR_SUCCESS)
    {
        if ((dataType.compare("int") != 0) && (dataType.compare("float") != 0) && (dataType.compare("phonebook") != 0))
        {
            std::cerr << "Unexpected data type '" << dataType << "' in input file" << std::endl;
            inStream.close();
            rc = ERROR_INVALID_DATA;
        }
    }

    Qsort<int> *qsi = nullptr;
    if (dataType.compare("int") == 0)
    {
        qsi = new Qsort<int>();
    }
    Qsort<float> *qsf = nullptr;
    if (dataType.compare("float") == 0)
    {
        qsf = new Qsort<float>();
    }
    Qsort<Book> *qsb = nullptr;
    if (dataType.compare("phonebook") == 0)
    {
        qsb = new Qsort<Book>();
    }

    std::vector<int> *vi = nullptr;
    if (dataType.compare("int") == 0)
    {
        vi = new std::vector<int>();
    }
    std::vector<float> *vf = nullptr;
    if (dataType.compare("float") == 0)
    {
        vf = new std::vector<float>();
    }
    std::vector<Book> *vb = nullptr;
    if (dataType.compare("phonebook") == 0)
    {
        vb = new std::vector<Book>();
    }

    if (rc == ERROR_SUCCESS)
    {
        int counter = 0;
        std::string line;
        int ei;
        float ef;
        Book eb;
        std::string err;

        if (qsi != nullptr)
        {
            while ((rc == ERROR_SUCCESS) && (inStream >> line))
            {
                if (fromString<int>(line, &ei, err))
                {
                    vi->push_back(ei);
                    counter++;
                }
                else
                {
                    std::cerr << err << std::endl;
                    rc = ERROR_INVALID_DATA;
                }
            }
        }
        else if (qsf != nullptr)
        {
            while ((rc == ERROR_SUCCESS) && (inStream >> line))
            {
                if (fromString<float>(line, &ef, err))
                {
                    vf->push_back(ef);
                    counter++;
                }
                else
                {
                    std::cerr << err << std::endl;
                    rc = ERROR_INVALID_DATA;
                }
            }
        }
        else if (qsb != nullptr)
        {
            std::string f, n, p;
            uint64_t pn;
            while ((rc == ERROR_SUCCESS) && (inStream >> f >> n >> p >> pn))
            {
                eb.Surname = f;
                eb.Name = n;
                eb.Patronymic = p;
                eb.Number = pn;
                vb->push_back(eb);
                Book eb;
                counter++;
            }
        }
        inStream.close();
        if ((rc == ERROR_SUCCESS) && (counter != nItems))
        {
            std::cerr << "Wrong number of items in file, expected " << nItems << ", read " << counter << std::endl;
            rc = ERROR_INVALID_DATA;
        }
    }

    std::ofstream outStream;
    if (rc == ERROR_SUCCESS)
    {
        outStream.open(outFileName);
        if (!outStream.is_open())
        {
            std::cerr << "Error creating output file '" << outFileName << "'" << std::endl;
            rc = ERROR_FILE_NOT_FOUND;
        }
    }

    if (rc == ERROR_SUCCESS)
    {
        if (qsi != nullptr)
        {
            qsi->SetA(*vi);
            qsi->Sort((sortType.compare("ascending") == 0));
            std::vector<int> ri;
            qsi->GetA(ri);
            for (size_t i = 0; i < ri.size(); i++)
            {
                outStream << toString<int>(ri[i]) << std::endl;
            }
        }
        else if (qsf != nullptr)
        {
            qsf->SetA(*vf);
            qsf->Sort((sortType.compare("ascending") == 0));
            std::vector<float> rf;
            qsf->GetA(rf);
            for (size_t i = 0; i < rf.size(); i++)
            {
                outStream << toString<float>(rf[i]) << std::endl;
            }
        }
        else if (qsb != nullptr)
        {
            qsb->SetA(*vb);
            qsb->Sort((sortType.compare("ascending") == 0));
            std::vector<Book> rb;
            qsb->GetA(rb);
            for (size_t i = 0; i < rb.size(); i++)
            {
                outStream << toString<Book>(rb[i]) << std::endl;
            }
        }

        outStream.close();
    }

    // Clear memory
    if (qsi != nullptr)
    {
        delete qsi;
        delete vi;
    }
    if (qsf != nullptr)
    {
        delete qsf;
        delete vf;
    }
    if (qsb != nullptr)
    {
        delete qsb;
        delete vb;
    }

    return rc;
}
