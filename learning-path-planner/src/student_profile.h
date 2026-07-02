#ifndef STUDENT_PROFILE_H
#define STUDENT_PROFILE_H

#include <string>
#include <set>

using namespace std;

struct StudentProfile {
    string name;
    set<string> masteredModules;
    string targetModule;
    double timeBudgetHours;
};

// Profil sample "Dimas" (CLAUDE.md bagian 5.2): mastered={M1}, target=M7, budget=45 jam.
StudentProfile getSampleProfileDimas();

// Profil sample "Sari" (CLAUDE.md bagian 5.3): mastered={M1,M2,M3}, target=M7, budget=20 jam.
StudentProfile getSampleProfileSari();

#endif
