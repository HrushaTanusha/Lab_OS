#include <Windows.h>
#include <iostream>
#include <vector>

void marker(void* param);
void printArray(int* arr, int arraySize);

CRITICAL_SECTION cs; //объект CRITICAL_SECTION cs создается и инициализируется с помощью функции InitializeCriticalSection для защиты доступа к общему массиву arr, к которому могут обращаться все потоки одновременно

HANDLE hContinue; //HANDLE hContinue создается с помощью функции CreateEvent и используется для синхронизации между потоками. Когда главный поток сигнализирует событие, все запущенные потоки могут продолжить свою работу. В коде это реализуется с помощью функции PulseEvent(hContinue), которая сигнализирует событие hContinue, и функции WaitForMultipleObjects(2, events, FALSE, INFINITE), которая ждет сигнала на событии hContinue или на событии завершения потока.
HANDLE* markersH;
HANDLE* waitH;
HANDLE* finishH;
//объекты HANDLE markersH, waitH и finishH являются массивами дескрипторов потоков, созданных с помощью функции CreateThread. Потоки используются для выполнения задач в фоновом режиме.
struct PARAMETERS
{
    int* arr;
    int arrSize;
    int ID;
};

int main()
{
    int arraySize;
    std::cout << "Enter the size of array" << std::endl;
    std::cin >> arraySize;

    int* arr = new int[arraySize] {0};

    InitializeCriticalSection(&cs);

    int markersNum;
    std::cout << "Enter the number of markers" << std::endl;
    std::cin >> markersNum;


    hContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
    markersH = new HANDLE[markersNum];
    waitH = new HANDLE[markersNum];
    finishH = new HANDLE[markersNum];
    for (int i = 0; i < markersNum; i++)
    {
        finishH[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        waitH[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        markersH[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)marker, new PARAMETERS{ arr,arraySize,i + 1 }, 0, NULL);

    }

    WaitForMultipleObjects(markersNum, waitH, true, INFINITE);


    std::vector<bool> isTerminated(markersNum, false);
    for (int i = 0; i < markersNum; i++)
    {

        std::cout << "***" << std::endl;

        for (int j = 0; j < markersNum; j++)
        {
            if (!isTerminated[j])
                ResetEvent(waitH[j]);
        }

        PulseEvent(hContinue);

        WaitForMultipleObjects(markersNum, waitH, true, INFINITE);


        printArray(arr, arraySize);

        int markerTermID;
        do
        {
            std::cout << "Enter the ID of the thread to be terminated: " << std::endl;
            std::cin >> markerTermID;
        } while ((markerTermID <= 0 || markerTermID > markersNum) || isTerminated[markerTermID - 1]);

        markerTermID--;

        SetEvent(finishH[markerTermID]);
        WaitForSingleObject(markersH[markerTermID], INFINITE);
        CloseHandle(markersH[markerTermID]);
        isTerminated[markerTermID] = true;

        printArray(arr, arraySize);

    }

    for (int i = 0; i < markersNum; i++)
    {
        CloseHandle(waitH[i]);
        CloseHandle(finishH[i]);
    }
}

void marker(void* param)
{
    PARAMETERS* parameters = (PARAMETERS*)param;
    srand(parameters->ID);
    std::vector<int> indexes;

    HANDLE events[2] = { hContinue,finishH[parameters->ID - 1] };

    while (true)
    {
        SetEvent(waitH[parameters->ID - 1]);

        WaitForMultipleObjects(2, events, FALSE, INFINITE);
        if (WaitForSingleObject(finishH[parameters->ID - 1], 0) == WAIT_OBJECT_0)
            break;


        int randNum;
        while (true)
        {
            randNum = rand() % parameters->arrSize;
            EnterCriticalSection(&cs);
            if (parameters->arr[randNum] != 0)
            {
                break;
            }
            Sleep(5);
            parameters->arr[randNum] = parameters->ID;
            LeaveCriticalSection(&cs);
            indexes.push_back(randNum);
            Sleep(5);
        }

        std::cout << parameters->ID << " " << indexes.size() << " " << randNum << std::endl;
        LeaveCriticalSection(&cs);

    }

    EnterCriticalSection(&cs);
    for (int index : indexes)
    {
        parameters->arr[index] = 0;
    }
    LeaveCriticalSection(&cs);

    ExitThread(1);

}

void printArray(int* arr, int arraySize)
{
    std::cout << "The array: " << std::endl;
    for (int j = 0; j < arraySize; j++)
    {
        std::cout << arr[j] << " ";
    }
    std::cout << "\n";
}