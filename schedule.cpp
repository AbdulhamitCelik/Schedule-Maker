#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>
#include <map>
#include <limits>

int block_size = 0;

std::vector<std::tuple<int, int, std::string>> initialise_timetable() {
    std::vector<std::tuple<int, int, std::string>> timetable;
    std::tuple<int, int, std::string> temp;

    while (true) {
        try {
            while (block_size < 1 || block_size > 60) {
                std::cout << "Enter size of a block in minutes (1 to 60)\n\t--";
                std::cin >> block_size;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        catch (...) {
            std::cout << "Incorrect kind of input provided";
            continue;
        }

        for (int i = 0; i < (24*60); i += block_size) {
            temp = std::tuple(i / 60, i % 60,  "empty");
            timetable.push_back(temp);
        }
        break;
    }

    std::cout << "\nTimetable has been (re)initialised successfully. \n\n";
    block_size = 0;
    return timetable;
}

std::vector<std::tuple<int, int, std::string>> input_into_timetable(std::vector<std::tuple<int, int, std::string>> timetable) {
    std::tuple<int, int, std::string> temp;

    while(true) {
        int hour, minute, blocks = 0;
        std::string activity = "";

        std::cout << "Type with the format or type 'exit': \n- hour (00 to 23 inclusive) \n- minute (intervals of " << block_size << " starting from 00 up to 59) \n- activity (any string \" x y z\") \n- e.g. 05:06 Something\n\t--";
        std::getline(std::cin, activity);

        if(activity == "exit") {
            break;
        }

        try {
            hour = (activity[0]-'0')*10 + (activity[1]-'0');
            activity.erase(0, 3);
            minute = (activity[0]-'0')*10 + (activity[1]-'0');
            activity.erase(0, 3);
        }
        catch (...) {
            std::cout << "Incorrect kind of input provided";
            continue;
        }

        if(hour > 23 || hour < 0 || !(minute % 15 == 0 && minute >=0 && minute <=45)) {
            std::cout << "\nPlease use the correct format\n\n";
            continue;
        }

        try {
            while (blocks <= 0 || blocks > (60/block_size)*24) {
                std::cout << "Enter number of blocks to take up (1 block is" << block_size << " minutes long)\n\t--";
                std::cin >> blocks;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (blocks <= 0) {
                    std::cout << "Amount of Blocks must be greater than 0\n";
                }
            }
        }
        catch (...) {
            std::cout << "Incorrect kind of input provided";
            continue;
        }

        for (int i = 0; i < blocks; i++){
            int newhour = hour + int((minute + 15*i)/60);
            int newminute = (minute + 15*i)%60;
            temp = std::tuple(newhour, newminute, activity);
            timetable[(newhour*60 + newminute)/15] = temp;
        }
        std::cout << "\nActivity has been inputted into Timetable. \n\n";
    }
    std::cout << "\nInputting into Timetable complete. \n\n";
    return timetable;
}

void print_timetable(std::vector<std::tuple<int, int, std::string>> timetable) {
    int hour, minute, counter = 0;
    std::string activity = "";

    for (int i = 0; i < timetable.size(); i++) {
        if (counter == 0) {
            std::cout << "TIME \tACTIVITY\n\n";
            counter++;
            i--;
            continue;
        }

        std::tie(hour, minute, activity) = timetable[i];
        if (minute > 9) {
            if (hour < 10) {
                std::cout << "0" << std::to_string(hour) + ":" + std::to_string(minute) + "\t" + activity + "\n";
            } else {
                std::cout << std::to_string(hour) + ":" + std::to_string(minute) + "\t" + activity + "\n";
            }
        } else {
            if (hour < 10) {
                std::cout << "0" << std::to_string(hour) + ":0" + std::to_string(minute) + "\t" + activity + "\n";
            } else {
                std::cout << std::to_string(hour) + ":0" + std::to_string(minute) + "\t" + activity + "\n";
            }
        }
    }

    std::cout << "\nTimetable has been printed successfully. \n\n";
}

void output_to_file(std::vector<std::tuple<int, int, std::string>> timetable){
    std::string filename;
    int hour, minute, counter = 0;
    std::string activity = "";


    std::cout << "Enter filename with file extension .csv with no spaces: ";
    std::cin >> filename;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // instantiating ofstream and opening a file
    std::ofstream *file = new std::ofstream();
    file -> open(filename, std::ios::out); //std::ios::out enables write mode and overwrites everything in the file if it exists or creates a new one

    // writing to file
    for (int i = 0; i < timetable.size(); i++) {
        if(counter == 0){
            (*file) << "TIME,ACTIVITY" << std::endl;
            counter++;
            i--;
            continue;
        }

        // format per line is: hour,min,activity,
        std::tie(hour, minute, activity) = timetable[i];
        if (minute == 0 && hour < 10) {
            (*file) << "0" << std::to_string(hour) + "," + std::to_string(minute) + "0," + activity << std::endl;
        } else if (minute == 0 && hour >= 10){
            (*file) << std::to_string(hour) + "," + std::to_string(minute) + "0," + activity << std::endl;
        } else if (hour < 10) {
            (*file) << "0" << std::to_string(hour) + "," + std::to_string(minute) + "," + activity << std::endl;
        } else {
            (*file) << std::to_string(hour) + "," + std::to_string(minute) + "," + activity << std::endl;
        }
    }

    std::cout << "\nTimetable has been written to file successfully. \n\n";
}

std::vector<std::tuple<int, int, std::string>> input_from_file() {
    std::vector<std::tuple<int, int, std::string>> timetable;
    std::tuple<int, int, std::string> temp;
    std::string filename;
    std::string data;
    std::string temp_str;
    int hour, minute = 0;
    int counter = 0;
    std::string activity = "";

    std::cout << "Enter filename with file extension: ";
    std::cin >> filename;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::ifstream *file = new std::ifstream();
    file -> open(filename, std::ios::in);
    while (std::getline(*file, data)) {

        if (counter++ < 1) continue;

        std::stringstream stream(data);
        std::getline(stream, temp_str, ',');
        hour = std::stoi(temp_str);
        std::getline(stream, temp_str, ',');
        minute = std::stoi(temp_str);
        std::getline(stream, temp_str, ',');

        temp = std::tuple(hour, minute, temp_str);
        timetable.push_back(temp);
    }

    std::cout << "\nTimetable has been fetched from file successfully. \n\n";

    return timetable;
}

void check_timeslot(std::vector<std::tuple<int, int, std::string>> timetable) {
    std::string activity, c;
    int hour, minute, a, b = 0;

    while(true) {
        std::cout << "Type with the format or exit: hour (00 to 24) minute (00 to 45 in intervals of 15) activity (any string \" x y z\") \n\t--";
        std::getline(std::cin, activity);
        if(activity == "exit") {
            break;
        }

        hour = (activity[0]-'0')*10 + (activity[1]-'0');
        activity.erase(0, 3);
        minute = (activity[0]-'0')*10 + (activity[1]-'0');

        if(hour > 23 || hour < 0 || !(minute % 15 == 0 && minute >=0 && minute <=45)) {
            std::cout << "\nPlease use the correct format\n\n";
            continue;
        }

        std::tie(a, b, c) = timetable[(hour*60 + minute)/15];
        std::cout << "\n\nTIME \tACTIVITY\n";
        if (minute == 0 && hour < 10) {
            std::cout << "0" << std::to_string(hour) + ":" + std::to_string(minute) + "0\t" + c + "\n";
        } else if (minute == 0 && hour >= 10){
            std::cout << std::to_string(hour) + ":" + std::to_string(minute) + "0\t" + c + "\n";
        } else if (hour < 10) {
            std::cout << "0" << std::to_string(hour) + ":" + std::to_string(minute) + "\t" + c + "\n";
        } else {
            std::cout << std::to_string(hour) + ":" + std::to_string(minute) + "\t" + c + "\n";
        }
        std::cout << "\nTimeslot has been checked. \n\n";
    }

    std::cout << "\nChecking timeslots complete. \n\n";
}

void calculate_activity_durations(std::vector<std::tuple<int, int, std::string>> timetable) {
    std::map<std::string, int> activity_duration;
    std::string usr_in, activity;
    int hour, minute = 0;

    for (int i = 0; i < timetable.size(); i++) {
        std::tie(hour, minute, activity) = timetable[i];
        if(activity_duration.contains(activity)) {
            activity_duration[activity] += 15;
        }
        else {
            activity_duration[activity] = 15;
        }
    }

    std::cout << "Enter activity to check duration of or press ~~ to exit: \n\t--";
    while(std::getline(std::cin, usr_in) && usr_in != "~~") {
        std::cout << "\n" << usr_in << " has duration: " << activity_duration[usr_in] << " minutes, or " << (activity_duration[usr_in]/60) << " hours\n";
        std::cout << "Enter activity to check duration of or press ~~ to exit: \n\t--";
    }

    std::cout << "\nActivity durations have been checked successfully. \n\n";

}

std::vector<std::tuple<int, int, std::string>> call_function(int page, int subpage, std::vector<std::tuple<int, int, std::string>> timetable) {
    switch (page) {
        case 0:
            // Main Page
            switch (subpage) {
                case 1:
                    timetable = initialise_timetable();
                    break;
                case 2:
                    print_timetable(timetable);
                    break;
                case 3:
                    timetable = input_from_file();
                    break;
                case 4:
                    timetable = input_into_timetable(timetable);
                    break;
                case 5:
                    output_to_file(timetable);
                    break;
                case 6:
                    check_timeslot(timetable);
                    break;
                case 7:
                    calculate_activity_durations(timetable);
                default:
                    break;
            }
            break;
        /*case 1:
            switch (subpage) {
                case 0:
                    break;
                case 1:
                    break;
            }
            break;
        default:
            break;*/
    }

    return timetable;
}

void populate_page(int page) {
    switch (page) {
        case 0:
            std::cout << "1. Initialise a new Timetable\n";
            std::cout << "2. Print a Timetable onto the Screen\n";
            std::cout << "3. Get a Pre-Existing Timetable from a File\n";
            std::cout << "4. Input Activities into Timetable\n";
            std::cout << "5. Output Current Timetable to a File\n";
            std::cout << "6. Check Activity in a Specific Time Slot in Current Timetable\n";
            std::cout << "7. Check Specific Activity Total Duration in Current Timetable\n";
            std::cout << "8. exit\n";
            break;
        case 1:
            break;
        default:
            break;
    }
    std::cout << "-- ";
}

void user_interface(std::vector<std::tuple<int, int, std::string>> timetable) {
    int page, subpage = 0;
    std::string input;
    std::set<int> subpages = {1, 2, 3, 4, 5, 6, 7};

    while (true) {
        populate_page(0);
        try {
            std::getline(std::cin, input);
            subpage = std::stoi(input);
        }
        catch(...){
            std::cout << "\nEnter a single number in the options. \n\n";
            continue;
        }

        if(!subpages.contains(subpage) && subpage == 8) {
            std::cout << "\nScheduler Builder usage complete. \n\n";
            break;
        }
        if(!subpages.contains(subpage)) {
            std::cout << "\nEnter a single number in the options. \n\n";
            continue;
        }
        timetable = call_function(0, subpage, timetable);
    }
}

void revision_manager() {
    // create main folder
    // create a folder for each subject
    // subfolder for each topic
    // put resources in topic folders
    // put log file in main, subject and topic folders to keep track of last time studied, total study time and notes written by date
    // create a file with paths to folders and subfolders
}

int main() {
    std::vector<std::tuple<int, int, std::string>> timetable;

    user_interface(timetable);
}
