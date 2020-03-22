#include <windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <random>

using namespace std;
#define SLEEP_TIME_MARKER_THREAD 5

typedef struct MyData {
    //vector<int>** array;
    int index_number;
} MYDATA, * PMYDATA;

void create_array();

void free_memory();

HANDLE create_thread(int index, DWORD(*f)(LPVOID));

DWORD marker(const LPVOID lp_parameter);

void output_array();

int ask_number_of_threads();

void create_handle_arrays(const int amount);

void init_handle_arrays();

void resume_threads();

void reset_event_from_thread(int num);

void set_event_from_thread(int num);

void reset_event_from_main(int num);

void set_event_from_main(int num);

void destroy_thread(vector<int>& filled, PMYDATA& data);

CRITICAL_SECTION cr_section;
vector<HANDLE>* h_events_from_thread;
vector<HANDLE>* h_events_from_main;
vector<int>* my_array;
vector <bool>* is_ended;
vector<HANDLE>* h_threads_list;

int main()
{
    create_array();
    int amount = ask_number_of_threads();
    create_handle_arrays(amount);

    InitializeCriticalSection(&cr_section);

    resume_threads();

    while (find(is_ended->begin(), is_ended->end(), false) != is_ended->end())
    {
        WaitForMultipleObjects(h_events_from_thread->size(), h_events_from_thread->data(), TRUE, INFINITE);

        for (int i = 0; i < amount; i++)
        {
            if (!is_ended->at(i))
                reset_event_from_thread(i);
        }

        cout << "Output in main before destroying thread \n";
        output_array();
        cout << "You need to destroy one thread \n";

        const int num_to_destroy = ask_number_of_threads() - 1;
        is_ended->at(num_to_destroy) = true;

        SetEvent(h_events_from_main->at(num_to_destroy));
        WaitForSingleObject(h_events_from_thread->at(num_to_destroy), INFINITE);     	//wait till marker thread destroyed himself

        cout << "Output in main after destroying thread \n";
        output_array();

        for (int i = 0; i < amount; i++)
        {
            if (!is_ended->at(i))
                SetEvent(h_events_from_main->at(i));
        }

    }

    DeleteCriticalSection(&cr_section);
    free_memory();
    return 0;
}

DWORD marker(const LPVOID lp_parameter)
{
    auto data = static_cast<PMYDATA>(lp_parameter);
    srand(data->index_number);
    vector<int> filled;

    while (true)
    {
        EnterCriticalSection(&cr_section);
        int r = rand() % my_array->size();
        if (my_array->at(r) == 0) {
            Sleep(SLEEP_TIME_MARKER_THREAD);
            my_array->at(r) = data->index_number + 1;
            filled.push_back(r);
            Sleep(SLEEP_TIME_MARKER_THREAD);
            LeaveCriticalSection(&cr_section);
        }
        else
        {
            LeaveCriticalSection(&cr_section);
            set_event_from_thread(data->index_number);
            WaitForSingleObject(h_events_from_main->at(data->index_number), INFINITE);
            reset_event_from_main(data->index_number);

            if (is_ended->at(data->index_number))
            {
                destroy_thread(filled, data);
                return 0;
            }
        }
    }
}

void reset_event_from_thread(int num)
{
    ResetEvent(h_events_from_thread->at(num));
}

void set_event_from_thread(int num)
{
    SetEvent(h_events_from_thread->at(num));
}

void set_event_from_main(int num)
{
    SetEvent(h_events_from_main->at(num));
}

void reset_event_from_main(int num)
{
    ResetEvent(h_events_from_main->at(num));
}

void destroy_thread(vector<int>& filled, PMYDATA& data)
{
    for (int i = 0; i < filled.size(); i++)
    {
        my_array->at(filled[i]) = 0;
    }

    set_event_from_thread(data->index_number);
    delete data;
}

HANDLE create_thread(int index, DWORD(*f)(LPVOID))
{
    MYDATA* param = new MYDATA();

    param->index_number = index;
    HANDLE h_thread = CreateThread(
        NULL,
        0,
        LPTHREAD_START_ROUTINE(f),
        param,
        CREATE_SUSPENDED,
        NULL);

    if (h_thread == NULL)
    {
        cout << "Thread can not be created" << endl;
        exit(1);
    }

    return h_thread;
}

void init_handle_arrays()
{
    for (int i = 0; i < h_threads_list->size(); i++)
    {
        h_events_from_thread->at(i) = CreateEvent(NULL, TRUE, FALSE, NULL);
        h_events_from_main->at(i) = CreateEvent(NULL, TRUE, FALSE, NULL);
        h_threads_list->at(i) = create_thread(i, marker);
    }
}

void resume_threads()
{
    for (int i = 0; i < h_threads_list->size(); i++)
    {
        ResumeThread(h_threads_list->at(i));
    }
}


void create_handle_arrays(const int amount)
{
    is_ended = new vector<bool>(amount, false);
    h_threads_list = new vector<HANDLE>(amount, nullptr);
    h_events_from_thread = new vector<HANDLE>(amount);
    h_events_from_main = new vector<HANDLE>(amount);
    init_handle_arrays();
}

void create_array()
{
    int size;
    cout << "Enter array size ";
    cin >> size;

    my_array = new vector<int>(size);

    for (int i = 0; i < size; i++)
    {
        my_array->at(i) = 0;
    }
}

void free_memory()
{
    delete h_events_from_thread;
    delete h_events_from_main;
    delete my_array;
    delete is_ended;
    delete h_threads_list;
}

void output_array()
{
    cout << "Array: ";
    for (int i = 0; i < my_array->size(); i++)
    {
        cout << my_array->at(i) << " ";
    }
    cout << endl;
}

int ask_number_of_threads()
{
    int ans;
    cout << "Enter number of threads ";
    cin >> ans;
    return ans;
}