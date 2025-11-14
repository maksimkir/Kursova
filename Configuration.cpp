#include "Configuration.h"
#include <limits>

// Ініціалізація статичних констант
const std::string Configuration::PRODUCT_DATA_FILE = "store_items.csv";
const std::string Configuration::USER_DATA_FILE = "users.txt";

// Ініціалізація статичного logger
static Logger loggerInstance;
Logger* Configuration::s_logger = &loggerInstance;

void Configuration::ClearInputBuffer()
{
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}