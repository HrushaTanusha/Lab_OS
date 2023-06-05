#include <Windows.h>
#include <iostream>
#include <cmath>

struct parametr
{
    int* array;
    int arraySize;
    int min, max, arr_average;
};

void min_max(void* param);
void average(void* param);

int getValidInteger();
int getArrSize();

int main()
{
    parametr parameters;

    parameters.arraySize = getArrSize();

    parameters.array = new int[parameters.arraySize];
    for (int i = 0; i < parameters.arraySize; i++)
    {
        std::cout << "Element " << i + 1 << ": ";
        parameters.array[i] = getValidInteger();
    }


    HANDLE hThreadMin_max, hThreadAverage;
    DWORD  IDThreadMin_max, IDThreadAverage;
    hThreadMin_max = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)min_max, &parameters, 0, &IDThreadMin_max);
    hThreadAverage = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)average, &parameters, 0, &IDThreadAverage);

    WaitForSingleObject(hThreadMin_max, INFINITE);
    
    WaitForSingleObject(hThreadAverage, INFINITE);
    CloseHandle(hThreadAverage);
    CloseHandle(hThreadMin_max);

    for (int i = 0; i < parameters.arraySize; i++)
    {
        if (parameters.array[i] == parameters.min || parameters.array[i] == parameters.max)
        {
            parameters.array[i] = parameters.arr_average;
        }
    }

    std::cout << "Array after replacement: ";
    for (int i = 0; i < parameters.arraySize; i++)
    {
        std::cout << parameters.array[i] << " ";
    }

    delete[] parameters.array;

}

void min_max(void* param)
{
    parametr* parameters = (parametr*)param;

    parameters->min = parameters->array[0];
    parameters->max = parameters->array[0];

    for (int i = 1; i < parameters->arraySize; i++)
    {
        if (parameters->array[i] < parameters->min)
            parameters->min = parameters->array[i];

        Sleep(7);

        if (parameters->array[i] > parameters->max)
            parameters->max = parameters->array[i];

        Sleep(7);
    }

    std::cout << "min number: " << parameters->min << "\nmax number: " << parameters->max << std::endl;

    ExitThread(1);

}
void average(void* param)
{
    parametr* parameters = (parametr*)param;
    double arrSum = 0;

    for (int i = 0; i < parameters->arraySize; i++)
    {
        arrSum += parameters->array[i];
        Sleep(12);
    }

    parameters->arr_average = round(arrSum / parameters->arraySize);
    std::cout << "array's average: " << parameters->arr_average << std::endl;

    ExitThread(1);
}

int getValidInteger()
{
    double num;
    while (true)
    {
        if (std::cin >> num && num == (int)num)
        {
            return num;
        }
        else
        {
            std::cout << "Please input an integer value." << std::endl;
            std::cin.clear();
            while (std::cin.get() != '\n');
        }
    }

}

int getArrSize()
{
    int arrSize;

    while (true)
    {
        std::cout << "Enter array size: ";
        arrSize = getValidInteger();
        if (arrSize <= 0)
        {
            std::cout << "Please enter the integer that is bigger than 0" << std::endl;
        }
        else
        {
            break;
        }

    }
    system("pause");
    return arrSize;
}

