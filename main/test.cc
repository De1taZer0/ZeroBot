#include <hv/requests.h>
#include <string>

int main()
{
    using string = std::string;

    SetConsoleOutputCP(65001);

    std::hash<string> hash;

    for(int i = 1; i <= 100; ++i)
    {
        string a = std::to_string(i);
        std::cout << static_cast<double>(hash(a+a+a+a)) / SIZE_MAX * 100 << std::endl;
    }

    system("pause");
    return 0;
}