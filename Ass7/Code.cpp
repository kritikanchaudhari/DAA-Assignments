#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>

using namespace std;

class ExamScheduler {
private:
    map<string, string> courses;
    map<string, vector<string>> enrollments;
    map<string, int> rooms;
    map<string, set<string>> graph;
    map<string, int> courseStudents;

public:

    ExamScheduler(
        map<string, string> c,
        map<string, vector<string>> e,
        map<string, int> r
    ) {
        courses = c;
        enrollments = e;
        rooms = r;

        for (auto &course : courses)
            graph[course.first] = {};

        buildGraph();
    }

    void buildGraph() {

        for (auto &student : enrollments) {

            vector<string> studentCourses = student.second;

            for (string course : studentCourses)
                courseStudents[course]++;

            for (int i = 0; i < studentCourses.size(); i++) {

                for (int j = i + 1;
                     j < studentCourses.size(); j++) {

                    string c1 = studentCourses[i];
                    string c2 = studentCourses[j];

                    if (c1 != c2) {
                        graph[c1].insert(c2);
                        graph[c2].insert(c1);
                    }
                }
            }
        }
    }

    map<string, int> greedyColoring() {

        map<string, int> color;

        for (auto &course : courses) {

            string current = course.first;
            set<int> usedColors;

            for (string neighbor : graph[current]) {

                if (color.find(neighbor) != color.end())
                    usedColors.insert(color[neighbor]);
            }

            int c = 0;

            while (usedColors.count(c))
                c++;

            color[current] = c;
        }

        return color;
    }

    map<string, int> welshPowell() {

        vector<string> order;

        for (auto &course : courses)
            order.push_back(course.first);

        sort(order.begin(), order.end(),
            [&](string a, string b) {

                return graph[a].size() > graph[b].size();
            });

        map<string, int> color;

        for (string current : order) {

            set<int> usedColors;

            for (string neighbor : graph[current]) {

                if (color.find(neighbor) != color.end())
                    usedColors.insert(color[neighbor]);
            }

            int c = 0;

            while (usedColors.count(c))
                c++;

            color[current] = c;
        }

        return color;
    }

    map<string, int> dsatur() {

        map<string, int> color;
        set<string> uncolored;

        for (auto &course : courses)
            uncolored.insert(course.first);

        while (!uncolored.empty()) {

            string selected = "";
            int bestSaturation = -1;
            int bestDegree = -1;

            for (string course : uncolored) {

                set<int> neighborColors;

                for (string neighbor : graph[course]) {

                    if (color.find(neighbor) != color.end())
                        neighborColors.insert(color[neighbor]);
                }

                int saturation = neighborColors.size();
                int degree = graph[course].size();

                if (saturation > bestSaturation ||
                    (saturation == bestSaturation &&
                     degree > bestDegree)) {

                    selected = course;
                    bestSaturation = saturation;
                    bestDegree = degree;
                }
            }

            set<int> usedColors;

            for (string neighbor : graph[selected]) {

                if (color.find(neighbor) != color.end())
                    usedColors.insert(color[neighbor]);
            }

            int c = 0;

            while (usedColors.count(c))
                c++;

            color[selected] = c;
            uncolored.erase(selected);
        }

        return color;
    }

    int numberOfColors(map<string, int> color) {

        set<int> colors;

        for (auto &x : color)
            colors.insert(x.second);

        return colors.size();
    }

    bool validateColoring(map<string, int> color) {

        for (auto &course : graph) {

            string current = course.first;

            for (string neighbor : course.second) {

                if (color[current] == color[neighbor])
                    return false;
            }
        }

        return true;
    }

    bool allocateRooms(
        map<string, int> color,
        map<int, map<string, vector<string>>> &schedule
    ) {

        for (int slot = 0;
             slot < numberOfColors(color);
             slot++) {

            vector<string> slotCourses;

            for (auto &course : color) {

                if (course.second == slot)
                    slotCourses.push_back(course.first);
            }

            sort(slotCourses.begin(), slotCourses.end(),
                [&](string a, string b) {

                    return courseStudents[a] >
                           courseStudents[b];
                });

            set<string> availableRooms;

            for (auto &room : rooms)
                availableRooms.insert(room.first);

            for (string course : slotCourses) {

                int requiredStudents =
                    courseStudents[course];

                int totalCapacity = 0;

                vector<string> selectedRooms;

                vector<string> roomList(
                    availableRooms.begin(),
                    availableRooms.end()
                );

                sort(roomList.begin(), roomList.end(),
                    [&](string a, string b) {

                        return rooms[a] > rooms[b];
                    });

                for (string room : roomList) {

                    selectedRooms.push_back(room);
                    totalCapacity += rooms[room];

                    availableRooms.erase(room);

                    if (totalCapacity >= requiredStudents)
                        break;
                }

                if (totalCapacity < requiredStudents)
                    return false;

                schedule[slot][course] =
                    selectedRooms;
            }
        }

        return true;
    }

    void printGraph() {

        cout << "\nCONFLICT GRAPH\n";
        cout << "--------------\n";

        for (auto &course : graph) {

            cout << course.first << " -> ";

            for (string neighbor : course.second)
                cout << neighbor << " ";

            cout << endl;
        }
    }

    void printSchedule(
        map<string, int> color,
        map<int, map<string, vector<string>>> schedule
    ) {

        cout << "\nFINAL EXAM TIMETABLE\n";

        for (int slot = 0;
             slot < numberOfColors(color);
             slot++) {

            cout << "\nTime Slot "
                 << slot + 1 << "\n";


            for (auto &course : schedule[slot]) {

                string courseID = course.first;

                cout << courseID
                     << " - "
                     << courses[courseID]
                     << " | Students: "
                     << courseStudents[courseID]
                     << " | Room(s): ";

                for (string room : course.second) {

                    cout << room
                         << "("
                         << rooms[room]
                         << ") ";
                }

                cout << endl;
            }
        }
    }
};

int main() {

    int nCourses;

    cout << "UNIVERSITY EXAM TIMETABLE SYSTEM\n";

    cout << "\nEnter number of courses: ";
    cin >> nCourses;

    map<string, string> courses;

    cout << "\nEnter course details\n";

    for (int i = 1; i <= nCourses; i++) {

        string id;
        string name;

        cout << "Enter course ID: ";
        cin >> id;

        cout << "Enter course name: ";
        cin >> ws;
        getline(cin, name);

        courses[id] = name;
    }

    int nStudents;

    cout << "\nEnter number of students: ";
    cin >> nStudents;

    map<string, vector<string>> enrollments;

    cout << "\nEnter student enrollments\n";

    for (int i = 1; i <= nStudents; i++) {

        string studentID;
        int numberOfCourses;

        cout << "\nEnter student ID: ";
        cin >> studentID;

        cout << "Enter number of courses for "
             << studentID << ": ";

        cin >> numberOfCourses;

        cout << "Enter course IDs: ";

        for (int j = 0; j < numberOfCourses; j++) {

            string courseID;
            cin >> courseID;

            if (courses.find(courseID) != courses.end()) {
                enrollments[studentID].push_back(courseID);
            }
            else {
                cout << "Invalid course ID: "
                     << courseID << endl;
            }
        }
    }

    int nRooms;

    cout << "\nEnter number of rooms: ";
    cin >> nRooms;

    map<string, int> rooms;

    cout << "\nEnter room details\n";

    for (int i = 1; i <= nRooms; i++) {

        string roomID;
        int capacity;

        cout << "Enter room ID: ";
        cin >> roomID;

        cout << "Enter capacity: ";
        cin >> capacity;

        rooms[roomID] = capacity;
    }

    ExamScheduler scheduler(
        courses,
        enrollments,
        rooms
    );

    scheduler.printGraph();

    map<string, int> greedy =
        scheduler.greedyColoring();

    map<string, int> welshPowell =
        scheduler.welshPowell();

    map<string, int> dsatur =
        scheduler.dsatur();

    cout << "\nCOLORING ALGORITHM COMPARISON\n";

    cout << "Greedy       : "
         << scheduler.numberOfColors(greedy)
         << " time slots\n";

    cout << "Welsh-Powell : "
         << scheduler.numberOfColors(welshPowell)
         << " time slots\n";

    cout << "DSATUR       : "
         << scheduler.numberOfColors(dsatur)
         << " time slots\n";

    cout << "\nVALIDATION\n";

    cout << "Greedy       : "
         << (scheduler.validateColoring(greedy)
             ? "Valid" : "Invalid")
         << endl;

    cout << "Welsh-Powell : "
         << (scheduler.validateColoring(welshPowell)
             ? "Valid" : "Invalid")
         << endl;

    cout << "DSATUR       : "
         << (scheduler.validateColoring(dsatur)
             ? "Valid" : "Invalid")
         << endl;

    int greedyColors =
        scheduler.numberOfColors(greedy);

    int wpColors =
        scheduler.numberOfColors(welshPowell);

    int dsaturColors =
        scheduler.numberOfColors(dsatur);

    int minimumColors =
        min({
            greedyColors,
            wpColors,
            dsaturColors
        });

    string bestAlgorithm;
    map<string, int> bestColoring;

    if (greedyColors == minimumColors) {

        bestAlgorithm = "Greedy";
        bestColoring = greedy;

    }
    else if (wpColors == minimumColors) {

        bestAlgorithm = "Welsh-Powell";
        bestColoring = welshPowell;

    }
    else {

        bestAlgorithm = "DSATUR";
        bestColoring = dsatur;
    }

    cout << "\nBEST ALGORITHM\n";

    cout << bestAlgorithm
         << " selected with "
         << minimumColors
         << " time slots.\n";

    map<int, map<string, vector<string>>> schedule;

    bool roomSuccess =
        scheduler.allocateRooms(
            bestColoring,
            schedule
        );

    if (roomSuccess) {

        scheduler.printSchedule(
            bestColoring,
            schedule
        );

    }
    else {

        cout << "\nROOM ALLOCATION ERROR\n";
        cout << "Not enough room capacity "
             << "to schedule all exams.\n";
    }

    return 0;
}
