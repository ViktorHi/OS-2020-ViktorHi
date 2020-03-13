#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
#define SLEEP_TIME_AVERAGE_THREAD 12
#define SLEEP_TIME_MIN_MAX_THREAD 7

typedef struct MyData {
    int * array;
    int array_size;
} MYDATA, * PMYDATA;

void create_array(MYDATA** param);

void free_memory(PMYDATA param);

HANDLE create_thread(MYDATA* param, DWORD(*f)(LPVOID));

void wait_for_thread_end(const HANDLE h_thread);

DWORD min_max_thread(const LPVOID lp_parameter);

DWORD average_thread(const LPVOID lp_parameter);

void find_and_replace(const PMYDATA data);

void output_array(const PMYDATA data);

int max_array_element;
int min_array_element;
int array_average_element;
CRITICAL_SECTION cr_section;

int main()
{
    PMYDATA param=nullptr; 

    create_array(&param);
	
    InitializeCriticalSection(&cr_section);
    HANDLE h_min_max=create_thread(param, min_max_thread);
    HANDLE h_average=create_thread(param, average_thread);
	
    wait_for_thread_end(h_min_max);
    wait_for_thread_end(h_average);
    DeleteCriticalSection(&cr_section);
	
    find_and_replace(param);
    output_array(param);
	
    free_memory(param);
    return 0;
}

void wait_for_thread_end(const HANDLE  h_thread)
{
    WaitForSingleObject(h_thread, INFINITE);
    CloseHandle(h_thread);
}

HANDLE create_thread(MYDATA* param,DWORD (*f)(LPVOID))
{
    HANDLE h_thread = CreateThread(
        NULL,
        0, 
        LPTHREAD_START_ROUTINE(f), 
        param,    
        0,       
        NULL);
	
	if (h_thread == NULL)
    {
        cout << "Thread can not be created" << endl;
        exit(1);
    }
	
	return h_thread;
}

DWORD min_max_thread(const LPVOID lp_parameter)
{
    auto data = static_cast<PMYDATA>(lp_parameter);

    int max_temp, min_temp;
    max_temp = min_temp = data->array[0];
	
    for(int i=1;i<data->array_size;i++)
    {
        if (max_temp < data->array[i])
            max_temp = data->array[i];
        if (min_temp > data->array[i])
            min_temp = data->array[i];
        Sleep(SLEEP_TIME_MIN_MAX_THREAD);
    }

    min_array_element = min_temp;
    max_array_element = max_temp;
    
    EnterCriticalSection(&cr_section);
    cout << "min element " << min_temp<<endl;
    cout << "max element " << max_temp<<endl;
    LeaveCriticalSection(&cr_section);
	return 0;
}

DWORD average_thread(const LPVOID lp_parameter)
{
    auto data = static_cast<PMYDATA>(lp_parameter);
    int average_temp=0;

    for (int i = 0; i < data->array_size; i++)
    {
        average_temp += data->array[i];
        Sleep(SLEEP_TIME_AVERAGE_THREAD);
    }
	
    array_average_element = average_temp/data->array_size;
	
    EnterCriticalSection(&cr_section);
    cout << "average element " << array_average_element << endl;
    LeaveCriticalSection(&cr_section);
	
    return 0;
}

void create_array(MYDATA**  param)
{
    int size;
    cout << "Enter array size ";
    cin >> size;
    *param= new MyData;
    (*param)->array = new int[size];
    (*param)->array_size = size;
    cout << "enter array ";
	for(int i=0; i<size;i++)
	{
        scanf_s("%d", &(*param)->array[i]);
	}
	
}

void free_memory(const PMYDATA param)
{
    delete[] param->array;
    delete param;
}

void find_and_replace(const PMYDATA data)
{
	for(int i=0;i<data->array_size;i++)
	{
		if(data->array[i]==min_array_element || data->array[i] == max_array_element)
		{
            data->array[i] = array_average_element;
		}
	}
}

void output_array(const PMYDATA data)
{
    cout << "Result array: ";
	for(int i=0; i<data->array_size;i++)
	{
        cout<< data->array[i] << " ";
	}
}